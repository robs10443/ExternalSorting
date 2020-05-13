#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <numeric>
#include <ctime>

using namespace std;

//To generate the random integer values
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
int getRand(int l, int r){
    uniform_int_distribution<int> uid(l, r);
    return uid(rng);
}

string addingExtensionToTheFileName(string file_name){
	bool flag = false;
	for(auto it : file_name){
		if(it == '.'){
			flag = true;
			break;
		}
	}
	if(flag == false){
		file_name = file_name + ".txt";
	}
	return file_name;
}

//Returns the file name generated
string generatorForRandomString(){
	int current_size_of_file = 0;
	int maximum_length_of_word = 100;
	int length_of_the_word;
	int size_of_file;
	string file_name;
	
	cout << "Enter the name of the file : ";
	cin >> file_name;
	
	file_name = addingExtensionToTheFileName(file_name);
	
	cout << "Enter the size of the file : ";
	cin >> size_of_file;
	
	fstream current_file(file_name.c_str(),ios::out);
	
	cout << "------------------------------------------------------\n";
	cout << "Generating Random file....\n";
	cout << "------------------------------------------------------\n";
	
	
	while(current_size_of_file < size_of_file){
		length_of_the_word = getRand(1,maximum_length_of_word);
		
		for(int i = 0 ; i < length_of_the_word ; ++i){
			current_file << ((char)(getRand(0,25) + 'a'));
		}
		
		current_size_of_file += length_of_the_word;
		
		if(current_size_of_file + 1 < size_of_file){
			current_file << ' ';
			current_size_of_file++;
		}
		
	}
	
	current_file.close();
	cout << "------------------------------------------------------\n";
	cout << "File generation Ended\n";
	cout << "------------------------------------------------------\n";
	return file_name;
}

//to insert the data(vector of sorted string) into the file
void insertIntoFile(vector<string> &data, string file_name){
	fstream current_file(file_name.c_str(),ios::out);
	int data_size = data.size();
	for(int i = 0 ;i < data_size ; ++i){
		current_file << data[i].c_str();
		if(i != (data_size - 1)) {
			current_file << ' ';
		}
	}
	current_file.close();
}

//divide the original file into groups of memory available
//file_name is the original file
//available_memory is refering to the RAM available
vector<string> distributionOfDataIntoFile(string original_file_name,int available_memory){
	
	cout << "------------------------------------------------------\n";
	cout << "Distributing the data into the files..............\n";
	cout << "------------------------------------------------------\n";
	
	fstream current_file(original_file_name.c_str(),ios::in);
	
	int total_size_of_file;
	
	current_file.seekg( 0, current_file.end );
	total_size_of_file = current_file.tellg();
	current_file.seekg( 0, current_file.beg );
	
	int total_number_of_files = 0;
	
	vector<string> data, files_name;
	
	int current_size_of_data = 0;
	
	
	while(!current_file.eof()){
		
		string word;
		current_file >> word;
		if(current_size_of_data + word.length() + 1 < available_memory){
			data.push_back( word );
		}else{
			sort(data.begin(),data.end());
			string current_file_name = "Process_0_" + to_string(total_number_of_files) + ".txt";
			
			total_number_of_files++;
			
			insertIntoFile(data,current_file_name);
			
			files_name.push_back(current_file_name);
			
			current_size_of_data = 0;
			
			data.clear();
			data.push_back(word);
		}
		current_size_of_data += word.size();
	}
	
	if(current_size_of_data != 0){
		sort(data.begin(),data.end());
		string current_file_name = "Process_0_" + to_string(total_number_of_files) + ".txt";
		
		total_number_of_files++;
		
		insertIntoFile(data,current_file_name);
		
		files_name.push_back(current_file_name);
		
		current_size_of_data = 0;
		
		data.clear();
	}
	cout << "------------------------------------------------------\n";
	cout << "Distribution Ended\n";
	cout << "------------------------------------------------------\n";
	return files_name;
}

vector<string> dataFromTheFile(fstream &outFile,int size_of_each_block){
	vector<string> data;
	int current_size_of_vector = 0;
	while( (!outFile.eof()) && current_size_of_vector < size_of_each_block){
		string word;
		outFile >> word;
		data.push_back(word);
		current_size_of_vector += word.size();
	}
	return data;
}

