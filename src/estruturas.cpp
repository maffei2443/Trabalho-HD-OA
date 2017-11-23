#include "estruturas.hpp"
#include "view.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>	// for 'memset'
#include <math.h>	// for 'ceil'
#include <vector>
using namespace std;

inline ui which_cylinder(ui cluster){
	return cluster / ((Qtt :: HARDDRIVE / Qtt :: CYLINDER) * (Qtt :: TRACK / Qtt :: CLUSTER));
}

ui which_track(ui cluster){
	ui aux = 0;
	ui div = (Qtt :: HARDDRIVE / Qtt :: CYLINDER) * (Qtt :: TRACK / Qtt :: CLUSTER);
	if(cluster >= div)
		aux = which_cylinder(cluster);
	cluster -= aux * div;
	return cluster / (Qtt :: TRACK / Qtt :: CLUSTER);
}

ui which_cluster(ui cluster){
	return ( cluster - which_cylinder(cluster) * 75 - which_track(cluster) * 15 );
}
// class Fatlist

Fatlist :: Fatlist(string const& file, ui const& first){
	full = true;
	this->s_name(file);
	this->first_sector = first;
}

// class FatEent



// class FatTable

FatTable :: FatTable() : fatlist(0){//, fatent(0){
	for(ui i = 0; i < (Qtt :: HARDDRIVE / Qtt :: CLUSTER); i++)
		unused.insert(i);
	memset(fatent, (Qtt :: HARDDRIVE / Qtt :: CLUSTER) * sizeof(FatEnt), 0);
	memset(clusters, 750 * sizeof(bool), false);
	usado = 0;
	if (clusters[0] == true || clusters[749] == true)
	{
		throw 99;
	}
}

ui FatTable :: insert(const char* name, const ui& size){
	
}

/*
vector<ui> FatTable :: alloc_space(const string& file, ui size){
	cout << "Clusters livres ANTES:: " << this->unused.size() << endl;
	cout << "Size recebido : "<< size << endl;
	ui qtt = (int) ceil((float)((int)size) / (int)Qtt::CLUSTER);		// Pega o nº do setor no qual estah localizado o ultimo bytee valido
	cout << "Em termos de clusters: " << qtt << endl;
	
	const ui ret = qtt;
	if(qtt > this->unused.size() )	{cout << "!!!!!!!!!! HD nao suporta mais tal arquivo !!!!!!!!!!" << endl;	return vector<ui>(0);}

	// Checa se existe arquivo de mesmo nome. Se sim, RETURN.
	for(auto it = this->fatlist.cbegin(); it != fatlist.cend(); it++){
		auto aux = (*it);
		string old_file = aux.g_file_name();
		if( old_file == file ){
			cout << "Erro : arquivo de mesmo nome jah existente.\n";
			return vector<ui>(0);
		}
	}
	/////////////////////////////////////////////////////////
//	cout << "FIRST :: " << first << endl;
	ui first = *(this->unused.begin());	// first === primeiro cluster ocupado
										// a posicao do começo do arquivo.
	
	if(fatlist_reuse.size() == 0)	// Nao tem espaco no vector pra reutilizar
		this->fatlist.push_back(Fatlist(file, first));	// Insere o nome/primeiro cluster
	else{
		this->fatlist[ *(this->fatlist_reuse.begin()) ] = Fatlist(file, first);
		this->fatlist_reuse.erase( *(this->fatlist_reuse.begin()) );
	}

	
	vector<ui> lista(0);

	this->unused.erase(first);	// Nao eh mais nao usado
	this->used.insert(first);	// Agora eh usado
	if(qtt < 2){	// Arquivo vazio ou com tam < Qtt :: CLUSTER
		lista.push_back( first );	// Lista de posicoes do cluster do arquivo recebido
		if(unused.size() == 0)
			this->full = true;
		fatent[first] = FatEnt(true, true, -1);	// used = true, eof = true, next = -1 (valor qqer)
		cout << "^^^^^ Arquivo \"" << file << "\" ocupa apenas um cluster\n";
//		cout << cluster << endl;
		return lista;
	}


	qtt--;

	ui where_cylinder = which_cylinder(first);
	ui where_treck = which_track(first);
	ui where_cluster = first - where_cylinder * 150 - where_treck * 15;
	
	ui next = where_cluster + where_treck * 15 + where_cylinder * 150;
	fatent[first] = FatEnt(true, false, next);
	bool change_cylinder = false;
	for(;qtt > 0; qtt--){	// Quando qtt == 1, eh o ultimo setor a ser inserido

		where_treck++;	// Tentar colocar na proxima trilha
		where_treck %= 5;

		if(where_treck == 0)	// Tentar colocar no proximo cluster  da trilha 0.
			where_cluster++;
		where_cluster %= 15;

		if(where_cluster == 0 and where_treck == 0)	// Passar pro proximo cilindro.
			where_cylinder++;
		where_cylinder %= 10;
		next = where_cluster + where_treck * 15 + where_cylinder * 150;

		if( unused.count( next ) != 0 ){// Nao usou o proximo proposto insere ele.
			lista.push_back(first);
			this->unused.erase(first);
			this->used.insert(first);
			fatent[first] = FatEnt(true, false, next);
			first = next;
		}

	}

	// Inserir o ultimo setor; calcular também seu tamanho exato

	fatent[first] = FatEnt(true, true, -1);
	lista.push_back( first );
	this->unused.erase( first );
	this->used.insert( first );
	// Todo os cluster na tbela fat adequadamente enumerados
	for(auto it = lista.cbegin(); it != lista.cend(); it++)
		cout << "Cluster ocupado : " << (*it) << endl;
	return lista;
	getchar();
	getchar();
}
*/

