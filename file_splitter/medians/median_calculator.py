import os
import statistics
import numpy as np


class TestCaseResults:
    def __init__(self, line):
        tokens = line.split()
        self.test_case_number = int(tokens[0])
        self.upper_best = int(tokens[1])
        self.gomea_mrpds_text = [tokens[index].translate(str.maketrans('', '', '()')) for index in range(2, 9, 2)]
        self.gomea_arpds_text = [tokens[index].translate(str.maketrans('', '', '()')) for index in range(3, 10, 2)]
        self.gmeda_mrpd_text = tokens[10]
        self.gmeda_arpd_text = tokens[11].translate(str.maketrans('', '', '()'))

        self.gomea_mrpd_best_text = None
        self.gomea_arpd_best_text = None

        self.p3_mrpd = None
        self.p3_arpd = None
        self.p3_best_found = None
        self.p3_upper_best_reached = None
        self.calculate_best_mrpd_gomea_statistics()

    def set_p3_values(self, mrpd, arpd, p3_best_found):
        self.p3_mrpd = mrpd
        self.p3_arpd = arpd
        self.p3_best_found = p3_best_found
        self.p3_upper_best_reached = p3_best_found <= self.upper_best

    def calculate_best_mrpd_gomea_statistics(self):
        mrpds = np.array(list(map(lambda x: float(x), self.gomea_mrpds_text)))
        best_index = np.argmin(mrpds)
        self.gomea_mrpd_best_text = self.gomea_mrpds_text[best_index]
        self.gomea_arpd_best_text = self.gomea_arpds_text[best_index]

    def to_long_txt_line(self):
        return f"{self.test_case_number} {self.upper_best} {self.p3_mrpd} ({self.p3_arpd}) {self.gomea_mrpd_best_text} ({self.gomea_arpd_best_text}) {self.gmeda_mrpd_text} ({self.gomea_arpd_best_text}) \n"


def save_test_case_results(test_case_results: [TestCaseResults]):
    result = ["Index Best P3MRPD(ARPD) GOMEAMRPD(ARPD) GMEDAMRPD(ARPD)\n"]
    for (test_case) in test_case_results:
        result.append(test_case.to_long_txt_line())

    with open("whole_results_statistics.txt", 'wt') as stats_file:
        stats_file.writelines(result)


def read_gomea_gm_eda_results(file_name):
    with open(file_name, 'rt') as bests:
        lines = bests.readlines()
        return list(map(lambda line: TestCaseResults(line), lines))


def read_upper_bests(file_name):
    with open(file_name, 'rt') as bests:
        lines = bests.readlines()
        tokens = [line.split() for line in lines]
        result = [(test_case_name, int(best_value)) for (test_case_name, best_value) in tokens]
        return result


def read_results(summary_file_path):
    with open(summary_file_path, 'rt') as summary:
        lines = summary.readlines()
        bests = list(map(lambda line: -int(line.split(";")[1]), lines))
        return bests


def calculate_statistics(test_case_name, upper_best):
    summary_file_name = os.path.join('results', test_case_name, 'summary.csv')
    if os.path.isfile(summary_file_name):
        bests_found = read_results(summary_file_name)
        percentage_deviations = [100 * (best_found - upper_best) / upper_best for best_found in bests_found]
        mrpd = statistics.median(percentage_deviations)
        arpd = statistics.mean(percentage_deviations)
        best_ever_found = max(bests_found)
        is_at_least_as_good = best_ever_found <= upper_best
        return test_case_name, upper_best, round(mrpd, 2), round(arpd, 2), best_ever_found, is_at_least_as_good
    else:
        return None


def calculate_all_statistics(upper_bests):
    stats = map(lambda upper_best: calculate_statistics(upper_best[0], upper_best[1]), upper_bests)
    stats = list(filter(lambda stat: stat is not None, stats))
    return stats


def merge_with_p3_results(gomea_gm_eda_results):
    results = []
    for test_case_result in gomea_gm_eda_results:
        p3_result = calculate_statistics(str(test_case_result.test_case_number), test_case_result.upper_best)
        if p3_result is not None:
            (_, _, mrpd, arpd, best_ever_found, _) = p3_result
            test_case_result.set_p3_values(mrpd, arpd, best_ever_found)
            results.append(test_case_result)
    return results


def save_txt_file(stats):
    result = []
    for (test_case_name, upper_best, mrpd, arpd, best_ever_found, is_at_least_as_good) in stats:
        result.append(f"{test_case_name}, {upper_best}, {mrpd} ({arpd}) {is_at_least_as_good}\n")

    with open("result_statistics.txt", 'wt') as stats_file:
        stats_file.writelines(result)


def save_tex_file(stats):
    pass


if __name__ == '__main__':
    test_case_results = read_gomea_gm_eda_results('gomea_gm_eda_enumerated.txt')
    merged_results = merge_with_p3_results(test_case_results)
    save_test_case_results(merged_results)