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
    virtual Expression *diff() = 0;
    virtual void print() = 0;
};

class Number : public Expression {
    double N;
public:
    Number(double n): N(n) {}
    Expression *diff() {
        return new Number(0);
    }
    void print() {
        out << N;
    }
};

class Variable : public Expression {
    char X;
public:
    Variable(char x): X(x) {}
    Expression *diff() {
        return new Number(1);
    }
    void print() {
        out << X;
    }
};

class Add : public Expression {
    Expression *E1, *E2;
public:
    Add(Expression *e1, Expression *e2): E1(e1), E2(e2) {}
    Expression *diff() {
        return new Add(E1->diff(), E2->diff());
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
    Expression *diff() {
        return new Sub(E1->diff(), E2->diff());
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
    Expression *diff() {
        return new Add(new Mul(E1->diff(), E2), new Mul(E1, E2->diff()));
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
    Expression *diff() {
        return new Div(new Sub(new Mul(E1->diff(), E2), new Mul(E1, E2->diff())), new Mul(E2, E2));
    }
    void print() {
        out << "(";
        E1->print();
        out << ")/(";
        E2->print();
        out << ")";
    }
};

Expression *scan(int prior = 0) {
    Expression *e;
    char c;
    
    while (in >> c && !in.eof()) {
        if (c == ')') return e;
        if (c == '(') e = scan();
        if (c == 'x') e = new Variable(c);
        if (c == '*') e = new Mul(e, scan(1));
        if (c == '/') e = new Div(e, scan(1));
        if (c == '+') e = new Add(e, scan());
        if (c == '-') e = new Sub(e, scan());
        
        if (c >= '0' && c <= '9') {
            int num = 0;
            char prev = c;
            while (c >= '0' && c <= '9' && c != '\0' && !in.eof()) {
                num = 10 * num + (c - '0');
                prev = c;
                in >> c;
            }
            in.putback(c);
            c = prev;
            e = new Number(num);
        }

        if (prior == 1) return e;
    }
    return e;
}

int main() {
    Expression *e = scan();
    Expression *de=e->diff();
    de->print();
    return 0;
}