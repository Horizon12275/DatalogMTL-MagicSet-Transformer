#include <algorithm>
#include <functional>
#include <literal.h>
#include <stdexcept>

using namespace std;

Operator::Operator(string name, Interval interval) {
    vector<string> operators = {"Boxminus", "Boxplus", "Diamondplus", "Diamondminus", "Since", "Until"};
    auto p_operator = find(operators.begin(), operators.end(), name);
    if (p_operator == operators.end())
        throw invalid_argument("invalid operator name.");

    this->name = name;
    this->interval = interval;
}

bool Operator::__eq__(Operator other) {
    if (this->name == other.name && this->interval.__eq__(other.interval))
        return true;
    else
        return false;
}

bool Operator::operator == (const Operator & other) const {
    // Operator another = const_cast<Operator &>(other);
    // another.name = "test";
    // return this->__eq__(another);
    if (this->name == other.name && this->interval == other.interval)
        return true;
    else
        return false;
}

string Operator::__str__() {
    return this->name + this->interval.__str__();
}

Operator::Operator (const Operator & other) {
    this->name = other.name;
    this->interval = other.interval;
}

Operator& Operator::operator = (const Operator & other) {
    this->name = other.name;
    this->interval = other.interval;
    return *this;
}

Literal::Literal() : Base(0) {
    this->atom = Atom();
    this->operators = vector<Operator>();
}

Literal::Literal(Atom atom) : Base(0) {
    this->atom = atom;
    this->operators = vector<Operator>();
}

Literal::Literal(Atom atom, vector<Operator> operators) : Base(0) {
    this->atom = atom;
    this->operators = operators;
}

string Literal::get_predicate() {
    return this->atom.get_predicate();
}

vector<Term> Literal::get_entity() {
    return this->atom.get_entity();
}

string Literal::get_op_name() {
    if (this->operators.size() != 0)
        return this->operators[0].name;
    else
        return "";
}

bool Literal::set_entity(vector<Term> entity) {
    this->atom.set_entity(entity);
    return true;
}

bool Literal::__eq__(Literal other) {
    if (this->atom == other.atom && this->operators == other.operators)
        return true;
    else
        return false;
}

bool Literal::operator == (const Literal & other) const {
    if (this->atom == other.atom && this->operators == other.operators)
        return true;
    else
        return false;
}

string Literal::__str__() {
    string str = "";
    vector<Operator> operators = this->operators;
    for (auto iter = operators.begin(); iter != operators.end(); iter ++)
        str += (*iter).__str__();
    str += this->atom.__str__();
    return str;
}

string Literal::__str_without_interval__() {
    string str = "";
    str += this->atom.__str_without_term__();
    return str;
}

size_t Literal::__hash__() {
    hash<string> hash_fn;
    return hash_fn("Literal" + this->__str__());
}

Literal::Literal (const Literal & other) : Base(0) {
    this->atom = other.atom;
    this->operators = other.operators;
}

Literal& Literal::operator = (const Literal & other) {
    this->atom = other.atom;
    this->operators = other.operators;
    this->Base::type = other.Base::type;
    return *this;
}

BinaryLiteral::BinaryLiteral() : Base(1) {
    this->left_atom = Atom();
    this->right_atom = Atom();
    this->op = Operator();
}

BinaryLiteral::BinaryLiteral(Atom left_atom, Atom right_atom, Operator op) : Base(1)  {
    this->left_atom = left_atom;
    this->right_atom = right_atom;
    this->op = op;
}

string BinaryLiteral::get_op_name() {
    return this->op.name;
}

bool BinaryLiteral::__eq__(BinaryLiteral other) {
    if (this->left_atom == other.left_atom && 
            this->right_atom == other.right_atom && 
                this->op == other.op)
        return true;
    else
        return false;
}

bool BinaryLiteral::operator == (const BinaryLiteral & other) const {
    if (this->left_atom == other.left_atom && 
            this->right_atom == other.right_atom && 
                this->op == other.op)
        return true;
    else
        return false;
}

string BinaryLiteral::__str__() {
    return this->left_atom.__str__() + 
            this->op.__str__() + this->right_atom.__str__();
}

size_t BinaryLiteral::__hash__() {
    hash<string> hash_fn;
    return hash_fn("BiLiteral:" + this->__str__());
}

BinaryLiteral::BinaryLiteral (const BinaryLiteral & other) : Base(1) {
    this->left_atom = other.left_atom;
    this->right_atom = other.right_atom;
    this->op = other.op;
}

BinaryLiteral& BinaryLiteral::operator = (const BinaryLiteral & other) {
    this->left_atom = other.left_atom;
    this->right_atom = other.right_atom;
    this->op = other.op;
    this->Base::type = other.Base::type;
    return *this;
}