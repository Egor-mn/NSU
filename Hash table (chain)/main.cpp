//
//  main.cpp
//  Hash table (chain)
//
//  Created by Егор Манылов on 19.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#include <fstream>
#include <iostream>
#include <functional>
#include <string>

using namespace std;

template <typename K>
size_t getHash(K key) {
    std::hash<K> simple_hash;
    return simple_hash(key);
}

template <typename K, typename T>
class List {
    struct Node{
        Node *next;
        K key;
        T value;
        Node(K key, T value): key(key), value(value) {};
    };
    Node *top;
public:
    List(): top(NULL) {};
    ~List() {
        while (top != NULL){
            Node *t = top;
            top = top->next;
            delete t;
        }
    }
    
    int add(K key, T value) {
        Node *n = new Node(key, value);
        n->next = NULL;
        
        if (top == NULL){
            top = n;
            return 1;
        }
        
        if (top->key == key) {
            top->value = value;
            return 0;
        }
        
        Node *t = top;
        while (t->next != NULL) {
            t = t->next;
            if (t->key == key) {
                t->value = value;
                return 0;
            }
        }
        t->next = n;
        return 1;
    }
    
    bool empty() {
        return top == NULL;
    }
    
    int del(K key) {
        if (this->empty()) return 0;
        Node *t = top;
        if (top ->key == key) {
            top = top->next;
            delete t;
            return 1;
        }
        
        while (t->next != NULL) {
            Node *p = t;
            t = t->next;
            if (t->key == key) {
                p->next = t->next;
                delete t;
                return 1;
            }
        }
        return 0;
    }
    
    T find(K key){
        if (this->empty()) return NULL;
        if (top->key == key)
            return top->value;
        
        Node *t = top;
        while (t->next != NULL) {
            t = t->next;
            
            if (t->key == key)
                return t->value;
        }
        return NULL;
    }
};

template <typename K, typename T>
class HashMap {
    int size, count;
    K *keys;
    List<K, T> *a;
public:
    HashMap() {
        count = 0;
        size = 1;
        keys = new K[size];
        a = new List<K, T>[size];
    }
    
    void updateKeys(K key) {
        keys[count] = key;
    }
    
    void add(K key, T value) {
        int x = getHash(key) % size;
        if (a[x].add(key, value) == 1) {
            updateKeys(key);
            count++;
        }
    }
    
    T find(K key) {
        int x = getHash(key) % size;
        return a[x].find(key);
    }
    
    void del(K key) {
        int x = getHash(key) % size;
        count -= a[x].del(key);
    }
    
    int getSize() {
        return count;
    }
    
    int uniqueValue() {
        HashMap<T, int> hash_v;
        for (int i = 0; i < count; i++)
            hash_v.add(this->find(keys[i]), 1);
        return hash_v.getSize();
    }
    
    void reHash() {
        if (count > 0.6 * size) {
            HashMap<K, T> *n_hash = new HashMap<K, T>();
            n_hash->size = this->size * 2;
            for (int i = 0; i < count; i++)
                n_hash->add(keys[i], this->find(keys[i]));
            
            delete[] a;
            this->a = n_hash->a;
            this->size = n_hash->size;
            delete n_hash;
        }
    }
};

template <typename K, typename T>
void doAll(int n, std::ifstream& in, std::ofstream& out) {
    K key;
    T value;
    HashMap<K, T> hash1;
    char c;
    while (n-- != 0) {
        hash1.reHash();
        in >> c;
        if (c == 'A') {
            in >> key;
            in >> value;
            hash1.add(key, value);
        }
        if (c == 'R') {
            in >> key;
            hash1.del(key);
        }
    }
    out << hash1.getSize();
    out << ' ';
    out << hash1.uniqueValue();
}

int main() {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");
    
    char a, b;
    int n;
    
    in >> a;
    in >> b;
    in >> n;
    
    if (a == 'I' && b == 'I') doAll<int, int>       (n, in, out);
    if (a == 'I' && b == 'D') doAll<int, double>    (n, in, out);
    if (a == 'I' && b == 'S') doAll<int, string>    (n, in, out);
    if (a == 'D' && b == 'I') doAll<double, int>    (n, in, out);
    if (a == 'D' && b == 'D') doAll<double, double> (n, in, out);
    if (a == 'D' && b == 'S') doAll<double, string> (n, in, out);
    if (a == 'S' && b == 'I') doAll<string, int>    (n, in, out);
    if (a == 'S' && b == 'D') doAll<string, double> (n, in, out);
    if (a == 'S' && b == 'S') doAll<string, string> (n, in, out);
    
    in.close();
    out.close();

}











