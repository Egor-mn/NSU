//
//  myHashTable.cpp
//  Hash table (chain)
//
//  Created by Egor Manylov on 25.10.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#include "myHashTable.hpp"

//implementation of iterator class

template <typename K, typename T>
HashMap<K, T>::iterator::iterator(HashMap *hash, int num):
        hash(hash), iteration(num) { }

template <typename K, typename T>
typename HashMap<K, T>::iterator& HashMap<K, T>::iterator::operator++() {
    iteration++;
    return *this;
}

template <typename K, typename T>
bool HashMap<K, T>::iterator::operator!=(iterator it) {
    return iteration != it.iteration;
}

template <typename K, typename T>
T HashMap<K, T>::iterator::operator*() {
    return hash->find(hash->keys[iteration]);
}

template <typename K, typename T>
T* HashMap<K, T>::iterator::operator->() {
    return hash->find(hash->keys[iteration]);
}

//implementation of HashMap class

template <typename K, typename T>
HashMap<K, T>::HashMap(int size): count(0), size(size) {
    a = new List<Element<K, T>>[size];
}

template <typename K, typename T>
HashMap<K, T>::~HashMap() {
    delete[] a;
}

template <typename K, typename T>
typename HashMap<K, T>::iterator HashMap<K, T>::begin() {
    return new iterator(this, 0);
}

template <typename K, typename T>
typename HashMap<K, T>::iterator HashMap<K, T>::end() {
    return new iterator(count);
}

template <typename K, typename T>
void HashMap<K, T>::add(K key, T value) {
    int x = getHash(key) % size;
    for (typename List<Element<K, T>>::iterator it = a[x].begin(); it != a[x].end(); ++it)
        if (it->key == key)
            return a[x].update(it.index(), new Element<K, T>(key, value));
        
    a[x].push_back(new Element<K, T>(key, value));
    count++;
}

template <typename K, typename T>
T* HashMap<K, T>::find(K key) {
    int x = getHash(key) % size;
    
    for (typename List<Element<K, T>>::iterator it = a[x].begin(); it != a[x].end(); ++it)
        if (it->key == key)
            return &it->value;
    
    return NULL;
}

template <typename K, typename T>
void HashMap<K, T>::del(K key) {
    int x = getHash(key) % size;
    
    for (typename List<Element<K, T>>::iterator it = a[x].begin(); it != a[x].end(); ++it)
        if (it->key == key) {
            count--;
            return a[x].remove(it.index());
        }
}

template <typename K, typename T>
int HashMap<K, T>::getSize() {
    return count;
}

template <typename K, typename T>
int HashMap<K, T>::uniqueValue() {
    HashMap<T, int> hash_v(size);
    
    for (int i = 0; i < size; i++)
        if (!a[i].empty())
            for (int j = 0; a[i].get(j); j++)
                hash_v.add(a[i].get(j)->value, 0);
    
    return hash_v.getSize();
}

template <typename K, typename T>
void HashMap<K, T>::reHash() {
    if (count > 0.6 * size) {
        HashMap<K, T> *n_hash = new HashMap<K, T>(size * 2);
        for (int i = 0; i < size; i++) {
            if (!a[i].empty())
                for (int j = 0; a[i].get(j); j++)
                    n_hash->add(a[i].get(j)->key, a[i].get(j)->value);
            
        }
        
        List<Element<K, T>> *b = a;
        this->a = n_hash->a;
        this->size = n_hash->size;
        n_hash->a = b;
        delete n_hash;
    }
}