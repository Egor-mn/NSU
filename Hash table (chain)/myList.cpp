//
//  myList.cpp
//  Hash table (chain)
//
//  Created by Egor Manylov on 25.10.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#include "myList.hpp"


template <typename E>
List<E>::iterator::iterator(List<E> *lst, int num):
    lst(lst), iteration(num) { };

template <typename E>
typename List<E>::iterator& List<E>::iterator::operator++() {
    ++iteration;
    return *this;
}

template <typename E>
bool List<E>::iterator::operator!=(iterator it) {
    return iteration != it.iteration;
}

template <typename E>
E List<E>::iterator::operator*() {
    return lst->get(iteration);
}

template <typename E>
E* List<E>::iterator::operator->() {
    return lst->get(iteration);
}

template <typename E>
int List<E>::iterator::index() {
    return iteration;
}

template <typename E>
typename List<E>::iterator List<E>::begin() {
    return *new iterator(this, 0);
}

template <typename E>
typename List<E>::iterator List<E>::end() {
    return *new iterator(this, size);
}

template <typename E>
List<E>::List(): top(nullptr), size(0) {};

template <typename E>
List<E>::~List() {
    while (top != nullptr){
        Node *t = top;
        top = top->next;
        delete t->element;
        delete t;
    }
}

template <typename E>
void List<E>::push_back(E *element) {
    Node *n = new Node(element);
    n->next = nullptr;
    
    size++;
    if (top == nullptr) {
        top = n;
        return;
    }
    
    Node *t = top;
    while (t->next)
        t = t->next;

    t->next = n;
}

template <typename E>
bool List<E>::empty() {
    return top == nullptr;
}

template <typename E>
void List<E>::update(int index, E *element) {
    if (this->empty()) return;
    if (index == 0) {
        Node *n = new Node(element);
        n->next = top->next;
        Node *d = top;
        delete d;
        top = n;
        return;
    }
    
    Node *t = top;
    
    while (--index != 0)
        t = t->next;
    
    if (t) {
        Node *d = t->next;
        Node *n = new Node(element);
        n->next = d->next;
        t->next = n;
        delete d;
    }
}

template <typename E>
void List<E>::remove(int index) {
    if (this->empty()) return;
    if (index == 0) {
        Node *t = top;
        top = top->next;
        delete t;
        size--;
        return;
    }
    
    Node *t = top;
    
    while (--index != 0)
        t = t->next;
    
    if (t) {
        Node *d = t->next;
        t->next = d->next;
        delete d;
    }
    size--;
}

template <typename E>
E* List<E>::get(int index) {
    if (this->empty()) return nullptr;
    if (index == 0) {
        return top->element;
    }
    
    Node *t = top;
    
    while (index-- != 0)
        t = t->next;
    
    if (t) {
        return t->element;
    }
    return nullptr;
}