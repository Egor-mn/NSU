//
//  main.cpp
//  diff
//
//  Created by Егор Манылов on 29.09.15.
//  Copyright (c) 2015 Egor_Mn. All rights reserved.
//

#include <iostream>

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
        std::cout << N;
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
        std::cout << X;
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
        std::cout << "(";
        E1->print();
        std::cout << "+";
        E2->print();
        std::cout << ")";
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
        std::cout << "(";
        E1->print();
        std::cout << "-";
        E2->print();
        std::cout << ")";
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
        std::cout << "(";
        E1->print();
        std::cout << "*";
        E2->print();
        std::cout << ")";
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
        std::cout << "(";
        E1->print();
        std::cout << ")/(";
        E2->print();
        std::cout << ")";
    }
};

int main() {
    Expression *e = new Add(new Number(3), new Mul (new Number (2), new Variable('x')));
    e->print();
    std::cout << std::endl;
    Expression*de=e->diff();
    de->print();
    return 0;
}