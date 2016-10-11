#include <Windows.h>
#include <iostream>
#include <fstream>
#include <string>
#include <conio.h>

using namespace std;


string ORIGINAL_FILE_LOCATION = "C:\\Users\\Totoro\\Documents\\Visual Studio 2015\\Projects\\UsedStepOne\\UsedStepOne\\Debug\\Original_File.txt";

struct tree
{
	int value;
	char ch;
	tree *left;
	tree *right;
};

struct input
{
	int frequency;
	char character;
	tree *address;
};

//This node using to encode and decode
struct encrypt_decrypt_node
{
	string character;
	string encode_string;
};

void Frequency(string FileInput, input *number_input, int &number);
void bubble(input *number_input, int counter);
void post_order_transversal(tree* n); //Function transversal tree to check built tree
tree* create_node_tree(); //Create a new node tree, assign left, right to NULL and return new node tree
tree* create_tree_from_list_input_and_size_of_list_input(input *num_input, int size_of_list);
void create_huffman_code(tree* root_tree, int* a, int depth, ofstream &huffman_code_txt); //Function receive root_tree to transversal all tree to create Huffman code, and write to file Huffman_code.txt
void create_huffman_code_and_write_to_file(tree* root_tree); //Function create file "Huffman_Code.txt" and Call function create_huffman_code
void read_huffman_code(encrypt_decrypt_node *ed_node, int &number_ed_node);
void encrypt_huffman(); //Encode data of “Original_File.txt” with "Huffman_Code.txt" and save it in “Huffman_Pack.txt”
void decrypt_huffman(); //Decode data of “Huffman_Pack.txt” with "Huffman_Code.txt" and save it in “Huffman_Unpack.txt”.

int main(int argc, char* argv[])
{
	tree *root_tree = new tree;
	input *num_input = new input[256];
	int number = 0; //Size of list input

					//Count frequency and return root node (num_input) and size of list (number)
	Frequency(ORIGINAL_FILE_LOCATION, num_input, number);

	//Create tree and get the root node
	root_tree = create_tree_from_list_input_and_size_of_list_input(num_input, number);
	//cout << "Post Order:" << endl;
	//post_order_transversal(root_tree);

	create_huffman_code_and_write_to_file(root_tree);

	encrypt_huffman();

	decrypt_huffman();

	cout << "Please check files Original_File.txt, Huffman_Code.txt, Huffman_Pack.txt, and Huffman_Unpack.txt to see the resut" << endl;
	return 0;
}

void Frequency(string FileInput, input *number_input, int &number) {
	char Chr;
	int Count[256];
	ifstream fin(FileInput);
	for (int i = 0; i < 256; i++)
		Count[i] = 0;
	while (1)
	{
		fin >> Chr;
		if (fin.eof())
			break;
		Count[int(Chr)]++;
	}

	for (int i = 0; i < 256; i++)
	{
		if (Count[i] > 0)
		{
			number_input[number].address = '\0';
			number_input[number].character = char(i);
			number_input[number].frequency = Count[i];
			number++;
		}
	}
}

void bubble(input *number_input, int counter)
{
	for (int i = 0; i<counter - 1; i++)
		for (int j = i + 1; j<counter; j++)
			if (number_input[i].frequency < number_input[j].frequency)
			{
				input *temp = new input;

				temp->address = number_input[i].address;
				temp->character = number_input[i].character;
				temp->frequency = number_input[i].frequency;
				number_input[i].address = number_input[j].address;
				number_input[i].character = number_input[j].character;
				number_input[i].frequency = number_input[j].frequency;
				number_input[j].address = temp->address;
				number_input[j].character = temp->character;
				number_input[j].frequency = temp->frequency;

				delete temp;
			}
}

//Function use to test tree
void post_order_transversal(tree* n) {
	if (n != NULL) {
		cout << n->ch << " - " << n->value << endl;
		post_order_transversal(n->left);
		post_order_transversal(n->right);
	}
}

tree* create_node_tree() {
	tree* temp_tree_node = new tree;
	temp_tree_node->left = NULL;
	temp_tree_node->right = NULL;

	return temp_tree_node;
}

