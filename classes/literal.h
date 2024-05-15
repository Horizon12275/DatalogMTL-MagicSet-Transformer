#ifndef LITERAL_H
#define LITERAL_H

#include <string>
#include <vector>
#include <interval.h>
#include <atom.h>

using namespace std;

class Operator {
public:
    string name;
    Interval interval;

    Operator() = default;
    Operator(string name, Interval interval);
    bool __eq__(Operator other);
    bool operator == (const Operator & other) const;
    string __str__();

    Operator(const Operator& other);
    Operator& operator = (const Operator & other);
};



class Literal : public Base{
public:
    Atom atom;
    vector<Operator> operators;

    Literal();
    Literal(Atom atom);
    Literal(Atom atom, vector<Operator> operators);

    string get_predicate();
    vector<Term> get_entity();
    string get_op_name();
    bool set_entity(vector<Term> entity); 
    bool __eq__(Literal other);
    bool operator == (const Literal & other) const;
    string __str__();
    string __str_without_interval__();
    size_t __hash__();

    Literal(const Literal& other);
    Literal& operator = (const Literal & other);
};

class BinaryLiteral: public Base {
public:
   Atom left_atom;
    Atom right_atom;
    Operator op;

    BinaryLiteral();
    BinaryLiteral(Atom left_atom, Atom right_atom, Operator op);
    string get_op_name();
    bool __eq__(BinaryLiteral other);
    bool operator == (const BinaryLiteral & other) const;
    string __str__();
    size_t __hash__();

    BinaryLiteral(const BinaryLiteral& other);
    BinaryLiteral& operator = (const BinaryLiteral & other);
};

#endif