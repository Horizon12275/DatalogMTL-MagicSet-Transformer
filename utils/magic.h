#ifndef MAGIC_H
#define MAGIC_H
#include <iostream>
#include <stack>
#include <set>
#include <vector>
#include "rule.h"

class MagicSet {
public:
    stack<Literal> S;
    set<Rule> adornedRules;
    set<Rule> modifiedRules;
    set<Rule> magicRules;

    set<Rule> MS(Literal Q, vector<Rule> P) {
        modifiedRules.clear();
        magicRules = BuildQuerySeeds(Q, S);

        // while (!S.empty()) {
        //     Literal pAlpha = S.top();
        //     S.pop();

        //     for (Rule& r : P) {
        //         if (r.head.get_predicate() == pAlpha.get_predicate()) {
        //             //adornedRules.insert(Adorn(r, pAlpha, S));
        //         }
        //     }
        // }

        // for (const Rule& ra : adornedRules) {
        //     //magicRules.insert(Generate(ra));
        // }

        // for (const Rule& ra : adornedRules) {
        //     //modifiedRules.insert(Modify(ra));
        // }

        return magicRules;
    }

private:
    set<Rule> BuildQuerySeeds(Literal query, stack<Literal>& S) {
        set<Rule> magicRules;

        // // Adorn the query literal
        // Literal adornedQuery = adornLiteral(query);
        // S.push(adornedQuery);

        // // Generate magic seed rule for the adorned query literal
        // Rule magicSeedRule;
        // magicSeedRule.head = adornedQuery;
        // magicRules.insert(magicSeedRule);

        return magicRules;
    }

    // Function to adorn a literal with binding information
    Literal adornLiteral(Literal literal) {
        Literal adornedLiteral;
        // adornedLiteral.atom = literal.atom;

        // // Adorn each argument of the atom based on its type (constant or variable)
        // for (size_t i = 0; i < literal.atom.entity.size(); ++i) {
            
        // }

        return adornedLiteral;
    }

    set<Rule> Adorn(Rule r, Literal pAlpha, stack<Literal> &S)
    {
        set<Rule> adornedRules;

        // // Extract the head of the rule r
        // Literal headLiteral = r.head;

        // // Check if the headLiteral matches with the adorned predicate pAlpha
        // if (headLiteral.atom.predicate == pAlpha.atom.predicate) {
        //     // Adorn the arguments of the headLiteral based on the adornment of pAlpha
        //     vector<Term> adornedEntity;
        //     for (size_t i = 0; i < pAlpha.atom.entity.size(); ++i) {
        //         Term term = pAlpha.atom.entity[i];
        //         if (term.type == "variable") {
        //             adornedEntity.push_back(term); // If it is a variable, keep it as it is
        //         }
        //         else {
        //             adornedEntity.push_back(headLiteral.atom.entity[i]); // If it is a constant, match it with the headLiteral's argument
        //         }
        //     }

        //     // Create a new adorned headLiteral with the adorned arguments
        //     Literal adornedHeadLiteral = Literal(Atom(pAlpha.atom.predicate, adornedEntity));

        //     // Create a new adorned rule with the adorned head and the same body
        //     //Rule adornedRule = Rule(adornedHeadLiteral, r.body);

        //     // Push the adorned headLiteral into the stack S for further processing
        //     S.push(adornedHeadLiteral.atom);

        //     // Add the adorned rule to the set of adorned rules
        //     //adornedRules.insert(adornedRule);
        // }

        return adornedRules;
    }

    vector<Rule> Generate(const Rule &adorned_rule)
    {
        vector<Rule> magic_rules;

        // // 针对输入的adorned_rule生成magic rules
        // for (const Base* base : adorned_rule.body) {
        //     const Literal* adorned_literal = dynamic_cast<const Literal*>(base);
        //     if (adorned_literal) {
        //         Rule magic_rule;
        //         Literal magic_literal = GenerateMagicLiteral(*adorned_literal);
        //         magic_rule.head = magic_literal;
        //         magic_rule.body.push_back(new Literal(adorned_rule.head));

        //         // 将EDB atoms添加到magic rule的body中
        //         for (const Base* body_base : adorned_rule.body) {
        //             const Literal* body_literal = dynamic_cast<const Literal*>(body_base);
        //             if (body_literal && IsEDBAtom(*body_literal)) {
        //                 magic_rule.body.push_back(new Literal(*body_literal));
        //             }
        //         }

        //         magic_rules.push_back(magic_rule);
        //     }
        // }

        return magic_rules;
    }

    Literal GenerateMagicLiteral(const Literal& adorned_literal) {
        // 生成magic version的adorned literal
        Literal magic_literal = adorned_literal;
        // magic_literal.atom.predicate = "magic_" + adorned_literal.atom.predicate;

        // // 去除free variables的operator
        // vector<Operator> updated_operators;
        // for (const Operator& op : magic_literal.operators) {
        //     if (op.interval.left_value != -1 || op.interval.right_value != -1) {
        //         updated_operators.push_back(op);
        //     }
        // }
        // magic_literal.operators = updated_operators;

        return magic_literal;
    }

    bool IsEDBAtom(const Literal& literal) {
        // 检查是否是EDB atom
        return 0;
    }

    Rule Modify(const Rule& adorned_rule) {
        Rule modified_rule;

        // // Copy the head from the adorned rule
        // modified_rule.head = adorned_rule.head;

        // // Insert magic atom in the body of the rule
        // Literal magic_head;
        // Atom magic_atom;
        // magic_atom.predicate = "magic_" + adorned_rule.head.atom.predicate;

        // // Remove adornments from all other predicates in the body
        // for (auto base_ptr : adorned_rule.body) {
        //     if (Literal* literal_ptr = dynamic_cast<Literal*>(base_ptr)) {
        //         Literal literal = *literal_ptr;
        //         Literal modified_literal = literal;
        //         modified_literal.operators.clear();  // Clear operators to remove adornments

        //         // Check if the literal is the head, if yes add magic atom
        //         if (literal.atom.predicate == adorned_rule.head.atom.predicate) {
        //             Operator magic_op;
        //             magic_op.name = "magic";
        //             modified_literal.operators.push_back(magic_op);
        //         }

        //         modified_rule.body.push_back(new Literal(modified_literal));
        //     }
        // }
        return modified_rule;
    }
};

#endif