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

double read_time(vector<ui>);
double write_time(vector<ui>);

inline ui which_cylinder(ui cluster){
//	printf("ui which_cylinder(%u) == %u\n", cluster, (2 * cluster) / ((Qtt :: HARDDRIVE / Qtt :: CYLINDER) * (Qtt :: TRACK / Qtt :: CLUSTER)));
	return (2 * cluster) / ((Qtt :: HARDDRIVE / Qtt :: CYLINDER) * (Qtt :: TRACK / Qtt :: CLUSTER));
}

ui which_track(ui cluster){
	ui aux = 0;
	ui div = 75;
	if(cluster >= div){
		aux = which_cylinder(cluster);
		printf("[which_track :: cylinder] aux == %u\n", aux);
	}
	cluster -= aux * div;
	return cluster / (Qtt :: TRACK / Qtt :: CLUSTER);	//  15
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
	memset(clusters, 750 * sizeof(bool), false);	// CAN'T BE '0'
	usado = 0;
	if (clusters[0] == true || clusters[749] == true)
	{
		throw 99;
	}
}

ui FatTable :: insert(const char* name, const ui& size){
	
}


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

	ui i, j, k;
	k = cylinder;
	i = cluster;
	int debug = 0;		// throw se dar mais de 749 iteracoes :/
	for(; alloc == false;){		
		printf("i == %u\n", i);
		bool it = true;
		for(j = track + 1; alloc == false; j++){
			j %= 5;
			if(j == 0 and it == false){				// Passou pela ultima trilha; passar ao proximo cluster 
									// da primeira trilha do mesmo cilindro.
					break;
			}
			else if(this->clusters[i + 15 * j + 75 * k] == false){
				alloc = true;
				onde = (i + 15 * j + 75 * k);
				this->clusters[i + 15 * j + 75 * k] = true;
			}
			it = false;
		}
		track = -1;	// Para sempre começar pela trilha mais em cima, com exceçao claro da primeira na qual
					// tenta-se inserir na TRILHA SEGUINTE aa passada por parametro.
					// Passar para o proximo cluster, pois nao inseriu em nenhuma das trilhas abaixo.
		i++;		// Iterar AQUI!! Senao, zoa a questao da comparacao com 'mod'.
		i %= 15;	
		printf("Nao incrementa pq?? %u\n", i);
		if(i == mod){		// FINALMENTE deu a primeira volta.
			printf("primeira volta\n");
			mod = -1;		// Para nao mais entrar aqui.
			first = false;	// Apenas para nao dar voltas a toa :)
			i = 0;			// Zera o cluster
		}
		else if(i == 0 and !first ){	// Nao eh mais a primeira volta. Checar se o cluster zerou; se sim, trocar de cilindro.
				k++;		
				k %= 10;	// Para nao estourar o cilindro.
		}
		
		debug++;
		if(debug > 750)	{cout << "Erro em FatTable :: alloc_nxt" << debug << "\n";	throw 99;}
	}
	return onde;
}

vector<ui> FatTable :: alloc_space(const string& file, ui size){
	ui qtt = (int) ceil((float)((int)size) / (int)Qtt::CLUSTER);		// Pega o nº do cluster necessarios
	
	// Checa para ver se ainda cabe nos clusters restantes.
	if((this->usado + qtt) > 750)	{cout << "!!!!!!!!!! HD nao suporta mais tal arquivo !!!!!!!!!!" << endl;	return vector<ui>(0);}
	this->usado += qtt;
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
	ui next;

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
		goto end;
	}

	qtt--;

	ui where_cylinder, where_treck, where_cluster;
	where_cylinder = which_cylinder(first);
	where_treck = which_track(first);
	where_cluster = first - where_cylinder * 75 - where_treck * 15;
	// Parametros sao as posicoes do cluster predecessor.
	next = alloc_nxt(where_cluster, where_treck, where_cylinder);	// Proximo cilindro alocado para insercao.
	// assert(qtt >= 2)
	while( qtt > 0 ){	// Inserir o resto do arquivo.
		lista.push_back( first );
		this->clusters[first] = true;
		this->fatent[first] = FatEnt(true, false, next);

		first = next;
		where_cylinder = which_cylinder(first);
		where_treck = which_track(first);
		where_cluster = first - where_cylinder * 75 - where_treck * 15;
		next = alloc_nxt(where_cluster, where_treck, where_cylinder);

		qtt--;
	}
	where_cylinder = which_cylinder(next);
	where_treck = which_track(next);
	where_cluster = next - where_cylinder * 75 - where_treck * 15;
	this->clusters[next] = true;
	this->fatent[next] = FatEnt(true, false, first);

	// Inserir o ultimo setor; calcular também seu tamanho exato
