from sys import argv, exit
from csv import reader, DictReader
import csv

# Check that correct command-line arguments provided
if len(argv) != 3:
    print("Usage: dna.py [DNA database .csv] [DNA sample .txt]")
    exit(1)

# Open the DNA sequence and read its contents into memory - as a string
with open(argv[2], "r") as file:
    sample = str(file.read())

# Open the database CSV file and read its contents into memory
with open(argv[1], "r") as datafile:
    database = reader(datafile)
    #Save STR's in a list
    for row in database:
        str = row
        str.pop(0)
        break

#convert database to a dict with str's are keys
strSequences = {}
for item in str:
    strSequences[item] = 1


# Calculate longest repeat of each STR in the DNA sample
## For each STR, count how many times it repeats starting from each position in sequence
repeat = 1
max_repeat = 1

for key in strSequences:
    for i in range(len(sample)):
        if sample[i : i + (len(key) * 2)] == key + key:
            repeat +=1
            i += len(key)

        #Once reaches end of STR repeat, store size of repeat in max_repeat if it is the largest repeat so far
        else:
            if repeat > max_repeat:
                max_repeat = repeat

    #Store max repeat of current STR in dict and reset counter variables ready for next STR
    strSequences[key] = max_repeat
    repeat = 1
    max_repeat = 1


# Compare computed STR counts against each row in CSV database file to find a match
with open(argv[1], newline='') as datafile:
    database = DictReader(datafile)

    for person in database:
        match = 0
        # compares the sequences to every person and prints name before leaving the program if there is a match
        for sample in strSequences:
            if strSequences[sample] == int(person[sample]):
                match += 1
        if match == len(strSequences):
            print(person['name'])
            exit()

    print("No match")