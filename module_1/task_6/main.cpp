/*
Даны неотрицательные целые числа n,k и массив целых чисел из [0..10^9] размера n.
Требуется найти k-ю порядковую статистику. т.е. напечатать число,
которое бы стояло на позиции с индексом k (0..n-1) в отсортированном массиве. 

6_2. Реализуйте стратегию выбора опорного элемента “медиана трёх”.
Функцию Partition реализуйте методом прохода
двумя итераторами от конца массива к началу.
*/

#include <iostream>
#include <sstream>
#include <assert.h>

template <class T>
class CmpDefault {
    public:
    int operator()(const T& l, const T& r) const {return l - r;}
};

template<class T, class Compare = CmpDefault<T>>
size_t getMedian(T* arr, int n, const Compare& cmp = CmpDefault<T>()) {
    assert(arr != nullptr);
    size_t l_i = 0; // a
    size_t mid_i = n / 2; // b
    size_t r_i = n - 1;  // c

    // cmp - isLess
    if (cmp(arr[l_i], arr[mid_i]) > 0) {
        if (cmp(arr[r_i], arr[l_i]) > 0) {
            return l_i;
        }
        return (cmp(arr[mid_i], arr[r_i]) > 0) ? mid_i : r_i;
    }
    if (cmp(arr[r_i], arr[mid_i] > 0)) {
        return mid_i;
    }
    return (cmp(arr[l_i], arr[r_i] > 0)) ? l_i : r_i;

}
template <class T, class Compare = CmpDefault<T>>
int Partition(T* arr, int n, const Compare& cmp = CmpDefault<T>()) {
    if (n <= 1) {
        return 0;
    }
    const size_t pivot_ind = getMedian(arr, n, cmp);
    std::swap(arr[n-1], arr[pivot_ind]);
    T pivot = arr[n-1];
    int i = n - 2;
    int j = n - 2;
    while (j >= 0) {
        if (cmp(arr[j], pivot) <= 0) {
            j--;
        } else {
            std::swap(arr[j--], arr[i--]);
        }
    }
    std::swap(arr[i + 1], arr[n - 1]);
    i++;

    return i;

}
void print_arr(int *arr, int n) {
    for (int i = 0; i < n; i++) {
        std::cout << arr[i] << " ";
    }
    std::cout << std::endl;
}
template<class T, class Compare = CmpDefault<T>>
T FindStats(T* arr, int n, int k, const Compare& cmp = CmpDefault<T>()) {
    int part = Partition(arr, n, cmp);
    while (part != k) {
        if (part > k) {
            n = part;
        } else {
            arr = arr + part + 1;
            n = n - part - 1;
            k = k - part - 1;
        }
        part = Partition(arr, n, cmp);
    }
    return arr[part];
}
void run(std::istream &in, std::ostream &out) {
    size_t n = 0;
    size_t k = 0;
    in >> n >> k;
    assert(n >= 0 && k >= 0);
    if (n == 0) {
        out << "";
        return;
    }
    assert(k < n);
    int *arr = new int[n];
    for (int i = 0; i < n; ++i) {
        in >> arr[i];
    }
    out << FindStats(arr, n, k);
    
    delete []arr;

}
void testFindStat() {
    {
        std::stringstream in;
        std::stringstream out;
        in << "10 4 1 2 3 4 5 6 7 8 9 10";
        run(in, out);
        assert(out.str() == "5");
        std::cout << "OK" << std::endl;
    }
     {
        std::stringstream in;
        std::stringstream out;
        in << "10 0 3 6 5 7 2 9 8 10 4 1";
        run(in, out);
        assert(out.str() == "1");
        std::cout << "OK" << std::endl;
    }
     {
        std::stringstream in;
        std::stringstream out;
        in << "10 9 0 0 0 0 0 0 0 0 0 1";
        run(in, out);
        assert(out.str() == "1");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "5 4 9 8 7 6 5";
        run(in, out);
        assert(out.str() == "9");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "5 4 5 6 7 8 9";
        run(in, out);
        assert(out.str() == "9");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "1 0 -1";
        run(in, out);
        assert(out.str() == "-1");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "10 0 1 1 1 1 1 1 0 1 1 1";
        run(in, out);
        assert(out.str() == "0");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "5 0 1 1 1 1 -1";
        run(in, out);
        assert(out.str() == "-1");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "2 1 2 -10";
        run(in, out);
        assert(out.str() == "2");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "0 0";
        run(in, out);
        assert(out.str() == "");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "5 4 1 1 1 1 1";
        run(in, out);
        assert(out.str() == "1");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "5 0 10 -31 10 -10 -10";
        run(in, out);
        assert(out.str() == "-31");
        std::cout << "OK" << std::endl;
    }
     {
        std::stringstream in;
        std::stringstream out;
        in << "5 0 -31 -31 -31 -31 -31";
        run(in, out);
        assert(out.str() == "-31");
        std::cout << "OK" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "1 0 -31";
        run(in, out);
        assert(out.str() == "-31");
        std::cout << "OK" << std::endl;
    }
}
int main() {
    run(std::cin, std::cout);
    // testFindStat();
    return 0;

}
