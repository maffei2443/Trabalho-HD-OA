#include "view.hpp"
#include <iostream>
#include <cstdio>
#define CLEAR system("cls || clear")
namespace view{
	using namespace std;

	void cls(){
		CLEAR;
	}

	void continui(){
		cout << "Pressione \"ENTER\" para continuar...\n";
		getchar();
		cls();
	}
	void menu_principal(){
		view :: cls();
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
		const char * nome = "NOME:\t\t\t";
		const char* tam = "TAMANHO EM BLOCOS\t";
		const char* loc = "LOCALIZAÇAO (CLUSTERS)\t";
		const char * sec = "LOCALIZACAO (SETORES)";
		printf("%s%20s%20s\t%20s\n", nome, tam, loc, sec);
	}

	void empty_FAT(){
		cout << "Tabela FAT vazia...\n";
	}
	void lotado_HD(){
		cout << "HD lotado\n";
		view :: continui();
	}
	void inseriu_cilindro(uint x){
		cout << "Inserido no cilindro " << x << endl;		
		view :: continui();
	}

	void arquivo_nao_encontrado(){
		cout << "Desculpe, o arquivo buscado nao existe no corrente";
		cout << " local de busca.\n";
		view :: continui();
	}

	void exit(){
		cout << "Por favor, volte logo... Sentiremos sua falta\n:)\n";
	}
}

#undef CLEAR