import os

RESULT_PATH = "results"


def saveTestCase(index, nJobs, nMachines, machineTimes):
    newFileName = f"flowshop_{nJobs}_{nMachines}_{index}.txt"
    filePath = os.path.join(RESULT_PATH, newFileName)
    lines = [" ".join(line.split()) + "\n" for line in machineTimes]
    header = f"{nJobs} {nMachines}\n"
    with open(filePath, mode="wt") as fileToWrite:
        fileToWrite.write(header)
        fileToWrite.writelines(lines)


def split(file):
    with open(file, mode="rt") as openedFile:
        lines = openedFile.readlines()
    metadata = lines[1].split()
    nJobs, nMachines = int(metadata[0]), int(metadata[1])
    testCases = [lines[i * nMachines + (i + 1) * 3: i * nMachines + (i + 1) * 3 + nMachines] for i in range(10)]
    for i, testCaseData in enumerate(testCases):
        saveTestCase(i, nJobs, nMachines, testCaseData)


if __name__ == '__main__':
    for file in os.listdir("."):
        if file.endswith(".txt"):
            split(file)