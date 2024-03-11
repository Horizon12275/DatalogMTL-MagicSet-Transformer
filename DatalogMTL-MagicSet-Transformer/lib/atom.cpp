#include <functional>
#include <atom.h>

using namespace std;

Atom::Atom(string predicate) : Base(2) {
    this->predicate = predicate;
    this->entity = vector<Term>();
    this->interval = Interval(-1, -1, true, true);
}

Atom::Atom(string predicate, vector<Term> entity) : Base(2) {
    this->predicate = predicate;
    this->entity = entity;
    this->interval = Interval(-1, -1, true, true);
}

Atom::Atom(string predicate, Interval interval) : Base(2) {
    this->predicate = predicate;
    this->entity = vector<Term>();
    this->interval = interval;
}


Atom::Atom(string predicate, vector<Term> entity, Interval interval) : Base(2) {
    this->predicate = predicate;
    this->entity = entity;
    this->interval = interval;
}

bool Atom::__eq__(Atom other) {
    if (this->predicate == other.predicate && 
            this->entity == other.entity &&
                this->interval == other.interval)
        return true;
    else
        return false;
}

bool Atom::operator == (const Atom & other) const {
    if (this->predicate == other.predicate && 
            this->entity == other.entity &&
                this->interval == other.interval)
        return true;
    else
        return false;
}

string Atom::__str__() {
    if (!this->interval.is_none()) {
        if (this->entity.size() > 0)
            return this->predicate + "(" + 
                    Term::termlist_to_str(this->entity) + ")@" + 
                        this->interval.__str__();
        else
            return this->predicate + "@" + this->interval.__str__();
    }
    else {
        if (this->entity.size() > 0)
            return this->predicate + "(" + Term::termlist_to_str(this->entity) + ")";
        else
            return this->predicate;
    }
}

size_t Atom::__hash__() {
    hash<string> hash_fn;
    if (this->entity.size() > 0)
        return hash_fn("Atom" + this->predicate +  
                            Term::termlist_to_str(this->entity) +  
                                this->interval.__str__());
    else
        return hash_fn("Atom" + this->predicate + this->interval.__str__());
}

string Atom::get_predicate() {
    return this->predicate;
}

vector<Term> Atom::get_entity() {
    return this->entity;
}

bool Atom::set_entity(vector<Term> entity) {
    this->entity = entity;
    return true;
}

Atom::Atom (const Atom & other) : Base(2) {
    this->predicate = other.predicate;
    this->entity = other.entity;
    this->interval = other.interval;
}

Atom& Atom::operator = (const Atom & other)  {
    this->predicate = other.predicate;
    this->entity = other.entity;
    this->interval = other.interval;
    this->Base::type = other.Base::type;
    return *this;
}