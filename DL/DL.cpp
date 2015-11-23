//
//  DL.cpp
//  DL
//
//  Created by Egor Manylov on 22.11.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#include "DL.h"

int DLexpression::getValue() {
    throw "ERROR";
}

DLexpression *DLexpression::body() {
    throw "ERROR";
}
std::string DLexpression::id() {
    throw "ERROR";
}

void DLexpression::print(std::ofstream& output) {
    throw "ERROR";
}

void DLexpression::env::addVar(std::string id, DLexpression *ex) {
    env_map[id] = ex;
}

DLexpression *DLexpression::env::fromEnv(std::string id) {
    if (env_map.find(id) != env_map.end())
        return env_map[id];
    else throw "ERROR";
}

DLexpression *DLval::eval(env l_env) {
    return this;
}

int DLval::getValue() {
    return val;
}

void DLval::print(std::ofstream& output) {
    output << "(val " << val << ")" << std::endl;
}

DLexpression *DLvar::eval(env l_env) {
    return l_env.fromEnv(id);
}

DLexpression *DLadd::eval(env l_env) {
    return new DLval(ex1->eval(l_env)->getValue() + ex2->eval(l_env)->getValue());
}

DLexpression *DLif::eval(env l_env) {
    if (ex1->eval(l_env)->getValue() > ex2->eval(l_env)->getValue())
        return e_then->eval(l_env);
    else
        return e_else->eval(l_env);
}

DLexpression *DLlet::eval(env l_env) {
    l_env.addVar(id, value->eval());
    return body->eval(l_env);
}

DLexpression *DLfunction::eval(env l_env) {
    return this;
}

DLexpression *DLcall::eval(env l_env) {
    l_env.addVar(f_expr->eval(l_env)->id(), arg_expr->eval(l_env));
    return f_expr->eval(l_env)->body()->eval(l_env);
}

void makeExpression(std::stack <DLexpression *> *result, std::stack <std::string> *procedure) {
    
    if (procedure->empty())
        return;
    
    std::string str;
    do {
        str = procedure->top();
        procedure->pop();
    } while (str == " " && !procedure->empty());
    
    std::istringstream iss (str, std::istringstream::in);
    
    std::string str1;
    iss >> str1;
    
    if (str1 == "val") {
        int integer;
        iss >> integer;
        result->push(new DLval(integer));
    }
    
    if (str1 == "var") {
        std::string id;
        iss >> id;
        result->push(new DLvar(id));
    }
    
    if (str1 == "add") {
        DLexpression *e2 = result->top();
        result->pop();
        DLexpression *e1 = result->top();
        result->pop();
        result->push(new DLadd(e1, e2));
    }
    
    if (str1 == "else") {
        str1 = procedure->top();
        procedure->pop();
        if (str1 == "then") {
            str1 = procedure->top();
            procedure->pop();
            if (str1 == "if") {
                DLexpression *e4 = result->top();
                result->pop();
                DLexpression *e3 = result->top();
                result->pop();
                DLexpression *e2 = result->top();
                result->pop();
                DLexpression *e1 = result->top();
                result->pop();
                result->push(new DLif(e1, e2, e3, e4));
            }
        }
    }
    
    if (str1 == "in") {
        str1 = procedure->top();
        procedure->pop();
        std::istringstream iss2 (str1, std::istringstream::in);
        std::string str2;
        iss2 >> str2;
        if (str2 == "let") {
            iss2 >> str2;
            DLexpression *e2 = result->top();
            result->pop();
            DLexpression *e1 = result->top();
            result->pop();
            result->push(new DLlet(str2, e1, e2));
        }
    }
    
    if (str1 == "function") {
        iss >> str1;
        DLexpression *e1 = result->top();
        result->pop();
        result->push(new DLfunction(str1, e1));
    }
    
    if (str1 == "call") {
        DLexpression *e2 = result->top();
        result->pop();
        DLexpression *e1 = result->top();
        result->pop();
        result->push(new DLcall(e1, e2));
    }
}

DLexpression* scan(std::istream& input) {
    char c;
    std::stack<DLexpression*> result;
    std::stack<std::string> procedure;
    std::string str;
    
    input >> std::noskipws;
    while (!input.eof()) {
        input >> c;
        if (c == '(') {
            if (!str.empty())
                procedure.push(str);
            
            str.clear();
        } else if (c == ')') {
            if (!str.empty())
                procedure.push(str);
            
            str.clear();
            makeExpression(&result, &procedure);
        }  else if (c != '\n')
            str.push_back(c);
    }
    
    return result.top();
}