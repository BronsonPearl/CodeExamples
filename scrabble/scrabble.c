#include <ctype.h>
#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

int compute_score(string word);

int main(void)
{
    // Get input words from both players
    string word1 = get_string("Player 1: ");
    string word2 = get_string("Player 2: ");

    // Score both words
    int score1 = compute_score(word1);
    int score2 = compute_score(word2);

    //Print the winner
    if (score1 > score2)
    {
        printf("Player 1 wins!\n");
    }

    if (score2 > score1)
    {
        printf("Player 2 wins!\n");
    }

    if (score1 == score2)
    {
        printf("Tie!\n");
    }
}

int compute_score(string word)
{

    // Points assigned to each letter of the alphabet
    int score = 0;
    int POINTS[] = {1, 3, 3, 2, 1, 4, 2, 4, 1, 8, 5, 1, 3, 1, 1, 3, 10, 1, 1, 1, 1, 4, 4, 8, 4, 10};
    char alpha[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

    // Computes and return score for string

    for (int i = 0; i < strlen(word); i++)
    {
        for (int j = 0; j < 26; j++)
        {
            //converts from uppcase to lowercase letter
            if (isupper(word[i]))
            {
                word[i] = word[i] + 32;
            }

            // assigns points if current letter in string is equal to a letter in the alphabet
            if (word[i] == alpha[j])
            {
                score = score + POINTS[j];
            }
            else
            {
                score = score + 0;
            }
        }
    }
    return score;
}