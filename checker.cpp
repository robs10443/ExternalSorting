#include <iostream>
#include <fstream>
#include <string>

using namespace std;

int main(int argc,char **argv){
	if(argc < 2){
		cout << "Wrong Arguments\n";;
		return 0;
	}
	string input_file = argv[1];
	string output_file = argv[2];
	fstream inFile(input_file.c_str(),ios::in );
	fstream outFile(output_file.c_str(),ios::in);
	int word_count_infile = 0,word_count_outfile = 0;
	inFile.seekg(0,inFile.beg);
	while(!inFile.eof()){
		string word;
		inFile >> word;
		if(word != ""){
			word_count_infile++;
		}
	}
	outFile.seekg(0,outFile.beg);
	while(!outFile.eof()){
		string word;
		outFile >> word;
		if(word != ""){
			word_count_outfile++;
		}
	}
	if(word_count_outfile != word_count_infile){
		cout << "Bug in your code\n";
		return 0;
	}else{
		cout << word_count_outfile << "\n";
		cout << "Length are same no new character added\n";
	}
	string pre;
	outFile >> pre;
	while(!outFile.eof()){
		string word;
		outFile >> word;
		if(word != ""){
			if(word < pre){
				cout << "Sorting is not done properly. Please Check!!";
				return 0;
			}
			pre = word;
		}
	}
	cout << "All set!!";
	outFile.close();
	//inFile.close();
}
