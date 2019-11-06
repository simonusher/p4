import os
import sys
import statistics
last_lines = []
values = []
best = int(sys.argv[1])
for filename in os.listdir(os.getcwd()):
    with open(filename) as f:
        if filename == "merger.py":
            continue
        lines = f.readlines()
        last_line = lines[-1]
        value = int(last_line.split(";")[1])
        last_lines.append(last_line)
        values.append(value)

percents = [100 * ((-x) - best) / best for x in values]
median = statistics.median(percents)
mean = statistics.mean(percents)
# for x in percents:
#     last_lines.append(str(x))
last_lines.append("srenia;" + str(mean) + ";\n")
last_lines.append("mediana;" + str(median) + ";\n")
with open('summary.csv', 'wt') as summary:
    summary.writelines(last_lines)