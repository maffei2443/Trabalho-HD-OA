#include "estruturas.hpp"
#include "view.hpp"
#include <iostream>
#include <string>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>	// for 'memset'
#include <math.h>	// for 'ceil'
using namespace std;


// class ClusterFake
// n : em qual setor termina o cluster
ClusterFake :: ClusterFake(const char cluster[Qtt :: CLUSTER], const ui& n, const ui& size){
	for(ui i = 0; i < Qtt :: CLUSTER; i++)
		this->cluster[i] = cluster[i];
	this->last = n;
	this->size = size;
}


// class Fatlist

Fatlist :: Fatlist(string const& file, ui const& first){

	this->s_name(file);
	this->first_sector = first;
}

// class FatTable

ui FatTable :: insert(const string& name, const ui& size){
	
}

// class Sector
// private

// public

bool Sector :: insert_last(const char cluster[Qtt :: CLUSTER], const ui& size){
	if(size == 3);	// TODO: impllementar insercao diferenciada; talvez uma struct
					// que contenha cluster e o indice da posicao final; de esse
					// indice > 1024, nao eh o ultimo setor do cluster.
	else{
		for(ui i = size * Qtt :: SECTOR; i < (size+1) * Qtt :: SECTOR; i++)
			this->byte_s[i % Qtt :: SECTOR] = cluster[i];
	}
	this->used = true;
	this->full = true;	// corrigir; devo fazer isso mesmo?///
}

bool Sector :: insert(const char cluster[Qtt :: CLUSTER], const ui& next){
	if(next == 3);	// TODO: impllementar insercao diferenciada; talvez uma struct
					// que contenha cluster e o indice da posicao final; de esse
					// indice > 1024, nao eh o ultimo setor do cluster.
	else{
		for(ui i = next * Qtt :: SECTOR; i < (next+1) * Qtt :: SECTOR; i++)
			this->byte_s[i % Qtt :: SECTOR] = cluster[i];
	}
	this->used = true;
	this->full = true;	// corrigir; devo fazer isso mesmo?///
}



// class Cluster

// URGENTE :: VERIFICAR AS DUSA FUNCOES ABAIXO!!
ui Cluster :: insert_last(const char cluster[Qtt :: CLUSTER], const ui& size){
	cout << "Inserrindo no cluster\n";
	this->used = true;
	this->next = -1;			// Como eh o ultimo setor, logo eh o ultimo cluster.

	for(ui i = 0; i < this->MAX; i++){
		for(ui j = 0; j < Qtt :: SECTOR; j++)
			this->sector[i].insert_last(cluster, i);
	}
	return true;
}

ui Cluster :: insert(const char cluster[Qtt :: CLUSTER], const ui& next){
	cout << "Inserrindo no cluster\n";
	this->used = true;
	for(ui i = 0; i < this->MAX; i++){
		for(ui j = 0; j < Qtt :: SECTOR; j++)
			this->sector[i].insert(cluster, i);
	}
	return true;
}


// class Track
// private

// public

bool Track :: s_full(){
	if(this->set_used.size() == this->CLUSTERS)
		this->full = true;
	else
		this->full = false;
	return this->full;
}

// Retorna o numero do novo cluster ocupado
ui Track :: insert_last(const char cluster[Qtt :: CLUSTER], const ui& size){
	if(this->full == true)	return false;

	for(ui x = 0; x < this->CLUSTERS; x++){
		if(this->cluster[x].g_used() == false){	// Encontrou cluster livre
			cout << "Inseriu no cluster " << x << endl;
			this->cluster[x].insert_last(cluster, size);
			this->set_used.insert(x);
			this->s_full();
			return x;
		}	
	}
	throw "Mal-definido as trilhas cheias :/\n";
}

ui Track :: insert(const char cluster[Qtt :: CLUSTER], const ui& next){
	if(this->full == true)	return false;

	for(ui x = 0; x < this->CLUSTERS; x++){
		if(this->cluster[x].g_used() == false){	// Encontrou cluster livre
			cout << "Inseriu no cluster " << x << endl;
			this->cluster[x].insert(cluster, next);
			this->set_used.insert(x);
			this->s_full();
			return x;
		}	
	}
	throw "Mal-definido as trilhas cheias :/\n";
}


// class Cylinder

// private

bool Cylinder :: s_full(){
	if(set_used.size() == this->MAX)
		this->full = true;
	else
		this->full = false;
	cout << "Cilndros usados :: " << set_used.size() << endl;
	return this->full;
}

// public
ui Cylinder :: insert_last(const char cluster[Qtt :: CLUSTER], const ui& size){	// TODO: implementar isso de verdade.
	if(this->full == true)	return false;
	for(ui x = 0; x <= this->MAX; x++){
		if( this->track[x].g_full() == false ){
			cout << "Inseriu na trilha " << x << endl;
			this->track[x].insert_last(cluster, size);
			if( this->track[x].g_full() == true ){	// Se lotar a trilha, fazer os procedimentos
				this->set_used.insert(x);
			}					
			x = this->MAX;
		}
	}
	this->used++;		// +1 um cluster utilizado
	this->s_full();
	return true;
}

