with open('gomea_gm_eda_results.txt', 'rt') as bests:
    lines = bests.readlines()
    result = []
    for i, line in enumerate(lines):
        result.append(f"{i+1} {line}")
    with open('gomea_gm_eda_enumerated.txt', 'wt') as res:
        res.writelines(result)