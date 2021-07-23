from sys import argv, exit
import csv
import re


def main():
    if len(argv) != 3:
        print("Usage: python dna.py data.csv sequence.txt")
        sys.exit(1)

    db = argv[1]
    file = argv[2]
    # read the csv file
    csvfile = open(db, "r")
    reader = csv.reader(csvfile)
    # get the STRs seq
    STRseq = next(reader)[1:]

    # get the dna seq
    with open(file, 'r') as file:
        seq = file.read()
    dnaMaxNum = []

    # find the compute the number of the longest sonsecutive repeats
    for STR in STRseq:
        dnaMaxNum.append(dnacheck(seq, STR))

    for row in reader:
        match_point = 0
        for i in range(len(STRseq)):
            if int(row[i+1]) != dnaMaxNum[i]:
                continue
            match_point += 1

        if match_point == len(STRseq):
            print(row[0])
            exit(0)
    file.close
    print("No match")


def dnacheck(seq, STRs):
    # r mean the original string
    # {STRs} mean the repeated string
    # \1 mean the reference of the repeated string
    # * mean repeat the string as much as it can
    p = rf'({STRs})\1*'
    pattern = re.compile(p)
    match = []
    # find the pattern in seq
    for i in pattern.finditer(seq):
        match.append(i)
    maxNum = 0
    # find the longest repeated seq
    for i in range(len(match)):
        if match[i].group().count(STRs) > maxNum:
            maxNum = match[i].group().count(STRs)
    return maxNum


main()
