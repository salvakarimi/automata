#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <queue>
using namespace std;
//we store the nfa in this 2D vector
vector<vector<string> > nfa;

//we will store the DFA in this 2d vector
vector<vector<string> > dfa;


//functon reads input file
//v[0][i] alphabet
//v[1][i] states
//v[2][i] start
//v[3][i] ends

void readFile(){
	vector<string> row;
	string filename = "NFA_Input_2.txt";

	ifstream input(filename.c_str());
		 if(!input){
				std::cerr << "Error in opening file: " << filename << "\n";
				return ;
			}
			std::vector<std::vector<std::string> > words;
			std::string line;

			while(std::getline(input, line)){
				std::stringstream ss;
				ss << line;
				std::vector<std::string> row;
				while (!ss.eof()) {
					std::string tempStr;
					ss >> tempStr;
					row.push_back(tempStr);
				}
				nfa.push_back(row);
			}
			nfa[0].pop_back();
			nfa[0].push_back("λ");
			//input.close();
}
//prints 2D vector
void printVector(vector<vector<string> > v){
		for (int i = 0; i < v.size(); i++) { 
			for (int j = 0; j < v[i].size(); j++) 
				cout << v[i][j] << " "; 
			cout << endl; 
		} 
}
//prints 1D vector
void printv(vector<string> v){
	for(int i=0;i<v.size();i++){
		cout<<v[i]<<",";
	}
}
//checks if a string issubstring of the other
int isSubstring(string s1, string s2) 
{ 
	int M = s1.length(); 
	int N = s2.length(); 
  
	/* A loop to slide pat[] one by one */
	for (int i = 0; i <= N - M; i++) { 
		int j; 
		/* For current index i, check for pattern match */
		for (j = 0; j < M; j++) 
			if (s2[i + j] != s1[j]) 
				break; 
		if (j == M) 
			return i; 
	} 
  
	return -1; 
} 

//gets union of epsilon closure and destination for non epsilon actions
vector<string> vecUnion(vector<string> v1,vector<string> v2){
	vector<string> v3;
	for (int i=0;i<v1.size();i++) {
		v3.push_back(v1[i]);	
	}
	for (int j=0;j<v2.size();j++) {
		if (std::count(v3.begin(), v3.end(), v2[j]))
			// "Element found";
			;
		else
			v3.push_back(v2[j]);
			// "Element not found";
	}
	return v3;
	
}
//convert vector to string
string vec2str(vector<string> v){
	std::string s;
	for (std::vector<std::string>::const_iterator i = v.begin(); i != v.end(); ++i)
		s += *i;
	return s;
}
vector<string> str2vec(string str){
	vector<string> out;
	string s1;
	for(int i=0;i<str.size();i+=2){
		s1+=str[i];
		s1+=str[i+1];
		out.push_back(s1);
		s1="";
	}
	return out;
}




// C++ program to check whether two strings are anagrams 
// of each other 

/* function to check whether two strings are anagram of  
each other */
bool areAnagram(string str1, string str2) 
{ 
	// Get lengths of both strings 
	int n1 = str1.length(); 
	int n2 = str2.length(); 
  
	// If length of both strings is not same, then they 
	// cannot be anagram 
	if (n1 != n2) 
		return false; 
  
	// Sort both the strings 
	sort(str1.begin(), str1.end()); 
	sort(str2.begin(), str2.end()); 
  
	// Compare sorted strings 
	for (int i = 0; i < n1; i++) 
		if (str1[i] != str2[i]) 
			return false; 
  
	return true; 
} 

//finds epsilon closure
vector<string> eClosure(string start){
	//e holds the epsilon closure states
	vector<string> e;
	e.push_back(start);
	bool done=false;
		for(int i=0;i<nfa.size();i++){
			if(nfa[i][0]== start && nfa[i][1]=="λ"){
			e.push_back(nfa[i][2]);
			string j;
			j=nfa[i][2];
			for(int k;k<nfa.size();k++){
				if(nfa[k][0]==j && nfa[k][1]=="λ" ){
					e.push_back(nfa[k][2]);
				}	
				else {
					done=true;
				}
			}
		}
	}
	if (done==true)
		return e;
	else 
		return e;
}

