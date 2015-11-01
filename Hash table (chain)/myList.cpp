//
//  myList.cpp
//  Hash table (chain)
//
//  Created by Egor Manylov on 25.10.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#include "myList.hpp"


template <typename E>
List<E>::iterator::iterator(Node *node):
    node(node) { };

template <typename E>
typename List<E>::iterator& List<E>::iterator::operator++() {
    if (node->next)
        node = node->next;
    return *this;
}

template <typename E>
bool List<E>::iterator::operator!=(iterator it) {
    return node != it.node;
}

template <typename E>
bool List<E>::iterator::operator==(iterator it) {
    return node == it.node;
}

template <typename E>
E List<E>::iterator::operator*() {
    return *node->element;
}

template <typename E>
E* List<E>::iterator::operator->() {
    return node->element;
}

template <typename E>
typename List<E>::iterator List<E>::begin() {
    List<E>::iterator it = *new iterator(this->top);
    return it;
}

template <typename E>
typename List<E>::iterator List<E>::end() {
    Node *tail = top;
    if (tail)
        while (tail->next)
            tail = tail->next;
    
    List<E>::iterator it = *new iterator(tail);
    return it;
}

template <typename E>
typename List<E>::iterator List<E>::erase(iterator it) {
    return ++it;
}

template <typename E>
List<E>::List(): top(new Node(nullptr)), list_size(0) {};

template <typename E>
List<E>::~List() {
    while (top != nullptr){
        Node *t = top;
        top = top->next;
        if (t->element)
            delete t->element;
        delete t;
    }
}

template <typename E>
void List<E>::push_back(E *element) {
    list_size++;
    
    Node *t = top;
    while (t->next)
        t = t->next;

    t->element = element;
    t->next = new Node(nullptr);
}

template <typename E>
bool List<E>::empty() {
    return top->element == nullptr;
}

template <typename E>
void List<E>::update(int index, E *element) {
    if (this->empty()) return;
    if (index == 0)
        top->element = element;
    
    Node *t = top;
    
    while (--index != 0 && t->next) 
        t = t->next;
    
    t->element = element;
}

template <typename E>
void List<E>::remove(int index) {
    if (this->empty() || index > list_size) return;
    
    Node *t = top;
    if (index == 0)
        top = top->next;
    else {
        while (--index != 0 && t->next)
            t = t->next;

        Node *p = t;
        t = t->next;
        p->next = t->next;
    }
    
    list_size--;
    if (t->next)
        delete t;
    return;
}

template <typename E>
E* List<E>::get(int index) {
    if (this->empty() || index > list_size) return nullptr;
    
    Node *t = top;
    while (index-- != 0)
        t = t->next;
    
    return t->element;
}

template <typename E>
size_t List<E>::size() {
    return list_size;
}