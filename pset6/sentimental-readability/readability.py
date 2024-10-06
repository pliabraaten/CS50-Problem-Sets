from cs50 import get_string

text = get_string("Text: ")

word_count = 1  # One more word than spaces in a sentence
sentence_count = 0
letter_count = 0

# Loop through each character and count letters, words, and sentences
for char in text:
    if char == " ":
        word_count += 1
    elif char == "." or char == "!" or char == "?":
        sentence_count += 1
    elif char.isalpha():
        letter_count += 1

# Calculate variables
L = (letter_count / word_count) * 100
S = (sentence_count / word_count) * 100

# Coleman-Liau index
index = round(0.0588 * L - 0.296 * S - 15.8)

if index < 1:
    print("Before Grade 1")
elif index > 16:
    print("Grade 16+")
else:
    print(f"Grade {index}")
