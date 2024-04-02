#ifndef PARSER_H
#define PARSER_H

#pragma warning(disable:4996)

#include <iostream>
#include <regex>
#include <cctype>
#include <rule.h>
#include <util.h>
#include <ctime>

using namespace std;

class LiteralUnion
{
public:
    size_t type;    // 0: Atom; 1: literal; 2: binary_literal
    Atom atom;
    Literal literal;
    BinaryLiteral binary_literal;

    LiteralUnion() = default;

    LiteralUnion(Atom atom) {
        this->atom = atom;
        this->type = 2;
    }
    LiteralUnion(Literal literal) {
        this->literal = literal;
        this->type = 0;
    }
    LiteralUnion(BinaryLiteral binary_literal) {
        this->binary_literal = binary_literal;
        this->type = 1;
    }

    Base* get_literal() {
        if (this->type == 2) 
            return &this->atom;
        else if (this->type == 0)
            return &this->literal;
        else 
            return &this->binary_literal;
    }

    LiteralUnion (const LiteralUnion & other) {
        this->atom = other.atom;
        this->literal = other.literal;
        this->binary_literal = other.binary_literal;
        this->type = other.type;
    }

    LiteralUnion& operator = (const LiteralUnion & other) {
        this->atom = other.atom;
        this->literal = other.literal;
        this->binary_literal = other.binary_literal;
        this->type = other.type;
        return *this;
    }
};

const string ATOM_PATTERN = "(.*)\\((.*)\\)";
const string FACT_WITH_ENTITY_PATTERN = "(.*)\\((.*)\\)@(.*)";
const string FACT_WITHOUT_ENTITY_PATTERN = "(.*)@(.*)";
const string INTERVAL_TWO_POINTS_PATTERN = "(^[\\(,\\[])(-?\\d+\\.?\\d*|-inf),(-?\\d+\\.?\\d*|\\+?inf)([\\),\\]])$";
const string INTERVAL_ONE_POINT_PATTERN =  "(^[\\(,\\[])(-?\\d+\\.?\\d*|\\+?inf)([\\),\\]])$";
const string OPERATOR_TWO_POINTS_PATTERN = "(.*)([\\(,\\[])(-?\\d+\\.?\\d*|-inf),(-?\\d+\\.?\\d*|\\+?inf)([\\),\\]])$";
const string OPERATOR_ONE_POINT_PATTERN =  "(.*)([\\(,\\[])(-?\\d+\\.?\\d*|-inf)([\\),\\]])$";

Atom parse_atom(string atom_str) {
    regex base_regex(ATOM_PATTERN);
    smatch base_match;
    regex_match(atom_str, base_match, base_regex);
    if (base_match.size() > 1) {
        string predicate = base_match[1];
        string entity = base_match[2];
        vector<string> entity_vec = split_str(entity, ",");
        vector<Term> entities;
        for (auto iter = entity_vec.begin(); iter != entity_vec.end(); iter ++) {
            string token = *iter;
            if (isupper(token[0]))
                entities.push_back(Term(token, "variable"));
            else
                entities.push_back(Term(token, "constant"));
        }
        return Atom(predicate, entities);
    }
    else
        return Atom(atom_str);
}

vector<string> parse_body(string body) {
    vector<string> literals;
    bool left = false;
    string current_str = "";
    for (auto iter = body.begin(); iter != body.end(); iter ++) {
        char item = *iter;
        if (item != ',') {
            if (item == '(' || item == '[')
                left = true;
            if (item == ')' || item == ']')
                left = false;
            current_str += item;
        }
        else {
            if (!left) {
                literals.push_back(current_str);
                current_str = "";
            }
            else
                current_str += item;
        }
    }
    literals.push_back(current_str);
    return literals;
}

Operator parse_operator(string operator_str) {
    regex two_points_regex(OPERATOR_TWO_POINTS_PATTERN);
    smatch two_points_match;
    regex_match(operator_str, two_points_match, two_points_regex);
    if (two_points_match.size() < 1) {
        regex one_point_regex(OPERATOR_ONE_POINT_PATTERN);
        smatch one_point_match;
        regex_match(operator_str, one_point_match, one_point_regex);
        if (one_point_match.size() < 1) {
            throw invalid_argument("invalid operator string.");
        }
        string left_value_str = one_point_match[3];
        string right_value_str = one_point_match[3];
        double left_value = stod(left_value_str);
        double right_value = stod(right_value_str);
        bool left_open = false, right_open = false;
        if (one_point_match[2] == "(")
            left_open = true;
        if (one_point_match[4] == ")")
            right_open = true;
        return Operator(one_point_match[1], 
                        Interval(left_value, right_value, left_open, right_open));
    }
    else {
        string left_value_str = two_points_match[3];
        string right_value_str = two_points_match[4];
        double left_value = stod(left_value_str);
        double right_value = stod(right_value_str);
        bool left_open = false, right_open = false;
        if (two_points_match[2] == "(")
            left_open = true;
        if (two_points_match[5] == ")")
            right_open = true;
        return Operator(two_points_match[1], 
                        Interval(left_value, right_value, left_open, right_open));
    }
}

