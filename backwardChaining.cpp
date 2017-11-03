/*
 * main.cpp
 *
 *  Created on: Sep 28, 2017
 *      Author: pag11
 */
#include <algorithm>
#include <stdio.h>
#include <ctype.h>
#include<iostream>
#include <stdlib.h>
#include<vector>
#include<stack>
#include<string.h>
#include <string>
#include<fstream>
#include"forwardChaining.h"
using namespace std;

struct conclusionList {
	int ruleNumber;
	string conclusion;
	string result;
};

struct variableList {
	string name;
	string status;
	string value;
};

struct conclusionStack {
	int ruleNumber;
	int clauseNumber;
};

struct Rule {
	int ruleNumber;
	bool visited;
};

struct limit {
	int clauseLimit;
};

class my_stack: public std::stack<conclusionStack> {
public:
	using std::stack<conclusionStack>::c;
	// expose the container
};

//Conclusion List
conclusionList conclt[25];

//Variable List
variableList varlt[48];

Rule ruleList[25];
limit variableLimit[25];
my_stack st;
conclusionStack cs;
string clause_varlt[101];
string inputVariable;
int ruleNo_present;

int get_clauseNo(int);
void determine_member_concl_list(string);
bool checkInstantiation(string);
void showConclusion(int);
string getConclusionName(int);
bool checkNextClauseValue(int);
bool instantiate(int);

int main() {
	bool flag = false;
	conclusionStack temp;
	int temp_clauseNumber;
	int temp_ruleNumber;
	string temp_str;
	int count = 0;

	ifstream myfile, myfile2, myfile3, myfile4, myfile5;
	myfile.open("conclusions.txt");
	cout << "Conclusion List" << endl;
	cout << "---------------------------------------------------------------"
			<< endl;

    //Reading and initializing the Conclusion List
	while (getline(myfile, conclt[count].conclusion)) {
		getline(myfile, temp_str);
		conclt[count].ruleNumber = atoi(temp_str.c_str());
		cout << "Conclusion " << count + 1 << " : " << conclt[count].conclusion
				<< endl;
		conclt[count].result = "";
		count++;
	}
	cout << endl;
	cout << "Variable List" << endl;
	cout << "---------------------------------------------------------------"
			<< endl;
	count = 0;

	//Reading and initializing the Variable List
	myfile2.open("variableList.txt");
	while (getline(myfile2, varlt[count].name)) {
		cout << "Variable " << count + 1 << " : " << varlt[count].name
				<< endl;
		varlt[count].status = "NI";
		count++;
	}
	cout << endl;
	count = 1;

	//Reading and initializing the Clause Variable List
	myfile3.open("clauseVariableList.txt");
	while (getline(myfile3, clause_varlt[count])) {
		count++;
	}
	string question[48];
	count = 0;

	myfile4.open("questions.txt");
	while (getline(myfile4, question[count])) {
		count++;
	}

	cout << endl;
	cout << "Cancer List" << endl;
	cout << "---------------------------------------------------------------"
			<< endl;

	count = 1;
	string c;
	myfile5.open("CancerList.txt");
	while (getline(myfile5, c)) {
		cout << c << endl;
		count++;
	}

	cout << endl;
	cout << "---------------------------------------------------------------"
			<< endl;
	cout << endl;

	for (int i = 0; i < 25; i++) {
		ruleList[i].ruleNumber = (i * 10) + 10;
		ruleList[i].visited = false;
	}

	for (int i = 0; i < 25; i++) {
		variableLimit[i].clauseLimit = 0;
	}
	char *found;
	do {
		cout << "Enter conclusion" << endl;
		getline(cin, inputVariable);

		transform(inputVariable.begin(), inputVariable.end(),
				inputVariable.begin(), ::tolower);

		char str[inputVariable.size()];
		strcpy(str, inputVariable.c_str());

		for (int i = 0; i < 25; i++) {
			char target[conclt[i].conclusion.size()];
			strcpy(target, conclt[i].conclusion.c_str());
			found = strstr(str, target);
			if (found) {
				inputVariable = target;
				break;
			}
		}
		if (!found) {
			cout << "The system can detect only cancer. Please enter valid conclusion"
					<< endl;
		}
	} while (!found);

	//checking if the input from user is present in the conclusion list
	determine_member_concl_list(inputVariable);

	if (ruleNo_present == 0) {
		cout << "Rule cannot be Found";
	} else {
		int limit = 1;
		string val;
		string clauseValue = "";

		while (st.size() != 0) {
			temp = st.top();
			temp_clauseNumber = temp.clauseNumber;
			temp_ruleNumber = temp.ruleNumber;
			if (clause_varlt[temp_clauseNumber] != ""
					&& variableLimit[(temp_ruleNumber - 10) / 10].clauseLimit
							<= 4) {
				variableLimit[(temp_ruleNumber - 10) / 10].clauseLimit++;
				clauseValue = clause_varlt[temp_clauseNumber];
			}
			ruleNo_present = 0;

			//Checking if the Clause Value is already instantiated
			if (!checkInstantiation(clauseValue)) {
				determine_member_concl_list(clauseValue);
				if (ruleNo_present == 0) {

					for (int i = 0; i < 48; i++) {
						if (clauseValue.compare(varlt[i].name) == 0) {
							do {
								cout << question[i] << " : ";
								getline(cin, val);
								transform(val.begin(), val.end(), val.begin(),
										::tolower);
								if (!(val == "yes" || val == "no")) {
									cout<< "Invalid input.Please enter yes or no"
											<< endl;
								}
							} while (!(val == "yes" || val == "no"));

							varlt[i].value = val;
							varlt[i].status = "I";
							break;
						}
					}
				}
			}
			if (ruleNo_present == 0) {
                    //checking if the next element in clause variable list is empty
				if (!checkNextClauseValue(temp_clauseNumber)) {
					if (instantiate(temp.ruleNumber)) {
						showConclusion(temp.ruleNumber);
						break;
					}

					st.pop();
					if (val == "no") {
						while (st.size() != 0)
							st.pop();
					}
					if (st.size() != 0) {
						st.c[st.size() - 1].clauseNumber++;
						if (variableLimit[(st.c[st.size() - 1].ruleNumber - 10)/ 10].clauseLimit <= 4
								&& checkNextClauseValue(st.c[st.size() - 1].clauseNumber))
							st.c[st.size() - 1].clauseNumber++;
					} else {
						ruleNo_present = 0;
						determine_member_concl_list(inputVariable);
					}
				} else if (val == "no") {
					while (st.size() != 0)
						st.pop();
					if (temp.ruleNumber == 250) {
						if (instantiate(temp.ruleNumber)) {
							showConclusion(temp.ruleNumber);
							break;
						}
					}
					determine_member_concl_list(inputVariable);
				} else {
					if (variableLimit[(st.c[st.size() - 1].ruleNumber - 10) / 10].clauseLimit
							<= 4)
						st.c[st.size() - 1].clauseNumber++;
				}
			}

		}

	}
	cout << endl << endl;
	forward_chaining f;
	if (inputVariable != "cancer")
		f.start(inputVariable);
    cout<<endl;
}

