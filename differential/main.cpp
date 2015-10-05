//
//  main.cpp
//  diff
//
//  Created by Егор Манылов on 29.09.15.
//  Copyright (c) 2015 Egor_Mn. All rights reserved.
//

#include <iostream>
#include <fstream>

std::ifstream in("input.txt");
std::ofstream out("output.txt");

class Expression {
public:
    virtual Expression *diff(char x) = 0;
    virtual Expression *simplify() = 0;
    virtual void print() = 0;
    virtual bool operator== (double x) { return false; }
};

class Number : public Expression {
    double N;
public:
    Number(double n) : N(n) {}
    bool operator== (double x){ return N == x; }
    Expression *diff(char x) { return new Number(0); }
    Expression *simplify() { return this; }
    void print() { out << N; }
};

class Variable : public Expression {
    char X;
public:
    Variable(char x) : X(x) {}
    Expression *diff(char x) {
        if (X == x) return new Number(1);
        else return new Number(0);
    }
    Expression *simplify() { return this; }
    void print() { out << X; }
};

class Add : public Expression {
    Expression *E1, *E2;
public:
    Add(Expression *e1, Expression *e2) : E1(e1), E2(e2) {}
    Expression *diff(char x) {
        return new Add(E1->diff(x), E2->diff(x));
    }
    Expression *simplify() {
        E1 = E1->simplify();
        E2 = E2->simplify();
        if (*E1 == 0) return E2;
        if (*E2 == 0) return E1;
        return this;
    }
    void print() {
        out << "(";
        E1->print();
        out << "+";
        E2->print();
        out << ")";
    }
};

class Sub : public Expression {
    Expression *E1, *E2;
public:
    Sub(Expression *e1, Expression *e2) : E1(e1), E2(e2) {}
    Expression *diff(char x) {
        return new Sub(E1->diff(x), E2->diff(x));
    }
    Expression *simplify() {
        E1 = E1->simplify();
        E2 = E2->simplify();
        if (*E1 == 0) return E2;
        if (*E2 == 0) return E1;
        return this;
    }
    void print() {
        out << "(";
        E1->print();
        out << "-";
        E2->print();
        out << ")";
    }
};

class Mul : public Expression {
    Expression *E1, *E2;
public:
    Mul(Expression *e1, Expression *e2) : E1(e1), E2(e2) {}
    Expression *diff(char x) {
        return new Add(new Mul(E1->diff(x), E2), new Mul(E1, E2->diff(x)));
    }
    Expression *simplify() {
        E1 = E1->simplify();
        E2 = E2->simplify();
        if (*E1 == 0 || *E2 == 0) return new Number(0);
        if (*E1 == 1) return E2;
        if (*E2 == 1) return E1;
        return this;
    }
    void print() {
        out << "(";
        E1->print();
        out << "*";
        E2->print();
        out << ")";
    }
};

class Div : public Expression {
    Expression *E1, *E2;
public:
    Div(Expression *e1, Expression *e2) : E1(e1), E2(e2) {}
    Expression *diff(char x) {
        return new Div(new Sub(new Mul(E1->diff(x), E2), new Mul(E1, E2->diff(x))), new Mul(E2, E2));
    }
    Expression *simplify() {
        E1 = E1->simplify();
        E2 = E2->simplify();
        if (*E1 == 0) return new Number(0);
        if (*E2 == 1) return E1;
        return this;
    }
    void print() {
        out << "(";
        E1->print();
        out << ")/(";
        E2->print();
        out << ")";
    }
};

class Stack{
    class Node{
    public:
        Node* next;
        char c;
        Node(char c){ this->c = c; }
    };
    Node *top;
public:
    Stack(){ top = NULL; }
    ~Stack(){
        while (top != NULL){
            Node* t = top;
            top = top->next;
            delete t;
        }
    }
    void push(char c){
        Node *n = new Node(c);
        n->next = top;
        top = n;
    }
    char pop() {
        Node* t = top;
        top = top->next;
        int tdata = t->c;
        delete(t);
        return tdata;
    }
    bool empty() {
        return top == NULL;
    }
    char last(){
        if (this->empty())
            return 0;
        return top->c;
    }
};

class Stack_ex{
    class Node{
    public:
        Node* next;
        Expression *c;
        Node(Expression *e) : c(e) {}
    };
    Node *top;
public:
    Stack_ex(){ top = NULL; }
    ~Stack_ex(){
        while (top != NULL){
            Node* t = top;
            top = top->next;
            delete t;
        }
    }
    void push(Expression *c){
        Node *n = new Node(c);
        n->next = top;
        top = n;
    }
    bool empty() {
        return top == NULL;
    }
    Expression *pop(){
        Node* t = top;
        top = top->next;
        Expression *tdata = t->c;
        delete(t);
        return tdata;
    }
};

Number *readNumber(char c) {
    in.putback(c);
    double x; in >> x;
    return new Number(x);
}

int priority(char c) {
    switch (c) {
        case '(': return 0;
        case ')': return 0;
        case '-': return 1;
        case '+': return 1;
        case '*': return 2;
        case '/': return 2;
        default: return 0;
    }
}

Expression *scan() {
    Expression *e = new Number(0);
    char c;
    
    Stack a;
    Stack_ex res;
    
    while (in >> c && !in.eof()) {
        if (c >= '0' && c <= '9') {
            e = readNumber(c);
            res.push(e);
        }
        
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
            e = new Variable(c);
            res.push(e);
        }
        
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            if (a.empty() || c == '(') {
                a.push(c);
            }
            else {
                while (priority(c) <= priority(a.last()) && a.last() != '(') {
                    Expression *e2 = res.pop();
                    Expression *e1 = res.pop();
                    
                    switch (a.pop()) {
                        case '-': { res.push(new Sub(e1, e2)); break; }
                        case '+': { res.push(new Add(e1, e2)); break; }
                        case '*': { res.push(new Mul(e1, e2)); break; }
                        case '/': { res.push(new Div(e1, e2)); break; }
                    }                    
                }
                if (c == ')') a.pop();
                else a.push(c);
            }
        }
    }
    
    while (!a.empty()) {
        Expression *e2 = res.pop();
        Expression *e1 = res.pop();
        
        switch (a.pop()) {
            case '-': { res.push(new Sub(e1, e2)); break; }
            case '+': { res.push(new Add(e1, e2)); break; }
            case '*': { res.push(new Mul(e1, e2)); break; }
            case '/': { res.push(new Div(e1, e2)); break; }
        }
    }
    return res.pop();
}

int main() {
    Expression *e = scan();
    Expression *de = e->diff('x');
    de->print();
    
    in.close();
    out.close();
    return 0;
}