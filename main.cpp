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

	//�������������Ĳο�����
	Atom atom1("predicate1");						  // ����һ��Atom����
	Atom atom2("predicate2", {Term("X"), Term("Y")}); // ����һ��Atom����
	atom1 = atom2;									  // ��ֵ

	vector<string> rules;																								 // ���ڴ�Ź����ַ���
	rules.push_back("Boxminus[0,5]a1:ResearchAssistantCandidate(aX,bY,ans,Z):-Boxminus[0,5]a1:UndergraduateStudent(X)"); // �������ַ�������vector
	rules.push_back("a1:ResearchAssistantCandidate(X):-Diamondminus[0,2]a1:GraduateStudent(X)");
	rules.push_back("a1:ResearchAssistant(X):- a1:publicationAuthor(Y,X),Boxminus[0,1]a1:ResearchAssistantCandidate(X)");
	vector<Rule> ruleList; 									// ���ڴ�Ž�����Ĺ���

	// load program test
	std::cout << "Loading program test:" << std::endl;
	ruleList = load_program(rules); 						// ���������ַ���
	std::cout << endl;

	// rulehead operator name change test
	std::cout << "Rulehead operator name change test:" << std::endl;
	ruleList[0].head.operators[0].name = "Boxplus"; 		// �޸�Rule head�е�operator
	std::cout << ruleList[0].__str__() << std::endl;

	// rulehead name change test
	std::cout << "Rulehead name change test:" << std::endl;
	ruleList[0].head.atom.predicate = "magic_" + ruleList[0].head.atom.predicate + "_b"; // �޸�Rule head�е�predicate
	std::cout << ruleList[0].__str__() << std::endl;

	// rulebody name and operator change test
	std::cout << "Rulebody name change test:" << std::endl;
	Base *basePtr = ruleList[0].body[0]; 					// ��ȡRule body�еĵ�һ��Baseָ��
	if (Atom *atomPtr = dynamic_cast<Atom *>(basePtr))
	{								 						// ���ָ���Ƿ�ָ��Atom��Literal����������Ӧ������ת��
		atomPtr->predicate = "name"; 						// ֱ���޸�Atom��predicate
	}
	else if (Literal *literalPtr = dynamic_cast<Literal *>(basePtr))
	{											   			// ���ָ���Ƿ�ָ��Atom��Literal����������Ӧ������ת��
		literalPtr->atom.predicate = "name";	   			// �޸�Literal��Atom��predicate
		literalPtr->operators[0].name = "Boxplus"; 			// �޸�Literal�е�operator
	}
	else
	{
		cout << "Unsupported type in Rule body." << endl;
	}
	std::cout << ruleList[0].__str__() << std::endl;
}