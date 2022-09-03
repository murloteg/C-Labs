#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>

enum Statuses
{
    OK = 0,
    BAD_INPUT = 1
};

enum Consts
{
    MIN_RADIX = 2,
    MAX_RADIX = 16,
    MAX_SIZE_OF_NUMBER = 14
};

int GetLengthOfIntPart(const char* input)
{
    char* pointIndex = strchr(input, '.');
    if (pointIndex != NULL)
    {
        return (int) (pointIndex - input);
    }
    
    return (int) strlen(input);
}

int GetValue(char digit)
{
    if (digit >= '0' && digit <= '9')
    {
        return digit - '0';
    }

    else if (digit >= 'a' && digit <= 'z')
    {
        return digit - 'a' + 10;
    }

    else if (digit >= 'A' && digit <= 'Z')
    {
        return digit - 'A' + 10;
    }

    else
    {
        return -1;
    }
}

long long CountIntValue(const char* input, int length, int base)
{
    long long value = 0;
    for (int i = 0; i < length; ++i)
    {
        value = value * base + GetValue(input[i]);
    }

    return value;
}

double CountFracValue(const char* input, int length, int base)
{
    double value = 0;
    for (int i = 0; i < length; ++i)
    {
        value += GetValue(input[i]) * pow(base, -i - 1);
    }

    return value;
}

void PrintIntValue(long long value, int base)
{
    char digits[] = "0123456789abcdef";
    if (value < base)
    {
        printf("%c", digits[value]);
        return;
    }
    PrintIntValue(value / base, base);
    printf("%c", digits[value % base]);
}

void PrintFracValue(double value, int base)
{
    int length = MAX_SIZE_OF_NUMBER;
    for (int i = 0; i < length; ++i)
    {
        char digits[] = "0123456789abcdef";
        double nextValue = value * base;
        int digit = (int) nextValue;
        printf("%c", digits[digit]);
        value = nextValue - digit;
    }
}

int CountPoint(const char* input, int length)
{
    int counter = 0;
    for (int i = 0; i < length; ++i)
    {
        if (input[i] == '.')
        {
            ++counter;
        }
    }
    
    return counter;
}

bool IsPermittedSymbol(char currentSymbol)
{
    return ((currentSymbol >= '0' && currentSymbol <= '9') || (currentSymbol >= 'a' && currentSymbol <= 'z') ||
    (currentSymbol >= 'A' && currentSymbol <= 'Z') || (currentSymbol == '.'));
}

bool IsGoodFracPart(const char* input, int lengthOfFracPart, int lengthOfIntPart)
{
    for (int i = lengthOfIntPart + 1; i <= lengthOfFracPart; ++i)
    {
        if (IsPermittedSymbol(input[i]) == false)
        {
            return false;
        }
    }
    
    return true;
}

enum Statuses CheckInput(int baseFrom, int baseTo, const char* input, int lengthOfFracPart, int lengthOfIntPart, int length)
{
    for (int i = 0; i < length; ++i)
    {
        if (IsPermittedSymbol(input[i]) == false)
        {
            return BAD_INPUT;
        }
    }

    if (lengthOfIntPart != length && lengthOfFracPart <= 0)
    {
        return BAD_INPUT;
    }

    else if (lengthOfIntPart <= 0)
    {
        return BAD_INPUT;
    }

    char maxChar = input[0];
    for (int i = 1; i < length; ++i)
    {
        if (input[i] > maxChar)
        {
            maxChar = input[i];
        }
    }

    int maxDigit;
    if (maxChar >= '0' && maxChar <= '9')
    {
        maxDigit = maxChar - '0';
    }

    else if (maxChar >= 'a' && maxChar <= 'z')
    {
        maxDigit = maxChar - 'a' + 10;
    }

    else
    {
        maxDigit = maxChar - 'A' + 10;
    }

    if (baseFrom < MIN_RADIX || baseFrom > MAX_RADIX || baseTo < MIN_RADIX || baseTo > MAX_RADIX)
    {
        return BAD_INPUT;
    }

    else if (maxDigit >= baseFrom)
    {
        return BAD_INPUT;
    }

    if (IsGoodFracPart(input, lengthOfFracPart, lengthOfIntPart) == false)
    {
        return BAD_INPUT;
    }

    int pointCounter = CountPoint(input, length);
    if (pointCounter > 1)
    {
        return BAD_INPUT;
    }

    return OK;
}

int main(void)
{
    char input[MAX_SIZE_OF_NUMBER];
    int baseFrom, baseTo;
    if (scanf("%d %d %13s", &baseFrom, &baseTo, input) != 3) {}

    int length = (int) strlen(input);
    int lengthOfIntPart = GetLengthOfIntPart(input);
    int lengthOfFracPart = length - lengthOfIntPart - 1;

    if (CheckInput(baseFrom, baseTo, input, lengthOfFracPart, lengthOfIntPart, length) == BAD_INPUT)
    {
        printf("bad input");
        return 0;
    }

    long long intValue = CountIntValue(input, lengthOfIntPart, baseFrom);
    PrintIntValue(intValue, baseTo);

    if (lengthOfFracPart > 0)
    {
        double fracValue = CountFracValue(input + lengthOfIntPart + 1, lengthOfFracPart, baseFrom);
        printf(".");
        PrintFracValue(fracValue, baseTo);
    }

    return 0;
}
