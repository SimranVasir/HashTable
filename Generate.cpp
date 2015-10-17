#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <fstream>
#include <time.h>
using namespace std;

/*This file creates the data to be inserted into the hashtable.*/

void random(char r[21]);

/*generates the data to be inserted into the table. Takes an int n as the number of data rows to insert*/
void generate(string filename, int n) {
  char line[21];
  ofstream myfile (filename.c_str());
  srand ( time(NULL) );

  if (myfile.is_open())
  {
	  for(int i = 0; i<n; i++){
		random(line);
		myfile << line << " " << i+1 << endl;
	  }
	  myfile.close();
  }
  else cout << "Unable to open file";
  return;
}

void random(char r[21]){
  int num;
  // initialize random seed: 
  
  	
  
	for(int k = 0; k<20; k++){
		num = rand() % 26;// + 1;
		r[k] = 'A' + num;	
	}
  r[20] = '\0';
  return;
}



int main (int argc, char *argv[]) {
	
	generate("newData.txt", 23);
}