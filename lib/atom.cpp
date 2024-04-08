#include <functional>
#include <atom.h>
#include <literal.h>

using namespace std;

Atom::Atom(string predicate) : Base(2)
{
    this->predicate = predicate;
    this->entity = vector<Term>();
    this->interval = Interval(-1, -1, true, true);
    this->bflist_size = 0;
    this->isMagic = false;
    this->bflist = vector<char>();
}

Atom::Atom(string predicate, vector<Term> entity) : Base(2)
{
    this->predicate = predicate;
    this->entity = entity;
    this->interval = Interval(-1, -1, true, true);
    this->bflist_size = (int)entity.size();
    this->isMagic = false;
    this->bflist = vector<char>();
}

Atom::Atom(string predicate, Interval interval) : Base(2)
{
    this->predicate = predicate;
    this->entity = vector<Term>();
    this->interval = interval;
    this->bflist_size = 0;
    this->isMagic = false;
    this->bflist = vector<char>();
}

Atom::Atom(string predicate, vector<Term> entity, Interval interval) : Base(2)
{
    this->predicate = predicate;
    this->entity = entity;
    this->interval = interval;
    this->bflist_size = (int)entity.size();
    this->isMagic = false;
    this->bflist = vector<char>();
}

bool Atom::__eq__(Atom other)
{
    if (this->predicate == other.predicate &&
        this->entity == other.entity &&
        this->interval == other.interval &&
        this->type == other.type &&
        this->bflist == other.bflist &&
        this->bflist_size == other.bflist_size &&
        this->isMagic == other.isMagic)
        return true;
    else
        return false;
}

bool Atom::operator==(const Atom &other) const
{
    if (this->predicate == other.predicate &&
        this->entity == other.entity &&
        this->interval == other.interval &&
        this->type == other.type &&
        this->bflist == other.bflist &&
        this->bflist_size == other.bflist_size &&
        this->isMagic == other.isMagic)
        return true;
    else
        return false;
}

string Atom::__str__()
{
    if (!this->interval.is_none())
    {
        if (this->entity.size() > 0)
            return Atom::isMagic_to_string() +
                   this->predicate + 
                   Atom::bflist_to_string() + "(" +
                   Term::termlist_to_str(this->entity) + ")@" +
                   this->interval.__str__();
        else
            return Atom::isMagic_to_string() + this->predicate + "@" + this->interval.__str__();
    }
    else
    {
        if (this->entity.size() > 0)
            return Atom::isMagic_to_string() + this->predicate + Atom::bflist_to_string() + "(" + Term::termlist_to_str(this->entity) + ")";
        if(this->bflist.size() > 0)
            return Atom::isMagic_to_string() + this->predicate + Atom::bflist_to_string();
        else
            return Atom::isMagic_to_string() + this->predicate;
    }
}

string Atom::__str_without_term__()
{
    return Atom::isMagic_to_string() + this->predicate + Atom::bflist_to_string();
}

string Atom::bflist_to_string()
{
    if(bflist.size() == 0)
        return "";
    string str_out = "_";
    for (char c : this->bflist)
    {
        str_out += c;
    }
    return str_out;
}

string Atom::isMagic_to_string()
{
    if (this->isMagic)
        return "magic_";
    else
        return "";
}

size_t Atom::__hash__()
{
    hash<string> hash_fn;
    if (this->entity.size() > 0)
        return hash_fn("Atom" + this->predicate +
                       Term::termlist_to_str(this->entity) +
                       this->interval.__str__());
    else
        return hash_fn("Atom" + this->predicate + this->interval.__str__());
}

string Atom::get_predicate()
{
    return this->predicate;
}

vector<Term> Atom::get_entity()
{
    return this->entity;
}

bool Atom::set_entity(vector<Term> entity)
{
    this->entity = entity;
    return true;
}

Atom::Atom(const Atom &other) : Base(2)
{
    this->predicate = other.predicate;
    this->entity = other.entity;
    this->interval = other.interval;
    this->type = other.type;
    this->bflist = other.bflist;
    this->bflist_size = other.bflist_size;
    this->isMagic = other.isMagic;
}

Atom &Atom::operator=(const Atom &other)
{
    this->predicate = other.predicate;
    this->entity = other.entity;
    this->interval = other.interval;
    this->type = other.type;
    this->bflist = other.bflist;
    this->bflist_size = other.bflist_size;
    this->isMagic = other.isMagic;
    return *this;
}