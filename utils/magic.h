#ifndef MAGIC_H
#define MAGIC_H
#include <iostream>
#include <stack>
#include <set>
#include <vector>
#include "rule.h"

class MagicSet {
public:
    Literal magicFact;
    stack<Literal> S;
    vector<Rule> adornedRules;
    vector<Rule> modifiedRules;
    vector<Rule> magicRules;
    set<string> IDBList;
    set<string> adornedHistory;

    vector<Rule> MS(Literal Q, vector<Rule> P) {
        modifiedRules.clear();
        adornedRules.clear();
        magicRules.clear();
        IDBList.clear();
        adornedHistory.clear();
        magicFact = Literal();
        S = stack<Literal>();

        BuildQuerySeeds(Q, S); // Build query seeds and generate magicFact
        BuildIDBList(P); // Build IDB list
        PrintIDBList();

        // cout << "Query: " << Q.__str__() << endl;
        // cout << "Query Seeds: " << endl;
        // while (!S.empty()) {
        //     cout << S.top().__str__() << endl;
        //     S.pop();
        // }
        // for (Rule& r : P) {
        //     cout << "Rule: " << r.__str__() << endl;
        // }

        while (!S.empty()) {
            Literal pAlpha = S.top();
            S.pop();
            if (adornedHistory.find(pAlpha.__str_without_interval__()) == adornedHistory.end()){
                for (Rule &r : P)
                {
                    // printAdornedHistory();
                    if (r.head.get_predicate() == pAlpha.get_predicate())
                    {
                        adornedRules.push_back(Adorn(r, pAlpha, S));
                        //printAdornedHistory();
                    }
                }
            }
        }

        // generate magic rules
        for (const Rule& ra : adornedRules) {
            vector<Rule> generatedRules = Generate(ra);
            for (const Rule& rg : generatedRules) {
                magicRules.push_back(rg);
            }
        }

        // modify the adorned rules
        for (const Rule& ra : adornedRules) {
            magicRules.push_back(Modify(ra));
        }

        // print results
        printMagicFact();
        printMagicRules();

        return magicRules;
    }

private:
    void BuildQuerySeeds(Literal query, stack<Literal>& S) {
        // generate magic fact
        // if the term in the query is a constant, then the corresponding char of bfList is b, which means bound variable
        // if the term in the query is a variable, then the corresponding char of bfList is f, which means free variable
        magicFact = query;
        //magicFact.atom.predicate = "magic_" + magicFact.atom.predicate + "_";
        
        for (Term& term : magicFact.get_entity()) {
            if (term.get_type() == "variable") {
                magicFact.atom.bflist.push_back('f');
                //magicFact.atom.predicate += "f";
            } else {
                magicFact.atom.bflist.push_back('b');
                //magicFact.atom.predicate += "b";
            }
        }
        // build query seeds
        S.push(magicFact);

        magicFact.atom.isMagic = true;
        //printMagicFact();
    }

