#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
} pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
bool check_cycle(int winner, int loser);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }
        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    // Check if name entered is a candidate name
    for (int i = 0; i < candidate_count; i++)
    {
        // If voted name is a candidate, then rank that candidate
        if (strcmp(name, candidates[i]) == 0)
        {
            ranks[rank] = i;
            return true;
        }
    }
    // Return false for invalid vote if name doesn't match any candidates
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{
    // Loop through each voter's ranks
    for (int i = 0; i < candidate_count; i++)
    {
        // Loop through the rest of the ranked candidates
        for (int j = i + 1; j < candidate_count; j++)
        {
            // The ranks[i] candidate is ranked higher than the candidate at Ranks[i+1]
            preferences[ranks[i]][ranks[j]]++;
        }
    }
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    // Note: pairs is an array of multiple pair types
    // Loop through different candidate pairings
    // And determine winner/loser based on preference matrix
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Since we are only comparing i and j once, then need to compare preferences both ways
            // Reminder: preferences[i][j] = number of people who prefer i over j
            if (preferences[i][j] > preferences[j][i])
            {
                // If count of votes preferring i over j, then set i as winner and j as loser
                // Since pair_count starts at 0, the current element matches the count
                pairs[pair_count].winner = i;
                pairs[pair_count].loser = j;
                // Update the count of pairs
                pair_count++;
            }
            else if (preferences[i][j] < preferences[j][i])
            {
                pairs[pair_count].winner = j;
                pairs[pair_count].loser = i;
                // Update the count of pairs
                pair_count++;
            }
            // Increment pair_count instead if statements so equal preferences are not included
        }
    }
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // Bubble Sort
    // Loop through pairs
    int unsorted_count = pair_count;
    while (unsorted_count > 0)
    {
        for (int i = 0; i < unsorted_count - 1; i++)
        {
            // Check if victory of pairs[i] is less than victory of pairs[i+1], then swap position
            if (preferences[pairs[i].winner][pairs[i].loser] <
                preferences[pairs[i + 1].winner][pairs[i + 1].loser])
            {
                // Create temp value to hold lower value before swapping position to the right
                pair lower_value = pairs[i];
                pairs[i] = pairs[i + 1];
                pairs[i + 1] = lower_value;
            }
        }
        // After checking all positions in array (lowest position should be farthest right)
        // Then count that element as sorted
        unsorted_count--;
    }
    return;
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    // Loop through sorted pairs
    for (int i = 0; i < pair_count; i++)
    {
        // Lock in pair, if there isn't a cycle back to the winner of the pair
        // True means there is a cycle
        if (!check_cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Check if pair creates a cycle
bool check_cycle(int winner, int loser)
{
    // Loser variable stays the same candidate that was passed into the function
    // Below, the loser variable cycles through the locked pairs
    // If the if the cycled loser value is ever the same candidate as the winner
    // From the original pair, cycle is true
    if (loser == winner)
    {
        return true;
    }

    // Loop through candidates to check recursively to check for cycle
    for (int i = 0; i < candidate_count; i++)
    {
        // Check is the Loser is a locked winner over i [candidate], and
        if (locked[loser][i] && check_cycle(winner, i))
        {
            return true;
        }
    }
    // If no cycle is found, return false
    return false;
}

// Print the winner of the election
void print_winner(void)
{
    // Create array to track if candidates have a lost edge
    bool paired_lost[candidate_count];

    // Initialize all candidates to false (no lost edge)
    for (int i = 0; i < candidate_count; i++)
    {
        paired_lost[i] = false;
    }

    // Loop through locked pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            // If candidate has lost edge, set their value in paired_lost as true
            if (locked[i][j] == true)
            {
                paired_lost[j] = true;
            }
        }
    }
    // Loop through paired_lost looking for the candidate with no lost edges (still set as false)
    for (int i = 0; i < candidate_count; i++)
    {
        if (paired_lost[i] == false)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}