int get_clauseNo(int rN) {
	int clauseNo;
	clauseNo = 4 * ((rN / 10) - 1) + 1;
	return clauseNo;
}

void determine_member_concl_list(string iV) {
	char str[iV.size()];
	strcpy(str, iV.c_str());
	char *found;
	for (int i = 0; i < 25; i++) {
		char target[conclt[i].conclusion.size()];
		strcpy(target, conclt[i].conclusion.c_str());
		found = strstr(str, target);
		if (found && ruleList[i].visited == false) {
			ruleNo_present = 1;
			cs.ruleNumber = conclt[i].ruleNumber;
			cs.clauseNumber = get_clauseNo(conclt[i].ruleNumber);
			st.push(cs);
			ruleList[i].visited = true;
			break;
		}
	}
}

bool checkInstantiation(string clauseValue) {
	for (int i = 0; i < 48; i++) {
		if (varlt[i].name == clauseValue) {
			if (varlt[i].status != "NI") {
				return true;
			}
		}
	}
	return false;
}

void showConclusion(int rN) {
	string result = conclt[(rN - 10) / 10].result;
	if (result == "no") {
		cout << endl << "You do not have cancer";
	} else if (result == "") {
		cout << endl << "Invalid Inputs";
	} else if (result == "yes") {
		cout << endl << "Yes you have " << inputVariable;
	}
}

string getConclusionName(int rN) {
	return conclt[(rN - 10) / 10].conclusion;
}

bool checkNextClauseValue(int clN) {
	if (clause_varlt[++clN] != "" && clN < 101) {
		return true;
	}
	return false;
}