    Rule Adorn(Rule r, Literal pAlpha, stack<Literal> &S)
    {
        Rule adornedRule; // used to record the adorned rule
        set<string> appearedTerms; // used to record the terms that have appeared in the adorned rule

        // adorn the rule according to the bflist of pAlpha and by SIPS method
        // if the term in the rule is a constant, then the corresponding char of bfList is b, which means bound variable
        // if the term in the rule is a variable, then the corresponding char of bfList is f, which means free variable

        //intialize the adorned rule
        adornedRule = r;

        //std::cout << "Rule: " << r.__str__() << std::endl;

        //transform the head of the rule according to the bflist of pAlpha
        //adornedRule.head.atom.predicate = adornedRule.head.atom.predicate + "_";
        for (Term &term : pAlpha.get_entity())
        {
            if (term.get_type() == "variable")
            {
                adornedRule.head.atom.bflist.push_back('f');
                //adornedRule.head.atom.predicate += "f";
            }
            else
            {
                adornedRule.head.atom.bflist.push_back('b');
                //adornedRule.head.atom.predicate += "b";
            }
        }

        //initialize the appearedTerms set
        for (Term &term : adornedRule.head.get_entity())
        {
            appearedTerms.insert(term.name);
        }

        adornedHistory.insert(pAlpha.__str_without_interval__());

        // //print appearedTerms
        // for (string term : appearedTerms)
        // {
        //     std::cout << term << " ";
        // }

        //transform the body of the rule
        //SIPS method: 
        //from left to right, check each atom or literal in the body of the rule
        //if a term in the current atom or literal has appeared in the appearedTerms set, then it is a bound variable, otherwise it is a free variable
        //after checking an atom or a literal, add all the terms in it to the appearedTerms set
        for (int i = 0; i < adornedRule.body.size(); i++)
        {
            Base *basePtr = adornedRule.body[i]; // get the pointer of the base in the body
            if (Atom *atomPtr = dynamic_cast<Atom *>(basePtr))
            {                                // check if the pointer points to Atom or Literal, and do the corresponding type conversion
                //cout << "Atom: " << atomPtr->__str__() << endl;

                //atomPtr->predicate = atomPtr->predicate + "_";
                for (Term &term : atomPtr->entity)
                {
                    if (appearedTerms.find(term.name) != appearedTerms.end())
                    {
                        atomPtr->bflist.push_back('b');
                        //atomPtr->predicate += "b";
                    }
                    else
                    {
                        atomPtr->bflist.push_back('f');
                        //atomPtr->predicate += "f";
                    }
                }

                // before insert into adornedHistory, we need to revise the type of term in the literal according to its bfList
                for (int i = 0; i < atomPtr->entity.size(); i++)
                {
                    if (atomPtr->bflist[i] == 'b')
                    {
                        atomPtr->entity[i].set_type("constant");
                    }
                    else
                    {
                        atomPtr->entity[i].set_type("variable");
                    }
                }
                S.push(Literal(*atomPtr));
                //adornedHistory.insert(atomPtr->__str__());
                //printAdornedHistory();

                for (Term &term : atomPtr->entity)
                {
                    appearedTerms.insert(term.name);
                }
            }
            else if (Literal *literalPtr = dynamic_cast<Literal *>(basePtr))
            {                               // check if the pointer points to Atom or Literal, and do the corresponding type conversion
                //cout << "Literal: " << literalPtr->__str__() << endl;
                //literalPtr->atom.predicate = literalPtr->atom.predicate + "_";
                for (Term &term : literalPtr->atom.entity)
                {
                    if (appearedTerms.find(term.name) != appearedTerms.end())
                    {
                        literalPtr->atom.bflist.push_back('b');
                        //literalPtr->atom.predicate += "b";
                    }
                    else
                    {
                        literalPtr->atom.bflist.push_back('f');
                        //literalPtr->atom.predicate += "f";
                    }
                }

                // before insert into adornedHistory, we need to revise the type of term in the literal according to its bfList
                for (int i = 0; i < literalPtr->atom.entity.size(); i++)
                {
                    if (literalPtr->atom.bflist[i] == 'b')
                    {
                        literalPtr->atom.entity[i].set_type("constant");
                    }
                    else
                    {
                        literalPtr->atom.entity[i].set_type("variable");
                    }
                }
                S.push(*literalPtr);
                //adornedHistory.insert(literalPtr->__str_without_interval__());
                //printAdornedHistory();

                for (Term &term : literalPtr->atom.entity)
                {
                    appearedTerms.insert(term.name);
                }
            }
        }

        //std::cout << "Adorned Rule: " << adornedRule.__str__() << std::endl;
        return adornedRule;
    }

    void BuildIDBList(vector<Rule> P)
    {
        for (Rule &r : P)
        {
            IDBList.insert(r.head.get_predicate());
        }
    }

    void PrintIDBList()
    {
        std::cout << "IDB List: " << std::endl;
        for (string idb : IDBList)
        {
            std::cout << idb << " ";
        }
        std::cout << std::endl;
    }

