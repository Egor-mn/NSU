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

DLexpression *DLexpression::body(env l_env) {
    throw "ERROR";
}
std::string DLexpression::id() {
    throw "ERROR";
}

void DLexpression::print(std::ostream& output) {
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

DLval::DLval(int n): val(n) {}

DLexpression *DLval::eval(env l_env) {
    return this;
}

int DLval::getValue() {
    return val;
}

void DLval::print(std::ostream& output) {
    output << "(val " << val << ")" << std::endl;
}

DLvar::DLvar(std::string id): id(id) {}

DLexpression *DLvar::eval(env l_env) {
    return l_env.fromEnv(id);
}

DLadd::DLadd(DLexpression *e1, DLexpression *e2): ex1(e1), ex2(e2) {}

DLexpression *DLadd::eval(env l_env) {
    return new DLval(ex1->eval(l_env)->getValue() + ex2->eval(l_env)->getValue());
}

DLadd::~DLadd() {
    delete ex1;
    delete ex2;
}

DLif::DLif(DLexpression *e1, DLexpression *e2, DLexpression *e_then, DLexpression *e_else): ex1(e1), ex2(e2), e_then(e_then), e_else(e_else) {}

DLexpression *DLif::eval(env l_env) {
    if (ex1->eval(l_env)->getValue() > ex2->eval(l_env)->getValue())
        return e_then->eval(l_env);
    else
        return e_else->eval(l_env);
}

DLif::~DLif() {
    delete ex1;
    delete ex2;
    delete e_then;
    delete e_else;
}

DLlet::DLlet(std::string id, DLexpression *value, DLexpression *body): id(id), value(value), body(body) {}

DLexpression *DLlet::eval(env l_env) {
    l_env.addVar(id, value->eval(l_env));
    return body->eval(l_env);
}

DLlet::~DLlet() {
    delete value;
    delete body;
}

DLfunction::DLfunction(std::string id, DLexpression *ex): arg_id(id), f_body(ex) {}

DLfunction::~DLfunction() {
    delete f_body;
}

std::string DLfunction::id() {
    return arg_id;
}

DLexpression *DLfunction::body(env l_env) {
    myEnv.addVar(arg_id, l_env.fromEnv(arg_id));
    for (auto it = myEnv.env_map.begin(); it != myEnv.env_map.end(); ++it)
        l_env.addVar(it->first, it->second);
    
    return f_body->eval(l_env);
}

DLexpression *DLfunction::eval(env l_env) {
    myEnv = l_env;
    return this;
}

DLcall::DLcall(DLexpression *e1, DLexpression *e2): f_expr(e1), arg_expr(e2) {}

DLcall::~DLcall() {
    delete f_expr;
    delete arg_expr;
}

DLexpression *DLcall::eval(env l_env) {
    l_env.addVar(f_expr->eval(l_env)->id(), arg_expr->eval(l_env));
    return f_expr->eval(l_env)->body(l_env);
}

std::string makeExp(std::string& str) {
    std::string exp_str;
    int j = 0, n = 0;
    
    while(str[j] != '(')
        j++;
    
    for (; j < str.size(); j++) {
        if (str[j] == '(') n++;
        if (str[j] == ')') n--;
        if (n == 0)        break;
        exp_str.push_back(str[j]);
    }
    
    str.erase(0, ++j);
    
    return exp_str;
}

DLexpression* scan(std::string str) {
    str.erase(0, 1);
    std::istringstream iss (str, std::istringstream::in);
    std::string oper;
    iss >> oper;

    if (oper == "val") {
        int integer;
        iss >> integer;
        return new DLval(integer);
    }
    else if (oper == "var") {
        std::string id;
        iss >> id;
        return new DLvar(id);
    }
    else if (oper == "add") {
        std::string s1 = makeExp(str);
        std::string s2 = makeExp(str);
        return new DLadd(scan(s1), scan(s2));
    }
    else if (oper == "if") {
        std::string s1 = makeExp(str);
        std::string s2 = makeExp(str);
        std::string s3 = makeExp(str);
        std::string s4 = makeExp(str);
        return new DLif(scan(s1), scan(s2), scan(s3), scan(s4));
    }
    else if (oper == "let") {
        std::string id;
        iss >> id;
        std::string s1 = makeExp(str);
        std::string s2 = makeExp(str);
        return new DLlet(id, scan(s1), scan(s2));
    }
    else if (oper == "function") {
        std::string arg_id;
        iss >> arg_id;
        std::string s1 = makeExp(str);
        return new DLfunction(arg_id, scan(s1));
    }
    else if (oper == "call") {
        std::string s1 = makeExp(str);
        std::string s2 = makeExp(str);
        
        std::istringstream iss1 (s1, std::istringstream::in);
        char c;
        std::string expr;
        iss1 >> c >> expr;
        if (expr == "function") {
            std::string name, arg_id;
            iss1 >> name >> arg_id;
            s1.erase(0, 1);
            s1 = "(" + expr + " " + arg_id + " " + makeExp(s1) + ")";
            return new DLlet(name, scan(s1), new DLcall(new DLvar(name), scan(s2)));
        } else
            return new DLcall(scan(s1), scan(s2));
    }
    throw "ERROR";
}