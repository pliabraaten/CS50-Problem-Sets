import csv
import sys
import pprint


def main():

    # Check for command-line usage
    if len(sys.argv) != 3:
        # print("Enter: csv file name, txt file name")
        sys.exit()

    # Read database csv file into a variable
    rows = []   # Create create rows (list) of dictionaries
    with open(f"{sys.argv[1]}") as database_file:
        reader = csv.DictReader(database_file)
        # print(reader.fieldnames)
        for row in reader:
            for STR in row:
                if STR != 'name':
                    row[STR] = int(row[STR])
            rows.append(row)
            # pprint.pprint(row)

    # Read DNA sequence txt file into a variable
    with open(f"{sys.argv[2]}") as dna_file:
        dna = dna_file.read()

    # Find longest match of each STR in DNA sequence
    longest_matches = {}    # Create empty dictionary for longest match values
    for STR in reader.fieldnames[1:]:
        longest_matches[STR] = longest_match(dna, STR)

    # print(f"longest_matches dict: {longest_matches}")

    # Check database for matching profiles
    for person in rows:
        if all(person[STR] == longest_matches[STR] for STR in longest_matches):
            print(f"{person['name']}")
            break
    else:
        print("No match")

    return


def longest_match(sequence, subsequence):
    """Returns length of longest run of subsequence in sequence."""

    # Initialize variables
    longest_run = 0
    subsequence_length = len(subsequence)
    sequence_length = len(sequence)

    # Check each character in sequence for most consecutive runs of subsequence
    for i in range(sequence_length):

        # Initialize count of consecutive runs
        count = 0

        # Check for a subsequence match in a "substring" (a subset of characters) within sequence
        # If a match, move substring to next potential match in sequence
        # Continue moving substring and checking for matches until out of consecutive matches
        while True:

            # Adjust substring start and end
            start = i + count * subsequence_length
            end = start + subsequence_length

            # If there is a match in the substring
            if sequence[start:end] == subsequence:
                count += 1

            # If there is no match in the substring
            else:
                break

        # Update most consecutive matches found
        longest_run = max(longest_run, count)

    # After checking for runs at each character in seqeuence, return longest run found
    return longest_run


main()
