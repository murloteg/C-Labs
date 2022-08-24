#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define uc unsigned char

enum Consts
{
    VOID_SYMBOL = '\0',
    MAX_SAMPLE_SIZE = 16,
    TABLE_SIZE = 256
};

uc* InputSample(FILE* in)
{
    uc* sample = (uc*) malloc(sizeof(uc) * MAX_SAMPLE_SIZE);
    assert(sample);

    for (int i = 0; i < MAX_SAMPLE_SIZE; ++i)
    {
        sample[i] = VOID_SYMBOL;
    }

    int lengthOfSample = 0;
    for (int i = 0; i < MAX_SAMPLE_SIZE; ++i)
    {
        uc currentChar = (uc) fgetc(in);
        if (currentChar == '\n')
        {
            break;
        }

        sample[i] = currentChar;
        ++lengthOfSample;
    }

    if (lengthOfSample == MAX_SAMPLE_SIZE)
    {
        uc controlCharacter = (uc) fgetc(in);
        if (controlCharacter != '\n') {}
    }

    return sample;
}

short GetLengthOfSample(const uc* sample)
{
    short length = 0;
    for (int i = 0; i < MAX_SAMPLE_SIZE; ++i)
    {
        if (sample[i] != VOID_SYMBOL)
        {
            ++length;
        }
    }

    return length;
}

short* GenerateShiftTable(const uc* sample)
{
    short* shiftTable = (short*) malloc(sizeof(short) * TABLE_SIZE);
    assert(shiftTable);

    short lengthOfSample = GetLengthOfSample(sample);
    for (int i = 0; i < TABLE_SIZE; ++i)
    {
        shiftTable[i] = lengthOfSample;
    }

    short distance = 1;
    for (int i = lengthOfSample - 2; i >= 0; --i)
    {
        if (shiftTable[sample[i]] == lengthOfSample)
        {
            shiftTable[sample[i]] = distance;
        }
        ++distance;
    }

    return shiftTable;
}

uc* CreateBuffer(FILE* in, short lengthOfSample)
{
    uc* buffer = (uc*) malloc(sizeof(uc) * lengthOfSample);
    assert(buffer);
    for (int i = 0; i < lengthOfSample; ++i)
    {
        buffer[i] = VOID_SYMBOL;
    }
    if ((int) fread(buffer, sizeof(uc), lengthOfSample, in) != lengthOfSample) {}

    return buffer;
}

void ShiftBuffer(uc* buffer, short shiftValue, int lengthOfSample, FILE* in)
{
    if (shiftValue == lengthOfSample)
    {
        int numberOfRead = (int) fread(buffer, sizeof(uc), lengthOfSample, in);
        if (numberOfRead != lengthOfSample)
        {
            for (int i = numberOfRead; i < lengthOfSample; ++i)
            {
                buffer[i] = VOID_SYMBOL;
            }
        }
        return;
    }

    int numberOfOldElements = 0;
    for (int i = 0; i < lengthOfSample - shiftValue; ++i)
    {
        buffer[i] = buffer[i + shiftValue];
        ++numberOfOldElements;
    }

    int numberOfRead = (int) fread(&buffer[numberOfOldElements], sizeof(uc), lengthOfSample - numberOfOldElements, in);
    if (numberOfRead != lengthOfSample - numberOfOldElements)
    {
        for (int i = numberOfRead; i < lengthOfSample; ++i)
        {
            buffer[i] = VOID_SYMBOL;
        }
    }
}

void Compare(const uc* sample, uc* buffer, const short* shiftTable, int* indexInText, int* currentIndex, short lengthOfSample, FILE* in)
{
    int distanceFromEnd = 0;
    while (*currentIndex >= 0)
    {
        printf("%d ", (*indexInText) - distanceFromEnd);
        if (sample[*currentIndex] == buffer[*currentIndex])
        {
            ++distanceFromEnd;
            --(*currentIndex);
        }

        else
        {
            (*indexInText) += shiftTable[buffer[lengthOfSample - 1]];
            ShiftBuffer(buffer, shiftTable[buffer[lengthOfSample - 1]], lengthOfSample, in);
            (*currentIndex) = lengthOfSample - 1;
            return;
        }
    }

    ShiftBuffer(buffer, lengthOfSample, lengthOfSample, in);
    (*indexInText) += lengthOfSample;
    (*currentIndex) = lengthOfSample - 1;
}

void BoyerMooreAlgorithm(uc* sample, uc* buffer, short* shiftTable, int* indexInText, FILE* in)
{
    short lengthOfSample = GetLengthOfSample(sample);
    int currentIndex = lengthOfSample - 1;
    while (!feof(in))
    {
        Compare(sample, buffer, shiftTable, indexInText, &currentIndex, lengthOfSample, in);
    }
}

void CleanUp(uc* sample, short* shiftTable, uc* buffer, FILE* in)
{
    free(sample);
    free(shiftTable);
    free(buffer);
    fclose(in);
}

int main(void)
{
    FILE* in = fopen("in.txt", "r");
    assert(in);

    uc* sample = InputSample(in);
    short* shiftTable = GenerateShiftTable(sample);
    uc* buffer = CreateBuffer(in, GetLengthOfSample(sample));
    int indexInText = GetLengthOfSample(sample);
    BoyerMooreAlgorithm(sample, buffer, shiftTable, &indexInText, in);

    CleanUp(sample, shiftTable, buffer, in);

    return 0;
}
