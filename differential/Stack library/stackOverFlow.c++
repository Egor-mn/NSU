//
//  stackOverFlow.cpp
//  diff
//
//  Created by Егор Манылов on 05.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#include "stackOverFlow.h"

template <class T>
Stack<T>::Stack(){
    top = nullptr;
}

template <class T>
Stack<T>::~Stack(){
    while (top != nullptr){
        Element *t = top;
        top = top->next;
        delete t;
    }
}

template <class T>
void Stack<T>::push(const T var) {
    Element *n = new Element(var);
    n->next = top;
    top = n;
}

template <class T>
bool Stack<T>::empty() {
    return top == nullptr;
}

template <class T>
T Stack<T>::pop() {
    if (this->empty()) return nullptr;
    Element *t = top;
    top = top->next;
    T tdata = t->var;
    delete t;
    return tdata;
}

template <class T>
T Stack<T>::back(){
    if (this->empty()) return nullptr;
    return top->var;
}