// Obs: parte do pre-suposto que existe algum espaço livre.
// Obs2 : NAO modifica o ESTADO de FatEnt
ui FatTable :: alloc_first(){
	for(int l = 0; l < 5; l++)	 			// tenta inserir na primeira superficie
		for(int k = 0; k < 10; k++)		// do primeiro cilindro que encontrar
			for(int i = 0; i < 15; i++)			// que tenha um cluster livre
				if( this->clusters[ i + k * 75 + l * 15 ] == false)
					return (i + k * 75 + l * 15);
}

ui FatTable :: alloc_nxt(ui cluster, ui track, ui cylinder){
	ui onde;
	if(this->full == true){cout << "FatTable :: alloc_nxt throw\n";throw 999;}

	ui mod = cluster;
	bool first = true;
	bool alloc = false;	

	int i, j, k;
	k = cylinder;
	int debug = 0;		// throw se dar mais de 749 iteracoes :/
	for(i = cluster; alloc == false;){		

		for(j = track + 1; alloc == false and j != 0; j++){
			j %= 5;
			if(j == 0){				// Passou pela ultima trilha; passar ao proximo cluster 
									// da primeira trilha do mesmo cilindro.
					break;
			}
			else if(this->clusters[i + 15 * j + 75 * k] == false){
				alloc = true;
				onde = (i + 15 * j + 75 * k);
			}
		}
		track = -1;	// Para sempre começar pela trilha mais em cima, com exceçao claro da primeira na qual
					// tenta-se inserir na TRILHA SEGUINTE aa passada por parametro.
					// Passar para o proximo cluster, pois nao inseriu em nenhuma das trilhas abaixo.
		i++;		// Iterar AQUI!! Senao, zoa a questao da comparacao com 'mod'.
		i %= 15;	
		if(i == mod){		// FINALMENTE deu a primeira volta.
			mod = -1;		// Para nao mais entrar aqui.
			first = false;	// Apenas para nao dar voltas a toa :)
			i = 0;			// Zera o cluster
		}
		else if(i == 0 and !first ){	// Nao eh mais a primeira volta. Checar se o cluster zerou; se sim, trocar de cilindro.
				k++;		
				k %= 10;	// Para nao estourar o cilindro.
		}
		
		debug++;
		if(debug > 750)	{cout << "Erro em FatTable :: alloc_nxt" << debug << "\n";}
	}
	return onde;
}

