/*
 * 7_3. Binary MSD для long long.
Дан массив неотрицательных целых 64-разрядных чисел.
Количество чисел не больше 10^6.
Отсортировать массив методом MSD по битам (бинарный QuickSort).
 */

#include <iostream>
#include <algorithm>
#include <bits/stdc++.h>


using num_t = unsigned long long;

#define COUNT_BITS (sizeof(num_t) * CHAR_BIT)

num_t digit(num_t num, size_t digit) {
    num_t mask = 1LL << (COUNT_BITS - digit - 1);
    num_t res = num & mask;
    return res;
}

void binQuickSort(num_t *arr, num_t l, num_t r, int cur) {
    num_t i = l;
    num_t j = r;

    if (r <= l || cur >= COUNT_BITS) {
        return;
    }
    while (j != i) {
        while (!digit(arr[i], cur) && i < j) {
            i++;
        }
        while (digit(arr[j], cur) && j > i) {
            j--;
        }
        std::swap(arr[i], arr[j]);
    }
    if (digit(arr[r], cur) == 0 || j == 0) {
        j++;
    }
    binQuickSort(arr, l, j - 1, cur + 1);
    binQuickSort(arr, j, r, cur + 1);
}

void sort(num_t *arr, size_t cnt) {
    binQuickSort(arr, 0, cnt - 1, 0);
}

int main() {
    size_t n = 0;
    std::cin >> n;

    num_t *arr = new num_t[n];
    for (size_t i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    sort(arr, n);

    for (size_t i = 0; i < n; ++i) {
        std::cout << arr[i] << " ";
    }

    delete[] arr;

    return 0;
}