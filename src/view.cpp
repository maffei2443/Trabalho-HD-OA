#include    <iostream>
using namespace std;
void menu_principal(void){
	cout << ("1 - Escrever arquivo\n");
	cout << "2 - Ler arquivo\n";
	cout << "3 - Apagar arquivo\n";
	cout << "Mostrar tabela FAT\n";
	cout << "Sair\n";
}

inline void opcao1(){
	cout << "Digite o nome do arquivo a ser lido do HD\n";
}

inline void opcao2(){
	cout << "Digite o nome do arquivo a ser garavado no HD real\n";
}

inline void opcao3(){
	cout << "Digite o nome do arquivo a ser apagado (do HD virtual)\n";
}

void opcao4(){
//
}