/*
Example: aaaabbccccdeababacaccdce
Simple explain Huffman Algorithm:
Suppose we have: list of character and frequency sorted
Algorithm repeat until list of character and frequency sorted have only one element
1/
a - 8, c - 8, b - 4, d - 2, e - 2(because the smaller node always on the left node)
sort -> same order

get 2 ndoe: d - 2, e - 2
left: e - 2
right: d - 2
sum: * - 4
sum->left = left, sum->right = right

delete 2 node d - 2, and e - 2
add sum node( * - 4 ) into list character and frequency sorted

2/
a - 8, c - 8, b - 4, * - 4
sort -> same order

get 2 ndoe: * - 4, b - 4
left: * - 4
right: b - 4
sum: * - 8
sum->left = left, sum->right = right

delete 2 node * - 4, and b - 4
add sum node( * - 8 ) into list character and frequency sorted

3/
a - 8, c - 8, * - 8
sort -> same order

get 2 ndoe: * - 8, c - 8
left: * - 8
right: c - 8
sum: * - 16
sum->left = left, sum->right = right

delete 2 node * - 8, and c - 8
add sum node( * - 16 ) into list character and frequency sorted

4/
a - 8, * - 16
sort -> * - 16, a - 8

get 2 ndoe: a - 8, * - 16
left: a - 8
right: * - 16
sum: * - 24
sum->left = left, sum->right = right

delete 2 node a - 8, and * - 16
add sum node( * - 24 ) into list character and frequency sorted

Done
*/
tree* create_tree_from_list_input_and_size_of_list_input(input *num_input, int size_of_list) {
	tree* sum = NULL;

	while (size_of_list > 1) {
		bubble(num_input, size_of_list);

		//Create 2 Node
		tree* left = create_node_tree();

		tree* right = create_node_tree();

		//Check the smallest node, because the smallest always go to left node
		//Assign to address saved in num_input
		if (num_input[size_of_list - 1].character == '*') {
			left = num_input[size_of_list - 1].address;
		}
		else {
			left->ch = num_input[size_of_list - 1].character;
			left->value = num_input[size_of_list - 1].frequency;
		}

		//Check the second smallest node
		if (num_input[size_of_list - 2].character == '*') {
			right = num_input[size_of_list - 2].address;
		}
		else {
			right->ch = num_input[size_of_list - 2].character;
			right->value = num_input[size_of_list - 2].frequency;
		}

		//Create sum node, add the value equal to sum of 2 nodes: left and right
		sum = create_node_tree();

		sum->ch = '*';
		sum->value = left->value + right->value;
		sum->left = left;
		sum->right = right;

		//Delete 2 last node and add sum node into list
		size_of_list -= 1;
		num_input[size_of_list - 1].character = sum->ch;
		num_input[size_of_list - 1].frequency = sum->value;
		//Save address into num_input list
		num_input[size_of_list - 1].address = sum;

		//cout << "after" << endl;
		//for (int i = 0; i < size_of_list; i++) {
		//	cout << num_input[i].character << " -- " << num_input[i].frequency << endl;
		//}

		//break;
	}

	return sum;
}

/*
Function transversal all leaf nodes
First function start with depth = 0
Function recursive transversal down the tree
If met left node assign 0 to the code appropriate with the depth
Else met right node assign 1
*/
void create_huffman_code(tree* root_tree, int* code, int depth, ofstream &write_file) {
	if (root_tree->left == NULL && root_tree->right == NULL) {
		//cout << "Leaf Found Key: " << root_tree->ch << "\t data: " << root_tree->value << endl;

		write_file << root_tree->ch << ":";
		//cout << "Code:";
		for (int i = 0; i < depth; i++) {
			write_file << code[i];
			//cout << code[i];
		}

		write_file << endl;
		//cout << endl;
	}

	if (root_tree->left != NULL) {
		code[depth] = 0;
		create_huffman_code(root_tree->left, code, depth + 1, write_file);
	}

	if (root_tree->right != NULL) {
		code[depth] = 1;
		create_huffman_code(root_tree->right, code, depth + 1, write_file);
	}
}

/*
Function create "Huffman_Code.txt"
Call function create_huffman_code(root_tree, code, depth, huffman_code_txt)
to write into file
*/
void create_huffman_code_and_write_to_file(tree* root_tree) {
	int *code = new int[256];
	int depth = 0;
	ofstream huffman_code_txt;
	huffman_code_txt.open("Huffman_Code.txt");

	create_huffman_code(root_tree, code, depth, huffman_code_txt);

	huffman_code_txt.close();
}

