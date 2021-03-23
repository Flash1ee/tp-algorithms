/*
Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
Найти количество таких пар индексов (i, j), что A[i] + B[j] = k.
n, m ≤ 100000.
Указание. Обходите массив B от конца к началу.
Требования:  Время работы O(n + m).
 */
#include <iostream>
#include <sstream>

void run(std::istream &in, std::ostream &out) {
    int n, m;
    in >> n;

    int *arr_a = new int[n];
    for (int i = 0; i < n; i++) {
        in >> arr_a[i];
    }
    in >> m;

    int *arr_b = new int[m];
    for (int i = 0; i < m; i++) {
        in >> arr_b[i];
    }
    int k = 0;
    in >> k;

    int left = 0;
    int right = m - 1;
    int count = 0;
    // a[i] + b[j] == k -> left++ && right--
    // a[i] + b[j] > k -> right--
    // a[i] + b[j] < k -> left++
    while (left < n && right > -1) {
        int cur = arr_a[left] + arr_b[right];
        if (cur == k) {
            ++count;
            ++left;
            --right;
        } else if (cur > k) {
            --right;
        } else {
            ++left;
        }
    }
    out << count << std::endl;
    delete[] arr_a;
    delete[] arr_b;
}

int main() {
    run(std::cin, std::cout);

    return 0;
}