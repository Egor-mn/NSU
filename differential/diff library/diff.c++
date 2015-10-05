//
//  diff.cpp
//  diff
//
//  Created by Егор Манылов on 05.10.15.
//  Copyright © 2015 Egor_Mn. All rights reserved.
//

#include "diff.h"

bool Expression::operator==(double x) {
    return false;
}

bool Number::operator== (double x) {
    return N == x;
}

Expression *Number::diff(char x) {
    return new Number(0);
}

Expression *Number::simplify() {
    return this;
}

void Number::print(std::ofstream& output) {
    output << N;
}

Expression *Variable::diff(char x) {
    if (x == var)
        return new Number(1);
    else
        return new Number(0);
}

Expression *Variable::simplify() {
    return this;
}

void Variable::print(std::ofstream& output) {
    output << var;
}

Expression *Add::diff(char x) {
    return new Add(ex1->diff(x), ex2->diff(x));
}

Expression *Add::simplify() {
    ex1 = ex1->simplify();
    ex2 = ex2->simplify();
    if (*ex1 == 0) return ex2;
    if (*ex2 == 0) return ex1;
    return this;
}

void Add::print(std::ofstream& output) {
    output << "(";
    ex1->print(output);
    output << "+";
    ex2->print(output);
    output << ")";
}

Expression *Sub::diff(char x) {
    return new Sub(ex1->diff(x), ex2->diff(x));
}

Expression *Sub::simplify() {
    ex1 = ex1->simplify();
    ex2 = ex2->simplify();
    if (*ex2 == 0) return ex1;
    return this;
}
void Sub::print(std::ofstream& output) {
    output << "(";
    ex1->print(output);
    output << "-";
    ex2->print(output);
    output << ")";
}

Expression *Mul::diff(char x) {
    return new Add(new Mul(ex1->diff(x), ex2), new Mul(ex1, ex2->diff(x)));
}
Expression *Mul::simplify() {
    ex1 = ex1->simplify();
    ex2 = ex2->simplify();
    if (*ex1 == 0 || *ex2 == 0) return new Number(0);
    if (*ex1 == 1) return ex2;
    if (*ex2 == 1) return ex1;
    return this;
}
void Mul::print(std::ofstream& output) {
    ex1->print(output);
    output << "*";
    ex2->print(output);
}

Expression *Div::diff(char x) {
    return new Div(new Sub(new Mul(ex1->diff(x), ex2), new Mul(ex1, ex2->diff(x))), new Mul(ex2, ex2));
}
Expression *Div::simplify() {
    ex1 = ex1->simplify();
    ex2 = ex2->simplify();
    if (*ex1 == 0) return new Number(0);
    if (*ex2 == 1) return ex1;
    return this;
}
void Div::print(std::ofstream& output) {
    output << "(";
    ex1->print(output);
    output << ")/(";
    ex2->print(output);
    output << ")";
}
