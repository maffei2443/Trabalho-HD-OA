#include <iostream>
#include <fstream>
#include <cstdio>
#include <typeinfo>
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "view.hpp"
#include "estruturas.hpp"

#define CLEAR system("clear || cls")

/*#ifdef _WIN32			// Tah printando caracteres estranhos :(
#define CLEAR system("cls")
#else
#define CLEAR system("clear")
#endif*/

using namespace std;


int main(void){

	HardDrive myHD;	
	auto x = myHD.g_n_cylinders();
	cout << x << endl;

	myHD.insert_file();
	myHD.insert_file();

	cout << "Clusters por trilha :: " << Track :: g_CLUSTERS() << endl;
	cout << "Clusters por cilindro :: " << Cylinder :: g_CLUSTERS() << endl;
	cout << "Clusters por HD :: " << HardDrive :: g_CLUSTERS() << endl;
	
	ifstream file("teste.txt", ifstream :: binary);
	if(file){
		FILE *fp = fopen("teste.txt", "r");
		FILE *fp2 = fopen("teste.txt", "r");
		char i[512];
		memset(i, 512, '0');
		fread(i, 512, 1, fp);
		cout << i << endl;
		fseek(fp, 300, 0);

		memset(i, 512, '0');
		fread(i, 512, 1, fp);
		if(strlen(i) == 0)
			cout << "Final do arquivo\n";
		cout << i << endl;
		fclose(fp);
		fclose(fp2);

	}

	return 10;































	cout << "Zueira\n";
	char op = 'e';
	while(1){
		CLEAR;
		view :: menu_principal();
		cin >> op;
		getchar();
		if(op == '1')
			view :: escreve_HD_virtual();
		else if(op == '2')		
			view :: le_HD_virtual();
		else if(op == '3')
			view :: apaga_HD_virtual();
		else if(op == '4')
			view :: mostra_FAT();
		else if(op == '5')
			break;
		getchar();
	}
	return 0;
}