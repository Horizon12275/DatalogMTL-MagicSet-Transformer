#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <vector>
#include <iostream>
#include <parser.h>
#include <rule.h>

using namespace std;

vector<Rule> load_program(vector<string> rules) {
    vector<Rule> program;
    for (auto iter = rules.begin(); iter != rules.end(); iter ++) {
        vector<Rule> rule = parse_rule(*iter);
        cout << rule[0].__str__() << "Loaded!" << endl;
        program.insert(program.end(), rule.begin(), rule.end());
    }
    return program;
}

#endif