LiteralUnion parse_literal(string literal) {
    const string pattern1 = "Boxminus[\\[,\\(]-?\\d+\\.?\\d*[\\),\\]]";;
    const string pattern2 = "Boxplus[\\[,\\(]-?\\d+\\.?\\d*[\\),\\]]";
    const string pattern3 = "Diamondminus[\\[,\\(]-?\\d+\\.?\\d*[\\),\\]]";
    const string pattern4 = "Diamondplus[\\[,\\(]-?\\d+\\.?\\d*[\\),\\]]";

    const string pattern5 = "Boxminus[\\[,\\(]-?\\d+\\.?\\d*,-?\\d+\\.?\\d*[\\),\\]]";
    const string pattern6 = "Boxplus[\\[,\\(]-?\\d+\\.?\\d*,-?\\d+\\.?\\d*[\\),\\]]";
    const string pattern7 = "Diamondminus[\\[,\\(]-?\\d+\\.?\\d*,-?\\d+\\.?\\d*[\\),\\]]";
    const string pattern8 = "Diamondplus[\\[,\\(]-?\\d+\\.?\\d*,-?\\d+\\.?\\d*[\\),\\]]";

    const string pattern9 =  "Boxminus[\\[,\\(]-?\\d+\\.?\\d*,inf[\\),\\]]";
    const string pattern10 = "Boxplus[\\[,\\(]-?\\d+\\.?\\d*,inf[\\),\\]]";
    const string pattern11 = "Diamondminus[\\[,\\(]-?\\d+\\.?\\d*,inf[\\),\\]]";
    const string pattern12 = "Diamondplus[\\[,\\(]-?\\d+\\.?\\d*,inf[\\),\\]]";

    const string pattern13 = "Boxminus[\\[,\\(]-inf,-?\\d+\\.?\\d*[\\),\\]]";
    const string pattern14 = "Boxplus[\\[,\\(]-inf*,-?\\d+\\.?\\d*[\\),\\]]";
    const string pattern15 = "Diamondminus[\\[,\\(]-inf,-?\\d+\\.?\\d*[\\),\\]]";
    const string pattern16 = "Diamondplus[\\[,\\(]-inf,-?\\d+\\.?\\d*[\\),\\]]";

    const string since_pattern1 = "Since[\\[,\\(]-?\\d+\\.?\\d*,-?\\d+\\.?\\d*[\\),\\]]";
    const string since_pattern3 = "Since[\\[,\\(]-?\\d+\\.?\\d*[\\),\\]]";
    const string until_pattern2 = "Until[\\[,\\(]-?\\d+\\.?\\d*,-?\\d+\\.?\\d*[\\),\\]]";
    const string unitl_pattern4 = "Until[\\[,\\(]-?\\d+\\.?\\d*[\\),\\]]";

    const string since_pattern00 = "Since[\\[,\\(]-?\\d+\\.?\\d*,inf[\\),\\]]";
    const string since_pattern01 = "Since[\\[,\\(]-inf,-?\\d+\\.?\\d*[\\),\\]]";

    const string until_pattern11 = "Until[\\[,\\(]-?\\d+\\.?\\d*,inf[\\),\\]]";
    const string until_pattern12 = "Until[\\[,\\(]-inf,-?\\d+\\.?\\d*[\\),\\]]";

    vector<string> since_until_patterns = {since_pattern00, since_pattern01, 
                                            until_pattern11, until_pattern12, 
                                            since_pattern1, since_pattern3, 
                                            until_pattern2, unitl_pattern4};
    string since_until_pattern = joinstr(since_until_patterns, "|");
    regex since_until_regex(since_until_pattern);
    smatch since_until_smatch;
    bool ret = regex_search(literal, since_until_smatch, since_until_regex);

    if (ret) {
        string op_str = since_until_smatch[0];
        auto begin_p = literal.find(op_str);
        string left_literal = literal.substr(0, begin_p);
        string right_literal = literal.substr(begin_p + op_str.size());
        Atom left_literal_atom = parse_atom(left_literal);
        Atom right_literal_atom = parse_atom(right_literal);
        Operator op = parse_operator(op_str);
        BinaryLiteral b_literal = BinaryLiteral(left_literal_atom, right_literal_atom, op);
        return LiteralUnion(b_literal);
    }
    else {
        vector<string> literal_patterns = {pattern9, pattern10, pattern11, pattern12, 
                                            pattern13, pattern14, pattern15, pattern16,
                                            pattern5, pattern6, pattern7, pattern8, 
                                            pattern1, pattern2, pattern3, pattern4};
        string literal_pattern = joinstr(literal_patterns, "|");
        regex literal_regex(literal_pattern);
        smatch literal_smatch;
        ret = regex_search(literal, literal_smatch, literal_regex);
        if (ret) {
            vector<Operator> operators;
            size_t atom_index = 0;
            while (ret) {
                string operator_str = literal_smatch[0];
                Operator opt = parse_operator(operator_str);
                operators.push_back(opt);
                atom_index += operator_str.size();
                auto begin_p = literal.find(operator_str);
                auto tmp_str = literal.substr(begin_p + operator_str.size());
                ret = regex_search(tmp_str, literal_smatch, literal_regex);
            }
            string atom_str = literal.substr(atom_index);
            Atom atom = parse_atom(atom_str);
            Literal s_literal = Literal(atom, operators);
            return LiteralUnion(s_literal);
        }
        else {
            Atom atom = parse_atom(literal);
            return LiteralUnion(atom);
        }
    }
}

