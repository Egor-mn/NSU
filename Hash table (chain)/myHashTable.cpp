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
HashMap<K, T>::iterator::iterator(HashMap *hash, size_t iteration, typename List<Element<K, T>>::iterator it):
        hash(hash), iteration(iteration), list_it(it) { }

template <typename K, typename T>
typename HashMap<K, T>::iterator& HashMap<K, T>::iterator::operator++() {
    HashMap<K, T>::iterator it = hash->end();
    
    if (*this == it) return *this;
    else if ((++list_it, *this == it) || (list_it != hash->buckets[iteration].end())) return *this;
    else {
        for (int i = 0; i < hash->size; i++) {
            iteration++;
            list_it = hash->buckets[iteration].begin();
            if (list_it != hash->buckets[iteration].end())
                return *this;
        }
    }
    return *this;
}

template <typename K, typename T>
bool HashMap<K, T>::iterator::operator!=(iterator it) {
    return list_it != it.list_it;
}

template <typename K, typename T>
bool HashMap<K, T>::iterator::operator==(iterator it) {
    return list_it == it.list_it;
}

template <typename K, typename T>
Element<K, T> HashMap<K, T>::iterator::operator*() {
    return *list_it;
}

template <typename K, typename T>
Element<K, T>* HashMap<K, T>::iterator::operator->() {
    return list_it.operator->();
}

//implementation of HashMap class

template <typename K, typename T>
HashMap<K, T>::HashMap(size_t size): hash_count(0), size(size) {
    buckets = new List<Element<K, T>>[size];
}

template <typename K, typename T>
HashMap<K, T>::~HashMap() {
    delete[] buckets;
}

template <typename K, typename T>
typename HashMap<K, T>::iterator HashMap<K, T>::begin() {
    typename List<Element<K, T>>::iterator new_list_iter = buckets[0].begin();
    typename HashMap<K, T>::iterator new_iter = *new iterator(this, 0, new_list_iter);
    for (size_t i = 0; i < size; i++)
        if(!buckets[i].empty()){
            new_list_iter = buckets[i].begin();
            new_iter = *new iterator(this, i, new_list_iter);
            return new_iter;
        }
    
    return new_iter;
}

template <typename K, typename T>
typename HashMap<K, T>::iterator HashMap<K, T>::end() {
    typename List<Element<K, T>>::iterator new_list_iter = buckets[0].end();
    typename HashMap<K, T>::iterator new_iter = *new iterator(this, 0, new_list_iter);
    size_t i = size - 1;
    while (i--)
        if (!buckets[i].empty()) {
            new_list_iter = buckets[i].end();
            new_iter = *new iterator(this, i, new_list_iter);
            return new_iter;
        }

    return new_iter;
}

template <typename K, typename T>
void HashMap<K, T>::insert(K key, T value) {
    if (size < 0.5 * hash_count)
        rehash(2 * size);
    size_t x = getBucketIndex(key);
    for (typename List<Element<K, T>>::iterator it = buckets[x].begin(); it != buckets[x].end(); ++it)
        if (it->key == key) {
            it->value = value;
            return;
        }
    
    buckets[x].push_back(new Element<K, T>(key, value));
    hash_count++;
    return;
}

template <typename K, typename T>
T* HashMap<K, T>::find(K key) {
    size_t x = getBucketIndex(key);
    
    for (typename List<Element<K, T>>::iterator it = buckets[x].begin(); it != buckets[x].end(); ++it)
        if (it->key == key)
            return &it->value;
    
    return nullptr;
}

template <typename K, typename T>
void HashMap<K, T>::erase(K key) {
    size_t x = getBucketIndex(key);
    
    int i = 0;    
    for (typename List<Element<K, T>>::iterator it = buckets[x].begin(); it != buckets[x].end(); ++it, i++)
        if (it->key == key) {
            hash_count--;
            buckets[x].remove(i);
            return;
        }
}

template <typename K, typename T>
size_t HashMap<K, T>::count() {
    return hash_count;
}

template <typename K, typename T>
size_t HashMap<K, T>::uniqueValue() {
    HashMap<T, int> hash_v(size);
    
    for (int i = 0; i < size; i++)
        if (!buckets[i].empty())
            for (int j = 0; buckets[i].get(j); j++)
                hash_v.insert(buckets[i].get(j)->value, 0);
    
    return hash_v.count();
}

template <typename K, typename T>
void HashMap<K, T>::rehash(size_t n) {
    if (n > hash_count) {
        HashMap<K, T> *n_hash = new HashMap<K, T>(n);
        for (int i = 0; i < size; i++) {
            if (!buckets[i].empty())
                for (int j = 0; buckets[i].get(j); j++)
                    n_hash->insert(buckets[i].get(j)->key, buckets[i].get(j)->value);
            
        }
        
        List<Element<K, T>> *b = buckets;
        this->buckets = n_hash->buckets;
        this->size = n_hash->size;
        n_hash->buckets = b;
        delete n_hash;
    }
}

template <typename K, typename T>
size_t HashMap<K, T>::getBucketIndex(K key) {
    return getHash(key) % size;
}