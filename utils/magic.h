#ifdef _HAS_STD_BYTE
#undef _HAS_STD_BYTE
#endif
#define _HAS_STD_BYTE 0

#ifndef MAGIC_H
#define MAGIC_H
#include <iostream>
#include <stack>
#include <set>
#include <vector>
#include <windows.h>
#include <conio.h>
#include "rule.h"
#include <atom.h>

extern HANDLE hConsole;

class MagicSet {
private:
    Literal magicFact;
    stack<Literal> S;
    vector<Rule> adornedRules;
    vector<Rule> modifiedRules;
    vector<Rule> magicRules;
    set<string> IDBList;
    set<string> adornedHistory;
public:
    vector<Rule> MS(Literal Q, vector<Rule> P) {
        clear();

        BuildQuerySeeds(Q, S); // Build query seeds and generate magicFact
        BuildIDBList(P); // Build IDB list
        // PrintIDBList();

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

        clearEDBbflist();

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
        printOriginalQuery(Q);
        printOriginalProgram(P);
        printAdornedRules();
        printMagicFact();
        printMagicProgram();

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
            //cout << term.name << " " << term.get_type() << endl;
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

    void adornAtom(Atom* atomPtr, set<string>& appearedTerms)
    {
        //cout << "Atom: " << atomPtr->__str__() << endl;
        //atomPtr->predicate = atomPtr->predicate + "_";
        for (Term& term : atomPtr->entity)
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
	}

    void addTermsToAppearedTerms(Atom* atomPtr, set<string>& appearedTerms) {
        if (IDBList.find(atomPtr->predicate) == IDBList.end()) {
            for (Term& term : atomPtr->entity) {
				appearedTerms.insert(term.name);
			}
		}
        else {
            for (int i = 0; i < atomPtr->entity.size(); i++) {
				Term term = atomPtr->entity[i];
				if (atomPtr->bflist[i] == 'b')
					appearedTerms.insert(term.name);
			}
		}
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
        /*for (Term &term : adornedRule.head.get_entity())
        {
            appearedTerms.insert(term.name);
        }*/

        auto entity = adornedRule.head.get_entity();

        for (int i = 0; i < entity.size(); i++)
        {
            Term term = entity[i];

            if (adornedRule.head.atom.bflist[i] == 'b')
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
            Base* basePtr = adornedRule.body[i]; // get the pointer of the base in the body
            if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr))
            {                                // check if the pointer points to Atom or Literal, and do the corresponding type conversion
                adornAtom(atomPtr, appearedTerms); 
                S.push(Literal(*atomPtr));
                //adornedHistory.insert(atomPtr->__str__());
                //printAdornedHistory();

                addTermsToAppearedTerms(atomPtr, appearedTerms);

                /*for (int i = 0; i < (atomPtr->entity).size(); i++)
                {
                    Term term = atomPtr->entity[i];

                    if ((atomPtr->bflist)[i] == 'b')
                        appearedTerms.insert(term.name);
                }*/
            }
            else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr))
            {                           
                adornAtom(&(literalPtr->atom), appearedTerms);
                S.push(*literalPtr);
                //adornedHistory.insert(literalPtr->__str_without_interval__());
                //printAdornedHistory();

                /*for (Term &term : literalPtr->atom.entity)
                {
                    appearedTerms.insert(term.name);
                }*/

                addTermsToAppearedTerms(&(literalPtr->atom), appearedTerms);

            }
            else if (BinaryLiteral* binaryLiteralPtr = dynamic_cast<BinaryLiteral*>(basePtr))
            {
                Atom* left_atom_ptr = &(binaryLiteralPtr->left_atom);
                adornAtom(left_atom_ptr, appearedTerms);
                S.push(Literal(*left_atom_ptr));

                Atom* right_atom_ptr = &(binaryLiteralPtr->right_atom);
                adornAtom(right_atom_ptr, appearedTerms);
                S.push(Literal(*right_atom_ptr));

                addTermsToAppearedTerms(&(binaryLiteralPtr->left_atom), appearedTerms);
                addTermsToAppearedTerms(&(binaryLiteralPtr->right_atom), appearedTerms);
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

    void clearTermWithf(Atom* atom) {
        auto bfIter = atom->bflist.begin();
        auto entityIter = atom->entity.begin();

        bool existB = false;

        while (bfIter != atom->bflist.end()) {
            if (*bfIter == 'f') {
                entityIter = atom->entity.erase(entityIter);
            }
            else {
                existB = true;
                ++entityIter;
            }
            ++bfIter;
        }

        if (existB) {

            auto bfIter = atom->bflist.begin();

            while (bfIter != atom->bflist.end()) {
                if (*bfIter == 'f') {
                    bfIter = atom->bflist.erase(bfIter);
                    atom->bflist_size--;
                }
                else {
                    ++bfIter;
                }
            }
        }

    }

    void MagicHead2Body(Literal& l) {
        l.atom.isMagic = true;
        // if the operator is boxminus, then change it to diamondminus
        // if the operator is boxplus, then change it to diamondplus
        if (l.get_op_name() == "Boxminus") {
            l.operators[0].name = "Diamondminus";
        }
        else if (l.get_op_name() == "Boxplus") {
            l.operators[0].name = "Diamondplus";
        }

        Atom* atom = &(l.atom);
        clearTermWithf(atom);
        
    }

    void MagicBody2Head(Literal& l) {
        l.atom.isMagic = true;
        // if the operator is diamondminus, then change it to boxminus
        // if the operator is diamondplus, then change it to boxplus
        if (l.get_op_name() == "Diamondminus") {
            l.operators[0].name = "Boxminus";
        }
        else if (l.get_op_name() == "Diamondplus") {
            l.operators[0].name = "Boxplus";
        }

        Atom* atom = &(l.atom);
        clearTermWithf(atom);
    }

    void MagicBody2HeadForBinary(BinaryLiteral& binaryLiteral, vector<Literal> &list) {
        string op_name;

        if (binaryLiteral.get_op_name() == "Since")
            op_name = "Boxminus";
        else 
            op_name = "Boxplus";

		Literal l;
		l.atom = binaryLiteral.left_atom;
		l.atom.isMagic = true;

        Interval new_interval = binaryLiteral.op.interval;
        new_interval.left_value = 0;
        new_interval.left_open = false;
        Operator LOp = Operator(op_name, new_interval);
		l.operators.push_back(LOp);

		Atom* atom = &(l.atom);
		clearTermWithf(atom);
		list.push_back(l);

		Literal r;
		r.atom = binaryLiteral.right_atom;
		r.atom.isMagic = true;

        Operator ROp = Operator(op_name, binaryLiteral.op.interval);
        r.operators.push_back(ROp);

        Atom* atom2 = &(r.atom);
        clearTermWithf(atom2);
        list.push_back(r);
	}

    void pushEDBBasePtrToRuleBody(Rule& rule, Base* basePtr) {
		if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
            if (IDBList.find(atomPtr->predicate) == IDBList.end()) {
                Atom* new_atom = new Atom(*atomPtr);
                rule.body.push_back(new_atom);
            }
		}
		else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) {
            if (IDBList.find(literalPtr->atom.predicate) == IDBList.end()) {
                Literal* new_literal = new Literal(*literalPtr);
                rule.body.push_back(new_literal);
            }
		}
        else if (BinaryLiteral* binaryLiteralPtr = dynamic_cast<BinaryLiteral*>(basePtr)) {
            if (IDBList.find(binaryLiteralPtr->left_atom.predicate) == IDBList.end() && IDBList.find(binaryLiteralPtr->right_atom.predicate) == IDBList.end()) {
                BinaryLiteral* new_binary_literal = new BinaryLiteral(*binaryLiteralPtr);
                rule.body.push_back(new_binary_literal);
            }
		}
	}

    vector<Rule> Generate(const Rule &adorned_rule)
    {
        vector<Rule> generated_rules;
        // scan the body of the adorned rule, if the predicate of the atom is in the IDB list, then generate a new rule
        // generate method:
        // create a pointer to the body of the adorned rule, when the pointer points to an IDB rule,
        // then create a new rule, the head of the new rule is currently pointed atom or literal after magic(isMagic = true),
        // the first atom or literal in the new body is the head of the adorned rule after magic(isMagic = true),
        // the rest of this generated rule's body contains all the EDB atoms or literals ahead of the pointer (already scanned) in the body of the adorned rule
        for(int i = 0; i < adorned_rule.body.size(); i++) {
            Base* basePtr = adorned_rule.body[i];
            if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
                // cout << "Atom: " << atomPtr->__str__() << endl;
                if (IDBList.find(atomPtr->predicate) != IDBList.end()) {
                    //cout << "IDB: " << atomPtr->predicate << endl;
                    Rule new_rule;
                    new_rule.head = Literal(*atomPtr);
                    MagicBody2Head(new_rule.head);

                    Literal *new_body_head = new Literal(adorned_rule.head);
                    MagicHead2Body(*new_body_head);
                    new_rule.body.push_back(new_body_head);
                    
                    // copy the body of the adorned rule to the body of the new rule (already scaned literals or atoms)
                    for(int j = 0; j < i; j++) {
                        Base* basePtr = adorned_rule.body[j];
                        pushEDBBasePtrToRuleBody(new_rule, basePtr);
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
                    MagicBody2Head(new_rule.head);

                    Literal *new_body_head = new Literal(adorned_rule.head);
                    MagicHead2Body(*new_body_head);
                    new_rule.body.push_back(new_body_head);
                    
                    // copy the body of the adorned rule (already scaned literals or atoms)
                    for(int j = 0; j < i; j++) {
                        Base* basePtr = adorned_rule.body[j];
                        pushEDBBasePtrToRuleBody(new_rule, basePtr);
                    }

                    generated_rules.push_back(new_rule);
                }
            }
            else if (BinaryLiteral* binaryLiteralPtr = dynamic_cast<BinaryLiteral*>(basePtr)) {
                if (IDBList.find(binaryLiteralPtr->left_atom.predicate) != IDBList.end() 
                    || IDBList.find(binaryLiteralPtr->right_atom.predicate) != IDBList.end()) {
                    vector<Literal> list;
                    MagicBody2HeadForBinary(*binaryLiteralPtr, list);

                    //cout << "IDB: " << literalPtr->atom.predicate << endl;
                    for (Literal& literal : list) {
                        Rule new_rule;
						new_rule.head = Literal(literal.atom);
						new_rule.head.operators = literal.operators;

						Literal *new_body_head = new Literal(adorned_rule.head);
						MagicHead2Body(*new_body_head);
						new_rule.body.push_back(new_body_head);
						
						// copy the body of the adorned rule (already scaned literals or atoms)
						for(int j = 0; j < i; j++) {
							Base* basePtr = adorned_rule.body[j];
							pushEDBBasePtrToRuleBody(new_rule, basePtr);
						}

						generated_rules.push_back(new_rule);
                    }
                }
            }
        }
        // cout << "Generated Rules: " << endl;
        // for (Rule& rule : generated_rules) {
        //     cout << rule.__str__() << endl;
        // }
        return generated_rules;
    }

    //Rule Modify(const Rule& adorned_rule) {
    //    Rule modified_rule;
    //    // moddify the rule : 
    //    // create a new literal with the same predicate and terms as the head of the adorned rule, and add magic to the predicate(isMagic = true)
    //    // then push the new literal to the first of the body vector of the adorned rule
    //    modified_rule.head = adorned_rule.head;
    //    Literal* new_head = new Literal(adorned_rule.head);
    //    new_head->atom.isMagic = true;
    //    // if the operator is boxminus, then change it to diamondminus
    //    // if the operator is boxplus, then change it to diamondplus
    //    if (new_head->get_op_name() == "Boxminus") {
    //        new_head->operators[0].name = "Diamondminus";
    //    } else if (new_head->get_op_name() == "Boxplus") {
    //        new_head->operators[0].name = "Diamondplus";
    //    }
    //    modified_rule.body.push_back(new_head);
    //    // copy the body of the adorned rule to the body of the modified rule
    //    for(Base* basePtr : adorned_rule.body) {
    //        if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
    //            Atom* new_atom = new Atom(*atomPtr);
    //            modified_rule.body.push_back(new_atom);
    //        } else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) {
    //            Literal* new_literal = new Literal(*literalPtr);
    //            modified_rule.body.push_back(new_literal);
    //        }
    //    }
    //    //cout << "Modified Rule: " << modified_rule.__str__() << endl;
    //    return modified_rule;
    //}

    Rule Modify(const Rule& adorned_rule) {
        Rule modified_rule;
        // moddify the rule : 
        // create a new literal with the same predicate and terms as the head of the adorned rule, and add magic to the predicate(isMagic = true)
        // then push the new literal to the first of the body vector of the adorned rule
        modified_rule.head = adorned_rule.head;
        Literal* new_head = new Literal(adorned_rule.head);
        modified_rule.head.atom.bflist.clear();
        MagicHead2Body(*new_head);
        modified_rule.body.push_back(new_head);
        // copy the body of the adorned rule to the body of the modified rule
        for (Base* basePtr : adorned_rule.body) {
            if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
                Atom* new_atom = new Atom(*atomPtr);
                new_atom->bflist.clear();
                modified_rule.body.push_back(new_atom);
            }
            else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) {
                Literal* new_literal = new Literal(*literalPtr);
                new_literal->atom.bflist.clear();
                modified_rule.body.push_back(new_literal);
            }
            else if (BinaryLiteral* binaryLiteralPtr = dynamic_cast<BinaryLiteral*>(basePtr)) {
				BinaryLiteral* new_binary_literal = new BinaryLiteral(*binaryLiteralPtr);
				new_binary_literal->left_atom.bflist.clear();
				new_binary_literal->right_atom.bflist.clear();
				modified_rule.body.push_back(new_binary_literal);
			}
        }
        //cout << "Modified Rule: " << modified_rule.__str__() << endl;
        return modified_rule;
    }

    void printMagicFact() {
        std::cout << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN); // set title color to green
        std::cout << "Magic Fact: " << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // set title color to white
        std::cout << "\t" << magicFact.__str__() << std::endl;

    }

    void printMagicProgram() {
        std::cout << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN); // set title color to green
        std::cout << "Magic Program: " << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // set title color to white
        for (Rule& rule : magicRules) {
            std::cout << "\t" << rule.__str__() << std::endl;
        }
    }

    void printAdornedRules() {
        std::cout << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN); // set title color to green
        std::cout << "Adorned Rules: " << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // set title color to white
        for (Rule& rule : adornedRules) {
            std::cout << "\t" << rule.__str__() << std::endl;
        }
    }

    void printAdornedHistory() {
        std::cout << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN); // set title color to green
        std::cout << "Adorned History: " << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // set title color to white
        for (string adorned : adornedHistory) {
            std::cout << adorned << std::endl;
        }
    }

    void printOriginalQuery(Literal Q) {
        std::cout << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN); // set title color to green
        std::cout << "Original Query: " << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // set title color to white
        std::cout << "\t" << Q.__str__() << std::endl;
    }

    void printOriginalProgram(vector<Rule> P) {
        std::cout << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_GREEN); // set title color to green
        std::cout << "Original Program: " << std::endl;
        SetConsoleTextAttribute(hConsole, FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE); // set title color to white
        for (Rule& rule : P) {
            std::cout << "\t" << rule.__str__() << std::endl;
        }
    }

    void clearEDBbflist() { // if the literal or atom is not in the IDB list, then clear the bflist of it
        for (Rule& rule : adornedRules) {
            if (IDBList.find(rule.head.get_predicate()) == IDBList.end()) {
                rule.head.atom.bflist.clear();
            }
            for (Base* basePtr : rule.body) {
                if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) {
                    if (IDBList.find(atomPtr->predicate) == IDBList.end()) {
                        atomPtr->bflist.clear();
                    }
                } else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) {
                    if (IDBList.find(literalPtr->atom.predicate) == IDBList.end()) {
                        literalPtr->atom.bflist.clear();
                    }
                } else if (BinaryLiteral* binaryLiteralPtr = dynamic_cast<BinaryLiteral*>(basePtr)) {
                    if (IDBList.find(binaryLiteralPtr->left_atom.predicate) == IDBList.end()) {
						binaryLiteralPtr->left_atom.bflist.clear();
					}
                    if (IDBList.find(binaryLiteralPtr->right_atom.predicate) == IDBList.end()) {
						binaryLiteralPtr->right_atom.bflist.clear();
					}
                }
            }
        }
    }

    void clear(){
        magicFact = Literal();
        while (!S.empty()) {
            S.pop();
        }
        adornedRules.clear();
        modifiedRules.clear();
        magicRules.clear();
        IDBList.clear();
        adornedHistory.clear();
    }
};

#endif