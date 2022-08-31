#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define uc unsigned char

enum Consts
{
    SIZE_OF_SAMPLE = 18,
    VOID_SYMBOL = '\0'
};

uc* InputSample(FILE* file)
{
    uc* sample = (uc*) malloc(sizeof(uc) * SIZE_OF_SAMPLE);
    assert(sample);

    for (int i = 0; i < SIZE_OF_SAMPLE; ++i)
    {
        sample[i] = VOID_SYMBOL;
    }

    int currentCharCode = fgetc(file);
    int index = 0;
    while (currentCharCode != '\n' && currentCharCode != EOF)
    {
        sample[index] = (uc) currentCharCode;
        currentCharCode = fgetc(file);
        ++index;
    }

    return sample;
}

uc* CreateBuffer(int lengthOfSample, int* lengthOfBuffer, FILE* in)
{
    uc* buffer = (uc*) malloc(sizeof(uc) * lengthOfSample);
    assert(buffer);
    *lengthOfBuffer = (int) fread(buffer, sizeof(char), lengthOfSample, in);
    return buffer;
}

int GetLength(const uc* sample)
{
    int length = 0;
    int index = 0;
    while (index < SIZE_OF_SAMPLE)
    {
        if (sample[index] == '\0')
        {
            break;
        }
        ++length;
        ++index;
    }

    return length;
}

int BinaryPower(int value, unsigned int degree)
{
    int result = 1;
    while (degree != 0)
    {
        if ((degree & 1) != 0)
        {
            result *= value;
        }
        value *= value;
        degree = degree >> 1;
    }

    return result;
}

int GetHashValue(const uc* text, int lengthOfSample)
{
    int hashOfSample = 0;
    for (int i = 0; i < lengthOfSample; ++i)
    {
        hashOfSample += ((text[i] % 3) * (BinaryPower(3, i)));
    }

    return hashOfSample;
}

int QuickGetHash(int previousHash, uc previousChar, const uc* buffer, int lengthOfBuffer)
{
    return ((previousHash - (previousChar % 3)) / 3) + (buffer[lengthOfBuffer - 1] % 3) * BinaryPower(3, lengthOfBuffer - 1);
}

void MoveBuffer(uc* buffer, uc nextChar, int lengthOfBuffer)
{
    for (int i = 0; i < lengthOfBuffer - 1; ++i)
    {
        buffer[i] = buffer[i + 1];
    }
    buffer[lengthOfBuffer - 1] = nextChar;
}

void PrintCurrentIndex(int index)
{
    printf("%d ", index + 1);
}

void PrintHash(int hash)
{
    printf("%d ", hash);
}

void CheckHashCollision(const uc* sample, const uc* buffer, int lengthOfSample, int bufferIndex)
{
    for (int i = 0; i < lengthOfSample; ++i)
    {
        if (sample[i] != buffer[i])
        {
            PrintCurrentIndex(bufferIndex);
            return;
        }
        PrintCurrentIndex(bufferIndex);
        ++bufferIndex;
    }
}

void RabinKarpAlgorithm(const uc* sample, const int lengthOfSample, const int hashOfSample, uc* buffer, int lengthOfBuffer, int bufferIndex, FILE* file)
{
    int hashOfBuffer = GetHashValue(buffer, lengthOfBuffer);
    if (hashOfBuffer == hashOfSample)
    {
        CheckHashCollision(sample, buffer, lengthOfSample, bufferIndex);
    }

    int currentChar = fgetc(file);
    while (currentChar != EOF)
    {
        ++bufferIndex;
        uc previousChar = buffer[0];
        MoveBuffer(buffer, (uc) currentChar, lengthOfBuffer);
        hashOfBuffer = QuickGetHash(hashOfBuffer, previousChar, buffer, lengthOfBuffer);
        if (hashOfBuffer == hashOfSample)
        {
            CheckHashCollision(sample, buffer, lengthOfSample, bufferIndex);
        }
        currentChar = fgetc(file);
    }
}

void CleanUp(uc* sample, uc* buffer, FILE* in)
{
    free(sample);
    free(buffer);
    fclose(in);
}

int main(void)
{
    FILE* in = fopen("in.txt", "r");
    assert(in);

    uc* sample = InputSample(in);
    int lengthOfSample = GetLength(sample);
    int hashOfSample = GetHashValue(sample, lengthOfSample);
    PrintHash(hashOfSample);

    int lengthOfBuffer = 0;
    uc* buffer = CreateBuffer(lengthOfSample, &lengthOfBuffer, in);
    if (lengthOfBuffer == 0)
    {
        CleanUp(sample, buffer, in);
        return 0;
    }

    int bufferIndex = 0;
    RabinKarpAlgorithm(sample, lengthOfSample, hashOfSample, buffer, lengthOfBuffer, bufferIndex, in);
    CleanUp(sample, buffer, in);

    return 0;
}
