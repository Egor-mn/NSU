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
    int size;
    int count;
    List<Element<K, T>> *a;
public:
    
    class iterator {
        HashMap *hash;
        int iteration;
        int depth;
    public:
        iterator(HashMap *hash, int num, int depth);
        iterator& operator++();
        bool      operator!=(iterator it);
        Element<K, T>  operator*();
        Element<K, T>* operator->();
    };
    
    iterator begin();
    iterator end();
    
     HashMap(int size);
    ~HashMap();
    
    void add(K key, T value);
    void del(K key);
    T*   find(K key);
    int  getSize();
    int  uniqueValue();
    void reHash();
};

#endif /* myHashTable_hpp */
