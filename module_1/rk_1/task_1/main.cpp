#include <iostream>

int search_first(int *arr, int n, int elem) {
    int first = 0;
    int last = n;
    while (first < last) {
        int mid = (first + last) / 2;
        if (arr[mid] < elem) {
            last = mid;
        } else {
            first = mid + 1;
        }
    }
    return first;
}

int main() {

    int n = 0;
    std::cin >> n;
    int *arr = new int[n];
    for (int i = 0; i < n; ++i) {
        std::cin >> arr[i];
    }

    int first = *arr;
    std::cout << search_first(arr, n, first);

    return 0;

}