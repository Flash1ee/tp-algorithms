/*
Даны два строго возрастающих массива целых чисел A[0..n) и B[0..m) и число k.
Найти количество таких пар индексов (i, j), что A[i] + B[j] = k.
n, m ≤ 100000.
Указание. Обходите массив B от конца к началу.
Требования:  Время работы O(n + m).
 */
#include <iostream>
#include <assert.h>

class Array {
private:
    int *arr;
    size_t count;
    size_t size;
public:
    Array() = delete;

    Array &operator=(const Array &b) = delete;

    Array(size_t cnt) : size(cnt), count(0) {
        arr = new int[cnt];
    }

    ~Array() {
        delete[]arr;
    }

    void push(int el);

    std::size_t getCountPair(Array &B, int k);

    std::size_t getSize();

    const int &operator[](const int index) const;

    friend std::istream &operator>>(std::istream &in, int value);

};

std::size_t Array::getCountPair(Array &B, int k) {
    int left = 0;
    int right = B.getSize() - 1;
    int cnt = 0;
    while (left < getSize() && right > -1) {
        int cur = (*this)[left] + B[right];
        if (cur == k) {
            ++cnt;
            ++left;
            --right;
        } else if (cur > k) {
            --right;
        } else {
            ++left;
        }
    }
    return cnt;
}

std::size_t Array::getSize() {
    return this->size;
}

void Array::push(int el) {
    arr[count] = el;
    count++;
}

const int &Array::operator[](const int index) const {
    assert(index > -1 && index < size);
    return arr[index];
}

std::istream &operator>>(std::istream &in, Array &array) {
    int tmp = 0;
    in >> tmp;
    array.push(tmp);

    return in;
}

int main() {
    int n, m;
    std::cin >> n;
    Array arr_a(n);
    for (int i = 0; i < n; i++) {
        std::cin >> arr_a;
    }
    std::cin >> m;
    Array arr_b(m);

    for (int i = 0; i < m; i++) {
        std::cin >> arr_b;
    }
    int k = 0;
    std::cin >> k;

    std::cout << arr_a.getCountPair(arr_b, k);

    return 0;
}