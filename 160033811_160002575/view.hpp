#ifndef _VIEW_HPP_
#define _VIEW_PHP_
#include<iosfwd>


#ifndef ui
	#define ui unsigned int
#endif


namespace view{

	using namespace std;
	void cls();

	void continui();
	void menu_principal();

	void escreve_HD_virtual();
	void le_HD_virtual();
	void apaga_HD_virtual();
	void mostra_FAT();	/// Devvve receber a TABELA FA como parametro
	void empty_FAT();

	void lotado_HD();
	void inseriu_cilindro(ui);
	void arquivo_nao_encontrado();

	void exit();
};
#endif