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

// TODO: gravar o arquivo por CLUSTERS, e o ultimo cluster setor a setor.

int main(void){

	HardDrive myHD;	

	ui op = 0;
	while(1){
		view :: menu_principal();
		cin >> op;
		if(op == 1){
			myHD.insert_file();
//			getchar();
		}
		else if(op == 2){
			myHD.show_file();
//			getchar();
		}

		else if(op == 3){
			myHD.remove_file();
//			getchar();
		}

		else if(op == 4){
			myHD.show_FAT();
//			getchar();
		}

		else if(op == 5){
			view :: exit();
//			getchar();
			break;
		}
		else
			view :: continui();
	}
	return 0;
}
/*
	auto x = myHD.g_n_cylinders();
	cout << x << endl;
//	for(int jj = 0; jj < 10; jj++)
	myHD.insert_file();

	myHD.show_file();

	
	string in;
	
	return 15;
/*	
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
//*/

/*



	cout << "Zueira\n";
	char op = 'e';
	while(1){
		CLEAR;
		view :: menu_principal();
		cin >> op;
//		getchar();
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
//		getchar();
	}
	return 0;
}
//*/