//this function will calculate the epsilon closure for new formed states
vector<string>complex_eClosure(vector<string> input){
	vector<string> output;
	vector<string>  tmp;
	vector<string>  tmp2;
	vector<vector<string> > tmptmp;
	for(int i=0;i<input.size();i++){
		tmptmp.push_back(eClosure(input[i]));
	}
	for(int i=0;i<tmptmp.size();i++){
		for (int j=0;j<tmptmp[i].size();j++){
			tmp.push_back(tmptmp[i][j]);
		}
	}
	for(int i=0;i<tmp.size();i++){
		if(count(tmp2.begin(),tmp2.end(),tmp[i])){;}
		else {tmp2.push_back(tmp[i]);}
	}
	return tmp2 ;
}




//where can start go with acation? (it must return the union of e-closure and places to go with "action"
vector<string> calculateDest(string start,string action){
	vector<string> destination;//vector of all the states we can go from start, using action
	for(int i=0;i<nfa.size();i++){
		if(nfa[i][0]==start && nfa[i][1]==action){
			destination.push_back(nfa[i][2]);
			destination.push_back(start);
		}
	}
	
	//destination=vecUnion(destination, eClosure(start));
	return destination;
}

//this function will caclulate the destinatin for new formed states alsa includes the epsilon transaction
vector<string> complex_calculateDest(vector<string> start,string action){
	vector<string> output;
	vector<string>  tmp;
	vector<string>  tmp2;
	vector<vector<string> > tmptmp;
	
	for(int i=0;i<start.size();i++){
		tmptmp.push_back(calculateDest(start[i],action));
	}

	for(int i=0;i<tmptmp.size();i++){
		for (int j=0;j<tmptmp[i].size();j++){
			tmp.push_back(tmptmp[i][j]);
		}
	}
	for(int i=0;i<tmp.size();i++){
		if(count(tmp2.begin(),tmp2.end(),tmp[i])){;}
		else {tmp2.push_back(tmp[i]);}
	}
	return tmp2 ;
	


}



//step 1
//function sets dfa's alphabet it must conclude λ
void DFAalphabet(){
	string s1;
	vector<string> row;
	for(int i=0;i<nfa[0].size()-1;i++){
			s1=nfa[0][i];
			row.push_back(s1);
	}
	dfa.push_back(row);
}


//true is it finds the vector in 2d vector and false if it doesn't
bool myFind(vector<vector<string> > v,vector<string> s){
	bool flag;
	for(int i=0;i<v.size();i++){
		if(s==v[i]){
			flag=true;
			break;
		}
		else {
			flag=false;
		}	
	}
	return flag;
}
//is there a vector in queue that isn't already in states?
bool isNewQueue(vector<vector<string> > queue,vector<vector<string> > states){ //all of queue's elements are in states
	bool flag;
	for(int i=0;i<queue.size();i++){
		for(int j=0;j<states.size();j++){
			if(vec2str(states[j])==vec2str(queue[i])){
				flag= true;
				break;
			}
			flag= false;
		}
	}
return flag;
}



//step 3
//function sets starting node of dfa from nfa in dfa[2][0]
//a string in the format of QiQj..Qk
vector<string> DFAstart(){
	vector<string> start;
	vector<string> tmp;
	string tmp1;
	tmp = eClosure(nfa[2][0]);//this is s0
	tmp1=vec2str(tmp);
	start.push_back(tmp1);
	
	return tmp;
}
//adds start to dfa
void addStart(){
	vector<string> start;
	vector<string> tmp;
	string tmp1;
	tmp = eClosure(nfa[2][0]);//this is s0
	tmp1=vec2str(tmp);
	start.push_back(tmp1);
	dfa.push_back(start);
}

