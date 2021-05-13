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
#include <sstream>

const int init = 8;
const double _coefFill = 3 / 4.;

struct QuadraticCollision {
    unsigned int operator()(unsigned int hash, size_t iter) const {
        return hash + iter + 1;
    }
};

struct HashGorner {
    unsigned int operator()(const std::string &str, int m = 7) const {
        unsigned int hash = 0;
        for (const auto &i : str) {
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
        unsigned int hash;

        HashElem() : status(true), hash(0) {};

        HashElem(const T &_data, unsigned int _hash) : status(true), hash(_hash) {};

        ~HashElem() = default;
    };

    std::vector<HashElem> table;
    H Hasher;
    C Probing;
    size_t size;
    size_t capacity;
    double coefFill;

    void growTable();

    bool Add(HashElem &data);
};

template<class T, class H, class C>
HashTable<T, H, C>::HashTable(const H &_hasher, const C &_probing) : capacity(init), size(0), table(init),
                                                                     Hasher(_hasher),
                                                                     Probing(_probing),
                                                                     coefFill(_coefFill) {}

template<class T, class H, class C>
HashTable<T, H, C>::~HashTable() {
    size = 0;
    capacity = 0;
}

template<class T, class H, class C>
bool HashTable<T, H, C>::Add(HashElem &data) {
    unsigned int absoluteHash = data.hash;
    unsigned int hash = absoluteHash % capacity;
    size_t iter = 0;
    while (iter < capacity) {
        if (table[hash].status && table[hash].data == data.data) {
            return false;
        }
        if (!table[hash].status) {
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
    if (size / static_cast<double>(capacity) >= coefFill) {
        growTable();
    }
    unsigned int absoluteHash = Hasher(_data);
    unsigned int hash = absoluteHash % capacity;
    size_t iter = 0;
    ssize_t first_del = -1;

    while (iter != capacity) {
        if (table[hash].status && table[hash].data == _data) {
            return false;
        }
        if (!table[hash].status && first_del == -1) {
            first_del = hash;
        }
        iter++;
        hash = Probing(hash, iter) % capacity;
    }
    if (first_del == -1) {
        table[hash].data = _data;
        table[hash].hash = absoluteHash;
        table[hash].status = true;
    } else {
        table[first_del].data = _data;
        table[first_del].hash = absoluteHash;
        table[first_del].status = true;
    }
    size++;
    return true;
}

template<class T, class H, class C>
bool HashTable<T, H, C>::Has(const T &_data) const {
    unsigned int hash = Hasher(_data) % capacity;
    size_t iter = 0;

    while (iter <= capacity) {
        if (table[hash].status && table[hash].data == _data) {
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
    size_t iter = 0;
    while (iter <= capacity) {
        if (table[hash].status && table[hash].data == _data) {
            table[hash].status = false;
            table[hash].data = "";
            table[hash].hash = 0;
            size--;
            return true;
        }
        iter++;
        hash = Probing(hash, iter) % capacity;
    }
    return false;
}

template<class T, class H, class C>
void HashTable<T, H, C>::growTable() {

    size_t old_capacity = this->capacity;
    std::vector<HashElem> new_buffer(this->capacity * 2);

    this->capacity *= 2;
    this->size = 0;
    std::swap(this->table, new_buffer);

    for (size_t i = 0; i < old_capacity; ++i) {
        if (new_buffer[i].status) {
            this->Add(new_buffer[i]);
        }
    }
}

void run(std::istream &input, std::ostream &output) {
    HashGorner Gorner;
    QuadraticCollision Collision;

    HashTable<std::string, HashGorner, QuadraticCollision> hashTb(Gorner, Collision);
    char operation = 0;
    std::string data;
    while (input >> operation >> data) {
        switch (operation) {
            case '+':
                output << (hashTb.Add(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '-':
                output << (hashTb.Delete(data) ? "OK" : "FAIL") << std::endl;
                break;
            case '?':
                output << (hashTb.Has(data) ? "OK" : "FAIL") << std::endl;
                break;
            default:
                assert(false);
        }
    }
}

void testHash() {
    {
        std::stringstream input;
        std::stringstream output;
        input << "+1 +2 -1 -2 ?1 ?2 +1 +2 -2 -1 ?1";
        run(input, output);
        assert(output.str() == "OK\nOK\nOK\nOK\nFAIL\nFAIL\nOK\nOK\nOK\nOK\nFAIL\n");
        std::cout << "SUCCESS" << std::endl;
    }
    {
        std::stringstream input;
        std::stringstream output;
        input << "+ hello\n"
                 "+ bye\n"
                 "? bye\n"
                 "+ bye\n"
                 "- bye\n"
                 "? bye\n"
                 "? hello";
        run(input, output);
        assert(output.str() == "OK\n"
                               "OK\n"
                               "OK\n"
                               "FAIL\n"
                               "OK\n"
                               "FAIL\n"
                               "OK\n");
        std::cout << "SUCCESS" << std::endl;
    }
}

int main() {
//    run(std::cin, std::cout);
    testHash();
    return 0;
}