vector<ui> FatTable :: alloc_space(const string& file, ui size){
	cout << "Clusters livres ANTES:: " << (750 - usado) << endl;
	cout << "Size recebido : "<< size << endl;
	ui qtt = (int) ceil((float)((int)size) / (int)Qtt::CLUSTER);		// Pega o nº do cluster necessarios
	cout << "Em termos de clusters: " << qtt << endl;
	
	// Checa para ver se ainda cabe nos clusters restantes.
	if(qtt > this->usado == 750 )	{cout << "!!!!!!!!!! HD nao suporta mais tal arquivo !!!!!!!!!!" << endl;	return vector<ui>(0);}

	//	Encontra o cluster adequado para a primeira posicao.
	// CLuster encontrado.

	// Checa se existe arquivo de mesmo nome. Se sim, RETURN.
	for(auto it = this->fatlist.cbegin(); it != fatlist.cend(); it++){
		auto aux = (*it);
		string old_file = aux.g_file_name();
		if( old_file == file ){
			cout << "Erro : arquivo de mesmo nome jah existente.\n";
			return vector<ui>(0);
		}
	}
	/////////////////////////////////////////////////////////

	ui first = alloc_first();
	this->clusters[first] = true;
	if(this->fatlist_reuse.size() == 0)	// Nao tem espaço para reaproveitar no vetor de nomes.
		this->fatlist.push_back( Fatlist(file, first) );
	else{
		auto reused = *(this->fatlist_reuse.cbegin());	
		this->fatlist[reused] = Fatlist(file, first);		// Reaproveita o espaço e 
		this->fatlist_reuse.erase( reused );				// remove esse espaço dos livres para reuso.
	}
		
	vector<ui> lista(0);				// Lista contendo os cluter ussados

	if(qtt < 2){	// Arquivo vazio ou com tam < Qtt :: CLUSTER
		lista.push_back( first );	// Lista de posicoes do cluster do arquivo recebido
		if(unused.size() == 0)
			this->full = true;
		fatent[first] = FatEnt(true, true, -1);	// used = true, eof = true, next = -1 (valor qqer)
		this->clusters[first] = true;
		if(qtt == 0)
			cout << "Arquivo vazio. TROLL\n";
		else
			cout << "^^^^^ Arquivo \"" << file << "\" ocupa apenas um cluster\n";
		cout << first << endl;
		return lista;
	}

	qtt--;

	ui where_cylinder, where_treck, where_cluster;
	where_cylinder = which_cylinder(first);
	where_treck = which_track(first);
	where_cluster = first - where_cylinder * 150 - where_treck * 15;
	// Parametros sao as posicoes do cluster predecessor.
	ui next = alloc_nxt(where_cluster, where_treck, where_cylinder);	// Proximo cilindro alocado para insercao.
	// assert(qtt >= 2)
	cout << "First :: " << first << endl;
	while( qtt > 0 ){	// Inserir o resto do arquivo.
		lista.push_back( first );
//		printf("first == %d\n", first);
//		printf("next == %d\n", next);
		this->clusters[first] = true;
		this->fatent[first] = FatEnt(true, false, next);
		qtt--;
		if(qtt > 0)
			first = next;

		where_cylinder = which_cylinder(first);
		where_treck = which_track(first);
		where_cluster = first - where_cylinder * 150 - where_treck * 15;
			
		next = alloc_nxt(where_cluster, where_treck, where_cylinder);
	}

	// Inserir o ultimo setor; calcular também seu tamanho exato

	this->clusters[first] = true;
	this->fatent[first] = FatEnt(true, false, next);	// used = true, eof = false, next = -1
	printf("pushed_back == %d\n", first);
	lista.push_back( next );
	this->fatent[next] = FatEnt(true, true, -1);	// used = true, eof = false, next = -1
	printf("pushed_back == %d\n", next);
	this->unused.erase( next );
	this->used.insert( next );
	// Todo os cluster na tbela fat adequadamente enumerados
	for(auto it = lista.cbegin(); it != lista.cend(); it++)
		cout << "Cluster ocupado : " << (*it) << endl;
	return lista;
	getchar();
	getchar();
}

