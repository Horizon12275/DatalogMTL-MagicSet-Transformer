#ifndef RULE_H
#define RULE_H

#include <string>
#include <vector>
#include <atom.h>
#include <literal.h>

using namespace std;

class Rule {
public:
    //Atom head;
    //MY CHANGE: change atom to literal
    Literal head; 
    vector<Base*> body;   // compatible with both Literal and BinaryLiteral

    Rule() = default;
    string __str__();
    Rule(const Rule &other);
    Rule &operator=(const Rule &other);
    // Rule(Atom head, vector<Literal> body_literal);
    // Rule(Atom head, vector<BinaryLiteral> body_binary);

};


#endif