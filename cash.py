from cs50 import get_float

#Prompt user for change amount
change = 0
while change <= 0:
    change = get_float("Change owed: ")


#Convert dollars to cents and convert to integer to prevent floating point imprecision
cents = round(change * 100)

#Calculate minimum number of coins to return
coins = 0

while cents >= 25:
    cents -= 25
    coins += 1

while cents >= 10:
    cents -= 10
    coins += 1

while cents >= 5:
    cents -= 5
    coins += 1

while cents >= 1:
    cents -= 1
    coins += 1

#Print number of coins to return
print(coins)