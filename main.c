#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DESC 0
#define ASC 1

#define WINDOW 2

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) (((x) < (y)) ? (x) : (y))

void printArray(int *arr, size_t n)
{
    int i;
    for (i = 0; i < n; i++)
    {
        printf("%d ", arr[i]);
    }
    printf("\n");
}

void swap(int *arr, int idx1, int idx2)
{
    int tmp = arr[idx1];
    arr[idx1] = arr[idx2];
    arr[idx2] = tmp;
}

void insertionSort(int *arr, size_t n, int asc)
{
    /*
        n uzunlugunda bir dizgeyi alip Insertion Sort algoritmasi ile siralar
        asc: ASC/DESC sekilde siralama icin. 
    */
    int i, j;
    for (i = 0; i < n - 1; i++) // Butun elemanlar icin
    {
        for (j = i + 1; j < n; j++) // i. elemandan itibaren 
        {
            if ((asc && arr[i] < arr[j]) || (!asc && arr[i] > arr[j]))
            {
                // ASC ise ilk sol eleman sag elemandan kucukse
                // veya DESC ise ve sol eleman sag elemandan buykse
                continue; // burasi sirali zaten gec
            }
            else
            {
                swap(arr, i, j); // degilse yerlerini degistir
            }
        }
    }
}

void merge_sorted(int *arr1, size_t arr1_s, int *arr2, size_t arr2_s, int *target, int asc)
{
    int i, i1, i2; // Genel sayac, ilk kismin sayaci, ikinci kismin sayaci
    size_t lim = arr1_s + arr2_s; // sonucun toplam uzunlugu
    i1 = 0;
    i2 = 0;
    for (i = 0; i < lim; i++)
    {
        if (i1 >= arr1_s || i2 >= arr2_s)
            break; // sayaclar limitlerden buyuk olursa donguyu kes
        if ((asc && (arr1[i1] < arr2[i2]) || !asc && ((arr1[i1] > arr2[i2])))) // ASC/DESC kontrolu
        {
            target[i] = arr1[i1]; // Sonuca ekle
            i1++; // Sayaci arttir
        }
        else
        {
            target[i] = arr2[i2];
            i2++;
        }
    }
    // Eger ilk dizgedeki elemanlar tukendiyse kalani ikinci dizgeden doldur
    if (i1 >= arr1_s)
    {
        for (; i2 < arr2_s; i2++)
        {
            target[i] = arr2[i2];
            i++;
        }
    }
    // Eger ikinci dizgedeki elemanlar tukendiyse kalani ilk dizgeden doldur
    if (i2 >= arr2_s)
    {
        for (; i1 < arr1_s; i1++)
        {
            target[i] = arr1[i1];
            i++;
        }
    }
}

void timsort(int *arr, int *target, int n, int asc)
{
    /*
        Kucuk alt-dizgeleri Insertion Sort ile sirala
    */
    int i = 0;
    for (; i < n; i += WINDOW)
    {
        insertionSort(&arr[i], MIN((n - i), WINDOW), asc);
    }
    /*
        Pencere araligini surekli iki kat arttirarak sirayla Merge Array uygula
    */
    int j;

    for (i = WINDOW; i < n; i *= 2) // i pencere araligi
    {
        for (j = 0; j < n; j += 2 * i)
        {
            // arr[j] sol parcanin baslangic yeri // MIN(i, n-j) sol parcanin uzunlugu
            // arr[i+j] sag parcanin baslangic yeri // MAX(0, MIN(i, n - j -i)) sag parcanin uzunlugu
            merge_sorted(&arr[j], MIN(i, n - j), &arr[i + j], MAX(0, MIN(i, n - j -i)), &target[j], asc);
        }
        // Siralama gerceklestikten sonra arr degiskenin sirali hale getir
        memcpy(arr, target, sizeof(int) * n);
    }
}

int main(int argc, char **argv)
{
    int arr[11] = {3, 15, 4, 1, -1, 5, 7, 12, 31, -4, -7};
    int *target = malloc(sizeof(int) * 11);
    timsort(arr, target, 11, ASC);
    printArray(target, 11);
    return 0;
}
