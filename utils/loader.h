#ifndef LOADER_H
#define LOADER_H

#include <string>
#include <vector>
#include <iostream>
#include <parser.h>
#include <rule.h>
#include <windows.h>
#include <conio.h>

using namespace std;

extern HANDLE hConsole;

vector<Rule> load_program(vector<string> rules) {
    vector<Rule> program;
    for (auto iter = rules.begin(); iter != rules.end(); iter ++) {
        vector<Rule> rule = parse_rule(*iter);
        //set color to grey
        SetConsoleTextAttribute(hConsole, 8);
        cout <<"Load Check:\t"<< rule[0].__str__() << "      Loaded!" << endl;
        //set color to white
        SetConsoleTextAttribute(hConsole, 15);
        program.insert(program.end(), rule.begin(), rule.end());
    }
    return program;
}

#endif