#ifndef INTERVAL_H
#define INTERVAL_H

#include <string>
#include <vector>

using namespace std;

class Interval {
public:
    double left_value = -1;
    double right_value = -1;
    bool left_open = true;
    bool right_open = true;

    Interval() = default;
    Interval(double left_value, double right_value, 
                bool left_open, bool right_open);
    static Interval _union(Interval v1, Interval v2);
    static bool inclusion(Interval v1, Interval v2);
    static Interval intersection(Interval v1, Interval v2);
    static Interval sub(Interval v1, Interval v2);
    static Interval circle_sub(Interval v1, Interval v2);
    static Interval add(Interval v1, Interval v2);
    static Interval circle_add(Interval v1, Interval v2);
    static bool compare(vector<Interval> intervals1, vector<Interval> intervals2);
    static bool check_validation(double left_value, double right_value, 
                                    bool left_open, bool right_open);
    string __str__();
    size_t __hash__();
    bool __eq__(Interval other);
    Interval __instance__();

    bool is_none();
    bool operator < (const Interval & other) const;   // operator overloading
    bool operator == (const Interval & other) const;
    Interval& operator = (const Interval & other);
    Interval(const Interval& other);
    double get_left_value();
    double get_right_value();
    bool get_left_open();
    bool get_right_open();
};

#endif