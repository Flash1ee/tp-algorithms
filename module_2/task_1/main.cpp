//
// Created by flashie on 25.04.2021.
//
/*
1_1. Для разрешения коллизий используйте квадратичное пробирование.
i-ая проба g(k, i)=g(k, i-1) + i (mod m). m - степень двойки.
*/

#include <iostream>
#include <vector>
#include <string>
#include <cassert>

const int init = 8;
const double _coefFill = 3 / 4.;

struct QuadraticCollision {
    unsigned int operator()(unsigned int hash, int iter) const {
        return hash + 1 / 2 * iter * (iter + 1);
    }
};

struct HashGorner {
    unsigned int operator()(const std::string &str, int m = 7) const {
        unsigned int hash = 0;
        for (const auto i : str) {
            hash = hash * m + i;
        }
        return hash;
    }


};

template<class T, class H, class C>
class HashTable {
public:
    HashTable() = delete;

    explicit HashTable(const H &_hasher, const C &_probing);

    HashTable(const HashTable &) = delete;

    HashTable &operator=(const HashTable &) = delete;

    ~HashTable();

    bool Has(const T &_data) const;

    bool Add(const T &_data);

    bool Delete(const T &_data);

private:
    struct HashElem {
        T data;
        bool status; //deleted == false

        HashElem() = delete;

        explicit HashElem(const T &_data) : data(_data), status(true) {};
    };

    std::vector<HashElem *> table;
    H Hasher;
    C Probing;
    size_t size;
    size_t capacity;
    double coefFill;

    void growTable();

    bool Add(HashElem *data);
};

template<class T, class H, class C>
HashTable<T, H, C>::HashTable(const H &_hasher, const C &_probing) : capacity(init), size(0), table(init),
                                                                     Hasher(_hasher),
                                                                     Probing(_probing),
                                                                     coefFill(_coefFill) {}

template<class T, class H, class C>
HashTable<T, H, C>::~HashTable() {
    for (auto *i : table) {
        delete i;
    }
    size = 0;
    capacity = 0;
}

template<class T, class H, class C>
bool HashTable<T, H, C>::Add(HashElem *data) {
    if (!data) {
        return false;
    }
    unsigned int hash = Hasher(data->data) % capacity;
    int iter = 0;
    while (table[hash] && iter != capacity) {
        if (table[hash]->status && table[hash]->data == data->data) {
            return false;
        }
        if (!table[hash]->status) {
            assert(false);
        }
        iter++;
        hash = Probing(hash, iter) % capacity;
    }
    table[hash] = data;
    size++;
    return true;


}

template<class T, class H, class C>
bool HashTable<T, H, C>::Add(const T &_data) {
    if (size / capacity >= coefFill) {
        growTable();
    }
    unsigned int hash = Hasher(_data) % capacity;
    int iter = 0;
    int first_del = -1;

    while (table[hash] && iter != capacity) {
        if (table[hash]->status && table[hash]->data == _data) {
            return false;
        }
        if (!table[hash]->status && first_del == -1) {
            first_del = hash;
        }
        iter++;
        hash = Probing(hash, iter) % capacity;
    }
    if (first_del == -1) {
        table[hash] = new HashElem(_data);
    } else {
        table[first_del]->data = _data;
        table[first_del]->status = true;
    }
    size++;
    return true;
}

template<class T, class H, class C>
bool HashTable<T, H, C>::Has(const T &_data) const {
    unsigned int hash = Hasher(_data) % capacity;
    int iter = 0;

    while (table[hash] && iter != capacity) {
        if (table[hash]->status && table[hash]->data == _data) {
            return true;
        }
        iter++;
        hash = Probing(hash, iter) % capacity;
    }
    return false;
}

template<class T, class H, class C>
bool HashTable<T, H, C>::Delete(const T &_data) {
    unsigned int hash = Hasher(_data) % capacity;
    int iter = 0;
    while (table[hash] && iter != capacity) {
        if (table[hash]->status && table[hash]->data == _data) {
            table[hash]->status = false;
            size--;
            return true;
        }
        iter++;
        hash = Probing(hash, iter);
    }
    return false;
}

template<class T, class H, class C>
void HashTable<T, H, C>::growTable() {

    size_t old_capacity = this->capacity;
    std::vector<HashElem *> new_buffer(this->capacity * 2);

    this->capacity *= 2;
    this->size = 0;
    std::swap(this->table, new_buffer);

    for (size_t i = 0; i < old_capacity; ++i) {
        if (new_buffer[i] && new_buffer[i]->status) {
            this->Add(new_buffer[i]);
        }
    }
}

int main() {
    HashGorner Gorner;
    QuadraticCollision Collision;

    HashTable<std::string, HashGorner, QuadraticCollision> hashTb(Gorner, Collision);
    char operation = 0;
    std::string data;
    while (std::cin >> operation >> data) {
        switch (operation) {
            case '+':
                std::cout << (hashTb.Add(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                std::cout << (hashTb.Delete(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                std::cout << (hashTb.Has(data) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);
        }
    }

    return 0;
}