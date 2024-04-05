#include<magic.h>
#include<loader.h>
#include<iostream>
#include<parser.h>

void test();

int main() {

	//test();

	// use magic set method
	MagicSet magicSet;

	// in this premature version, we only parse the head of the rule as the query, note that the body is empty and we need :- to separate the head and the body
	vector<string> querys; //input query
	vector<Rule> queryList;
	vector<string> rules; // input rules
	vector<Rule> ruleList;

	// // Example 1
	// querys.push_back("path(1,5):-"); // input query here
	// rules.push_back("path(X,Y):-edge(X,Y)"); // input rules here
	// rules.push_back("path(X,Y):-edge(X,Z),path(Z,Y)");

	// Example 2
	querys.push_back("ill(a):-");		 // input query here
	rules.push_back("Boxplus[0,10]ill(X):-Boxminus[0,10]infected(X)"); // input rules here
	rules.push_back("Boxplus[0,10]infected(X):-Meet(X,Y),ill(Y)");

	queryList = load_program(querys); // parse the query string
	ruleList = load_program(rules); // parse the rule string

	// //print rulelist
	// cout << "RuleList:" << endl;
	// for (Rule r : ruleList) {
	// 	cout << r.__str__() << endl;
	// }

	Literal query;
	query = queryList[0].head; // get the query
	vector<Rule> magicRules = magicSet.MS(query, ruleList); // get the magic rules

	return 0;
}

void test(){

	//用作基本操作的参考测试
	Atom atom1("predicate1");						  // 创建一个Atom对象
	Atom atom2("predicate2", {Term("X"), Term("Y")}); // 创建一个Atom对象
	atom1 = atom2;									  // 赋值

	vector<string> rules;																								 // 用于存放规则字符串
	rules.push_back("Boxminus[0,5]a1:ResearchAssistantCandidate(aX,bY,ans,Z):-Boxminus[0,5]a1:UndergraduateStudent(X)"); // 将规则字符串存入vector
	rules.push_back("a1:ResearchAssistantCandidate(X):-Diamondminus[0,2]a1:GraduateStudent(X)");
	rules.push_back("a1:ResearchAssistant(X):- a1:publicationAuthor(Y,X),Boxminus[0,1]a1:ResearchAssistantCandidate(X)");
	vector<Rule> ruleList; 									// 用于存放解析后的规则

	// load program test
	std::cout << "Loading program test:" << std::endl;
	ruleList = load_program(rules); 						// 解析规则字符串
	std::cout << endl;

	// rulehead operator name change test
	std::cout << "Rulehead operator name change test:" << std::endl;
	ruleList[0].head.operators[0].name = "Boxplus"; 		// 修改Rule head中的operator
	std::cout << ruleList[0].__str__() << std::endl;

	// rulehead name change test
	std::cout << "Rulehead name change test:" << std::endl;
	ruleList[0].head.atom.predicate = "magic_" + ruleList[0].head.atom.predicate + "_b"; // 修改Rule head中的predicate
	std::cout << ruleList[0].__str__() << std::endl;

	// rulebody name and operator change test
	std::cout << "Rulebody name change test:" << std::endl;
	Base *basePtr = ruleList[0].body[0]; 					// 获取Rule body中的第一个Base指针
	if (Atom *atomPtr = dynamic_cast<Atom *>(basePtr))
	{								 						// 检查指针是否指向Atom或Literal，并进行相应的类型转换
		atomPtr->predicate = "name"; 						// 直接修改Atom的predicate
	}
	else if (Literal *literalPtr = dynamic_cast<Literal *>(basePtr))
	{											   			// 检查指针是否指向Atom或Literal，并进行相应的类型转换
		literalPtr->atom.predicate = "name";	   			// 修改Literal中Atom的predicate
		literalPtr->operators[0].name = "Boxplus"; 			// 修改Literal中的operator
	}
	else
	{
		cout << "Unsupported type in Rule body." << endl;
	}
	std::cout << ruleList[0].__str__() << std::endl;
}