bool FatTable :: free_space(const string& get_out){
	bool found = false;
	ui first;
	ui index = 0;
	vector <ui> lista;
	for(auto it = this->fatlist.begin(); it != this->fatlist.end() and !found; it++){
		if( (*it).g_file_name() == get_out ){
			found = true;
			cout << "Esperado :: SIM encontrar o arquivo\n";
			lista = this->g_clusters_list(get_out);
			(*it) = Fatlist();		// Reseta objeto; pra poder usar novamente
			cout << "Esperado :: NAO encontrar o arquivo\n";
			auto lixo = this->g_clusters_list(get_out);
			getchar();
		}
		else
			index++;
	}

	if(found == true){

		cout << "Na funcao [FatTable :: free_space]\n";
		cout << "Arquivo foi ENNCONTRADO\n";
		cout << "Vamos exluir o arrquivo...\n";
		this->fatlist[index] = Fatlist();		// Para representar
		this->fatlist_reuse.insert(index);
		// for (auto it : lista){
		for(auto it = lista.begin(); it != lista.end(); it++){
			ui cluster = (*it);
			this->fatent[cluster] = FatEnt();	// Cluster livre para reuso
			this->clusters[cluster] = false;		// Cluster livre novamente :)
			this->used.erase( cluster );
			this->unused.insert( cluster );
		}
		this->full = false;
	}
	else{
		view :: arquivo_nao_encontrado();
		return false;
	}
	cout << "Apagou arquivo na funcao FatTable :: free_space\n";
	return true;
}

vector<ui> FatTable :: clusters_file(const string& file){
	if(file == file)	cout << "ok\n" ;
	bool again = true;
	vector<ui> ordem(0);
	for(auto x = fatlist.begin(); x != fatlist.end()
		 and (again == true); x++){
		Fatlist aux2 = (*x);
		string aux = aux2.g_file_name();
		if( aux == file ){	//    Encontrou o arquivo
			cout << "Opa! Cabeça em " << aux2.g_first_cluster() << endl;
			ui cluster = (*x).g_first_cluster();
			while(this->fatent[cluster].eof == false){
				ordem.push_back(cluster);
				cluster = this->fatent[cluster].next;
			}
			again = false;
		}
	}
	return ordem;
}

vector<ui> FatTable ::  g_clusters_list(const string& file){
//	cout << "Buscando por :: " << file << endl;
	int first, next;
	bool again = true;
	first = next = -1;
	for(auto it = this->fatlist.begin(); it != this->fatlist.end() and again; it++){
		if( (*it).g_file_name() == file){
			first = (*it).g_first_cluster();
//			cout << "Encontrou o arquivo.\nfirst == " << first << endl;;
			again = false;
		}
	}
	if(again == true){
		cout << "[FatTable ::  g_clusters_list] Arquivo nao encontrado\n";
		return vector<ui>(0);
	}
	vector<ui> lista(0);
//	cout << "Primeiro cluster do arquivo : " << first << endl;

	bool eof;
	if(this->fatent[first].eof == true);	// Caso especial de arquivo com apenas um cluster		
	else
		do{
	//		cout << "first == " << first << endl;
			lista.push_back(first);
			first = this->fatent[first].next;
			eof = this->fatent[first].eof;
		}while(eof == false);
	lista.push_back( first );
	//	cout << "first == " << first << endl;
//	getchar();
	return lista;
}

