#include <functional>
#include <interval.h>

using namespace std;

Interval::Interval(double left_value, double right_value,
                        bool left_open, bool right_open) {
    this->left_value = left_value;
    this->right_value = right_value;
    this->left_open = left_open;
    this->right_open = right_open;
}

double Interval::get_left_value() {return this->left_value;}
double Interval::get_right_value() {return this->right_value;}
bool Interval::get_left_open() {return this->left_open;}
bool Interval::get_right_open() {return this->right_open;}

//only left?
bool Interval::operator < (const Interval & other) const {
    if (this->left_value < other.left_value)
        return true;
    else if (this->left_value == other.left_value && this->left_open <= other.left_open)
        return true;
    else
        return false;
}

bool Interval::operator == (const Interval & other) const {
    return (this->left_value == other.left_value && this->right_value == other.right_value &&
            this->left_open == other.left_open && this->right_open == other.right_open);
}

Interval& Interval::operator = (const Interval & other) {
    this->left_value = other.left_value;
    this->right_value = other.right_value;
    this->left_open = other.left_open;
    this->right_open = other.right_open;
    return *this;
}

Interval::Interval (const Interval & other) {
    this->left_value = other.left_value;
    this->right_value = other.right_value;
    this->left_open = other.left_open;
    this->right_open = other.right_open;
}

bool Interval::is_none() {
    if (this->left_value == -1 && this->right_value == -1 && this->left_open && this->right_open)
        return true;
    else
        return false;
}

bool Interval::check_validation(double left_value, double right_value, 
                                            bool left_open, bool right_open) {
    if (left_value == right_value && (left_open || right_open))
        return false;
    else if (left_value > right_value)
        return false;
    else if (left_value == stod("inf") || right_value == stod("-inf"))
        return false;
    else if (left_value == stod("-inf") && !left_open)
        return false;
    else if (right_value == stod("inf") && !right_open)
        return false;
    
    return true;
}

Interval Interval::_union(Interval v1, Interval v2) {
    if (v1.left_value > v2.left_value) {
        Interval tmp = v1;
        v1 = v2;
        v2 = tmp;
    }
    // [1,2] & [1,3]
    if (v1.left_value == v2.left_value) {
        double left_value = v1.left_value;
        bool left_open;
        if (v1.left_open != v2.left_open)
            left_open = false;
        else
            left_open = v1.left_open;

        double right_value = max<double>(v1.right_value, v2.right_value);
        bool right_open;
        if (v1.right_value == v2.right_value && v1.right_open != v2.right_open) 
            right_open = false;
        else {
            if (right_value == v1.right_value) 
                right_open = v1.right_open;
            else
                right_open = v2.right_open;
        }
        return Interval(left_value, right_value, left_open, right_open);
    }
    // (1,2) & (2,3)
    else if (v2.left_value == v1.right_value && v2.left_open && v1.right_open)
        return Interval(-1, -1, true, true);
    
    else {
        // [1,2] & [3,4]
        if (v2.left_value > v1.right_value) 
            return Interval(-1, -1, true, true);

        else {  // [1,3] & [2,4]
            double left_value = v1.left_value;
            bool left_open = v1.left_open;
            double right_value = max<double>(v1.right_value, v2.right_value);
            bool right_open;
            if (v1.right_value == v2.right_value && v1.right_open != v2.right_open)
                right_open = false;
            else {
                if (right_value == v1.right_value)
                    right_open = v1.right_open;
                else
                    right_open = v2.right_open;
            }
            return Interval(left_value, right_value, left_open, right_open);
        }
    }
}

//inclusion
bool Interval::inclusion(Interval v1, Interval v2) {
    if (v1.left_value < v2.left_value || v1.right_value > v2.right_value)
        return false;
    else {
        if (v1.left_value == v2.left_value && v2.left_open && !v1.left_open)
            return false;
        else if (v1.right_value == v2.right_value && v2.right_open && !v1.right_open)
            return false;
        else
            return true;
    }
}

