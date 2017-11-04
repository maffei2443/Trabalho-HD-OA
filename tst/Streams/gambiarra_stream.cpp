#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
/*
using namespace std;

int main(){
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
using namespace std;
int main () {
	int length;
  ifstream is ("in.txt", ifstream :: binary);
  if (is) {
    // get length of file:
    is.seekg (0, is.end);
	length = is.tellg();

   FILE * fp = fopen("in.txt", "r");
   FILE * fp2 = fopen("out.txt", "w");
  char i[512];

  for(int j = 0; j < 3; j++){
    memset(i, 512, 0);
    fread(i, 33, 1, fp);
    printf("%s\n", i);
    fprintf(fp2, "%s\n", i);
    fseek(fp, 34, 0);
    
    memset(i, 512, 0);
    fread(i, 45, 1, fp);
    fprintf(fp2, "%s\n", i);
    printf("%s\n", i);
    fseek(fp, 34, 0);

    cout << "Tamanho : " << length << endl;
    rewind(fp);
  }
  fclose(fp);
  fclose(fp2);
}
 

  return 0;
}