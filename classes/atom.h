#ifndef ATOM_H
#define ATOM_H

#include <string>
#include <vector>
#include <term.h>
#include <interval.h>

using namespace std;

class Base {
public:
    int type;

    Base(int type) {
        this->type = type;
    }

    virtual ~Base() {} // 添加虚析构函数以启用多态

    Base(const Base & other) {
        this->type = other.type;
    }

    Base& operator = (const Base & other) {
        this->type = other.type;
        return *this;
    }
};

enum AtomType
{
    IDB,
    EDB
};

class Atom : public Base {
public:
    string predicate = "";
    vector<Term> entity = vector<Term>();
    Interval interval = Interval();

    // 新增成员变量
    AtomType type;    // 枚举类型成员，用于区分IDB和EDB
    vector<char> bflist; // 用于按顺序记录每个变量是否是bound变量或者是free变量
    int bflist_size;     // 用于记录bflist的大小，大小应和entity的大小相同
    bool isMagic = false; // 用于标记是否是magic atom

    Atom() : Base(2) {};
    Atom(string predicate);
    Atom(string predicate, vector<Term> entity);
    Atom(string predicate, Interval interval);
    Atom(string predicate, vector<Term> entity, Interval interval);

    bool __eq__(Atom other);
    bool operator == (const Atom & other) const;
    string __str__();
    size_t __hash__();

    string get_predicate();
    vector<Term> get_entity();
    bool set_entity(vector<Term> entity);

    Atom(const Atom& other);
    Atom& operator = (const Atom & other); // 重载赋值运算符，需注意修改
};

#endif