ui Cylinder :: insert(const char cluster[Qtt :: CLUSTER], const ui& next){	// TODO: implementar isso de verdade.
	if(this->full == true)	return false;
	for(ui x = 0; x <= this->MAX; x++){
		if( this->track[x].g_full() == false ){
			cout << "Inseriu na trilha " << x << endl;
			this->track[x].insert(cluster, next);
			if( this->track[x].g_full() == true ){	// Se lotar a trilha, fazer os procedimentos
				this->set_used.insert(x);
			}					
			x = this->MAX;
		}
	}
	this->used++;		// +1 um cluster utilizado
	this->s_full();
	return true;
}

// class HardDrive

// private

// Obs: 'size' eh em termos de BYTES
// Retorna o ultimo cilindro no qual o arquivo foi inserido

ui HardDrive :: set_full(){
	if(this->set_used.size() == CYLINDERS)
		this->full = true;	
	else
		this->full = false;
	return this->full;
}


ui HardDrive :: insert_file2( const char file[100], ui& size, ui cylinder = 0, ui offset = 0){
	cout << "clusters usados antes : " << this->used << endl;
	if((cylinder % CYLINDERS) == CYLINDERS - 1)	return CYLINDERS;	// Tentou insrecao em todos os cilindros e mesmo assim nao deu certo

	if(this->cylinder[cylinder%CYLINDERS].g_full() == true){
		cout << "Lotou cilindro " << cylinder << endl;
		return this->insert_file2(file,size, cylinder+1, offset);
	}
	else{		
		cout << "cylinder == " << cylinder << endl;
		// TODO : implementar a insercao no cilindro enquant houver clusters disponiveis
		char sec[Qtt :: CLUSTER];
		memset(sec, Qtt :: CLUSTER, '0');
		cout << file << endl;
		FILE *fp = fopen(file, "r");
		ui x = 0;

		bool full_cylinder = this->cylinder[cylinder%CYLINDERS].g_full();	// Enquanto cilindro tem espaco, insere nele.

		while((size  != 0) and (full_cylinder == false)){
			cout << "Inseriu no cilindro " << cylinder << endl;			
			x++;
			cout << x << endl;
			if((ui)ceil((float)((int)size)/Qtt :: CLUSTER)  > 1 ){	//    Mais de um cluster para ler.
				fread(sec, Qtt :: CLUSTER, 1, fp);
				fseek(fp, Qtt :: CLUSTER, offset);
				offset += Qtt :: CLUSTER;
				
				for(int p = 0; p < Qtt :: CLUSTER; p++)
					printf("%c", sec[p]);
				printf("\nProxima leitura:\n");
				size -= Qtt :: CLUSTER;
				ui next = 1;
				this->cylinder[cylinder].insert(sec, next);	// TODO: 'next' deve ser informado pela tabela FAT
			}
			else{
				cout << "\nFinal\n";
				fread(sec, size, 1, fp);
				for(int k = 0; k < size; k++)
					cout << sec[k];
				cout << endl << size << endl;
				cout <<  endl;
				offset += size ;
				size = 0;
				cout << "Final mesmo :/\n";
				this->cylinder[cylinder].insert_last(sec, size);		
			}
			cout << "Inserindo no cilindro ...\n";
//			this->cylinder[cylinder].insert(sec);

			cout << "Cluster cheio" << endl;
			this->used ++;	// Incrementa apenas quando LOTA um CLUSTER
			full_cylinder = this->cylinder[cylinder%CYLINDERS].g_full();

			
			if(this->cylinder[cylinder].g_full() == true){	// Se lotou um cilindro faz as rotinas necessarias
				this->set_used.insert(cylinder);
				this->set_full();
			}

			memset(sec, Qtt :: CLUSTER, '0');
			
		}
		if(size != 0){	// Arquivo nao coube no cilindro; continuar a  insrecao no proximo
			cout << ">>>>>>>>>>>>>>>>>Passando para o proximo cilindro :: " << cylinder+1 << endl;
			fclose(fp);
			return this->insert_file2(file,size, cylinder+1, offset);
		}
		// Essa parte abaixo vai gerar ateh 3 setores vazios :/.
		// Motivo :: completar o cluster.
		// Pq abaixo do 'if' acima? Porque na maioria das vezes eh espe-
		// rado o 'if' ter sua condicao verdadeira

		//...
		cout << "clusters usados depois : " << this->used << endl;
		fclose(fp);
	}
	return cylinder;
}

// public

bool HardDrive :: insert_file(){

	if(this->full == true)	{view :: lotado_HD(); return false;}
	view :: escreve_HD_virtual();

	char file[100];
	scanf("%[^\n]s", file);
	cout << file << endl;

	ifstream arq (file, ifstream :: binary);
	ui length = 0;

	if(arq){
		cout << "Yes, arquivo existe\n";
	    arq.seekg (0, arq.end);
		length = arq.tellg(); 
		cout << length << endl;
	}
	else
		cout << "Arquivo inexistente\n";



	ui ret = insert_file2(file, length,0, 0);
	cout << "Retornou da funcao insert_file2\n";
	if( ret == CYLINDERS ){
		view :: lotado_HD();
		return false;
	}
	else
		view :: inseriu_cilindro(ret);
	return true;
}