/*
Function parameter: *ed_node, &number_ed_node
read Huffman_Code, plit string
Add to linked_list ed_node
*/
void read_huffman_code(encrypt_decrypt_node *ed_node, int &number_ed_node) {
	ifstream huffman_code_txt;
	string str;
	string character;
	string encode_value;

	huffman_code_txt.open("Huffman_Code.txt");

	while (getline(huffman_code_txt, str)) {
		character = str.substr(0, 1);
		encode_value = str.substr(2, str.find("\n"));

		//cout << "character:" << character << endl;
		//cout << "encode_value:" << encode_value << endl;

		ed_node[number_ed_node].character = character;
		ed_node[number_ed_node].encode_string = encode_value;
		number_ed_node++;
	}

	huffman_code_txt.close();
}

//Encrypt file Original_File.txt and return file Huffman_Pack.txt
void encrypt_huffman() {
	encrypt_decrypt_node *ed_node = new encrypt_decrypt_node[256];
	int number_ed_node = 0; //Size of list encrypt_decrypt_node
	ifstream original_file;
	ofstream huffman_pack;
	char character;

	read_huffman_code(ed_node, number_ed_node);

	original_file.open(ORIGINAL_FILE_LOCATION);
	huffman_pack.open("Huffman_Pack.txt");
	while (original_file.get(character)) {
		for (int i = 0; i < number_ed_node; i++) {
			//cout << ed_node[i].character << "--" << ed_node[i].encode_string << endl;
			if (ed_node[i].character[0] == character) {
				huffman_pack << ed_node[i].encode_string;
			}
		}
	}

	original_file.close();
	huffman_pack.close();
}

//Decrypt file Huffman_Pack.txt and return file Huffman_Unpack.txt
/*
Simple idea:
get all ed_node(linked_list)
get number_ed_node

read the Huffman_Pack.txt
suppose first index need to scan is 0
scan all possible encode
if found assign new index need to scan equal: index scan + encode.length() - 1
(because function substr Returns a newly constructed string object with its value initialized to a copy of a substring of this object.)
Ex:
std::string str="We think in generalities, but we live in details.";
// (quoting Alfred N. Whitehead)

std::string str2 = str.substr (3,5);     // "think"

std::size_t pos = str.find("live");      // position of "live" in str

std::string str3 = str.substr (pos);     // get from "live" to the end
end the scan loop

begin scan Huffman_Pack.txt with the new index
*/
void decrypt_huffman() {
	encrypt_decrypt_node *ed_node = new encrypt_decrypt_node[256];
	int number_ed_node = 0; //Size of list encrypt_decrypt_node
	ifstream huffman_pack;
	ofstream huffman_unpack;
	string str_line;
	string total_text;

	read_huffman_code(ed_node, number_ed_node);

	huffman_pack.open("Huffman_Pack.txt");
	huffman_unpack.open("Huffman_Unpack.txt");

	while (getline(huffman_pack, str_line)) {
		total_text.append(str_line);
	}

	//cout << "total_text :" << total_text << endl;
	//cout << "total_text length:" << total_text.length() << endl;

	//Using function 
	for (int i = 0; i < total_text.length(); i++) {
		//Transversal all the text
		//Scan text with length of ed_node
		for (int j = 0; j < number_ed_node; j++) {
			//cout << "top i:" << i << endl;
			//Get the length appropriate with ed_node[j].encode_string.length()
			int ed_node_encode_string_length = ed_node[j].encode_string.length();
			//Substr the total_text appropriate with ed_node[j].encode_string
			string sub_total_text = total_text.substr(i, ed_node_encode_string_length);
			string ed_node_encode_string = ed_node[j].encode_string;

			//cout << "ed_node_encode_string_length" << " ---- " << ed_node_encode_string_length << endl;
			//cout << ed_node_encode_string << " ---- " << sub_total_text << endl;

			if (ed_node_encode_string == sub_total_text) {
				//cout << "Found!!" << endl;
				//cout << "ed_node[" << j << "].character: " << ed_node[j].character << endl;
				huffman_unpack << ed_node[j].character;
				i = i + ed_node_encode_string_length - 1;
				//cout << "bottom i:" << i << endl;

				//Found break and scan with the new i					
				break;
			}
		}
	}


	huffman_pack.close();
	huffman_unpack.close();
}