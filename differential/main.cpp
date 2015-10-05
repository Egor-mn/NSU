//
//  main.cpp
//  diff
//
//  Created by Егор Манылов on 29.09.15.
//  Copyright (c) 2015 Egor_Mn. All rights reserved.
//

#include <iostream>
#include <fstream>

class Expression {
public:
    virtual Expression *diff(char x) = 0;
    virtual Expression *simplify() = 0;
    virtual void print(std::ofstream& output) = 0;
    virtual bool operator== (double x) { return false; }
};

class Number : public Expression {
    double N;
public:
    Number(double n) : N(n) {}
    bool operator== (double x) { return N == x; }
    Expression *diff(char x) { return new Number(0); }
    Expression *simplify() { return this; }
    void print(std::ofstream& output) { output << N; }
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
    void print(std::ofstream& output) { output << X; }
};

class Add : public Expression {
    Expression *E1, *E2;
public:
    Add(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
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
    void print(std::ofstream& output) {
        output << "(";
        E1->print(output);
        output << "+";
        E2->print(output);
        output << ")";
    }
};

class Sub : public Expression {
    Expression *E1, *E2;
public:
    Sub(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
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
    void print(std::ofstream& output) {
        output << "(";
        E1->print(output);
        output << "-";
        E2->print(output);
        output << ")";
    }
};

class Mul : public Expression {
    Expression *E1, *E2;
public:
    Mul(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
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
    void print(std::ofstream& output) {
        output << "(";
        E1->print(output);
        output << "*";
        E2->print(output);
        output << ")";
    }
};

class Div : public Expression {
    Expression *E1, *E2;
public:
    Div(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
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
    void print(std::ofstream& output) {
        output << "(";
        E1->print(output);
        output << ")/(";
        E2->print(output);
        output << ")";
    }
};

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
        case '(': return 0;
        case ')': return 0;
        case '-': return 1;
        case '+': return 1;
        case '*': return 2;
        case '/': return 2;
        default: return 0;
    }
}

Expression *scan(std::ifstream& input) {
    Expression *e;
    char c;
    
    Stack <Expression *> Result;
    Stack <char> Procedure;
    
    while (input >> c && !input.eof()) {
        if (c >= '0' && c <= '9') {
            input.putback(c);
            double x; input >> x;
            Result.push(new Number(x));
        }
        
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
            e = new Variable(c);
            Result.push(e);
        }
        
        if (c == '+' || c == '-' || c == '*' || c == '/' || c == '(' || c == ')') {
            if (Procedure.empty() || c == '(') {
                Procedure.push(c);
            }
            else {
                while (priority(c) <= priority(Procedure.back()) && Procedure.back() != '(') {
                    Expression *e2 = Result.pop();
                    Expression *e1 = Result.pop();
                    
                    switch (Procedure.pop()) {
                        case '-': { Result.push(new Sub(e1, e2)); break; }
                        case '+': { Result.push(new Add(e1, e2)); break; }
                        case '*': { Result.push(new Mul(e1, e2)); break; }
                        case '/': { Result.push(new Div(e1, e2)); break; }
                    }                    
                }
                if (c == ')') Procedure.pop();
                else Procedure.push(c);
            }
        }
    }
    
    while (!Procedure.empty()) {
        Expression *e2 = Result.pop();
        Expression *e1 = Result.pop();
        
        switch (Procedure.pop()) {
            case '-': { Result.push(new Sub(e1, e2)); break; }
            case '+': { Result.push(new Add(e1, e2)); break; }
            case '*': { Result.push(new Mul(e1, e2)); break; }
            case '/': { Result.push(new Div(e1, e2)); break; }
        }
    }
    return Result.pop();
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