void FatTable :: show(){
	int count = 0;
	for(auto it = this->fatlist.begin(); it != this->fatlist.end() ;it++){
		string name = (it)->g_file_name();
		if(name.size() == 0)	continue;
		count++;
		auto lista = g_clusters_list(name);
		cout << "Numero de clusters :: " << lista.size() << endl;
		long unsigned int aux = name.size();
//		cout << "size == " << name.size() << endl;
		for(int i = 0; i < (20 - aux); i++ )
			name += " ";
		printf("%s\t%lu Bytes\t\t", name.c_str(),  lista.size() * Qtt :: CLUSTER);
		for(auto it2 = lista.cbegin(); it2 != lista.cend(); ){
			printf("%d", (*it2));
			it2++;
			if(it2 == lista.cend())
				printf("\n");
			else
				printf(", ");
		}
	}
	if(count == 0)
		view :: empty_FAT();
}



// class Sector

// private

// public

ui Sector :: insert(const char* cluster, const ui& next, const ui& pos, string op = string("")){	// pos == 0,1,2,3 (nº do setor NO cluster)
//	CAGUEI! Sa porra fica dando SEMPRE this->used == 1. Como?!?!
///	if(this->used > 0){cout<< this->used << " Erro: sobreposicao de setores\n";throw 1.1;}
	this->used = true;

	for(ui i = pos * Qtt :: SECTOR; i < (pos+1) * Qtt :: SECTOR; i++)
		this->byte_s[i % Qtt :: SECTOR] = cluster[i];

	this->used = true;
	if(op == "last"){
		this->next = -1;
		this->eof = true;
		this->last_valid = next;	// Ou seja, todos os bytes do setor sao utilizados
	}
	else{
		this->next = next;
		this->eof = false;
		this->last_valid = Qtt :: SECTOR;
	}
}

// class Cluster
// URGENTE :: VERIFICAR AS DUSA FUNCOES ABAIXO!!

ui Cluster :: insert(const char* cluster, const ui& next, string op = string("") ){
//	cout << "Inserindo no cluster\n";
//	if(this->used == true){cout<<"Erro: sobreposicao de clusters\n";throw 1.1;}
	this->used = true;
	if(op != "last"){
		this->next = next;
		this->eof = Qtt :: CLUSTER;		
	}
	else{	// Ultimo setor
//		cout << "Cluster :: insert <<<>>> Ultimo clusterrr \n";
		this->next = -1;
		this->eof = next;	// polimorfismo a nivel de parametro :o
		ui i;
/*		cout << "eof == " << eof << endl;
		cout << "Qtt :: SECTOR == " << Qtt :: SECTOR << endl;
		cout << "What?????!?! eof % Qtt :: SECTOR == " << (next / Qtt :: SECTOR) << endl;
//*/
		for(i = 0; i < eof / Qtt :: SECTOR; i++)			// Inserir em cada um dos setores
			this->sector[i].insert(cluster, next, i);
//		cout << "Cluster :: insert <<<>>> Ultimo setorrr\n";
		this->sector[i].insert(cluster, eof - i * Qtt :: SECTOR, i, op);
		return true;
	}
//	cout << "this->next " << this->next << endl;
//	cout << "next " << next << endl;
	
	for(ui i = 0; i < this->MAX; i++)			// Inserir em cada um dos setores
		this->sector[i].insert(cluster, next, i);

	return true;
}

// @@@ Pegar o tamanho do ultimo cluster pelo tamanho da string
char* Cluster :: g_cluster_content(){
	char* ret;
	ret = (char*)malloc((this->eof) * sizeof(char));


	for(ui i = 0; i < MAX; i++){
		memset(ret, eof, '\0');
		const char* noob = this->sector[i].g_byte_s();
		for(int k = 0; k < this->sector[i].g_last_valid(); k++)
			ret[k + i * Qtt :: SECTOR] = noob[k];
		
		if(this->sector[i].g_eof() == true)
			i = MAX;
	}
//	cout << "Testando conteudo do cluster\n";
/*	for(ui i = 0; i < MAX; i++){
		memset(ret, eof, '\0');
		for(int k = 0; k < this->sector[i].g_last_valid(); k++)
			cout << ret[k + i * Qtt :: SECTOR];
	}
	cout << endl;//*/
//	getchar();
	return ret;
}

