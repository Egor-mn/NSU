//
//  myList.hpp
//  Hash table (chain)
//
//  Created by Egor Manylov on 25.10.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#ifndef myList_hpp
#define myList_hpp

template <typename K, typename T>
struct Element{
    K key;
    T value;
    Element(K key, T value): key(key), value(value) {};
};

template <typename E>
class List {
    struct Node{
        Node *next;
        E *element;
        Node(E *el): element(el), next(nullptr) {};
    };
    Node *top;
    int size;
public:
    class iterator {
        List<E> *lst;
        int iteration;
    public:
        iterator(List<E> *lst, int num);
        iterator& operator++();
        bool      operator!=(iterator it);
        E         operator*();
        E*        operator->();
        int       index();
    };
    
    iterator begin();
    iterator end();
    
     List();
    ~List();
    
    void update(int index, E *element);
    void push_back(E *element);
    bool empty();
    void remove(int index);
    E*   get(int index);
};

#endif /* myList_hpp */
