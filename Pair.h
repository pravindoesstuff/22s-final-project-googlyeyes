//
// Created by pravin on 4/14/22.
//

#ifndef INC_22S_FINAL_PROJ_PAIR_H
#define INC_22S_FINAL_PROJ_PAIR_H


template<typename K, typename V>
class Pair {
    K key;
    V value;

    Pair(const K &key, const V &value) {
        this->key = key;
        this->value = value;
    }

    bool operator<(const K &key) { return this->key < key; }

public:
    Pair(const K &key) { this->key = key; }
};


#endif //INC_22S_FINAL_PROJ_PAIR_H
