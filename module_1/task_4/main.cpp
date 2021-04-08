/*
Дан массив натуральных чисел A[0..n), n не превосходит 10^8.
Так же задан размер некоторого окна (последовательно расположенных элементов
массива) в этом массиве k, k<=n. Требуется для каждого положения окна (от 0 и до
n-k) вывести значение максимума в окне. Требования: Скорость работы O(n log n),
память O(n). Формат входных данных: Вначале вводится n - количество элементов
массива. Затем вводится n строк со значением каждого элемента. Затем вводится k
- размер окна. Формат выходных данных: Разделенные пробелом значения максимумов
для каждого положения окна.
 */
#include <cassert>
#include <iostream>
#include <sstream>

#define INIT_SIZE 2

struct Elem {
  Elem() = default;

  Elem(int elem, int pos) : elem(elem), pos(pos) {}

  int elem;
  int pos;

  bool operator>(const Elem &right) { return elem > right.elem; }

  bool operator<(const Elem &right) { return elem < right.elem; }
  friend std::ostream &operator<<(std::ostream &out, const Elem &elem);
};

std::ostream &operator<<(std::ostream &out, const Elem &elem) {
  out << elem.elem;
  return out;
}

template <class T> class Array {
public:
  Array()
      : buffer(nullptr), bufSize(0), realSize(0), defaultInitSize(INIT_SIZE) {}

  ~Array() { delete[] buffer; }

  explicit Array(int size);

  void Add(const T &elem);

  bool IsEmpty();

  T &GetAt(int index) const;

  T &GetRef(int index);

  int Size() const { return realSize; }

  T &Last();

  void DeleteLast();

  T &operator[](int index) const { return GetAt(index); }

  T &operator[](int index) { return GetRef(index); }

private:
  T *buffer;
  int bufSize;
  int realSize;
  int defaultInitSize;

  void grow();
};

template <class T>
Array<T>::Array(int size)
    : bufSize(size), realSize(0), defaultInitSize(INIT_SIZE) {
  buffer = new T[size];
}

template <class T> bool Array<T>::IsEmpty() { return realSize == 0; }

template <class T> void Array<T>::Add(const T &elem) {
  if (bufSize == realSize) {
    grow();
  }
  assert(realSize < bufSize && buffer != nullptr);
  buffer[realSize++] = elem;
}

template <class T> T &Array<T>::GetAt(int index) const {
  assert(index >= 0 && index < realSize && buffer != nullptr);

  return buffer[index];
}

template <class T> T &Array<T>::GetRef(int index) {
  assert(index >= 0 && index < realSize && buffer != nullptr);
  return buffer[index];
}

template <class T> void Array<T>::grow() {
  int newBufferSize = std::max(bufSize * 2, defaultInitSize);
  T *newBuf = new T[newBufferSize];
  for (int i = 0; i < realSize; ++i) {
    newBuf[i] = buffer[i];
  }
  delete[] buffer;

  buffer = newBuf;
  bufSize = newBufferSize;
}

template <class T> T &Array<T>::Last() {
  assert(!IsEmpty());

  return buffer[realSize - 1];
}

template <class T> void Array<T>::DeleteLast() {
  assert(!IsEmpty());

  realSize--;
}

// Двоичная куча
template <class T> class Heap {
public:
  explicit Heap(int size, int (*cmp)(const T &a, const T &b));

  // Добавить элемент в кучу за O(log n)
  void Insert(T &elem);

  // Извлечь максимум из кучи за О(log n)
  T &ExtractMax();

  void Fill(const Array<T> &arr, int size);

  // Посмотреть значение максимума в куче за О(1)
  T &PeekMax();

private:
  Array<T> arr; // динамический массив для хранения эл-в кучи

  int (*cmp)(const T &a, const T &b);

  void buildHeap();

  // Спустить эл-т, который меньше дочерних
  void siftDown(int ind);

  // Поднять эл-т, который больше дочерних
  void siftUp(int ind);
};

void searchMaxInWindow(std::ostream &out, const Array<Elem> &arr,
                       int window_size,
                       int (*Cmp)(const Elem &a, const Elem &b));

