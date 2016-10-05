#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <list>
#include <numeric>
#include <random>
#include <map>

using namespace std;


struct TreeNode {
	int data;
	char key;
	TreeNode *left;
	TreeNode *right;
};

std::map<char, std::string> map_code;

template <typename T>
vector<size_t> sort_indexes(const vector<T> &v) {

	// initialize original index locations
	vector<size_t> idx(v.size());
	iota(idx.begin(), idx.end(), 0);

	// sort indexes based on comparing values in v
	sort(idx.begin(), idx.end(),
		[&v](size_t i1, size_t i2) {return v[i1] > v[i2]; });

	return idx;
}

void write_map_to_file_name(std::map<char, std::string> input_map, string output_filename) {
	ofstream write_file;
	write_file.open(output_filename);
	for each (std::pair<char, std::string> pair in input_map)
	{
		cout << "pair first:" << pair.first << endl;
		cout << "pair second:" << pair.second << endl;
		write_file << pair.first << "=" << pair.second << endl;
	}

	write_file.close();
}

void encrypt_txt_with_map_return_file(string txt, std::map<char, std::string> input_map, string output_filename) {
	ofstream write_file;
	write_file.open(output_filename);
	for (int j = 0; txt[j] != '\0'; ++j) {
		cout << "txt:" << txt[j] << endl;
		for each (std::pair<char, std::string> pair in input_map)
		{
			if (pair.first == txt[j]) {
				cout << "code :" << pair.second << endl;
				write_file << pair.second;
			}
		}
	}

	write_file << endl;
	write_file.close();
}

string read_file_to_text(string file_location) {
	string line;
	string return_text;
	ifstream myfile;
	myfile.open(file_location);
	if (myfile.is_open()) {
		while (getline(myfile, line)) {
			return_text.append(line);
		}
	}

	return return_text;
}


char* copy_array_char(char *char_array_left, int size) {
	char* return_array = new char[size * sizeof(char)];
	for (int i = 0; i < size; i++) {
		return_array[i] = char_array_left[i];
	}
	return return_array;
}

int* copy_array_int(int *char_array_left, int size) {
	int* return_array = new int[size * sizeof(int)];
	for (int i = 0; i < size; i++) {
		return_array[i] = char_array_left[i];
	}
	return return_array;
}

void sort_by_index(char char_arr[], int count_array[], int size) {
	char* copy_arr = copy_array_char(char_arr, size);
	int* copy_int = copy_array_int(count_array, size);
	int index_return = 0;
	// Sort by frequencies
	std::vector<int> v(count_array, count_array + size);
	for (int i : sort_indexes(v)) {
		//cout << char_arr[i] << " -- " << count_array[i] << endl;
		char_arr[index_return] = copy_arr[i];
		count_array[index_return] = copy_int[i];
		index_return++;
	}
}

string binary_to_string(int input) {
	if (input == 1) {
		return "1";
	}
	else {
		return "0";
	}
}

void generate_code(TreeNode* current_node, std::map<char, std::string> &output_map, int* a,  int current) {
	if (current_node->left == NULL && current_node->right == NULL) {
		string temp_code;
		cout << "Leaf Found Key: " << current_node->key << "\t data: " << current_node->data << endl;

		cout << "code:";
		for (int i = 0; i < current; i++) {
			cout << a[i];
			temp_code.append(binary_to_string(a[i]));
		}

		cout << endl;
		output_map.insert(std::pair<char, std::string>(current_node->key, temp_code));
	}


	if (current_node->left != NULL) {
		a[current] = 0;
		generate_code(current_node->left, output_map, a, current + 1);
	}

	if (current_node->right != NULL) {
		a[current] = 1;
		generate_code(current_node->right, output_map, a, current + 1);
	}
}

TreeNode* lookup(TreeNode* node, char key) {
	// 1. Base case == empty tree 
	// in that case, the target is not found so return false 
	if (node == NULL) {
		return(false);
	}
	else {
		// 2. see if found here 
		if (key == node->key) {
			cout << "key:" << node ->key << endl;
			cout << "data:" << node ->data << endl;
			return node;
		} 
		else {
			lookup(node->left, key);
			lookup(node->right, key);
		}
	}
}

void post_order(TreeNode* n) {
	if (n != NULL) {
		cout << n->key << " - " << n->data << endl;
		post_order(n->left);
		post_order(n->right);
	}
}



int main() {
	char array[5] = { 'a','b','c','d','e' };
	char origin_array[5] = { 'a','b','c','d','e' };
	string file_location = "C:\\Users\\School\\Documents\\Visual Studio 2015\\Projects\\Assignment3-File_Structure\\Original_File.txt";	
	string text = read_file_to_text(file_location);

	cout << text << endl;

	int i;
	int counts[5];
	for (i = 0; i < 5; ++i)
	{
		int j = 0, count = 0;
		for (j = 0; text[j] != '\0'; ++j)
		{
			if (array[i] == text[j])
				++count;
		}
		// Add count to parallel array of character
		counts[i] = count;
	}

	sort_by_index(array, counts, 5);
	
	/*for (int i = 0; i < 5; i++) {
		cout << "array" << array[i] << endl;
	}

	for (int i = 0; i < 5; i++) {
		cout << "counts" << counts[i] << endl;
	}*/

	char* copy_array = copy_array_char(array, 5);
	int* copy_counts = copy_array_int(counts, 5);
	
	int size_tree = 5;
	TreeNode *sum = NULL;
	while (size_tree > 1) {
		// (until only 1 node)  sort(a);  
		// 2 first element  
		TreeNode *left = new TreeNode;
		left->left = NULL;
		left->right = NULL;
		TreeNode *right = new TreeNode;
		right->left = NULL;
		right->right = NULL;


		if (array[size_tree - 1] == '*') {
			left = sum;
		}
		else {
			left->key = array[size_tree - 1];
			left->data = counts[size_tree - 1];
		}
		
		if (array[size_tree - 2] == '*') {
			right = sum;
		}
		else {
			right->key = array[size_tree - 2];
			right->data = counts[size_tree - 2];
		}

		// parent (sum)  
		sum = new TreeNode;
		sum->left = NULL;
		sum->right = NULL;

		sum->key = '*';
		sum->data = left->data  + right->data;
		sum->left = left;
		sum->right = right;

		// Delete 2 element & Replace Sum element
		size_tree -= 1;
		array[size_tree - 1] = sum->key;
		counts[size_tree - 1] = sum->data;

		// Sort 
		sort_by_index(array, counts, size_tree);

		for (int i = 0; i < size_tree; i++) {
			cout << array[i] << " -- " << counts[i] << endl;
		}

		cout << endl;
	}

	cout << "post_order" << endl;
	post_order(sum);

	std::string code;
	//generate_code(sum, code);

	int* temp_array = new int[10];
	generate_code(sum, map_code, temp_array, 0);

	write_map_to_file_name(map_code, "map_code");

	encrypt_txt_with_map_return_file(text, map_code, "encrypted_text");

	/*for (int i = 0; i < 5; i++) {
		cout << "copy_array " << copy_array[i] << " - copy_counts " << copy_counts[i] << endl;
		cout << "lookup " << lookup(sum, copy_array[i], copy_counts[i]) << endl;
	}*/

	return 0;
}


