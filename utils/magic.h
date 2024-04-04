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

    vector<Rule> MS(Literal Q, vector<Rule> P) {
        modifiedRules.clear();
        adornedRules.clear();
        magicRules.clear();
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
            for (Rule& r : P) {
                if (r.head.get_predicate() == pAlpha.get_predicate()) {
                    adornedRules.push_back(Adorn(r, pAlpha, S));
                }
            }
        }

        // for (const Rule& ra : adornedRules) {
        //     //magicRules.insert(Generate(ra));
        // }

        // for (const Rule& ra : adornedRules) {
        //     //modifiedRules.insert(Modify(ra));
        // }

        // print results
        printMagicFact();
        printMagicRules();

        return magicRules;
    }

private:
    void BuildQuerySeeds(Literal query, stack<Literal>& S) {
        // build query seeds
        S.push(query);
        // generate magic fact
        // if the term in the query is a constant, then the corresponding char of bfList is b, which means bound variable
        // if the term in the query is a variable, then the corresponding char of bfList is f, which means free variable
        magicFact = query;
        magicFact.atom.predicate = "magic_" + magicFact.atom.predicate + "_";
        magicFact.atom.isMagic = true;
        for (Term& term : magicFact.get_entity()) {
            if (term.get_type() == "variable") {
                magicFact.atom.bflist.push_back('f');
                magicFact.atom.predicate += "f";
            } else {
                magicFact.atom.bflist.push_back('b');
                magicFact.atom.predicate += "b";
            }
        }
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
        adornedRule.head.atom.predicate = adornedRule.head.atom.predicate + "_";
        for (Term &term : pAlpha.get_entity())
        {
            if (term.get_type() == "variable")
            {
                adornedRule.head.atom.bflist.push_back('f');
                adornedRule.head.atom.predicate += "f";
            }
            else
            {
                adornedRule.head.atom.bflist.push_back('b');
                adornedRule.head.atom.predicate += "b";
            }
        }

        //initialize the appearedTerms set
        for (Term &term : adornedRule.head.get_entity())
        {
            appearedTerms.insert(term.name);
        }

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
                cout << "Atom: " << atomPtr->__str__() << endl;
                atomPtr->predicate = atomPtr->predicate + "_";
                for (Term &term : atomPtr->entity)
                {
                    if (appearedTerms.find(term.name) != appearedTerms.end())
                    {
                        atomPtr->bflist.push_back('b');
                        atomPtr->predicate += "b";
                    }
                    else
                    {
                        atomPtr->bflist.push_back('f');
                        atomPtr->predicate += "f";
                    }
                }
                for (Term &term : atomPtr->entity)
                {
                    appearedTerms.insert(term.name);
                }
            }
            else if (Literal *literalPtr = dynamic_cast<Literal *>(basePtr))
            {                               // check if the pointer points to Atom or Literal, and do the corresponding type conversion
                cout << "Literal: " << literalPtr->__str__() << endl;
                literalPtr->atom.predicate = literalPtr->atom.predicate + "_";
                for (Term &term : literalPtr->atom.entity)
                {
                    if (appearedTerms.find(term.name) != appearedTerms.end())
                    {
                        literalPtr->atom.bflist.push_back('b');
                        literalPtr->atom.predicate += "b";
                    }
                    else
                    {
                        literalPtr->atom.bflist.push_back('f');
                        literalPtr->atom.predicate += "f";
                    }
                }
                for (Term &term : literalPtr->atom.entity)
                {
                    appearedTerms.insert(term.name);
                }
            }
        }

        std::cout << "Adorned Rule: " << adornedRule.__str__() << std::endl;
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
        vector<Rule> magic_rules;

        return magic_rules;
    }

    Rule Modify(const Rule& adorned_rule) {
        Rule modified_rule;

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
};

#endif