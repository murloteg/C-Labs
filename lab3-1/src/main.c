#include <stdio.h>
#include <stdlib.h>
#include <assert.h>

void PrintAboutError(void)
{
    printf("something went wrong");
}

int* GetArray(int numberOfElements)
{
    int* arrayWithNumbers = (int*) malloc(sizeof(int) * numberOfElements);
    assert(arrayWithNumbers);
    for (int i = 0; i < numberOfElements; ++i)
    {
        if (scanf("%d", &arrayWithNumbers[i]) != 1)
        {
            PrintAboutError();
        }
    }

    return arrayWithNumbers;
}

void Swap(int* first, int* second)
{
    int temporaryValue = *first;
    *first = *second;
    *second = temporaryValue;
}

int QuickSortUnit(int* arrayWithNumbers, int leftIndex, int rightIndex)
{
    int middleIndex = (leftIndex + rightIndex) / 2;
    int middleValue = arrayWithNumbers[middleIndex];
    while (leftIndex <= rightIndex)
    {
        while (arrayWithNumbers[leftIndex] < middleValue)
        {
            ++leftIndex;
        }

        while (middleValue < arrayWithNumbers[rightIndex])
        {
            --rightIndex;
        }

        if (leftIndex <= rightIndex)
        {
            Swap(&arrayWithNumbers[leftIndex], &arrayWithNumbers[rightIndex]);
            ++leftIndex;
            --rightIndex;
        }
    }

    return leftIndex;
}

void QuickSort(int* arrayWithNumbers, int leftIndex, int rightIndex)
{
    if (leftIndex < rightIndex)
    {
        int pivotIndex = QuickSortUnit(arrayWithNumbers, leftIndex, rightIndex);
        QuickSort(arrayWithNumbers, leftIndex, pivotIndex - 1);
        QuickSort(arrayWithNumbers, pivotIndex, rightIndex);
    }
}

void PrintArray(int* arrayWithNumbers, int numberOfElements)
{
    for (int i = 0; i < numberOfElements; ++i)
    {
        printf("%d ", arrayWithNumbers[i]);
    }
    printf("\n");
}

void CleanUp(int* arrayWithNumbers)
{
    free(arrayWithNumbers);
}

int main(void)
{
    int numberOfElements;
    if (scanf("%d", &numberOfElements) != 1)
    {
        PrintAboutError();
        return 0;
    }

    int* arrayWithNumbers = GetArray(numberOfElements);
    QuickSort(arrayWithNumbers, 0, numberOfElements - 1);
    PrintArray(arrayWithNumbers, numberOfElements);
    CleanUp(arrayWithNumbers);

    return 0;
}
