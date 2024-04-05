#ifndef TERM_H
#define TERM_H

#include <string>
#include <vector>

using namespace std;

class Term {
public:
    string name = "";
    string type = "constant";

    Term() = default;
    Term(string name, string type = "constant");

    string __str__();
    string get_type();
    string set_type(string type);
    size_t __hash__();
    bool __eq__(Term other);
    static vector<Term> const_str_to_termlist(string entity_str);
    static vector<Term> str_to_termlist(string entity_str);
    static string termlist_to_str(vector<Term> termlist);

    bool operator == (const Term & other) const;

    Term(const Term& other);
    Term& operator = (const Term & other);
};

#endif