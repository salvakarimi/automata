#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>

using namespace std;
//vector to read input file
vector<vector<string> > v;
vector<string> row;

//functon reads input file
void readFile(){
	string filename = "DFA_Input_1.txt";
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
				v.push_back(row);
			}
}


// Displaying the 2D vector 
void printVector(vector<vector<string> > v){
		for (int i = 0; i < v.size(); i++) { 
			for (int j = 0; j < v[i].size(); j++) 
				cout << v[i][j] << " "; 
			cout << endl; 
		} 
}
//function checks if entered string's alphabet are defined for the DFA
bool checkAlphabet(string str,vector<string> vec){
	bool check;
	vector<char> v2;
	string temp;
	for(int j=0;j<vec.size();j++){
		temp=vec[j];
		v2.push_back(temp[0]);
	}

	for(int i=0;i<str.size();i++){
		if (std::count(v2.begin(), v2.end(), str[i]))
			check = true;
		else{
			check = false;
			break;
		}
	}
	return check;
}

//calculate destination
string calculateDest(vector<vector<string> > v,string start,char action){
	int dummy;
	string t2;
	for (int i = 4; i < v.size(); i++) { 
		string temp;
		temp=v[i][1];
		 if(start==v[i][0] && temp[0]==action){
			dummy=i;
			break;
		}
		else{
			dummy=0;
		}
	}
	 t2=v[dummy][2];
	return t2;
}
//v[0][i] alphabet
//v[1][i] states
//v[2][i] start
//v[3][i] ends


int main(int argc, char *argv[]) {
	readFile();
	string str;
	cin>>str;
	int size;
	size = str.size();
	string points[size]; 
	bool accept;
	bool alphabet;


//ckecks entered string's alphabet
alphabet=checkAlphabet(str, v[0]);
if(alphabet==false){
	cout<<"entered string's alphabet is not defined for DFA"<<endl;
}

else{
//explores states
if(str.size() >1){
	for(int i=0; i<str.size();i++){
		if(i==0){
			points[i]=calculateDest(v,v[2][0],str[i]);
		}
		else {
			points[i]=calculateDest(v,points[i-1], str[i]);
		}
	}
}
else if(str.size()==1){
	points[str.size()-1]=calculateDest(v,v[2][0],str[0]);
}


// for loop checks if last charachter's state is a final state or not
	for (int j = 0; j < v[3].size()-1; j++){
		if(points[str.size()-1] == v[3][j]){
			accept=true;
			break;
		}
		else{
			accept=false;
			}	
	}
	
//print's correct msg for string
	if (accept==true) {
		cout<<"string accepted by DFA";
	}
	else {
		cout<<"string NOT accepted by DFA";
	}

}
	return 0;
}
