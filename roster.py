from cs50 import SQL
from sys import argv, exit
import csv

# Create db variable to access database
db = SQL("sqlite:///students.db")

# Check that correct number of command-line arguments provided
if len(argv) != 2:
    print("Usage: roster.py [house name]")
    exit(1)

# Query database for all students in given house, ordered alphabetically by last name then first name
students = db.execute("SELECT * FROM students WHERE house = (?) ORDER BY last, first", argv[1])

# Print each students name and birth year in that house
for row in students:
    first = row["first"]
    middle = row["middle"]
    last = row["last"]
    birth = row["birth"]
    
    if middle == None:
        print(f"{first} {last}, born {birth}")
        
    else:
        print(f"{first} {middle} {last}, born {birth}")