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
}

ui FatTable :: insert(const char* name, const ui& size){
	
}

vector<ui> FatTable :: alloc_space(const string& file, ui size){
	cout << "Size recebido : "<< size << endl;
	ui qtt = (int) ceil((float)((int)size) / (int)Qtt::CLUSTER);		// Pega o nº do setor no qual estah localizado o ultimo bytee valido
	cout << "Em termos de clusters: " << qtt << endl;
	
	const ui ret = qtt;
	if(qtt > this->unused.size() )	{cout << "!!!!!!!!!! HD nao suporta mais tal arquivo !!!!!!!!!!" << endl;	return vector<ui>(0);}


	vector<ui> lista(0);
	ui tmp = *(this->unused.begin());
	if( tmp != 0){cout << "Erro na inicialização de 'used'\n";	throw "sdfsdfsd";}
	ui first = *(this->unused.begin());	// first === primeiro cluster ocupado

								// a posicao do começo do arquivo.
	this->fatlist.push_back(Fatlist(file, first));	// Insere o nome/primeiro cluster
	
	
	this->unused.erase(first);	// Nao eh mais nao usado
	this->used.insert(first);	// Agora eh usado
	if(qtt < 2){	// Arquivo vazio ou com tam < Qtt :: CLUSTER
		lista.push_back( first );	// Lista de posicoes do cluster do arquivo rece-
		if(unused.size() == 0)
			this->full = true;
		fatent[first] = FatEnt(true, true, -1);	// used = true, eof = true, next = -1 (valor qqer)
		cout << "^^^^^ Arquivo \"" << file << "\" ocupa apenas um cluster\n";;
		return lista;
	}


	ui next;		// Proximo do primeiro setor.
	qtt--;
	next  = *(this->unused.begin());
	fatent[first] = FatEnt(true, false, next);
	for(;qtt > 0; qtt--){	// Quando qtt == 1, eh o ultimo setor a ser inserido
		lista.push_back( first );

		first = next;
		this->unused.erase(first);
		this->used.insert(first);
		next  = *(this->unused.begin());
		fatent[first] = FatEnt(true, false, next);

	}

	// Inserir o ultimo setor; calcular também seu tamanho exato

	fatent[first] = FatEnt(true, true, -1);
	lista.push_back( first );
	this->unused.erase( first );
	this->used.insert( first );
	// Todo os cluster na tbela fat adequadamente enumerados
	for(auto it = lista.cbegin(); it != lista.cend(); it++);
//		cout << "Cluster ocupado : " << (*it) << endl;
	return lista;
}

bool FatTable :: free_space(const string& get_out){
	bool found = false;
	ui first;
	for(auto it = this->fatlist.begin(); it != this->fatlist.end() and !found; it++){
		if( (*it).g_file_name() == get_out ){
			found = true;
			(*it) = Fatlist();		// Reseta objeto; pra poder usar novamente
		}
	}
	if(found == true){
		vector <ui> lista = this->g_clusters_list(get_out);
		for(auto it = lista.begin(); it != lista.end(); it++){
			ui cluster = (*it);
			this->fatent[cluster] = FatEnt();	// Cluster livre para reuso
			this->used.erase( cluster );
			this->unused.insert( cluster );
		}
		this->full = false;
	}
	else{
		view :: arquivo_nao_encontrado();
		return false;
	}
	
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
			cout << "Encontrou o arquivo.\n";
			again = false;
		}
	}
	if(again == true){
		cout << "Arquivo nao encontrado\n";
		return vector<ui>(0);
	}
	vector<ui> lista(0);
//	cout << "Primeiro cluster do arquivo : " << first << endl;

	bool eof;
	do{
		eof = this->fatent[first].eof;
		lista.push_back(first);
		first = this->fatent[first].next;
	}while(eof == false);
	return lista;
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
	if(this->used == true){cout<<"Erro: sobreposicao de clusters\n";throw 1.1;}
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
	if(this->full == true){cout << "LLotadaoO!\n";	return false;}

	ui div = Qtt :: TRACK / Qtt :: CLUSTER;
	ui where = which_track(first);

//	cout << "Trilha de insercao :: " << where << endl;
	if(this->track[where].insert(cluster, first - where * div, next, op));
//	else	cout << ">>> Erro em Cylinder :: insert\n";
	
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
	if(this->full == true){cout << "HD lotado insert_file()\n";	return false;}

	vector<ui> to_put = this->fat.alloc_space(file, size);

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
		for(int i = 0; i < Qtt :: CLUSTER; i++)
			cout << cluster[i];

//		cout << "cluster de insercao == " << to_put[j] << endl;
		ui where = which_cylinder( to_put[j] );

//		cout << "cilindro de insercao :: " << where << endl;
		if( !this->cylinder[where].insert( cluster, to_put[j] - where * div, to_put[j+1] ))	{cout << "Cylinder :: insert com problemas\n";}
	}
	// Inserir caso especial; ou seja, o ULTIMO cluster, que pode ser setores disperdiçados :/	
//	cout << "ULTIMO cluster de insercao == " << to_put[j] << endl;
	ui where = which_cylinder( to_put[j] );
//	cout << "ULTIMO cilindro de insercao :: " << where << endl;
	
	char last_cluster[size];
	fread(last_cluster, size, 1, fp);
		for(int i = 0; i < size; i++)
			cout << last_cluster[i];
//	cout << sizeof(last_cluster) << endl;

	string arg("last");
	if( !this->cylinder[where].insert( last_cluster, to_put[j] - where * div, size, arg ) )	{cout << "Cylinder :: insert com problemas\n";}
	getchar();
	printf("\n\n\n\n\n\n\n\n\n\n\n");
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

	char file[100] = "in";
//	scanf("%[^\n]s", file);
//	cout << file << endl;

	ifstream arq (file, ifstream :: binary);
	ui length = 0;

	if(arq){
//		cout << "Yes, arquivo existe\n";
	    arq.seekg (0, arq.end);
		length = arq.tellg(); 
//		cout << length << endl;
	}
	else
		cout << "Arquivo inexistente\n";


	ui ret = insert_file(file, length);
//	cout << "Retornou da funcao insert_file(file)\n";
	if(ret == false)	cout << "Oops! Sem espaço de armazenamento disponivell\n";
//	else	cout << "Insercao bem-sucedida !!!\n";
	return true;
}

bool HardDrive :: remove_file(){
 	string remove;
 	view :: apaga_HD_virtual();
 	cin >> remove;
	return this->fat.free_space(remove);
}

void HardDrive :: show_file(const string& file){
	auto lista = this->fat.g_clusters_list(file);
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