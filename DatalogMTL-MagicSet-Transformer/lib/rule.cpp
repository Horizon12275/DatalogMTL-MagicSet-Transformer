#include <rule.h>

using namespace std;

// Rule::Rule(Atom head, vector<Literal> body_literal) {
//     this->head = head;
//     this->body_literal = body_literal;
//     this->body_binary = vector<BinaryLiteral>();
//     this->type = 0;
// }

// Rule::Rule(Atom head, vector<BinaryLiteral> body_binary) {
//     this->head = head;
//     this->body_binary = body_binary;
//     this->body_literal = vector<Literal>();
//     this->type = 1;
// }

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

string Rule::__str__() {
    string str = this->head.__str__() + ":-";
    auto body = this->body;
    for (auto iter = body.begin(); iter != body.end(); iter ++) {
        if (iter != body.begin())
            str += ", ";

        if ((*iter)->type == 0)
            str += ((Literal *)(*iter))->__str__();
        else if ((*iter)->type == 1)
            str += ((BinaryLiteral *)(*iter))->__str__();
        else if ((*iter)->type == 2)
            str += ((Atom *)(*iter))->__str__();
    }
    return str;
}

Rule::Rule (const Rule & other) {
    this->head = other.head;
    vector<Base*> new_body;
    for (auto iter = other.body.begin(); iter != other.body.end(); iter ++) {
        Base* body_p = *iter;
        LiteralUnion literal_union;
        switch (body_p->type) {
            case 0: // Literal
            {
                auto literal_p = (Literal *) body_p;
                literal_union = LiteralUnion(*literal_p);
            } break;
            case 1: // BinaryLiteral
            {
                auto binary_literal_p = (BinaryLiteral *) body_p;
                literal_union = LiteralUnion(*binary_literal_p);
            } break;
            case 2: // Atom
            {
                auto atom_p = (Atom *) body_p;
                literal_union = LiteralUnion(*atom_p);
            } break;
        }
        LiteralUnion* literal_union_static = new LiteralUnion(literal_union);
        this->body.push_back(literal_union_static->get_literal());
    }
}

Rule& Rule::operator = (const Rule & other) {
    this->head = other.head;
    vector<Base*> new_body;
    for (auto iter = other.body.begin(); iter != other.body.end(); iter ++) {
        Base* body_p = *iter;
        LiteralUnion literal_union;
        switch (body_p->type) {
            case 0: // Literal
            {
                auto literal_p = (Literal *) body_p;
                literal_union = LiteralUnion(*literal_p);
            } break;
            case 1: // BinaryLiteral
            {
                auto binary_literal_p = (BinaryLiteral *) body_p;
                literal_union = LiteralUnion(*binary_literal_p);
            } break;
            case 2: // Atom
            {
                auto atom_p = (Atom *) body_p;
                literal_union = LiteralUnion(*atom_p);
            } break;
        }
        LiteralUnion* literal_union_static = new LiteralUnion(literal_union);
        this->body.push_back(literal_union_static->get_literal());
    }
    return *this;
}