//*
	this->clusters[first] = true;
	this->fatent[first] = FatEnt(true, true, -1);	// used = true, eof = false, next = -1
	lista.push_back( first );
	this->unused.erase( first );
	this->used.insert( first );
	// Todo os clusters na tsbela fat adequadamente enumerados
end:
	double tempoGravacao = write_time( lista );
	cout << "Tempo de gravacao == " << tempoGravacao << "ms" << endl;
	cout << "Tecle 'ENTER' para continuar...\n" ;
	getchar();
	return lista;
}

bool FatTable :: free_space(const string& get_out){
	bool found = false;
	ui first;
	ui index = 0;
	vector <ui> lista;
	for(auto it = this->fatlist.begin(); it != this->fatlist.end() and !found; it++){
		if( (*it).g_file_name() == get_out ){
			lista = g_clusters_list(get_out);
			found = true;
			(*it) = Fatlist();		// Reseta objeto; pra poder usar novamente
//			getchar();
		}
		else
			index++;
	}
	if(found == true){

		this->fatlist[index] = Fatlist();		// Para representar
		this->fatlist_reuse.insert(index);
		this->usado = this->usado - lista.size();
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
	return true;
}

vector<ui> FatTable :: clusters_file(const string& file){
	bool again = true;
	vector<ui> ordem(0);
	for(auto x = fatlist.begin(); x != fatlist.end()
		 and (again == true); x++){
		Fatlist aux2 = (*x);
		string aux = aux2.g_file_name();
		if( aux == file ){	//    Encontrou o arquivo
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
	int first;
	bool again = true;
	first = -1;
	for(auto it = this->fatlist.begin(); it != this->fatlist.end() and again; it++){
		if( (*it).g_file_name() == file){
			first = (*it).g_first_cluster();
			again = false;
		}
	}
	if(again == true){
		cout << "[FatTable ::  g_clusters_list] Arquivo nao encontrado\n";
		return vector<ui>(0);
	}
	if(first < 0){
		cout << "Primeiro cluster negativo. What?!?!\n";
		throw 120;
	}
	vector<ui> lista(0);
	if(this->fatent[first].eof == true);	// Caso especial de arquivo com apenas um cluster		
	else{
		bool eof;
		do{
			lista.push_back(first);
			first = this->fatent[first].next;
			eof = this->fatent[first].eof;
		}while(eof == false);
	}
	lista.push_back( first );
	return lista;
}

double read_time(vector<ui> lista){
	double tempoLeitura = 0;
	tempoLeitura += Time :: SEEK_MEAN;
	auto it = lista.cbegin();
	ui prev = (*it);
	it++;
	// Calcula o tempo de seeks A MENOS do primeiro seek, pois este eh DEFAULT.
	for(; it != lista.end(); it++){
		if(prev%75 != (*it)%75){	// Cilindros diferentes
			if(prev/75 < (*it)/75){	// Andou para cilindro de dentro
				if( ((*it)/75) - (prev/75) == 1 )	// Apenas um cilindro de diferenca.
					tempoLeitura = tempoLeitura + Time :: SEEK_MIN;	
				else	// Mais de um cilindro de distancia.
					tempoLeitura = tempoLeitura + Time :: SEEK_MEAN;
			}
			else if (prev/75 > (*it)/75)	// Volta do cilindro do centro para o da extremidade.
				tempoLeitura = tempoLeitura + Time :: SEEK_MEAN;
		}
		prev = (*it);
	}
	tempoLeitura = tempoLeitura + lista.size() * Time :: TRANSFER_CLUSTER;		// Um tempo de transferencia para cada cluster
	tempoLeitura = tempoLeitura + ceil(lista.size()/75.0) * Time :: LATENCY_MEAN;	// Um tempo de latencia para cada cilindro, pois teve se rodar uma trilha no pior caso
	return tempoLeitura;
}

double write_time(vector<ui> lista){
	return read_time(lista);
}

void FatTable :: show(){
	view :: mostra_FAT();
	int count = 0;
	for(auto it = this->fatlist.begin(); it != this->fatlist.end() ;it++){
		string name = (it)->g_file_name();
		if(name.size() == 0)	continue;
		count++;
		auto lista = g_clusters_list(name);
		double tempoLeitura = read_time( lista );
		cout << "\nt_leitura == " << tempoLeitura  << "ms" << endl;
		cout << "n_clusters :: " << lista.size() << endl;
		long unsigned int aux = name.size();
		for(int i = 0; i < (20 - aux); i++ )
			name += " ";
		printf("%s\t%lu Bytes\t\t", name.c_str(),  lista.size() * Qtt :: CLUSTER);
		int mod = 0;
		ui vet[20];											// Para depois imprimir os setores :/
		ui clust = 0;
		for(auto it2 = lista.cbegin(); it2 != lista.cend(); ){
			mod ++;
			mod %= 5;
			for(int i = 0;i < 4; i++)		// Calcula os setores a serem mostrados.
				vet[clust++] = ((*it2) + i);
			
			printf("%u", (*it2));
			it2++;
			if(it2 == lista.cend() || mod == 0){
				printf("\t\t");
				for(int i = 0; i < clust; i++)
					if(i+1 != clust)
						printf("%u, ", vet[i] );
					else
						printf("%u\n\t\t\t\t\t\t", vet[i]);
				clust = 0;
			}
			else
				printf(", ");
		}
	}
	printf("\n");
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
		this->next = -1;
		this->eof = next;	// polimorfismo a nivel de parametro :o
		ui i;
//*/
		for(i = 0; i < (eof / Qtt :: SECTOR) ; i++)			// Inserir em cada um dos setores
			this->sector[i].insert(cluster, next, i);
		this->sector[i].insert(cluster, eof - i * Qtt :: SECTOR, i, op);
		return true;
	}
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
	}
	
	return this->full;
}

// public

ui Cylinder :: insert(const char* cluster, ui first, const ui& next, string op = string("")){	// TODO: implementar isso de verdade.

	ui div = Qtt :: TRACK / Qtt :: CLUSTER;
	ui where = which_track(first);

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
//	for(ui i =0; i < to_put.size(); i++ );
//		cout << "Alocou cluster " << to_put[i] << endl;
	
	// ui tracks = (this->CYLINDERS * Qtt :: TRACK/Qtt :: CLUSTER);

	bool again = true;
	ui ultimo = to_put.size() - 1;
	ui j = 0;
	char cluster[Qtt :: CLUSTER];
	FILE *fp = fopen(file.c_str(), "r");
	ui offset = 0;					// Começa lendo a partir do inicio do arquivo.
	ui div = 75;
	for(; j < ultimo ; j++){	
		fread(cluster, Qtt :: CLUSTER, 1, fp);
		fseek(fp, Qtt :: CLUSTER, offset);
		offset += Qtt :: CLUSTER;
		size -= Qtt :: CLUSTER;
		ui where = which_cylinder( to_put[j] );
		if( !this->cylinder[where].insert( cluster, to_put[j] - where * div, to_put[j+1] ))	{cout << "Cylinder :: insert com problemas\n";	fclose(fp);throw 100;}
	}
	// Inserir caso especial; ou seja, o ULTIMO cluster, que pode ser setores disperdiçados :/	
	ui where = which_cylinder( to_put[j] );
	
	char last_cluster[size];
	fread(last_cluster, size, 1, fp);

	string arg("last");
	if( !this->cylinder[where].insert( last_cluster, to_put[j] - where * div, size, arg ) )	{cout << "Cylinder :: insert com problemas\n";}
	printf("\n");
	fclose(fp);
}

// public

ui HardDrive :: insert_file(){
	this->full = this->fat.g_full();
	if(this->full == true)	{view :: lotado_HD(); return false;}
	view :: escreve_HD_virtual();

	char file[100];
	getchar();
	scanf("%[^\n]s", file);

	ifstream arq (file, ifstream :: binary);
	ui length = 0;

	if(arq){
	    arq.seekg (0, arq.end);
		length = arq.tellg(); 
		if(length == 0)
			cout << "Arquivo nao existe ou tem tamanho nulo.\n";
	}
	else{
		cout << "Arquivo inexistente\n";
		return 0;
	}


	int ret = insert_file(file, length);
	if(ret == false)	cout << "Oops! Sem espaço de armazenamento disponivell\n";
	else if (ret == -1)	cout << "Oops! Arquivo de mesmo nome jah existente\n";
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
	string arq("");
	FILE * fp = fopen((file + "_cpy") .c_str(), "w+");
	int pp = 0;
	for(auto it = lista.begin(); it != lista.end(); it++){
		ui cluster = (*it);
		Cluster cl = this->g_cylinder(which_cylinder(cluster)).g_track(which_track(cluster)).g_cluster(cluster - which_cylinder(cluster) * 75 - which_track(cluster) * 15);
		char * i =  cl.g_cluster_content();
		ui eof = cl.g_eof();
		for(int l = 0; l < eof; l++){			
			fprintf(fp, "%c", i[l]);
		}	
		free(i);
	}
	fclose(fp);
	cout << endl;
}

void HardDrive :: show_FAT(){
	getchar();	
	this->fat.show();
	view :: continui();	
}
