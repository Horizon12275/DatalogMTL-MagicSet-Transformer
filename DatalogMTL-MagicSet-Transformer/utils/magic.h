#ifndef MAGIC_H
#define MAGIC_H
#include<loader.h>

void adornment(vector<Rule>& adornedRuleList,const Atom& query);
vector<Rule> generation_RuleList(const vector<Rule>& adornedRuleList);
vector<Rule> modification_RuleList(const vector<Rule>& adornedRuleList);

vector<Rule> magic(vector<Rule> originalRuleList,const Atom& query) {
	//TODO
	vector<Rule> adornedRuleList = originalRuleList; // initialize adornedRuleList

	adornment(adornedRuleList,query); // adornment
	vector<Rule> generatedRuleList = generation_RuleList(adornedRuleList); // generation
	vector<Rule> modifiedRuleList = modification_RuleList(adornedRuleList); // modification

	vector<Rule> magicRuleList = generatedRuleList; // initialize magicRuleList
	magicRuleList.insert(magicRuleList.end(), modifiedRuleList.begin(), modifiedRuleList.end()); // merge generatedRuleList and modifiedRuleList
	return magicRuleList;
}

void adornment(vector<Rule>& adornedRuleList,const Atom& query) {
	//TODO
}

vector<Rule> generation_RuleList(const vector<Rule>& adornedRuleList) {
	//TODO
	return adornedRuleList;
}

vector<Rule> modification_RuleList(const vector<Rule>& adornedRuleList) {
	//TODO
	return adornedRuleList;
}

#endif