// class Track
// private

// public

bool Track :: s_full(){
	if(this->used == this->CLUSTERS)
		this->full = true;
	else
		this->full = false;
	return this->full;
}

// Retorna o numero do novo cluster ocupado

ui Track :: insert(const char* cluster, ui now, const ui& next, string op = string("")){
	bool ret = this->cluster[now].insert(cluster, next, op);
	if(ret == false)	return false;
	
	this->used++;
	this->s_full();
//	cout << "cluster usados na track :: " << this->used << endl;
//	cout << "Saiu de Track :: insert\n";
	return true;
}

char* Track :: g_cluster_content(ui first){
	return this->cluster[first].g_cluster_content();
}
// class Cylinder

// private

bool Cylinder :: s_full(){
	if(used == this->MAX_CLUSTERS)
		this->full = true;
	else
		this->full = false;
	ui i = 0;
	for(auto x = this->full_tracks.begin(); x != this->full_tracks.end(); x++ ){
		if( (*x) > 14 )	i++;
		//cout << "Trilha com " << (*x) << " clusters\n";
	}
	
//	cout << "Trilhas cheias :: " << i << endl << endl;
	return this->full;
}

// public

ui Cylinder :: insert(const char* cluster, ui first, const ui& next, string op = string("")){	// TODO: implementar isso de verdade.
//	if(this->full == true){cout << "LLotadaoO!\n";	return false;}

	ui div = Qtt :: TRACK / Qtt :: CLUSTER;
	ui where = which_track(first);

//	cout << "Trilha de insercao :: " << where << endl;
	if(this->track[where].insert(cluster, first - where * div, next, op));
	else	cout << ">>> Erro em Cylinder :: insert\n";
	
	this->used++;
	this->full_tracks[where]++;
	this->s_full();
	return true;
}

char* Cylinder :: g_cluster_content(ui first){
	ui div = Qtt :: TRACK / Qtt :: CLUSTER;
	ui where = which_track(first);

//	cout << "\nTrilha de busca :: " << where << endl;
	auto x = this->track[where].g_cluster_content(first - where * div);
	return x;
}

// class HardDrive

// private

// Obs: 'size' eh em termos de BYTES
// Retorna o ultimo cilindro no qual o arquivo foi inserido

bool HardDrive :: set_full(  ){

	if(used == MAX_CLUSTERS)
		this->full = true;	
	else
		this->full = false;
	return this->full;
}