template <class T> void Heap<T>::siftDown(int ind) {
  int left = 2 * ind + 1;
  int right = 2 * ind + 2;

  // Ищем большего сына, если такой есть.
  int largest = ind;
  if (left < arr.Size() &&
      cmp(arr[left], arr[largest]) > 0 /*arr[left] > arr[ind]*/) {
    largest = left;
  }
  if (right < arr.Size() &&
      cmp(arr[right], arr[largest]) > 0 /*arr[right] > arr[largest]*/) {
    largest = right;
  }
  if (largest != ind) {
    std::swap(arr[ind], arr[largest]);
    siftDown(largest);
  }
}

template <class T> void Heap<T>::buildHeap() {
  for (int i = arr.Size() / 2 - 1; i >= 0; --i) {
    siftDown(i);
  }
}

template <class T> void Heap<T>::siftUp(int ind) {
  while (ind > 0) {
    int parent = (ind - 1) / 2;
    if (cmp(arr[ind], arr[parent]) <= 0 /*arr[ind] <= arr[parent]*/) {
      return;
    }
    std::swap(arr[ind], arr[parent]);
    ind = parent;
  }
}

template <class T> void Heap<T>::Insert(T &elem) {
  arr.Add(elem);
  siftUp(arr.Size() - 1);
}

template <class T> T &Heap<T>::ExtractMax() {
  assert(!arr.IsEmpty());
  // Запоминаем значения корня.
  //    int result = arr[0].elem;
  T &result = arr[0];
  // Переносим последний элемент в корень.
  arr[0] = arr.Last();
  arr.DeleteLast();
  // Вызываем SiftDown для корня.
  if (!arr.IsEmpty()) {
    siftDown(0);
  }
  return result;
}

template <class T> T &Heap<T>::PeekMax() {
  assert(!(arr.IsEmpty()));

  T &elem = arr[0];

  return elem;
}

template <class T> void Heap<T>::Fill(const Array<T> &arr, int size) {
  for (int i = 0; i < size; ++i) {
    this->arr.Add(arr.GetAt(i));
  }
  buildHeap();
}

template <class T>
Heap<T>::Heap(int size, int (*cmp)(const T &a, const T &b))
    : arr(size), cmp(cmp) {}

int Cmp(const Elem &A, const Elem &B) { return A.elem - B.elem; }

void searchMaxInWindow(std::ostream &out, const Array<Elem> &arr,
                       int window_size,
                       int (*Cmp)(const Elem &a, const Elem &b)) {
  int n = arr.Size();
  Heap<Elem> heap(n, Cmp);
  heap.Fill(arr, window_size);
  out << heap.PeekMax().elem << " ";

  int left = 1;
  int right = window_size - 1 + left;
  while (right < n) {
    heap.Insert(arr.GetAt(right));
    //        while (Cmp(heap, left)) {
    while (heap.PeekMax().pos < left) {
      heap.ExtractMax();
    }
    out << heap.PeekMax() << " ";
    left++;
    right++;
  }
  out << std::endl;
}

void run(std::istream &in, std::ostream &out) {
  int n = 0;
  in >> n;
  assert(n >= 0);

  Array<Elem> arr(n);
  int tmp = 0;
  for (int i = 0; i < n; ++i) {
    in >> tmp;
    arr.Add(Elem(tmp, i));
  }

  int window_size = 0;
  in >> window_size;
  assert(window_size >= 0);

  searchMaxInWindow(out, arr, window_size, Cmp);
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
  {
    std::stringstream in;
    std::stringstream out;
    in << "1 1 1 1";
    run(in, out);
    assert(out.str() == "1 \n");
    std::cout << "SUCCESS" << std::endl;
  }
  {
    std::stringstream in;
    std::stringstream out;
    in << "3 1 2 3 3";
    run(in, out);
    assert(out.str() == "3 \n");
    std::cout << "SUCCESS" << std::endl;
  }
}

int main() {
  testHeap();
  //    run(std::cin, std::cout);

  return EXIT_SUCCESS;
}
