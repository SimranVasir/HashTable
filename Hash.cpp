#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <time.h>
#include <math.h>
#include <string>
#include <cstring>

using namespace std;

const int TABLESIZE = 83;
double numEntries = 0;

struct Record{
	char key[20];
	int value;
	char flag;
};

struct Hashtable{
	Record table[TABLESIZE];
	bool Quad_Hash;
	bool Good_Hash;
	int probecounter;
} Table;

int GdH(char c[20]);

int BdH(char c[20]);

void init_table();

bool del(char c[20]);

bool insert(Record* rec);

bool search(int& index, char c[20], char intent);

int hash2(char c[20]);

Record *makeRec(string line);

Record *makenewRec(string line, int value);
/*
Starts the program, promts the user some info, 
takes the text file and creates the table from
the data in the text file, prompts user for other
tasks (ie. insert, search etc.). Unexplained behaviour occurs if input is wrong 
Pre: user enters the the correct characters during prompt process 
Post: creates a table with the 
*/
int main (int argc, char *argv[]) {
	double probecount;
	int index = 0;
	bool success;
	char seq;
	char hashf;
	char option;
	char c[20];
	Record *rec;
	int value = 0;

	string filename;
	string line;
	init_table(); // initialize table
	cout << "please enter a filename followed by .txt, (ie. name.txt)" << endl;
	cin >> filename;

	//gets single character
	while (true) {
		cout << "please choose a hash function, 'g' for good and 'b' for bad" << endl;
		cin >> hashf;
		if (hashf == 'g' || hashf == 'b') {
			break;
		}
		cout << "Invalid character, please try again" << endl;
	}
	//gets single character
	while (true) {
		cout << "please enter the probe sequence, 'd' for double hash 'q' for quadratic probing" << endl;
		cin >> seq;

		if (seq == 'd' || seq == 'q') {
			break;
		}
		cout << "Invalid character, please try again" << endl;
	}
	switch (hashf){
	case 'g': Table.Good_Hash = true; break;
	case 'b': Table.Good_Hash = false; break;
	default: exit(0);
	}
	switch (seq){
	case 'q': Table.Quad_Hash = true; break;
	case 'd': Table.Quad_Hash = false; break;
	default: exit(0);
	}

	// gets the input from file and inserts them into the table
	ifstream myfile (filename.c_str());
	if (myfile.is_open())
	{
		while (! myfile.eof() )
		{
			getline (myfile,line);
			if(line.empty()){ // checks if the string is an empty string
				break;	
			}
			rec = makeRec(line);			
			if(!insert(rec)){
				exit(0);
			}
		}
	probecount = Table.probecounter / numEntries;  
	cout << "probecount is " << probecount << endl;
    myfile.close();
	}
	else cout << "Unable to open file"; 
	// prompts the user for further choices after table is created with input from file
	while(true){
		cout << "please choose from an option" << endl;
		while(true){
			cout << "type 's' for search, 'd' for delete, 'i' for insert, 'q' to quit" << endl;
			cin >> option;
					
			if(option == 's' || option == 'i' || option == 'd' || option == 'q')
				break;
			else
				cout << "Invalid character, Please try again." << endl;
		}
		if(option == 'q'){
			exit(0);
		}

		if(option == 's'){
			cout << "please enter the key to search, just the key not the data" << endl;
			cout << "Key should be 20 letters long, not less or more" << endl; 
			cin >> line;
			success = search(index, strncpy(c , line.c_str(), 20), 'l');
			if(success)
				cout << "Key found, value is " << Table.table[index].value << endl;
			else 
				cout << "key not found" << endl;
		}
		if(option == 'd'){
			cout << "please enter the key to delete" << endl;
			cin >> line;
			if(del(strncpy(c, line.c_str(), 20))){
				cout << "Key found and deleted" << endl;
			}
		}
		if(option == 'i'){
			cout << "please enter the key insert" << endl;
			cin >> line;
			cout << "please enter data (an integer) associated with key" << endl;
			cin >> value;
			rec = makenewRec(line, value);
			if(!insert(rec)){
				exit(0);
			}

		}
	}

	return 0;
}
/*
The following bad hash function use the sum
of the ASCII character values of the string and 
mods it by the Table size.
PRE: a char array is passed in
POST: an integer representing the hash location is returned
*/
int BdH(char c[20]){
	int sum = 0;
	for(int i = 0; i<20; i++){
		sum = sum + (int) c[i];
		i++;
	}	
	return sum % TABLESIZE;	
}
/*
The following good hash function takes the first 5 letters of the key, 
assumes that to be a base 26 number(ie. A = 1, B=2, C = 3 etc.), 
converts that number to base 10 and mods the converted number 
by the Table Size 
PRE: a char array is passed in
POST: an integer representing the hash location is returned
*/

