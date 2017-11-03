#include <iostream>
#include <cstdio>
#include <typeinfo>
#include <vector>
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

	cout << "Clusters por trilha :: " << Track :: g_CLUSTERS() << endl;
	cout << "Clusters por cilindro :: " << Cylinder :: g_CLUSTERS() << endl;
	cout << "Clusters por HD :: " << HardDrive :: g_CLUSTERS() << endl;
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