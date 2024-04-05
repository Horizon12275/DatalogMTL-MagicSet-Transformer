#include <functional>
#include <algorithm>
#include <vector>
#include <string>
#include <term.h>

using namespace std;

Term::Term(string name, string type) {
    this->name = name;
    this->type = type;
}

string Term::__str__() {return this->name;}

string Term::get_type() {return this->type;}

string Term::set_type(string type) {
    this->type = type;
    return this->type;
}

size_t Term::__hash__() {
    hash<string> hash_fn;
    return hash_fn(this->type + this->name);
}

bool Term::__eq__(Term other) {
    return (this->name == other.name) && (this->type == other.get_type());
}

vector<Term> Term::const_str_to_termlist(string entity_str) {
    vector<Term> termlist;
    string delimiter = ",";
    string token;
    size_t pos = 0;
    while ((pos = entity_str.find(delimiter)) != string::npos) {
        token = entity_str.substr(0, pos);
        entity_str.erase(0, pos + delimiter.length());
        termlist.push_back(Term(token));
    }
    termlist.push_back(Term(entity_str));
    return termlist;
}

vector<string> split_term(string s, const string d) {
    if (s.empty()) return {};
    vector<string> res;
    std::string strs = s + d;
    size_t pos = strs.find(d);
    while (pos != string::npos) {
        string x = strs.substr(0, pos);
        if (x!="") res.push_back(x);
        strs = strs.substr(pos+d.size());
        pos = strs.find(d);
    }
    return res;
}


//termlist?
vector<Term> Term::str_to_termlist(string entity_str) {
    vector<Term> termlist;
    string delimiter = ",";
    string token;
    size_t pos = 0;
    while ((pos = entity_str.find(delimiter)) != string::npos) {
        token = entity_str.substr(0, pos);
        entity_str.erase(0, pos + delimiter.length());
        vector<string> token_result = split_term(token, "#");
        if (token_result[1] == "c")
            termlist.push_back(Term(token_result[0]));
        if (token_result[1] == "v")
            termlist.push_back(Term(token_result[0], "variable"));
    }
    vector<string> token_result = split_term(entity_str, "#");
    if (token_result[1] == "c")
        termlist.push_back(Term(token_result[0]));
    if (token_result[1] == "v")
        termlist.push_back(Term(token_result[0], "variable"));
    return termlist;
}


// Ascending Order
bool ascending(Term x, Term y) {
    return x.name.compare(y.name) < 0;
}

string Term::termlist_to_str(vector<Term> termlist) {
    string entity_str = "";
//    sort(termlist.begin(), termlist.end(), ascending);
    for (auto iter = termlist.begin(); iter != termlist.end(); iter ++) {
        if (iter != termlist.begin())
            entity_str += ",";
        entity_str += iter->name;
        if (iter->type == "constant")
            entity_str += ""; // "#c"
        if (iter->type == "variable")
            entity_str += ""; // "#v"
    }
    return entity_str;
}

bool Term::operator == (const Term & other) const {
    return (this->name == other.name) && (this->type == other.type);
}

Term::Term (const Term & other) {
    this->name = other.name;
    this->type = other.type;
}

Term& Term::operator = (const Term & other) {
    this->name = other.name;
    this->type = other.type;
    return *this;
}

