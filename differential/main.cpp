//
//  main.cpp
//  diff
//
//  Created by Егор Манылов on 29.09.15.
//  Copyright (c) 2015 Egor_Mn. All rights reserved.
//

#include <iostream>
#include <fstream>
#include "math.h"

std::ifstream in("input.txt");
std::ofstream out("output.txt");

char var = 'x';

class Expression {
public:
    virtual Expression *diff() = 0;
    virtual Expression *simplify() = 0;
    virtual void print() = 0;
    virtual bool operator== (double x) = 0;
};

class Number : public Expression {
    double N;
public:
    Number(double n): N(n) {}
    bool operator== (double x){ return N == x; }
    Expression *diff() { return new Number(0); }
    Expression *simplify() { return this; }
    void print() { out << N; }
};

class Variable : public Expression {
    char X;
public:
    Variable(char x): X(x) {}
    bool operator== (double x){ return false; }
    Expression *diff() { return new Number(1); }
    Expression *simplify() { return this; }
    void print() { out << X; }
};

class Constant : public Expression {
    char C;
public:
    Constant(char x): C(x) {}
    bool operator== (double x){ return false; }
    Expression *diff() { return new Number(0); }
    Expression *simplify() { return this; }
    void print() { out << C; }
};

class Add : public Expression {
    Expression *E1, *E2;
public:
    Add(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
    bool operator== (double x){ return false; }
    Expression *diff() {
        return new Add(E1->diff(), E2->diff());
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
    Expression *diff() {
        return new Sub(E1->diff(), E2->diff());
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
    Expression *diff() {
        return new Add(new Mul(E1->diff(), E2), new Mul(E1, E2->diff()));
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
    Expression *diff() {
        return new Div(new Sub(new Mul(E1->diff(), E2), new Mul(E1, E2->diff())), new Mul(E2, E2));
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
    bool point = false;
    double num = 0;
    int degree = 0;
    
    while ((c >= '0' && c <= '9' && !in.eof()) || c == '.') {
        if (c == '.') { point = true; in >> c; continue; }
        if (point) num += (c - '0') / pow(10.0, ++degree);
        else num = 10 * num + (c - '0');
        in >> c;
    }
    
    in.putback(c);
    return new Number(num);
}

Expression *scan(int priority = 0) {
    Expression *e;
    char c;
    
    while (in >> c && !in.eof()) {
        if (c == ')') return e;
        if (c == '(') e = scan();
        if (c == '*') e = new Mul(e, scan(1));
        if (c == '/') e = new Div(e, scan(1));
        if (c == '+') e = new Add(e, scan());
        if (c == '-') e = new Sub(e, scan());
        if (c == var) e = new Variable(c);
        if (c >= '0' && c <= '9') e = readNumber(c);
        if (((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) && c != var)
            e = new Constant(c);

        if (priority == 1) return e;
    }
    return e;
}

int main() {
    Expression *e = scan();
    Expression *de = e->diff();
    de = de->simplify();
    de->print();
    
    in.close();
    out.close();
    return 0;
}