int GdH(char c[20]){
	int sum = 0;
	int it = 0;
	for(int i = 4; i>=0; i--){
		it = (int) c[i] - 65;
		sum = sum + it * (int) pow(26.0,(4-i)); // converting to base 10
	}
	return sum % TABLESIZE;
	
}
/*
Initializes table to contain records with null key, e for flag, and 0 for value
PRE: true
POST: Table is full if empty records with the above description
*/
void init_table(){
	Record empty;
	empty.flag = 'e';
	empty.key[0] = '\0';
	empty.value = 0;
	for(int i = 0; i<TABLESIZE; i++){
		Table.table[i] = empty;
	}
	Table.probecounter = 0;
	return;
}
/*
Inserts records into the table if there is 
space. Insertion location of records depend 
on their key and probe sequence chosen.
PRE: Passing in a rec pointer
POST: if there was space in the table record goes into that space
	  otherwise the correct output message is displayed
*/
bool insert(Record* rec){
	int index = 0;
	
	if(!search(index, rec->key, 'i') && index != -1){
		Table.table[index] = *rec;
		numEntries++;
		return true;
	}
	else if(index == -1){
			cout << "Table cannot accomodate the request" << endl;
			return false;
		}
	else {
		cout << "key already exists" << endl;
		return false;
	}
}
/*
if there is a record in the table containing the key, 
then that record's key becomes null, its value becomes 0 
and its flag becomes 't' for tombstone. If key is not 
found then the appropriate message is displayed.
PRE: record pointer is passed in
POST: if key was found, the key and value pair is deleted and a tombstone is set at that place
*/
bool del(char c[20]){
	int index = 0;
	if(search(index, c, 'l')){
		Table.table[index].flag = 't';
		Table.table[index].value = 0;
		Table.table[index].key[0] = '\0';
		return true;
	}
	else{
		cout << "Key is not found" << endl;
		return false;
		}
}
/*
Searches if the key is in the table already or not. 
In a simple search(when intent == 'l') it looks for 
the key and if found return true and prints the value 
associated with the key. In a search with intent 'i' 
for insert, it searches for empty or tombstone location, 
if location found it returns false and the location. 
If no location it returns false and -1.
PRE: intent is either 'l' or 'i', char array holds 20 letters which is the key 
POST: 
*/
bool search(int& index, char c[20], char intent){
	int loc = 0;	
	int figure = 50;
	if(Table.Good_Hash)
		loc = GdH(c);
	else
		loc = BdH(c);

	if(intent == 'l'){
		for(int i = 0; i<TABLESIZE; i++){
			if(Table.Quad_Hash)
				index = (loc + i*i) % TABLESIZE;
			else
				index = (loc + i * hash2(c)) % TABLESIZE;            // assuming Table size will always be greater than 3 
			switch (Table.table[index].flag){
			case 'e': return false;
			case 't': continue;
			case 'f': figure = strncmp(Table.table[index].key, c, 20); if(!figure){cout << Table.table[index].value << endl; return true;} else continue;
			}
		}
		return false;
	}
	else if (intent == 'i'){
		for(int i = 0; i<TABLESIZE; i++){
			Table.probecounter = Table.probecounter + 1;
			if(Table.Quad_Hash)
				index = (loc + i*i) % TABLESIZE;
			else
				index = (loc + i * hash2(c)) % TABLESIZE;
			switch (Table.table[index].flag){
			case 'e': return false;
			case 't': return false;
			case 'f': continue;
			}
		}
		index = -1;
		return false;
	}
			
}
/*
The hash function used in double hashing probing sequence. 
It takes the first 5 letters of the key, finds the sum of 
the ASCII value of those letters and to give k. Hash 
function is q- (k%q) where q is prime number < TABLESIZE
PRE: char array contains at least 5 characters
POST: an integer representing the hash value is returned
*/
int hash2(char c[20] ){
	int q = 3; // assuming TABLESIZE is > 3
	int k = 0;
	for(int i = 0; i<5; i++){
		k = k + (int) c[i];
	} 
	int pos = q - (k % q);
	return pos;
}
/*
Takes a string that contains the key and value and constructs 
a Record struct from it and passes back a pointer to that struct
PRE: the string passed in contains the 20 character long key 
	 followed by space followed by followed by an integer
POST: a struct Record is created with the key and data 
	  integer and a pointer to it is passed back
*/
Record *makeRec(string line){
	char pch[20]; // stores the integer data from the string
				  // assumes it wont' be longer than 20 digits
	const char *a = line.c_str();
	cout << a << endl;
	Record *rec = new Record;
	rec->flag = 'f';
	strncpy(rec->key, a, 20); //copy the key into rec-> key 
	int i = 0;
	while(!a[i+21] == '\0' ){ // gets only the data value(an int) from the array 
		pch[i] = a[i+21];
		i++;
	}
	rec->value = atoi(pch);
	return rec;
}
/*
Similar to makeRec
PRE: a string containing 20 letter key is passed in and value is passed in
POST: a record is created an added to table with the key and value
*/
Record *makenewRec(string line, int value){
	const char *a = line.c_str();
	cout << a << endl;
	Record *rec = new Record;
	rec->flag = 'f';
	strncpy(rec->key, a, 20); //copy the key into rec-> key 
	rec->value = value;
	return rec;
}
