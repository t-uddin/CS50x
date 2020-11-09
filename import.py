from cs50 import SQL
from sys import argv, exit
import csv

# Create db variable to access database
db = SQL("sqlite:///students.db")

# Check that correct number of command-line arguments provided
if len(argv) != 2:
    print("Usage: import.py [characters.csv]")
    exit(1)

# Open CSV file and read it one line at a time
with open(argv[1], "r") as file:
    reader = csv.DictReader(file)

    # For each row, parse name and insert student into db
    for row in reader:
        name = row["name"].split()
        house = row["house"]
        birth = row["birth"]

        # If studnet has 3 names, insert first, middle, then last name
        if len(name) == 3:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", name[0], name[1], name[2], house, birth)

        # If student has 2 names, set middle name as NULL
        elif len(name) == 2:
            db.execute("INSERT INTO students(first, middle, last, house, birth) VALUES(?, ?, ?, ?, ?)", name[0], None, name[1], house, birth)

