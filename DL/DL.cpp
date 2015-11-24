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

DLexpression *DLval::eval(env l_env) {
    return this;
}

int DLval::getValue() {
    return val;
}

void DLval::print(std::ostream& output) {
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

std::string makeExp(std::string str, int& j) {
    std::string exp_str;
    
    while(str[j] != '(')
        j++;
    
    int n = 1;
    for (j++; j < str.size(); j++) {
        if (str[j] == '(') n++;
        if (str[j] == ')') n--;
        if (n == 0)        break;
        exp_str.push_back(str[j]);
    }
    j++;
    
    return exp_str;
}

DLexpression* scan(std::string str) {
    for (int i = 0; i < str.size(); i++) {
        if (str[i] == ' ')
            continue;
        else if (str[i] == 'v' && str[i + 1] == 'a' && str[i + 2] == 'l') {
            std::istringstream iss (str, std::istringstream::in);
            std::string id;
            int integer;
            iss >> id >> integer;
            return new DLval(integer);
        }
        else if (str[i] == 'v' && str[i + 1] == 'a' && str[i + 2] == 'r') {
            std::istringstream iss (str, std::istringstream::in);
            std::string id;
            iss >> id >> id;
            return new DLvar(id);
        }
        else if (str[i] == 'a' && str[i + 1] == 'd' && str[i + 2] == 'd') {
            int j = i + 3;
            std::string s1 = makeExp(str, j);
            std::string s2 = makeExp(str, j);
            return new DLadd(scan(s1), scan(s2));
        }
        else if (str[i] == 'i' && str[i + 1] == 'f') {
            int j = i + 2;
            std::string s1 = makeExp(str, j);
            std::string s2 = makeExp(str, j);
            std::string s3 = makeExp(str, j);
            std::string s4 = makeExp(str, j);
            return new DLif(scan(s1), scan(s2), scan(s3), scan(s4));
        }
        else if (str[i] == 'l' && str[i + 1] == 'e' && str[i + 2] == 't') {
            std::istringstream iss (str, std::istringstream::in);
            std::string id;
            iss >> id >> id;
            int j = i + 4;
            std::string s1 = makeExp(str, j);
            std::string s2 = makeExp(str, j);
            return new DLlet(id, scan(s1), scan(s2));
        }
        else if (str[i] == 'f' && str[i + 1] == 'u' && str[i + 2] == 'n' && str[i + 3] == 'c' && str[i + 4] == 't' && str[i + 5] == 'i' && str[i + 6] == 'o' && str[i + 7] == 'n') {
            std::istringstream iss (str, std::istringstream::in);
            std::string id;
            iss >> id >> id;
            int j = i + 8;
            std::string s1 = makeExp(str, j);
            return new DLfunction(id, scan(s1));
        }
        else if (str[i] == 'c' && str[i + 1] == 'a' && str[i + 2] == 'l' && str[i + 3] == 'l') {
            int j = i + 3;
            std::string s1 = makeExp(str, j);
            std::string s2 = makeExp(str, j);
            return new DLcall(scan(s1), scan(s2));
        }
    }
    throw "ERROR";
}