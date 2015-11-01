//
//  myHashTable.hpp
//  Hash table (chain)
//
//  Created by Egor Manylov on 25.10.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#ifndef myHashTable_hpp
#define myHashTable_hpp

#include "myList.cpp"
#include <functional>

template <typename K>
size_t getHash(K key) {
    std::hash<K> simple_hash;
    return simple_hash(key);
}

template <typename K, typename T>
class HashMap {
    size_t size;
    size_t hash_count;
    List<Element<K, T>> *buckets;
public:
    
    class iterator {
        HashMap *hash;
        size_t iteration;
        typename List<Element<K, T>>::iterator list_it;
    public:
        iterator(HashMap *hash, size_t iteration, typename List<Element<K, T>>::iterator it);
        iterator& operator++();
        bool      operator!=(iterator it);
        bool      operator==(iterator it);
        Element<K, T>  operator*();
        Element<K, T>* operator->();
    };
    
     HashMap(size_t size);
    ~HashMap();
    
    iterator begin();
    iterator end();
    
    void insert(K key, T value);
    void erase(K key);
    void rehash(size_t n);
    T*   find(K key);
    size_t uniqueValue();
    size_t count();
    size_t getBucketIndex(K key);
};

#endif /* myHashTable_hpp */
