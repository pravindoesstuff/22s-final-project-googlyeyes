//
// Created by pravin on 4/29/22.
//

#ifndef INC_22S_FINAL_PROJ_HASHMAP_H
#define INC_22S_FINAL_PROJ_HASHMAP_H

#define INITIAL_CAPACITY 2

#if !defined(INITIAL_CAPACITY) || INITIAL_CAPACITY <= 1
#error When INITIAL_CAPACITY <= 1, behavior is unpredicatable
#endif

#include <array>
#include <cstddef>
#include <functional>

// A fast implementation of a closed hash table
template<typename K, typename V>
class HashMap {
private:
    struct Pair {
        size_t hash;
        K key;
        V value;

        Pair() : hash(0) {}

        Pair(const K &key, const V &value) : key(key), value(value) {
            hash = std::hash<K>()(key);
        }
    };

    Pair *table;
    size_t table_size;

    void resize(size_t new_size) {
        Pair *new_table = new Pair[new_size];
        for (size_t i = 0; i < table_size; ++i) {
            size_t offset = 0;
            while (new_table[(table[i].hash + offset) % new_size].hash != 0) {
                ++offset;
            }
            new_table[(table[i].hash + offset) % table_size] = table[i];
        }
        delete[] table;
        table = new_table;
        table_size = new_size;
    }

public:
    size_t insertion_count;
    HashMap() : table(new Pair[INITIAL_CAPACITY]), table_size(INITIAL_CAPACITY), insertion_count(0) {}

    HashMap(const HashMap<K, V> &hash_map) {
        *this = hash_map;
    }

    HashMap(HashMap<K, V> &&hash_map) {
        *this = std::move(hash_map);
    }

    ~HashMap() {
        delete[] table;
    }

    HashMap<K, V> &operator=(const HashMap<K, V> &hash_map) {
        if (this != &hash_map) {
            delete[] this->table;
            this->table_size = hash_map.table_size;
            this->table = new Pair[this->table_size];
            this->insertion_count = hash_map.insertion_count;
            std::copy(hash_map.table, hash_map.table + hash_map.table_size, this->table);
        }
        return *this;
    }

    HashMap<K, V> &operator=(HashMap<K, V> &&hash_map) noexcept {
        if (this != &hash_map) {
            delete[] this->table;
            this->table = hash_map.table;
            this->table_size = hash_map.table_size;
            this->insertion_count = hash_map.insertion_count;
            hash_map.table = nullptr;
        }
        return *this;
    }

    void insert(const Pair &pair) {
        if (insertion_count == table_size) {
            resize(table_size * 2);
        }

        Pair table_pair;
        size_t offset = -1;
        do {
            ++offset;
            table_pair = table[(pair.hash + offset) % table_size];
        } while (table_pair.hash != 0 && table_pair.hash != pair.hash);

        table[(pair.hash + offset) % table_size] = pair;
        ++insertion_count;
    }

    V *find(const K &key) const {
        size_t starting_idx = std::hash<K>()(key) % table_size;
        for (size_t i = 0; i < table_size - 1; ++i) {
            Pair &pair = table[(i + starting_idx) % table_size];
            if (pair.key == key) {
                return &(pair.value);
            }
        }
        return nullptr;
    }

    V *begin() {
        return table;
    }

    V *end() {
        return table + table_size;
    }
};


#endif //INC_22S_FINAL_PROJ_HASHMAP_H
