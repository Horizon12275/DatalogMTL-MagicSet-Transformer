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