vector<string> passesOfMerging(vector<string> files_name,int memory_block,int size_of_each_block,int pass_number){
	cout << "------------------------------------------------------\n";
	cout << "\nPass " << pass_number << " started.....\n";
	cout << "------------------------------------------------------\n";
	int count_of_files = files_name.size();
	
	int total_file_to_be_created = ceil(count_of_files / (long double)(memory_block));
	

	vector<string> current_file_names;
	
	for(int i = 0; i < total_file_to_be_created ; ++i){
		string current_file_name = "Process_" + to_string(pass_number) + "_" + to_string(i) + ".txt";
		current_file_names.push_back(current_file_name);
		fstream current_file(current_file_name,ios::out);
		int start = i * memory_block;
		int end = min((i + 1) * memory_block, (int)files_name.size());
		
		vector<string> current_vector[memory_block];
		vector<fstream> previous_pass_file(memory_block);
		
		for(int j = start ; j < end ; ++j){
			string previous_pass_file_name = "Process_" + to_string(pass_number - 1) + "_" + to_string(j) + ".txt";
			previous_pass_file[j - start].open(previous_pass_file_name,ios::in);
		}
		
		for(int j = start ; j < end ; ++j){
			current_vector[j - start] = dataFromTheFile(previous_pass_file[j - start],size_of_each_block);
		}
		
		vector<int> current_pointer(memory_block,0);
		
		bool process_complete = false,for_space = false;
		
		while(!process_complete){
			int indx = -1;
			string min_of_all_files;
			min_of_all_files = "{";
			for(int j = start; j < end ; j++){
				if(current_pointer[j - start] < current_vector[j - start].size() && current_vector[j - start][current_pointer[j - start]] < min_of_all_files){
					indx = j - start;
					min_of_all_files = current_vector[j - start][current_pointer[j - start]];
				}
			}
			if(indx == -1){
				process_complete = true;
				break;
			}
			current_file << min_of_all_files.c_str();
			current_file << " ";
			current_pointer[indx]++;
			if(current_pointer[indx] == current_vector[indx].size()){
				current_vector[indx] = dataFromTheFile(previous_pass_file[indx],size_of_each_block);
				current_pointer[indx] = 0;
			}
		}
		current_file.close();
	}
	for(auto x : files_name){
		remove(x.c_str());
	}
	cout << "------------------------------------------------------\n";
	cout << "Pass " << pass_number << " ended\n\n";
	cout << "------------------------------------------------------\n";
	return current_file_names;
}

int main(){
	cout << "Do you want to genrate a random file (y/n) : ";
	char ch;
	cin >> ch;
	while(ch != 'y' && ch != 'Y' && ch != 'n' && ch != 'N'){
		cout << "Wrong Input!!\n";
		cout << "Do you want to genrate a random file (y/n) : ";
		cin >> ch;
	}
	string name_of_input_file;
	if(ch == 'Y' || ch == 'y'){
		name_of_input_file = generatorForRandomString();
	}else{
		cout << "Enter the name of the file (with extension): ";
		cin >> name_of_input_file;
	}
	
	int size_of_RAM,size_of_each_block,memory_block;
	
	cout << "Enter the number of memory_block you want to divde the ram into : ";
	cin >> memory_block;
	
	cout << "Enter the size of each block of RAM : ";
	cin >> size_of_each_block;
	
	
	size_of_RAM = memory_block*size_of_each_block;
	
	cout << "\t------------------------------------------------------\n";
	cout << "\n\n\n\tTotal RAM SIZE is : " << size_of_RAM << "bytes\n\n\n\n";
	cout << "\t------------------------------------------------------\n\n\n";
	
	const clock_t begin_time = clock();
	
	vector<string> temp = distributionOfDataIntoFile(name_of_input_file,size_of_RAM);
	
	int current_pass = 1;
	while(temp.size() > 1){
		temp = passesOfMerging(temp,memory_block,size_of_each_block,current_pass);
		current_pass++;
	}
	rename(temp[0].c_str(),"output.txt");
	cout << "------------------------------------------------------\n";
	cout << "Entire process took a total of: " << float(clock() - begin_time) / CLOCKS_PER_SEC * 1000 << " msec." << endl;
	cout << "------------------------------------------------------\n";
}
