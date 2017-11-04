#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
/*
using namespace std;

uint main(){
	FILE * fp = fopen("in.txt", "r");
	char i[512];

	fread(i, 512, 1, fp);
	printf("%s\n", i);
	printf("%lu\n", strlen(i));
	if(strlen(i) == 0)
		printf("FIM\n");
//	fscanf(fp, "%s\n", i);

	fclose(fp);
}
//*/


// read a file into memory
#include <iostream>     // std::cout
#include <fstream>      // std::ifstream
#include <cmath>
using namespace std;
int main () {
	uint length;
	ifstream is ("in.txt", ifstream :: binary);
	if (is) {
	    // get length of file:
    is.seekg (0, is.end);
	length = is.tellg();

	FILE * fp = fopen("in.txt", "r");
	FILE * fp2 = fopen("out.txt", "w");

	uint x = 512;
	uint y = 8;
	
	char i[x];
	memset(i , x, '0');

	uint it = length/y;
	uint off = 0;

	cout << it << endl;
	
	for(uint j = 0; j < it; j++){

	    fread(i, y, 1, fp);	 
	    fseek(fp, y, off);
   		off += y;
    	
	    for(uint k = 0; k < y; k++)
	    	printf("%c", i[k]);
	    memset(i , x, '0');
	}
	if(length > it * y){		// ainda falta um pouco pra ler    do arquivo
		
	    memset(i , x, '0');
		fread(i, length - it * y, 1, fp);
    	printf("%s\n", i);
	}


  fclose(fp);
  fclose(fp2);
}
 

  return 0;
}