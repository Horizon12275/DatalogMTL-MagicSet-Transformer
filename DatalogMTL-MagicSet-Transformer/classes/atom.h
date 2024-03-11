#ifndef ATOM_H
#define ATOM_H

#include <string>
#include <vector>
#include <term.h>
#include <interval.h>

using namespace std;

class Base {
public:
    int type;

    Base(int type) {
        this->type = type;
    }

    virtual ~Base() {} // 添加虚析构函数以启用多态

    Base(const Base & other) {
        this->type = other.type;
    }

    Base& operator = (const Base & other) {
        this->type = other.type;
        return *this;
    }
};

class Atom : public Base {
public:
    string predicate = "";
    vector<Term> entity = vector<Term>();
    Interval interval = Interval();

    Atom() : Base(2) {};
    Atom(string predicate);
    Atom(string predicate, vector<Term> entity);
    Atom(string predicate, Interval interval);
    Atom(string predicate, vector<Term> entity, Interval interval);

    bool __eq__(Atom other);
    bool operator == (const Atom & other) const;
    string __str__();
    size_t __hash__();

    string get_predicate();
    vector<Term> get_entity();
    bool set_entity(vector<Term> entity);

    Atom(const Atom& other);
    Atom& operator = (const Atom & other);
};

#endif