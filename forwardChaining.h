#ifndef FORWARD_CHAINING_H_INCLUDED
#define FORWARD_CHAINING_H_INCLUDED
#include <iostream>
#include <queue>
#include<string.h>
#include <string>
#include<fstream>
#include<algorithm>
using namespace std;
struct variable{
	string name;
	string status;
	string value;
};

class my_queue : public std::queue< variable > {
public:
    using std::queue<variable>::c; // expose the container
};

class forward_chaining{
public:

//Clause Variable Pointer
struct clauseVariablePointer{
	int ruleNumber;
	int clauseNumber;
};

struct Rule{
    int ruleNumber;
    bool visited;
};

struct limit{
 int clauseLimit;
};

struct clauseVariable{
    string clauseValue;
    bool visited=false;
};

string var;
string conclusion;

//Clause Variable List
clauseVariable clauseVariableList[49];
variable variableList[1];
my_queue conclusionVariableQueue;
Rule ruleList[5];
clauseVariablePointer cvp;
int ruleNo_present;
limit variableLimit[5];

string getclauseVariableName(int index){
    return clauseVariableList[index].clauseValue;
}

int get_RuleNo(int clauseNo){
	int ruleNo;
	ruleNo=((clauseNo/4)+1)*10;
	return ruleNo;
}

int get_clauseVariableIndex(string val){
    for(int i=1;i<49;i++){
        if(clauseVariableList[i].clauseValue==val &&clauseVariableList[i].visited==false){
            clauseVariableList[i].visited=true;
            return i;
        }
    }
    return -1;
}

bool determine_member_variableList(string value){
char str[value.size()];
	strcpy(str,value.c_str());
	char *found;

	char target[variableList[0].name.size()];
	strcpy(target,variableList[0].name.c_str());
	found=strstr(str,target);
    if(found)
	{
	    conclusionVariableQueue.push(variableList[0]);
	    return true;
	}
	return false;
}

bool checkInstantiation(string variableValue){
	if(variableList[0].name==variableValue){
			if(variableList[0].status!="NI"){
				return true;
			}
    }
    return false;
}


bool checkNextClauseValue(int clN){
    if(++clN<49 && clauseVariableList[clN].clauseValue!=""){
        return true;
    }
    return false;
}


int instantiate(int rN){

switch(rN){
case 10:
    if(variableList[0].value=="breast cancer"){
        conclusion="Hormone therapy and Radiation therapy ";
        return 1;
    }
    break;
case 20:
    if(variableList[0].value=="brain cancer"){
        conclusion="Neurosurgery and Radiation therapy ";
        return 1;
    }
    break;
case 30:
    if(variableList[0].value=="liver cancer"){
        conclusion="Interventional radiology and Gastroenterology ";
        return 1;
    }
    break;

case 40:
    if(variableList[0].value=="lung cancer"){
        conclusion="Interventional pulmonology and Immunotherapy ";
        return 1;
    }
    break;

case 50:
    if(variableList[0].value=="bone cancer"){
        conclusion="Orthopedic oncology and Targeted therapy ";
        return 1;
    }
    break;

case 60:
    if(variableList[0].value=="kidney cancer"){
        conclusion="Targeted therapy and Surgery ";
        return 1;
    }
    break;

case 70:
    if(variableList[0].value=="stomach cancer"){
        conclusion="Surgery and Metastic stomach cancer to the liver ";
        return 1;
    }
    break;

case 80:
    if(variableList[0].value=="oral cancer"){
        conclusion="Chemotherapy and Surgery ";
        return 1;
    }
    break;

case 90:
    if(variableList[0].value=="spinal cancer"){
        conclusion="Chemotherapy and Surgery ";
        return 1;
    }
    break;

case 100:
    if(variableList[0].value=="adrenal cancer"){
        conclusion="Chemotherapy and Surgery ";
        return 1;
    }
    break;

case 110:
    if(variableList[0].value=="acute lymphocytic leukemia cancer"){
        conclusion="Stem cell transplantation and Chemotherapy ";
        return 1;
    }
    break;

case 120:
    if(variableList[0].value=="gallbladder cancer"){
        conclusion="Chemotherapy and Surgery ";
        return 1;
    }
    break;
}
return 0;
}

void display(){
cout<<"Treatment = "<<conclusion;
}



void start(string cancerName){

 variableList[0].name="cancer";
 variableList[0].status="NI";

int count=1;
ifstream myfile;
myfile.open("FC_clauseVariableList.txt");
while(getline(myfile,clauseVariableList[count].clauseValue)){
        count++;
	}

int index=0;
int Icount=0;
int ruleNo;
string var1;
bool searchClause=true;
//cout<<"Enter variable";
//getline(cin,var);
var="cancer";
char str[var.size()];
	strcpy(str,var.c_str());
	char *found;
    for(int i=0;i<12;i++){
	char target[variableList[i].name.size()];
	strcpy(target,variableList[i].name.c_str());
	found=strstr(str,target);
	if(found){
        var=target;
        break;
	}
    }
    //checking if the input from user is present in the conclusion list
	if(!determine_member_variableList(var)){
        cout<<"Variable not found";
	}
	else{
        var=conclusionVariableQueue.c[0].name;
    while(conclusionVariableQueue.size()!=0){

        if(index!=0 && index!=-1){
            var=getclauseVariableName(index);
        }
        var1=var;
        int i=get_clauseVariableIndex(var);
        if(i!=-1){
            if(searchClause){
            index=i;
            ruleNo=get_RuleNo(index);
            }
        if(clauseVariableList[index].clauseValue!="" && Icount<4){
           cvp.ruleNumber=ruleNo;
           cvp.clauseNumber=index;
            if(!checkInstantiation(var)){
                string value;
                if(var=="cancer"){
                    value=cancerName;
                    transform(value.begin(), value.end(), value.begin(), ::tolower);
                    variableList[0].value=value;
                    variableList[0].status="I";
                }
            }
        }
        //checking if the next element in clause variable list is empty
            if(!checkNextClauseValue(index) && Icount<4){
                if(instantiate(ruleNo)==1){
                    display();
                    break;
                }
                else if(instantiate(ruleNo)==2){
                    searchClause=true;
                    Icount=0;
                    index=-1;
                }
                else{
                    if(Icount<3){
                    Icount++;
                    searchClause=false;
                    }
                    else{
                    Icount=0;
                    searchClause=true;
                    }
                index++;
                }
            }
            else{
                if(Icount<3){
                    Icount++;
                    searchClause=false;
                }
                else{
                    Icount=0;
                    searchClause=true;
                }
                index++;
            }
        }
         else
            conclusionVariableQueue.pop();
        }
	}

}

};

#endif // FORWARD_CHAINING_H_INCLUDED
