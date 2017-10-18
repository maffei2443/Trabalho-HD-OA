#include <iostream>
#include <typeinfo>
#include <vector>
#include "estruturas.hpp"
#include "view.hpp"

#define CLEAR for(int i =0; i < 50; i++)	cout << '\n';

/*#ifdef _WIN32			// Tah printando caracteres estranhos :(
#define CLEAR system("cls")
#else
#define CLEAR system("clear")
#endif*/

using namespace std;


int main(void){
	vector<int> v(2, 1);
	for(auto x : v){
		cout << x << endl;
	}
	char op = 'e';
	while(1){
		CLEAR;
		menu_principal();
		cin >> op;
		getchar();
		if(op == '1')
			escreve_HD_virtual();
		else if(op == '2')		
			le_HD_virtual();
		else if(op == '3')
			apaga_HD_virtual();
		else if(op == '4')
			mostra_FAT();
		else if(op == '5')
			break;
		getchar();
	}
	return 0;
}