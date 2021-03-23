/*
Дан отсортированный массив целых чисел A[0..n-1] и
массив целых чисел B[0..m-1].
Для каждого элемента массива B[i] найдите минимальный индекс k
минимального элемента массива A, равного или превосходящего B[i]:
A[k] >= B[i]. Если такого элемента нет, выведите n. n, m ≤ 10000.
Требования:
Время работы поиска k для каждого элемента B[i]: O(log(k)).
Внимание! В этой задаче для каждого B[i] сначала нужно определить диапазон
для бинарного поиска размером порядка k с помощью экспоненциального поиска,
 а потом уже в нем делать бинарный поиск.
*/
#include <assert.h>

#include <iostream>
#include <sstream>

class Array {
public:
    Array();

    Array(const int size);

    Array(const Array &) = delete;

    ~Array();

    void Add(size_t pos, int num);

    void Print(std::ostream &output);

    int Size() const;

    int Get(int pos) const;

    int GetMinPos(int pos) const;

    int BinSearch(int right, int value) const;

private:
    int size;
    int *data;
};

Array::Array() : size(0), data(nullptr) {}

Array::Array(const int size) {
    assert(size >= 0);
    data = new int[size];
    this->size = size;
}

Array::~Array() { delete[] data; }

void Array::Add(size_t pos, int num) {
    assert(data != nullptr && size != 0 && pos < size);
    data[pos] = num;
}

int Array::Size() const {
    return size;
}

int Array::Get(int pos) const {
    assert(pos >= 0 && pos < size);
    return data[pos];
}

int Array::GetMinPos(int num) const {
    if (*data >= num) {
        return 0;
    }
    int ind = 1;

    while (ind < Size() && data[ind] <= num) {
        ind *= 2;
    }

    if (ind < Size()) {
        return BinSearch(ind + 1, num);
    } else {
        return BinSearch(Size(), num);
    }
}

void Array::Print(std::ostream &output) {
    assert(data != nullptr);
    for (auto i = 0; i < size; i++) {
        if (data[i] != -1) {
            output << data[i] << " ";
        }
    }
    output << std::endl;
}
// Поиск первого вхождения num в массив, или первого большего
int Array::BinSearch(int right, int num) const {
    assert(right > 0 && right <= Size());
    int left = 0;
    while (left < right) {
        int mid = (left + right) / 2;
        if (data[mid] < num) {
            left = mid + 1;
        } else {
            right = mid;
        }
    }
    // Все элементы левее left строго меньше искомого num
    // Если не удалось найти num, то вернётся размер массива, в котором ищем - n
    return left;
}

void run(std::istream &input, std::ostream &output) {
    int n = 0;
    int m = 0;
    input >> n >> m;
    if (n == 0 || m == 0) {
        output << n << " " << std::endl;
        return;
    }
    Array target(n);
    Array src(m);
    int num = 0;

    for (auto i = 0; i < n; ++i) {
        input >> num;
        target.Add(i, num);
    }

    for (auto i = 0; i < m; ++i) {
        input >> num;
        src.Add(i, num);
    }

    Array result(m);
    int pos = -1;
    for (auto i = 0; i < src.Size(); ++i) {
        pos = target.GetMinPos(src.Get(i));
        result.Add(i, pos);
    }

    result.Print(output);
}

void testArray() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "3 2 5 6 7 8 8";
        run(input, output);
        assert(output.str() == "3 3 \n");
        std::cout << output.str();
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 1 1 2 2";
        run(input, output);
        assert(output.str() == "1 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;
    {
        std::stringstream input;
        std::stringstream output;
        input << "4 3 2 4 5 7 4 6 1";
        run(input, output);
        assert(output.str() == "1 3 0 \n");

    }
    std::cout << std::endl;

    {
        std::stringstream input;
        std::stringstream output;
        input << "4 3 2 4 4 5 2 3 3";
        run(input, output);
        assert(output.str() == "0 1 1 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;

    {
        std::stringstream input;
        std::stringstream output;
        input << "6 3 10 10 13 17 28 29 29 19 5";
        run(input, output);
        assert(output.str() == "5 4 0 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;

    {
        std::stringstream input;
        std::stringstream output;
        input << "0 0";
        run(input, output);
        assert(output.str() == "0 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;

    {
        std::stringstream input;
        std::stringstream output;
        input << "1 0";
        run(input, output);
        assert(output.str() == "1 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;

    {
        std::stringstream input;
        std::stringstream output;
        input << "0 1";
        run(input, output);
        assert(output.str() == "0 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 1 1 2 3 4 5 1";
        run(input, output);
        assert(output.str() == "0 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;
    {
        std::stringstream input;
        std::stringstream output;
        input << "5 1 1 2 3 4 5 5";
        run(input, output);
        assert(output.str() == "4 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;
    {
        std::stringstream input;
        std::stringstream output;
        input << "2 3 0 1000 -1 3 1001";
        run(input, output);
        assert(output.str() == "0 1 2 \n");
        std::cout << output.str();
    }
    std::cout << std::endl;

}


int main() {
//    run(std::cin, std::cout);
    testArray();

    return 0;
}