//checking if statement and getting conclusion
bool instantiate(int rN) {
	switch (rN) {
	case 10: {
		if (conclt[0].result == "")
			if (varlt[0].value == "yes" && conclt[1].result == "yes") {
				conclt[0].result = "yes";
				if (conclt[0].conclusion == inputVariable) {
					string temp = "breast ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 20: {
		if (conclt[1].result == "")
			if (varlt[2].value == "yes" && varlt[3].value == "yes") {
				conclt[1].result = "yes";
				if (conclt[1].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 30: {
		if (conclt[2].result == "")
			if (varlt[4].value == "yes" && conclt[3].result == "yes") {
				conclt[2].result = "yes";
				if (conclt[2].conclusion == inputVariable) {
					string temp = "brain ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 40: {
		if (conclt[3].result == "")
			if (varlt[6].value == "yes" && varlt[7].value == "yes") {
				conclt[3].result = "yes";
				if (conclt[3].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 50: {
		if (conclt[4].result == "")
			if (conclt[5].result == "yes" && varlt[9].value == "yes") {
				conclt[4].result = "yes";
				if (conclt[4].conclusion == inputVariable) {
					string temp = "liver ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 60: {
		if (conclt[5].result == "")
			if (varlt[10].value == "yes" && varlt[11].value == "yes") {
				conclt[5].result = "yes";
				if (conclt[5].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 70: {
		if (conclt[6].result == "")
			if (conclt[7].result == "yes" && varlt[13].value == "yes") {
				conclt[6].result = "yes";
				if (conclt[6].conclusion == inputVariable) {
					string temp = "lung ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 80: {
		if (conclt[7].result == "")
			if (varlt[14].value == "yes" && varlt[15].value == "yes") {
				conclt[7].result = "yes";
				if (conclt[7].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 90: {
		if (conclt[8].result == "")
			if (conclt[9].result == "yes" && varlt[17].value == "yes") {
				conclt[8].result = "yes";
				if (conclt[8].conclusion == inputVariable) {
					string temp = "bone ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 100: {
		if (conclt[9].result == "")
			if (varlt[18].value == "yes" && varlt[19].value == "yes") {
				conclt[9].result = "yes";
				if (conclt[9].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 110: {
		if (conclt[10].result == "")
			if (conclt[11].result == "yes" && varlt[21].value == "yes") {
				conclt[10].result = "yes";
				if (conclt[10].conclusion == inputVariable) {
					string temp = "kidney ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 120: {
		if (conclt[11].result == "")
			if (varlt[22].value == "yes" && varlt[23].value == "yes") {
				conclt[11].result = "yes";
				if (conclt[11].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 130: {
		if (conclt[12].result == "")
			if (conclt[13].result == "yes" && varlt[24].value == "yes") {
				conclt[12].result = "yes";
				if (conclt[12].conclusion == inputVariable) {
					string temp = "stomach ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 140: {
		if (conclt[13].result == "")
			if (varlt[26].value == "yes" && varlt[27].value == "yes") {
				conclt[13].result = "yes";
				if (conclt[13].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 150: {
		if (conclt[14].result == "")
			if (conclt[15].result == "yes" && varlt[28].value == "yes") {
				conclt[14].result = "yes";
				if (conclt[14].conclusion == inputVariable) {
					string temp = "oral ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 160: {
		if (conclt[15].result == "")
			if (varlt[30].value == "yes" && varlt[31].value == "yes") {
				conclt[15].result = "yes";
				if (conclt[15].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 170: {
		if (conclt[16].result == "")
			if (conclt[17].result == "yes" && varlt[32].value == "yes") {
				conclt[16].result = "yes";
				if (conclt[16].conclusion == inputVariable) {
					string temp = "spinal ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 180: {
		if (conclt[17].result == "")
			if (varlt[34].value == "yes" && varlt[35].value == "yes") {
				conclt[17].result = "yes";
				if (conclt[17].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 190: {
		if (conclt[18].result == "")
			if (conclt[19].result == "yes" && varlt[37].value == "yes") {
				conclt[18].result = "yes";
				if (conclt[18].conclusion == inputVariable) {
					string temp = "adrenal ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 200: {
		if (conclt[19].result == "")
			if (varlt[38].value == "yes" && varlt[39].value == "yes") {
				conclt[19].result = "yes";
				if (conclt[19].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 210: {
		if (conclt[20].result == "")
			if (conclt[21].result == "yes" && varlt[40].value == "yes") {
				conclt[20].result = "yes";
				if (conclt[20].conclusion == inputVariable) {
					string temp = "acute lymphocytic leukemia ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 220: {
		if (conclt[21].result == "")
			if (varlt[42].value == "yes" && varlt[43].value == "yes") {
				conclt[21].result = "yes";
				if (conclt[21].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 230: {
		if (conclt[22].result == "")
			if (conclt[23].result == "yes" && varlt[45].value == "yes") {
				conclt[22].result = "yes";
				if (conclt[22].conclusion == inputVariable) {
					string temp = "gallbladder ";
					inputVariable = temp.append(inputVariable);
					return true;
				} else
					return false;
			}
	}
		break;

	case 240: {
		if (conclt[23].result == "")
			if (varlt[46].value == "yes" && varlt[47].value == "yes") {
				conclt[23].result = "yes";
				if (conclt[23].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;

	case 250: {
		if (conclt[24].result == "")
			if (varlt[46].value == "no" || varlt[47].value == "no"
					|| varlt[45].value == "no") {
				conclt[24].result = "no";
				if (conclt[24].conclusion == inputVariable)
					return true;
				else
					return false;
			}
	}
		break;
	}
	return false;
}