ui HardDrive :: insert_file(const string& file, ui size){

	vector<ui> to_put = this->fat.alloc_space(file, size);
	if(to_put.size() == 0){
		cout << "[HardDrive :: insert_file] Erro : HD jah contem arquivo de mesmo nome.\n";
		cout << "ou\n";
		cout << "[HardDrive :: insert_file] Erro : HD nao tem espaço disponivel suficiente.\n";
		return 0;
	}
	for(ui i =0; i < to_put.size(); i++ );
//		cout << "Alocou cluster " << to_put[i] << endl;
	
	ui tracks = (this->CYLINDERS * Qtt :: TRACK/Qtt :: CLUSTER);

	bool again = true;
	ui ultimo = to_put.size() - 1;
	ui j = 0;
	char cluster[Qtt :: CLUSTER];
	FILE *fp = fopen(file.c_str(), "r");
	ui offset = 0;					// Começa lendo a partir do inicio do arquivo.
	ui div = (Qtt :: HARDDRIVE / Qtt :: CYLINDER) * (Qtt :: TRACK / Qtt :: CLUSTER);	//150
	for(; j < ultimo ; j++){	

		fread(cluster, Qtt :: CLUSTER, 1, fp);
		fseek(fp, Qtt :: CLUSTER, offset);
		offset += Qtt :: CLUSTER;
		size -= Qtt :: CLUSTER;
//		for(int i = 0; i < Qtt :: CLUSTER; i++)		// $$$$$
//			cout << cluster[i];

//		cout << "cluster de insercao == " << to_put[j] << endl;
		ui where = which_cylinder( to_put[j] );

//		cout << "cilindro de insercao :: " << where << endl;
		if( !this->cylinder[where].insert( cluster, to_put[j] - where * div, to_put[j+1] ))	{cout << "Cylinder :: insert com problemas\n";	throw 100;}
	}
	// Inserir caso especial; ou seja, o ULTIMO cluster, que pode ser setores disperdiçados :/	
//	cout << "ULTIMO cluster de insercao == " << to_put[j] << endl;
	ui where = which_cylinder( to_put[j] );
//	cout << "ULTIMO cilindro de insercao :: " << where << endl;
	
	char last_cluster[size];
	fread(last_cluster, size, 1, fp);
		for(int i = 0; i < size; i++);
//			cout << last_cluster[i];
//	cout << sizeof(last_cluster) << endl;

	string arg("last");
	if( !this->cylinder[where].insert( last_cluster, to_put[j] - where * div, size, arg ) )	{cout << "Cylinder :: insert com problemas\n";}
	getchar();
	printf("\n");
}
/*
char* HardDrive ::  g_cluster_content(ui first){
	return this->g_cylinder( which_cylinder(first) ).g_cluster_content(first);
}
*/
// public

ui HardDrive :: insert_file(){
	this->full = this->fat.g_full();
	if(this->full == true)	{view :: lotado_HD(); return false;}
	view :: escreve_HD_virtual();

	char file[100];
	getchar();
	scanf("%[^\n]s", file);
	cout << "Nome do arquivo lido :: " << file << endl;

	ifstream arq (file, ifstream :: binary);
	ui length = 0;

	if(arq){
//		cout << "Yes, arquivo existe\n";
	    arq.seekg (0, arq.end);
		length = arq.tellg(); 
//		cout << length << endl;
		if(length == 0)
			cout << "Arquivo nao existe ou tem tamanho nulo.\n";
	}
	else{
		cout << "Arquivo inexistente\n";
		return 0;
	}


	int ret = insert_file(file, length);
//	cout << "Retornou da funcao insert_file(file)\n";
	if(ret == false)	cout << "Oops! Sem espaço de armazenamento disponivell\n";
	else if (ret == -1)	cout << "Oops! Arquivo de mesmo nome jah existente\n";
//	else	cout << "Insercao bem-sucedida !!!\n";
	return true;
}

bool HardDrive :: remove_file(){
 	string remove;
 	view :: apaga_HD_virtual();
 	getchar();
 	cin >> remove;
	return this->fat.free_space(remove);
}

void HardDrive :: show_file(){
	string file;
	view :: le_HD_virtual();
	getchar();
	cin >> file;
	auto lista = this->fat.g_clusters_list(file);
	if(lista.size() == 0){cout << "Arquivo NAO ENCONTRADO!!\n";	return;}
//	cout << "DEBUGGIN :: tamanho da lista == " << lista.size() << endl;
	string arq("");
//	cout << "Erro logico?\n";
	for(auto it = lista.begin(); it != lista.end(); it++){
		ui cluster = (*it);
		char * i = this->g_cylinder(which_cylinder(cluster)).g_track(which_track(cluster)).g_cluster(cluster - which_cylinder(cluster) * 150 - which_track(cluster) * 15).g_cluster_content();
		cout << i;
//		printf("\n");
//		cout << "Numero de vezes que mostrei dessa vez :: " <<
//		this->g_cylinder(which_cylinder(cluster)).g_track(which_track(cluster)).g_cluster(cluster - which_cylinder(cluster) * 150 - which_track(cluster) * 15).g_eof() << endl;
		free(i);
	}
	cout << endl;
}

void HardDrive :: show_FAT(){
	getchar();
	view :: mostra_FAT();
	this->fat.show();
	view :: continui();	
}
