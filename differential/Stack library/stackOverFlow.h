//
//  stackOverFlow.hpp
//  diff
//
//  Created by Егор Манылов on 05.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#ifndef stackOverFlow_h
#define stackOverFlow_h
#endif

template <class T>
class Stack {
    class Element{
    public:
        Element *next;
        T var;
        Element(T x): var(x) {};
    };
    Element *top;
public:
    Stack();
    ~Stack();
    void push(const T var);
    bool empty();
    T pop();
    T back();
};