string random_return_name() {
    time_t now = time(0);
    tm *ltm = localtime(&now);
    string strname = to_string(ltm->tm_hour) + ":" +
                        to_string(ltm->tm_min) + ":" + 
                        to_string(ltm->tm_sec);
    return strname;
}

vector<Rule> parse_rule(string original_rule) {
//    cout << original_rule << endl;
    original_rule.erase(remove(original_rule.begin(), original_rule.end(), '\n'), original_rule.end());
    original_rule.erase(remove(original_rule.begin(), original_rule.end(), '\r'), original_rule.end());
    original_rule.erase(remove(original_rule.begin(), original_rule.end(), '\t'), original_rule.end());
    original_rule.erase(remove(original_rule.begin(), original_rule.end(), ' '), original_rule.end());
    
    string delimiter = ":-";
    auto del_p = original_rule.find(delimiter);
    if (del_p == string::npos) 
        throw invalid_argument("rule without :-");
    string head = original_rule.substr(0, del_p);
    string body = original_rule.substr(del_p + 2);
    // head
    LiteralUnion head_union = parse_literal(head);
    // body
    vector<LiteralUnion> literals;
    auto body_vec = parse_body(body);
    for (auto iter = body_vec.begin(); iter != body_vec.end(); iter ++) {
        string literal_str = *iter;
        LiteralUnion literal_union = parse_literal(literal_str);
        literals.push_back(literal_union);
    }

    // head: atom
    if (head_union.type == 2) {
        Rule rule;
        rule.head = head_union.atom;
        Literal* test_p = NULL;
        for (auto iter = literals.begin(); iter != literals.end(); iter ++) {
            try
            {
                LiteralUnion* literal_union_static = new LiteralUnion(*iter);
                test_p = (Literal*) literal_union_static->get_literal();
                rule.body.push_back(literal_union_static->get_literal());
            }
            catch(std::bad_alloc)
            {
                std::cerr << "alloc failed!" << '\n';
            }
        }
        vector<Rule> rules;
        rules.push_back(rule);
        return rules;
    }

    // head: literal or BLiteral (not used)
    else {
        //Atom new_head_atom; MY CHANGE
        if (head_union.type == 0) {
            Rule rule;
            rule.head = head_union.literal;
            Literal* test_p = NULL;
            for (auto iter = literals.begin(); iter != literals.end(); iter++) {
                try
                {
                    LiteralUnion* literal_union_static = new LiteralUnion(*iter);
                    test_p = (Literal*)literal_union_static->get_literal();
                    rule.body.push_back(literal_union_static->get_literal());
                }
                catch (std::bad_alloc)
                {
                    std::cerr << "alloc failed!" << '\n';
                }
            }
            vector<Rule> rules;
            rules.push_back(rule);
            return rules;

            // Literal
            //Literal head_atom_literal = head_union.literal;
            //new_head_atom = head_atom_literal.atom;
            //new_head_atom.predicate = new_head_atom.predicate + random_return_name();
            //Rule rule1;
            //rule1.head = new_head_atom;
            //for (auto iter = literals.begin(); iter != literals.end(); iter ++) 
            //    rule1.body.push_back((*iter).get_literal());
            //
            //Rule rule2;
            //rule2.head = new_head_atom;
            //if (head_atom_literal.operators[0].name == "Boxminus") {
            //    
            //    static BinaryLiteral rule_body = BinaryLiteral(
            //                                        Atom("Top"),
            //                                        Atom(new_head_atom.predicate),
            //                                        Operator("Until", head_atom_literal.operators[0].interval));
            //    rule2.body.push_back(&rule_body);
            //}
            //else if (head_atom_literal.operators[0].name == "Boxplus") {
            //    
            //}
        } 
        else {
            //TODO: BinaryLiteral
            vector<Rule> rules;
            return rules;
        }
    }
}

#endif