    vector<Rule> Generate(const Rule &adorned_rule)
    {
        vector<Rule> generated_rules;
        // scan the body of the adorned rule, if the predicate of the atom is in the IDB list, then generate a new rule
        // generate method:
        // create a pointer to the body of the adorned rule, when the pointer points to an IDB rule,
        // then create a new rule, the head of the new rule is currently pointed atom or literal after magic(isMagic = true),
        // the first atom or literal in the new body is the head of the adorned rule after magic(isMagic = true),
        // the rest of this generated rule's body contains all the atoms or literals ahead of the pointer (already scanned) in the body of the adorned rule
        for(int i = 0; i < adorned_rule.body.size(); i++) {
            Base* basePtr = adorned_rule.body[i];
            if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
                // cout << "Atom: " << atomPtr->__str__() << endl;
                if (IDBList.find(atomPtr->predicate) != IDBList.end()) {
                    //cout << "IDB: " << atomPtr->predicate << endl;
                    Rule new_rule;
                    new_rule.head = Literal(*atomPtr);
                    new_rule.head.atom.isMagic = true;
                    // if the operator is diamondminus, then change it to boxminus
                    // if the operator is diamondplus, then change it to boxplus
                    if (new_rule.head.get_op_name() == "Diamondminus") {
                        new_rule.head.operators[0].name = "Boxminus";
                    } else if (new_rule.head.get_op_name() == "Diamondplus") {
                        new_rule.head.operators[0].name = "Boxplus";
                    }
                    Literal *new_body_head = new Literal(adorned_rule.head);
                    new_body_head->atom.isMagic = true;
                    new_rule.body.push_back(new_body_head);
                    // if the operator is boxminus, then change it to diamondminus
                    // if the operator is boxplus, then change it to diamondplus
                    if (new_body_head->get_op_name() == "Boxminus") {
                        new_body_head->operators[0].name = "Diamondminus";
                    } else if (new_body_head->get_op_name() == "Boxplus") {
                        new_body_head->operators[0].name = "Diamondplus";
                    }
                    // copy the body of the adorned rule to the body of the new rule (already scaned literals or atoms)
                    for(int j = 0; j < i; j++) {
                        Base* basePtr = adorned_rule.body[j];
                        if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
                            Atom* new_atom = new Atom(*atomPtr);
                            new_rule.body.push_back(new_atom);
                        } else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) {
                            Literal* new_literal = new Literal(*literalPtr);
                            new_rule.body.push_back(new_literal);
                        }
                    }

                    generated_rules.push_back(new_rule);
                }
            } else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) {
                // cout << "Literal: " << literalPtr->__str__() << endl;
                if (IDBList.find(literalPtr->atom.predicate) != IDBList.end()) {
                    //cout << "IDB: " << literalPtr->atom.predicate << endl;
                    Rule new_rule;
                    new_rule.head = Literal(literalPtr->atom);
                    new_rule.head.operators = literalPtr->operators;
                    new_rule.head.atom.isMagic = true;
                    // if the operator is diamondminus, then change it to boxminus
                    // if the operator is diamondplus, then change it to boxplus
                    if (new_rule.head.get_op_name() == "Diamondminus") {
                        new_rule.head.operators[0].name = "Boxminus";
                    } else if (new_rule.head.get_op_name() == "Diamondplus") {
                        new_rule.head.operators[0].name = "Boxplus";
                    }
                    Literal *new_body_head = new Literal(adorned_rule.head);
                    new_body_head->atom.isMagic = true;
                    new_rule.body.push_back(new_body_head);
                    // if the operator is boxminus, then change it to diamondminus
                    // if the operator is boxplus, then change it to diamondplus
                    if (new_body_head->get_op_name() == "Boxminus") {
                        new_body_head->operators[0].name = "Diamondminus";
                    } else if (new_body_head->get_op_name() == "Boxplus") {
                        new_body_head->operators[0].name = "Diamondplus";
                    }
                    // copy the body of the adorned rule (already scaned literals or atoms)
                    for(int j = 0; j < i; j++) {
                        Base* basePtr = adorned_rule.body[j];
                        if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
                            Atom* new_atom = new Atom(*atomPtr);
                            new_rule.body.push_back(new_atom);
                        } else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) {
                            Literal* new_literal = new Literal(*literalPtr);
                            new_rule.body.push_back(new_literal);
                        }
                    }

                    generated_rules.push_back(new_rule);
                }
            }
        }
        // cout << "Generated Rules: " << endl;
        // for (Rule& rule : generated_rules) {
        //     cout << rule.__str__() << endl;
        // }
        return generated_rules;
    }

    Rule Modify(const Rule& adorned_rule) {
        Rule modified_rule;
        // moddify the rule : 
        // create a new literal with the same predicate and terms as the head of the adorned rule, and add magic to the predicate(isMagic = true)
        // then push the new literal to the first of the body vector of the adorned rule
        modified_rule.head = adorned_rule.head;
        Literal* new_head = new Literal(adorned_rule.head);
        new_head->atom.isMagic = true;
        // if the operator is boxminus, then change it to diamondminus
        // if the operator is boxplus, then change it to diamondplus
        if (new_head->get_op_name() == "Boxminus") {
            new_head->operators[0].name = "Diamondminus";
        } else if (new_head->get_op_name() == "Boxplus") {
            new_head->operators[0].name = "Diamondplus";
        }
        modified_rule.body.push_back(new_head);
        // copy the body of the adorned rule to the body of the modified rule
        for(Base* basePtr : adorned_rule.body) {
            if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
                Atom* new_atom = new Atom(*atomPtr);
                modified_rule.body.push_back(new_atom);
            } else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) {
                Literal* new_literal = new Literal(*literalPtr);
                modified_rule.body.push_back(new_literal);
            }
        }
        //cout << "Modified Rule: " << modified_rule.__str__() << endl;
        return modified_rule;
    }

    void printMagicFact() {
        std::cout << "Magic Fact: " << magicFact.__str__() << std::endl;
    }

    void printMagicRules() {
        std::cout << "Magic Rules: " << std::endl;
        for (Rule& rule : magicRules) {
            std::cout << rule.__str__() << std::endl;
        }
    }

    void printAdornedHistory() {
        std::cout << "Adorned History: " << std::endl;
        for (string adorned : adornedHistory) {
            std::cout << adorned << std::endl;
        }
    }
};

#endif