//
//  diff.h
//  diff
//
//  Created by Егор Манылов on 05.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#ifndef diff_h
#define diff_h
#endif

#include <iostream>
#include <fstream>

class Expression {
public:
    virtual Expression *diff(char x) = 0;
    virtual Expression *simplify() = 0;
    virtual void print(std::ofstream& output) = 0;
    virtual bool operator== (double x);
};

class Number : public Expression {
    double N;
public:
    Number(double n) : N(n) {}
    bool operator== (double x);
    Expression *diff(char x);
    Expression *simplify();
    void print(std::ofstream& output);
};

class Variable : public Expression {
    char var;
public:
    Variable(char x) : var(x) {}
    Expression *diff(char x);
    Expression *simplify();
    void print(std::ofstream& output);
};

class Add : public Expression {
    Expression *ex1, *ex2;
public:
    Add(Expression *ex1, Expression *ex2): ex1(ex1), ex2(ex2) {}
    Expression *diff(char x);
    Expression *simplify();
    void print(std::ofstream& output);
};

class Sub : public Expression {
    Expression *ex1, *ex2;
public:
    Sub(Expression *e1, Expression *e2): ex1(e1), ex2(e2) {}
    Expression *diff(char x);
    Expression *simplify();
    void print(std::ofstream& output);
};

class Mul : public Expression {
    Expression *ex1, *ex2;
public:
    Mul(Expression *e1, Expression *e2): ex1(e1), ex2(e2) {}
    Expression *diff(char x);
    Expression *simplify();
    void print(std::ofstream& output);
};

class Div : public Expression {
    Expression *ex1, *ex2;
public:
    Div(Expression *e1, Expression *e2): ex1(e1), ex2(e2) {}
    Expression *diff(char x);
    Expression *simplify();
    void print(std::ofstream& output);
};