// calculate the states of dfa
vector<string>  DFAstates(){
	vector<vector<string> > queue; //stores the unexplored
	vector<vector<string> > states;
	vector<string> out;
	states.push_back(DFAstart());//first state
	int j=0;
	bool flag;
	bool trap=false;
	for(int i=0;i<dfa[0].size();i++){
		if(complex_calculateDest(DFAstart(), dfa[0][i]).size()>0){
			queue.push_back(complex_calculateDest(DFAstart(), dfa[0][i]));
		}
		else {
			trap=true;
		}
	}
	while (isNewQueue(queue,states)==false) {//keep doing it until there is no new step
		for(int i=0;i<dfa[0].size();i++){//for all the alphabet
			
				if(areAnagram(vec2str(queue[j]), vec2str(states[i]))==false){
					states.push_back(queue[j]);
					queue[j].clear();

					queue.push_back(complex_calculateDest(queue[j], dfa[0][i]));
					 if( complex_calculateDest(queue[j], dfa[0][i]).size()==0 ){
						trap=true;
					}
					j++;
				}
				else {
					queue[j].clear();
					queue.push_back(complex_calculateDest(queue[j], dfa[0][i]));
					j++;
				}
			//}

		}		
	}
	for (int i=0;i<states.size();i++) {
		if(vec2str(states[i]).length()>0){
			out.push_back(vec2str(states[i]));
		}
		else{;}
	}
	

	return out;
}
void DFAstates_add(){
	vector<string> out1;
	out1=DFAstates();
	dfa.push_back(out1);
}


//step 4 is finals;setting the end states
void ends(){
	vector<string> states;
	vector<string> finals;
	states=DFAstates();
	string s1;
	for(int i=0;i<states.size();i++){
		for(int j=0;j<nfa[3].size();j++){
			if(isSubstring(nfa[3][j],states[i])>=0 and count(finals.begin(),finals.end(),states[i])==0){
				finals.push_back(states[i]);
			}
			else{;}
		}
	}
	dfa.push_back(finals);
	
}


//last step!
//ads the actiond to dfa
void DFAactions(){
	vector<string> states;
	vector<string> row;
	vector<string> row2;
	states=DFAstates();
	int size;
	if(count(states.begin(), states.end(), "tr")>0){
		size=states.size()-1;
		for(int j=0;j<dfa[0].size();j++){
			row2.push_back("tr");
			row2.push_back(dfa[0][j]);
			row2.push_back("tr");
			dfa.push_back(row2);
			row2.clear();
		}	
	}
	else{
		size=states.size();
	}
	for(int i =0;i<size;i++){
		for(int j=0;j<dfa[0].size();j++){
			row.push_back(states[i]);
			row.push_back(dfa[0][j]);
			row.push_back( vec2str( complex_calculateDest( str2vec(states[i]), dfa[0][j]) ) );
			dfa.push_back(row);
			row.clear();
		}
		
	}
	
}

int main(int argc, char *argv[]) {
	readFile();
	//v[0][i] alphabet
	//v[1][i] states
	//v[2][i] start
	//v[3][i] ends
	//pprint vector checked
	//eclosure checked
	//vector union checked
	//calculate dest checked
	//DFAalphabet checked
	//cout<<calculateDest("q1", "0");
	//addStart(); checked
	//complex_eClosure checked
	//complex_calculateDest checked
	//flag1 checked
	//are anagram checked

	vector<string> states;
	DFAalphabet();//step1
	DFAstates();
	DFAstates_add();//step2: //not checked
	addStart();//step3
	ends();//step4 //not checked
	DFAactions();//step5 //not checked
	printVector(dfa);

	
	//write into file
	ofstream Out("DFA_Output_2.txt");
	//ofstream Out("me_Output_2.txt");
	vector< vector<string> >::iterator row_it = dfa.begin();
	vector< vector<string> >::iterator row_end = dfa.end();
	
	for ( ; row_it != row_end; ++row_it )
	{
		
		vector<string>::iterator col_it = row_it->begin();
		vector<string>::iterator col_end = row_it->end();
		
		for ( ; col_it != col_end; ++col_it )
				Out << *col_it<<" ";
		Out << '\n';
	}
	
	Out.close();

}