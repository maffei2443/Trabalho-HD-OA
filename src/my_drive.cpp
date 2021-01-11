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
