#include "view.hpp"
#include <iostream>

namespace view{
	using namespace std;
	void menu_principal(){
		cout << "1 - Escrever arquivo\n";
		cout << "2 - Ler arquivo\n";
		cout << "3 - Apagar arquivo\n";
		cout << "4 - Mostrar tabela FAT\n";
		cout << "5 - Sair\n";
	}

	void escreve_HD_virtual(){
		cout << "Digite o nome do arquivo a ser lido do HD real e\n";
		cout << "gravado no HD virtual.\n";
	}

	void le_HD_virtual(){
		cout << "Digite o nome do arquivo a ser lido do HD virtual e\n";
		cout << "gravado no HD real.\n";

	}

	void apaga_HD_virtual(){
		cout << "Digite o nome do arquivo a ser apagado do HD virtual.\n";
	}

	void mostra_FAT(){
		cout << "Implementar impressao da tabela FAT\n";
	}
}