Interval Interval::intersection(Interval v1, Interval v2) {
    double left_value, right_value;
    bool left_open, right_open;
    if (v1.left_value > v2.left_value) {
        left_value = v1.left_value;
        left_open = v1.left_open;
    } 
    else if (v1.left_value < v2.left_value) {
        left_value = v2.left_value;
        left_open = v2.left_open;
    } 
    else {
        left_value = v1.left_value;
        if (v1.left_open != v2.left_open)
            left_open = true;
        else
            left_open = v1.left_open;
    }

    if (v1.right_value < v2.right_value) {
        right_value = v1.right_value;
        right_open = v1.right_open;
    }
    else if (v1.right_value > v2.right_value) {
        right_value = v2.right_value;
        right_open = v2.right_open;
    }
    else {
        right_value = v1.right_value;
        if (v1.right_open != v2.right_open)
            right_open = true;
        else
            right_open = v1.right_open;
    }

    if (!Interval::check_validation(left_value, right_value, left_open, right_open))
        return Interval(-1, -1, true, true);
    else
        return Interval(left_value, right_value, left_open, right_open);
}

string Interval::__str__() {
    string str_out = "";
    if (this->left_open)
        str_out += "(";
    else
        str_out += "[";
    
    str_out += to_string(this->left_value) + "," + to_string(this->right_value);

    if (this->right_open)
        str_out += ")";
    else
        str_out += "]";

    return str_out;
}

size_t Interval::__hash__() {
    hash<string> hash_fn;
    return hash_fn(to_string(this->left_open) + to_string(this->left_value) + 
                    to_string(this->right_open) + to_string(this->right_value));
}

bool Interval::__eq__(Interval other) {
    return (this->left_value == other.left_value && this->right_value == other.right_value &&
            this->left_open == other.left_open && this->right_open == other.right_open);
}

/*
v1 = (2,5)
v2 = (4,7) 
l = 2 - 7 = -5
r = 5 - 4 = 1

*/

Interval Interval::sub(Interval v1, Interval v2) {
    double left_value = v1.left_value - v2.right_value;
    double right_value = v1.right_value - v2.left_value;
    bool left_open = false;
    bool right_open = false;
    if (left_value == stod("-inf") || v1.left_open || v2.right_open)
        left_open = true;
    if (right_value == stod("inf") || v1.right_open || v2.left_open)
        right_open = true;
    return Interval(left_value, right_value, left_open, right_open);
}

/*
v1 = [2,5]
v2 = (4,7]
l = 2 - 4 = -2
r = 5 - 7 = -2

*/

Interval Interval::circle_sub(Interval v1, Interval v2) {
    double left_value = v1.left_value - v2.left_value;
    double right_value = v1.right_value - v2.right_value;
    bool left_open = false;
    bool right_open = false;
    if (left_value == stod("-inf") || (v1.left_open && !v2.left_open))
        left_open = true;
    if (right_value == stod("inf") || (v1.right_open && !v2.right_open))
        right_open = true;
    return Interval(left_value, right_value, left_open, right_open);
}

Interval Interval::add(Interval v1, Interval v2) {
    double left_value = v1.left_value + v2.left_value;
    double right_value = v1.right_value + v2.right_value;
    bool left_open = false;
    bool right_open = false;
    if (left_value == stod("-inf") || (v1.left_open || v2.left_open))
        left_open = true;
    if (right_value == stod("inf") || (v1.right_open || v2.right_open))
        right_open = true;
    return Interval(left_value, right_value, left_open, right_open);
}

Interval Interval::circle_add(Interval v1, Interval v2) {
    double left_value = v1.left_value + v2.right_value;
    double right_value = v1.right_value + v2.left_value;
    bool left_open = false;
    bool right_open = false;
    if (left_value == stod("-inf") || (v1.left_open && !v2.right_open))
        left_open = true;
    if (right_value == stod("inf") || (v1.right_open && !v2.left_open))
        right_open = true;
    return Interval(left_value, right_value, left_open, right_open);
}

//Used for comparing the vector whether the same
bool Interval::compare(vector<Interval> intervals1, vector<Interval> intervals2) {
    if (intervals1.size() != intervals2.size())
        return false;
    size_t length = intervals1.size();
    for (int i = 0; i < length; i ++) {
        if (!intervals1[i].__eq__(intervals2[i]))
            return false;
    }
    return true;
}

Interval Interval::__instance__() {
    return Interval(this->left_value, this->right_value, this->left_open, this->right_open);
}