from cs50 import get_string

# Prompt reader for text
text = get_string("Text: ")

# Count No. of letters, words and sentences in text
letters = 0
words = 1
sentences = 0

for i in range(len(text)):

    # letters
    if (text[i] >= 'a' and text[i] <= 'z') or (text[i] >= 'A' and text[i] <= 'Z'):
        letters += 1
        
    # words
    elif (text[i] == ' '):
        words += 1
        
    # sentences
    elif (text[i] == '.' or text[i] == '?' or text[i] == '!'):
        sentences += 1


# Calculate coleman-Liau index and round to nearest integer

L = (letters / words) * 100
S = (sentences / words) * 100

grade = round(0.0588 * L - 0.296 * S - 15.8)

# Output readability grade
if (grade >= 16):
    print("Grade 16+")
    
elif (grade < 1):
    print("Before Grade 1")

else:
    print("Grade", grade)
    