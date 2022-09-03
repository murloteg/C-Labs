#include <stdio.h>
#include <string.h>

enum Statuses
{
    BAD_INPUT = 0,
    GOOD_INPUT = 1,
    IS_SUCCESS_READING = 2
};

enum Consts
{
    MAX_SIZE_OF_PERMUTATION = 10
};

enum Statuses IsGoodInput(const char* permutation)
{
    int arrayOfDigits[MAX_SIZE_OF_PERMUTATION] = {0};
    int currentLength = (int) strlen(permutation);
    for (int i = 0; i < currentLength; ++i)
    {
        if (permutation[i] < '0' || permutation[i] > '9')
        {
            return BAD_INPUT;
        }

        arrayOfDigits[permutation[i] - '0'] += 1;
    }

    for (int i = 0; i < MAX_SIZE_OF_PERMUTATION; ++i)
    {
        if (arrayOfDigits[i] > 1)
        {
            return BAD_INPUT;
        }
    }

    return GOOD_INPUT;
}

enum Statuses ReadFirstPermutation(char* permutation)
{
    for (int i = 0; i < MAX_SIZE_OF_PERMUTATION + 1; ++i)
    {
        int currentChar = getchar();
        if (currentChar == EOF)
        {
            return BAD_INPUT;
        }

        else
        {
            permutation[i] = (char) currentChar;
        }

        if (permutation[i] == ' ')
        {
            return BAD_INPUT;
        }

        else if (permutation[i] == '\n')
        {
            permutation[i] = '\0';
            break;
        }

        if (i == MAX_SIZE_OF_PERMUTATION)
        {
            return BAD_INPUT;
        }
    }

    if (IsGoodInput(permutation) == BAD_INPUT)
    {
        return BAD_INPUT;
    }

    return IS_SUCCESS_READING;
}

void Swap(char* first, char* second)
{
    char temporaryChar = *first;
    *first = *second;
    *second = temporaryChar;
}

void PrintCurrentPermutation(char* permutation)
{
    int currentLength = (int) strlen(permutation);
    for (int i = 0; i < currentLength; ++i)
    {
        printf("%c", permutation[i]);
    }
    printf("\n");
}

int FindTurnIndex(const char* permutation)
{
    int currentLength = (int) strlen(permutation);
    for (int i = currentLength - 1; i > 0; --i)
    {
        if (permutation[i - 1] < permutation[i])
        {
            return (i - 1);
        }
    }

    return 0;
}

int FindMinElemIndex(char* permutation, int turnIndex)
{
    int currentLength = (int) strlen(permutation);
    int minElem = 9;
    int minElemIndex = -1;
    for (int j = turnIndex + 1; j < currentLength; ++j)
    {
        if ((permutation[turnIndex] < permutation[j]) && ((permutation[j] - '0') <= minElem))
        {
            minElem = (int) permutation[j];
            minElemIndex = j;
        }
    }

    return minElemIndex;
}

void ReverseSuffix(char* permutation, int turnIndex)
{
    int currentLength = (int) strlen(permutation);
    for (int i = turnIndex + 1, j = currentLength - 1; i < j; ++i, --j)
    {
        Swap(&permutation[i], &permutation[j]);
    }
}

void GenerateNextPermutation(char* permutation, int numberOfPermutations)
{
    while (numberOfPermutations > 0)
    {
        int turnIndex = FindTurnIndex(permutation);
        int minElemIndex = FindMinElemIndex(permutation, turnIndex);
        if (minElemIndex == -1)
        {
            return;
        }

        Swap(&permutation[turnIndex], &permutation[minElemIndex]);
        ReverseSuffix(permutation, turnIndex);
        PrintCurrentPermutation(permutation);
        --numberOfPermutations;
    }
}

int main(void)
{
    char permutation[MAX_SIZE_OF_PERMUTATION + 1];
    enum Statuses statusOfReading = ReadFirstPermutation(permutation);
    int numberOfPermutations;
    if (scanf("%d", &numberOfPermutations) != 1)
    {
        printf("can't read number of permutation");
        return 0;
    }

    if (statusOfReading == BAD_INPUT)
    {
        printf("bad input");
        return 0;
    }

    GenerateNextPermutation(permutation, numberOfPermutations);

    return 0;
}
