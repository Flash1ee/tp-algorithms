/*
Дан массив натуральных чисел A[0..n), n не превосходит 10^8.
Так же задан размер некоторого окна (последовательно расположенных элементов массива) в этом массиве k, k<=n.
Требуется для каждого положения окна (от 0 и до n-k) вывести значение максимума в окне.
Требования: Скорость работы O(n log n), память O(n).
Формат входных данных:
Вначале вводится n - количество элементов массива. Затем вводится n строк со значением каждого элемента.
Затем вводится k  - размер окна.
Формат выходных данных:
Разделенные пробелом значения максимумов для каждого положения окна.
 */
#include <iostream>
#include <cassert>
#include <sstream>

#define INIT_SIZE 2

struct Elem {
    Elem() = default;

    Elem(int elem, int pos) : elem(elem), pos(pos) {}

    int elem;
    int pos;
};

class Array {
public:
    Array() : buffer(nullptr), bufSize(0), realSize(0), defaultInitSize(INIT_SIZE) {}

    ~Array() { delete[] buffer; }

    explicit Array(int size);

    void Add(Elem elem);

    bool IsEmpty();

    Elem GetAt(int index) const;

    Elem &GetRef(int index);

    int Size() const { return realSize; }

    Elem Last();

    void DeleteLast();

    Elem operator[](int index) const { return GetAt(index); }

    Elem &operator[](int index) { return GetRef(index); }

private:
    Elem *buffer;
    int bufSize;
    int realSize;
    int defaultInitSize;

    void grow();
};

Array::Array(int size) : bufSize(size), realSize(0), defaultInitSize(INIT_SIZE) {
    buffer = new Elem[size];
}

bool Array::IsEmpty() {
    return realSize == 0;
}

void Array::Add(Elem elem) {
    if (bufSize == realSize) {
        grow();
    }
    assert(realSize < bufSize && buffer != nullptr);
    buffer[realSize++] = elem;
}

Elem Array::GetAt(int index) const {
    assert(index >= 0 && index < realSize && buffer != nullptr);

    return buffer[index];
}

Elem &Array::GetRef(int index) {
    assert(index >= 0 && index < realSize && buffer != nullptr);
    return buffer[index];
}

void Array::grow() {
    int newBufferSize = std::max(bufSize * 2, defaultInitSize);
    Elem *newBuf = new Elem[newBufferSize];
    for (int i = 0; i < realSize; ++i) {
        newBuf[i] = buffer[i];
    }
    delete[] buffer;

    buffer = newBuf;
    bufSize = newBufferSize;
}

Elem Array::Last() {
    assert(!IsEmpty());

    return buffer[realSize - 1];
}

void Array::DeleteLast() {
    assert(!IsEmpty());

    realSize--;
}


// Двоичная куча
class Heap {
public:
    explicit Heap(int size);

    // Добавить элемент в кучу за O(log n)
    void Insert(Elem elem);

    // Извлечь максимум из кучи за О(log n)
    int ExtractMax();

    void Fill(const Array &arr, int size);

    // Посмотреть значение максимума в куче за О(1)
    Elem PeekMax();

private:
    Array arr; // динамический массив для хранения эл-в кучи

    void buildHeap();

    // Спустить эл-т, который меньше дочерних
    void siftDown(int ind);

    // Поднять эл-т, который больше дочерних
    void siftUp(int ind);

};

void searchMaxInWindow(std::ostream &out, const Array &arr, int window_size);

void Heap::siftDown(int ind) {
    int left = 2 * ind + 1;
    int right = 2 * ind + 2;

    // Ищем большего сына, если такой есть.
    int largest = ind;
    if (left < arr.Size() && arr[left].elem > arr[ind].elem) {
        largest = left;
    }
    if (right < arr.Size() && arr[right].elem > arr[largest].elem) {
        largest = right;
    }
    if (largest != ind) {
        std::swap(arr[ind], arr[largest]);
        siftDown(largest);
    }
}

void Heap::buildHeap() {
    for (int i = arr.Size() / 2 - 1; i >= 0; --i) {
        siftDown(i);
    }
}

void Heap::siftUp(int ind) {
    while (ind > 0) {
        int parent = (ind - 1) / 2;
        if (arr[ind].elem <= arr[parent].elem) {
            return;
        }
        std::swap(arr[ind], arr[parent]);
        ind = parent;
    }
}

void Heap::Insert(Elem elem) {
    arr.Add(elem);
    siftUp(arr.Size() - 1);
}

int Heap::ExtractMax() {
    assert(!arr.IsEmpty());
    // Запоминаем значения корня.
    int result = arr[0].elem;
    // Переносим последний элемент в корень.
    arr[0] = arr.Last();
    arr.DeleteLast();
    // Вызываем SiftDown для корня.
    if (!arr.IsEmpty()) {
        siftDown(0);
    }
    return result;
}

Elem Heap::PeekMax() {
    assert(!(arr.IsEmpty()));

    Elem elem = arr[0];

    return elem;
}

void Heap::Fill(const Array &arr, int size) {
    for (int i = 0; i < size; ++i) {
        this->arr.Add(arr.GetAt(i));
    }
    buildHeap();
}

Heap::Heap(int size) :arr(size) {}

void searchMaxInWindow(std::ostream &out, const Array &arr, int window_size) {
    int n = arr.Size();
    Heap heap(n);
    heap.Fill(arr, window_size);
    out << heap.PeekMax().elem << " ";

    int left = 1;
    int right = window_size - 1 + left;
    while (right < n) {
        heap.Insert(arr.GetAt(right));
        while (heap.PeekMax().pos < left) {
            heap.ExtractMax();
        }
        out << heap.PeekMax().elem << " ";
        left++;
        right++;
    }
    out << std::endl;
}

void run(std::istream &in, std::ostream &out) {
    int n = 0;
    in >> n;
    assert(n >= 0);

    Array arr(n);
    int tmp = 0;
    for (int i = 0; i < n; ++i) {
        in >> tmp;
        arr.Add(Elem(tmp, i));
    }

    int window_size = 0;
    in >> window_size;
    assert(window_size >= 0);

    searchMaxInWindow(out, arr, window_size);
}

void testHeap() {
    {
        std::stringstream in;
        std::stringstream out;
        in << "3 1 2 3 2";
        run(in, out);
        assert(out.str() == "2 3 \n");
        std::cout << "SUCCESS" << std::endl;
    }
    {
        std::stringstream in;
        std::stringstream out;
        in << "9 0 7 3 8 4 5 10 4 6 4";
        run(in, out);
        assert(out.str() == "8 8 8 10 10 10 \n");
        std::cout << "SUCCESS" << std::endl;

    }

}

int main() {
//    testHeap();
    run(std::cin, std::cout);

    return EXIT_SUCCESS;
}