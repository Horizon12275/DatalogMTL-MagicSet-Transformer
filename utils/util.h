#ifndef UTIL_H
#define UTIL_H

#include <string>
#include <vector>

using namespace std;

vector<string> split_str(string s, const string d) {
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

string joinstr(vector<string> s_vec, string delimiter) {
    string str = "";
    for (auto iter = s_vec.begin(); iter != s_vec.end(); iter ++) {
        if (iter != s_vec.begin())
            str += delimiter;
        str += *iter;
    }
    return str;
}

#endif