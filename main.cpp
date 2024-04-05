#include<magic.h>
#include<loader.h>
#include<iostream>
#include<parser.h>
#include <fstream>

void test();

int main() {
	// // File I/O Configurations
	// std::ifstream inputFile("D:/Horizon/SJTU/Projects/ongoing/DatalogMTL-MagicSet-Transformer/test/input/input.txt"); // input file
	// if (!inputFile.is_open()) // check if the input file is opened successfully
	// {
	// 	std::cerr << "Failed to open input file!" << std::endl;
	// 	return 1;
	// }
	// std::string line;
	// std::string query_string;
	// std::vector<std::string> rules_string;

	// bool readingQuery = false;
	// bool readingRules = false;

	// std::ofstream outputFile("D:/Horizon/SJTU/Projects/ongoing/DatalogMTL-MagicSet-Transformer/test/output/output.txt"); // output file
	// std::streambuf *original_cout_streambuf = std::cout.rdbuf(); // save cout buffer
	// std::cout.rdbuf(outputFile.rdbuf()); // redirect cout to output file

	// while (std::getline(inputFile, line))
	// {
	// 	if (line.find("query:") != std::string::npos)
	// 	{
	// 		readingQuery = true;
	// 		readingRules = false;
	// 		continue;
	// 	}

	// 	if (line.find("rules:") != std::string::npos)
	// 	{
	// 		readingQuery = false;
	// 		readingRules = true;
	// 		continue; 
	// 	}

	// 	if (readingQuery)
	// 	{
	// 		query_string = line; 
	// 	}
	// 	else if (readingRules)
	// 	{
	// 		rules_string.push_back(line); 
	// 	}
	// }

	// //std::cout << "Query: " << query_string << std::endl;
	// //std::cout << "Rules:" << std::endl;
	// for (const auto &rule : rules_string)
	// {
	// 	std::cout << rule << std::endl;
	// }

	// use magic set method
	MagicSet magicSet;

	// in this premature version, we only parse the head of the rule as the query, note that the body is empty and we need :- to separate the head and the body
	vector<string> querys; //input query
	vector<Rule> queryList;
	vector<string> rules; // input rules
	vector<Rule> ruleList;

	// Example List Below

	// Example from input file
	// querys.push_back(query_string); // input query here
	// for (const auto &rule : rules_string)
	// {
	// 	rules.push_back(rule); // input rules here
	// }

	// // Example 1
	// querys.push_back("path(1,5):-"); // input query here
	// rules.push_back("path(X,Y):-edge(X,Y)"); // input rules here
	// rules.push_back("path(X,Y):-edge(X,Z),path(Z,Y)");

	// // Example 2
	// querys.push_back("ill(a):-");		 // input query here
	// rules.push_back("Boxplus[0,10]ill(X):-Boxminus[0,10]infected(X)"); // input rules here
	// rules.push_back("Boxplus[0,10]infected(X):-Meet(X,Y),ill(Y)");

	// // Example 3
	// querys.push_back("Infected(d):-");		 // input query here
	// rules.push_back("Boxplus[10,20]Infected(X):-Meet(X,Y),Infected(Y)");	// input rules here

	// // Example 4
	// querys.push_back("ill(d):-"); // input query here
	// rules.push_back("Boxplus[0,5]grow(X):-Boxminus[0,5]Infected(X)"); // input rules here
	// rules.push_back("Boxplus[0,10]ill(X):-Boxminus[0,6]grow(X)");
	// rules.push_back("Boxplus[0,4]Infected(X):-Meet(X,Y),ill(Y)");

	// // // Example 5
	// querys.push_back("Green(a):-"); // input query here
	// rules.push_back("Blue(X):-R2B(X),Boxminus[0,4]Red(X)"); // input rules here
	// rules.push_back("Green(X):-B2G(X,Y),Boxminus[0,5]Blue(X),Diamondminus[0,4]Blue(Y)"); // input rules here

	// // Example 6
	querys.push_back("Suspect(david):-"); // input query here
	rules.push_back("TransactionChain(X,Y) :- Transaction(X,Y), RedList(X)"); // input rules here
	rules.push_back("TransactionChain(X,Z) :- Diamondminus[0,24] TransactionChain(X,Y), Transaction(Y,Z)"); // input rules here
	rules.push_back("Boxplus[0,inf) Suspect(Y) :- TransactionChain(X,Y), HighRisk(Y)");				 // input rules here

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

	// Configuration of I/O
	// std::cout.rdbuf(original_cout_streambuf); // restore cout buffer
	// inputFile.close(); // close input file
	// outputFile.close(); // close output file
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