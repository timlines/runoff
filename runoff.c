#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    string name;
    int votes;
    bool eliminated;
}
candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, string name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    voter_count = get_int("Number of voters: ");
    if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Keep querying for votes
    for (int i = 0; i < voter_count; i++)
    {

        // Prinf the voter # for degugging
        // printf("Voter %i\n", i);

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // //print out the graph for debugging
    // for (int i = 0; i < voter_count; i++)
    // {
    //     printf("voter %i: ", i);

    //     for(int j = 0; j < candidate_count; j++)
    //     {
    //         printf("%i ", preferences[i][j]);
    //     }
    //     printf("\n");
    //  }
    // printf("\n");

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        //debug code to show tallied votes.
        // for (int i = 0; i < candidate_count; i++)
        //         printf("%s with %i votes\n", candidates[i].name, candidates[i].votes);

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, string name)
{
    // Record preference if vote is valid

    for (int i = 0; i < candidate_count; i++)
        if (strcmp(candidates[i].name, name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    // loop through the voters
    for (int i = 0; i < voter_count; i++)
    {
        //loop through the candidates
        for (int j = 0; j < candidate_count; j++)
            //check if the candidates have been eliminated
            if (!candidates[preferences[i][j]].eliminated)
            {
                //tally to votes
                candidates[preferences[i][j]].votes++;
                break;
            }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    //cycle through all candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if a single candidates has a majority of votes
        if (candidates[i].votes > (voter_count / 2))
        {
            //print the winner
            printf("%s\n", candidates[i].name);
            return true;
        }
    }
    //otherwise return to main thread to keep tabulating
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    //create an integer to store the minimum value
    int minimum = voter_count;

    //loop through all the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //check if the candidate has been eliminated
        if (!candidates[i].eliminated)
        {

            //compare the candidate votes to the minimum
            if (candidates[i].votes < minimum)
            {

                //store the minimum
                minimum = candidates[i].votes;
            }
        }
    }
    //print the minimum for debugging
    // printf("minimum: %i\n", minimum);

    //send minimum back to main loop
    return minimum;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    //cycle through the candidates
    for (int i = 0; i < candidate_count; i++)
    {

        //check if they have been eliminated
        if (!candidates[i].eliminated)
        {

            //check that votes are not equal to the minimum
            if (candidates[i].votes != min)
            {
                return false;
            }
        }
    }

    //otherwise if the all the votes equal the minimum it's a tie
    return true;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{

    //loop through all the candidates
    for (int i = 0; i < candidate_count; i++)
    {

        //check if they have been eliminated
        if (!candidates[i].eliminated)
        {

            //if they have the minimum number of votes
            if (candidates[i].votes == min)
            {
                //eliminate them from the race
                candidates[i].eliminated = true;

            }
        }
    }
}

/*
Runoff!

LOG:
2/17/2021
This should be fun! Let's get started.

The first section is the vote, where I need to collect the votes
to help with this I made a quick graph so I can see what's happening

 ./runoff alice bob charlie
Number of voters: 1
Rank 1: alice
voter0: 0 0 0
voter0: 0 0 0
voter0: 0 0 0

The graph shows the voter and their preferences.

for (int i = 0; i < candidate_count; i++)
    {
        printf("voter %i: ", voter);
        for(int j = 0; j < 3; j++)
        {
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }

I moved the chart to happen after the voter gives all three
but that had a suprising amount of bugs.

 ./runoff alice bob charlie
Number of voters: 2
Rank 1: alice
Rank 2: bob
Rank 3: charlie
voter 0: 0 0 0
voter 1: 0 0 0
voter 2: 0 0 0

Actually now I fixed it, so all the votes are cast, and then
it prints the preferences:

./runoff alice bob charlie
Number of voters: 3
Rank 1: alice
Rank 2: bob
Rank 3: charlie

Rank 1: alice
Rank 2: bob
Rank 3: charlie

Rank 1: bob
Rank 2: alice
Rank 3: charlie

voter 0: 0 0 0
voter 1: 0 0 0
voter 2: 0 0 0


but now I need to actually assign the preferences in vote.

The problem is I don't know how to assign values to a double
array[i][j].

Okay the preferences are now being recorded! The key was
to use the variable voter to designate the voter and then I could
set the next variable to be their rank.


2-23-2021

Okay this took me a while to figure out, it turns out the secret
probelem I was having was not inlcudin the break on line 175.

To solve this I really had to look around and find a working solution.

I rewrote the program several times but no luck, finily I started
looking online for other sample codes and I tested a few out
finilly one worked.

Then, I reworte my code again ironing out any bugs along the way.

Then I tested and debugged each function one at a time, checking
each line to make sure I understood what it was doing.

I finially found it was the break on line 175 that was causing the problem.




*/
