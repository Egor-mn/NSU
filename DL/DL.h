//
//  DL.hpp
//  DL
//
//  Created by Egor Manylov on 22.11.15.
//  Copyright © 2015 Egor Manylov. All rights reserved.
//

#ifndef DL_h
#define DL_h

#include <unordered_map>
#include <stack>
#include <sstream>
#include <fstream>

class DLexpression {
protected:
    struct env {
        std::unordered_map<std::string, DLexpression*> env_map;
        void addVar(std::string id, DLexpression *ex);
        DLexpression *fromEnv(std::string id);
    };
public:
    virtual ~DLexpression() {};
    virtual DLexpression *eval(env environment = env()) = 0;
    virtual int getValue();
    virtual DLexpression *body(env l_env);
    virtual std::string id();
    virtual void print(std::ostream& output);
};

class DLval : public DLexpression {
    int val;
    int getValue();
public:
    DLval(int n);
    DLexpression *eval(env environment = env());
    void print(std::ostream& output);
};

class DLvar : public DLexpression {
    std::string id;
public:
    DLvar(std::string id);
    DLexpression *eval(env environment = env());
};

class DLadd : public DLexpression {
    DLexpression *ex1, *ex2;
public:
    DLadd(DLexpression *e1, DLexpression *e2);
    ~DLadd();
    DLexpression *eval(env environment = env());
};

class DLif : public DLexpression {
    DLexpression *ex1, *ex2, *e_then, *e_else;
public:
    DLif(DLexpression *e1, DLexpression *e2, DLexpression *e_then, DLexpression *e_else);
    ~DLif();
    DLexpression *eval(env environment = env());
};

class DLlet : public DLexpression {
    std::string id;
    DLexpression *value;
    DLexpression *body;
public:
    DLlet(std::string id, DLexpression *value, DLexpression *body);
    ~DLlet();
    DLexpression *eval(env environment = env());
};

class DLfunction : public DLexpression {
    std::string arg_id;
    DLexpression *f_body;
    env myEnv;
public:
    DLfunction(std::string id, DLexpression *ex);
    ~DLfunction();
    std::string id();
    DLexpression *body(env l_env);
    DLexpression *eval(env environment = env());
};

class DLcall : public DLexpression {
    DLexpression *f_expr;
    DLexpression *arg_expr;
public:
    DLcall(DLexpression *e1, DLexpression *e2);
    ~DLcall();
    DLexpression *eval(env environment = env());
};

DLexpression* scan(std::string str);

#endif /* DL_h */