#include<loader.h>
#include<iostream>

int main() {

	vector<string> rules; // ���ڴ�Ź����ַ���
	rules.push_back("Boxminus[0,5]a1:ResearchAssistantCandidate(X):-Boxminus[0,5]a1:UndergraduateStudent(X)"); // �������ַ�������vector
	rules.push_back("a1:ResearchAssistantCandidate(X):-Diamondminus[0,2]a1:GraduateStudent(X)");
	rules.push_back("a1:ResearchAssistant(X):- a1:publicationAuthor(Y,X),Boxminus[0,1]a1:ResearchAssistantCandidate(X)");
	vector<Rule> ruleList; // ���ڴ�Ž�����Ĺ���

	//load program test
	std::cout << "Loading program test:" << std::endl;
	ruleList = load_program(rules); // ���������ַ���
	std::cout << endl;

	//rulehead operator name change test
	std::cout << "Rulehead operator name change test:" << std::endl;
	ruleList[0].head.operators[0].name = "Boxplus"; // �޸�Rule head�е�operator
	std::cout<< ruleList[0].__str__() << std::endl;

	//rulehead name change test
	std::cout << "Rulehead name change test:" << std::endl;
	ruleList[0].head.atom.predicate = "magic_" + ruleList[0].head.atom.predicate + "_b"; // �޸�Rule head�е�predicate
	std::cout<< ruleList[0].__str__() << std::endl;

	//rulebody name and operator change test
	std::cout << "Rulebody name change test:" << std::endl;
	Base* basePtr = ruleList[0].body[0]; // ��ȡRule body�еĵ�һ��Baseָ��
	if (Atom* atomPtr = dynamic_cast<Atom*>(basePtr)) { // ���ָ���Ƿ�ָ��Atom��Literal����������Ӧ������ת��
		atomPtr->predicate = "name"; // ֱ���޸�Atom��predicate
	}
	else if (Literal* literalPtr = dynamic_cast<Literal*>(basePtr)) { // ���ָ���Ƿ�ָ��Atom��Literal����������Ӧ������ת��
		literalPtr->atom.predicate = "name"; // �޸�Literal��Atom��predicate
		literalPtr->operators[0].name = "Boxplus"; // �޸�Literal�е�operator
	}
	else {
		cout << "Unsupported type in Rule body." << endl;
	}
	std::cout<< ruleList[0].__str__() << std::endl;

	return 0;
}