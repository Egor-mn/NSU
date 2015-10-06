//
//  main.cpp
//  diff
//
//  Created by Егор Манылов on 29.09.15.
//  Copyright (c) 2015 Egor_Mn. All rights reserved.
//

#include <iostream>
#include <fstream>

#include "diff.h"

template <class T>
class Stack {
    struct Element{
        Element *next;
        T var;
        Element(T x): var(x) {};
    };
    Element *top;
public:
    Stack(){
        top = NULL;
    }
    ~Stack(){
        while (top != NULL){
            Element *t = top;
            top = top->next;
            delete t;
        }
    }
    void push(const T var) {
        Element *n = new Element(var);
        n->next = top;
        top = n;
    }
    bool empty() {
        return top == NULL;
    }
    T pop() {
        if (this->empty()) return NULL;
        Element *t = top;
        top = top->next;
        T tdata = t->var;
        delete t;
        return tdata;
    }
    
    T back(){
        if (this->empty()) return NULL;
        return top->var;
    }
};

int priority(char c) {
    switch (c) {
        case '(': case ')': return 0;
        case '-': case '+': return 1;
        case '*': case '/': return 2;
        default: return 0;
    }
}

void makeExpression(Stack <Expression *> result, Stack <char> procedure) {
    Expression *e2 = result.pop();
    Expression *e1 = result.pop();
    
    switch (procedure.pop()) {
        case '-': { result.push(new Sub(e1, e2)); break; }
        case '+': { result.push(new Add(e1, e2)); break; }
        case '*': { result.push(new Mul(e1, e2)); break; }
        case '/': { result.push(new Div(e1, e2)); break; }
    }
}

Expression *scan(std::ifstream& input) {
    Expression *e;
    char c;
    
    Stack <Expression *> result;
    Stack <char> procedure;
    
    while (input >> c && !input.eof()) {
        if (c >= '0' && c <= '9') {
            input.putback(c);
            double x; input >> x;
            result.push(new Number(x));
        }
        
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
            e = new Variable(c);
            result.push(e);
        }
        
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            if (procedure.empty() || c == '(') {
                procedure.push(c);
            }
            else {
                while (priority(c) <= priority(procedure.back()) && procedure.back() != '(') {
                    makeExpression(result, procedure);
                }
                if (c == ')') procedure.pop();
                else procedure.push(c);
            }
        }
    }
    
    while (!procedure.empty()) {
        makeExpression(result, procedure);
    }
    return result.pop();
}

int main() {
    std::ifstream in("input.txt");
    std::ofstream out("output.txt");
    
    Expression *e = scan(in);
    Expression *de = e->diff('x');
    de->print(out);
    
    in.close();
    out.close();
    return 0;
}