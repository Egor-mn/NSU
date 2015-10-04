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
    virtual bool operator== (double x) = 0;
};

class Number : public Expression {
    double N;
public:
    Number(double n): N(n) {}
    bool operator== (double x){ return N == x; }
    Expression *diff(char x) { return new Number(0); }
    Expression *simplify() { return this; }
    void print() { out << N; }
};

class Variable : public Expression {
    char X;
public:
    Variable(char x): X(x) {}
    bool operator== (double x){ return false; }
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
    Add(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
    bool operator== (double x){ return false; }
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
    Sub(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
    bool operator== (double x){ return false; }
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
    Mul(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
    bool operator== (double x){ return false; }
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
    Div(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
    bool operator== (double x){ return false; }
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

Number *readNumber(char c) {
    in.putback(c);
    double x; in >> x;
    return new Number(x);
}

Expression *scan(int priority = 0) {
    Expression *e;
    char c;
    
    while (in >> c && !in.eof()) {
        char next;
        in >> next;
        in.putback(next);
        
        if (c == ')') return e;
        if (c == '(') e = scan();
        if (c == '*') e = new Mul(e, scan(1));
        if (c == '/') e = new Div(e, scan(1));
        if (c == '+') e = new Add(e, scan());
        if (c == '-') e = new Sub(e, scan());
        
        if (c >= '0' && c <= '9') { e = readNumber(c); if (next == ')') return e;}
        if ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')){
            e = new Variable(c);
            if (next == ')') return e;
        }
        
        
        if (priority == 1) return e;
    }
    return e;
}

int main() {
    Expression *e = scan();
    Expression *de = e->diff('x');
    de->print();
    
    in.close();
    out.close();
    return 0;
}