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

// class Fatlist

Fatlist :: Fatlist(string const& file, ui const& first){

	this->s_name(file);
	this->first_sector = first;
}

// class FatTable

void FatTable :: insert(const string& name, const ui& size){
	
}

// class Cylinder

bool Cylinder :: insert(const char sec[512]){
	return true;	// TODO: implementar isso de verdade.
}

// class HardDrive

// private

// Obs: 'size' eh em termos de BYTES
// Retorna o ultimo cilindro no qual o arquivo foi inserido
ui HardDrive :: insert_file2( const char file[100], ui& size, ui cylinder = 0, ui offset = 0){
	cout << "clusters usados antes : " << this->used << endl;
	if((cylinder % CYLINDERS) == CYLINDERS - 1)	return CYLINDERS;	// Tentou insrecao em todos os cilindros e mesmo assim nao deu certo

	if(this->cylinder[cylinder%CYLINDERS].g_full() == true)
		return this->insert_file2(file,size, cylinder+1, offset);
	else{		
		cout << "cylinder == " << cylinder << endl;
		// TODO : implementar a insercao no cilindro enquant houver clusters disponiveis
		char sec[512];
		memset(sec, 512, '0');
		cout << file << endl;
		FILE *fp = fopen(file, "r");
		ui count = 0;				// Cada vez que atinge 4, um clusterr forpreenchido
		ui x = 0;
		if(size == 0)	count = 0;	//  Para nao conflitar com a linha 78

		bool full_cylinder = this->cylinder[cylinder%CYLINDERS].g_full();	// Enquanto cilindro tem espaco, insere nele.

		while((size  != 0) and (full_cylinder == false)){
			count++;
			x++;
			cout << x << endl;
//			cout << "(ui)ceil( ((float) int(size))/Qtt :: SECTOR) == " << (ui)ceil( ((float) int(size))/Qtt :: SECTOR) << endl;
			if((ui)ceil((float)((int)size)/Qtt :: SECTOR)  > 1 ){	//    Mais de um cluster para ler.
//				cout << "normal\n";
				fread(sec, Qtt :: SECTOR, 1, fp);
				fseek(fp, Qtt :: SECTOR, offset);
				offset += Qtt :: SECTOR;
				
				for(int p = 0; p < Qtt :: SECTOR; p++)
					printf("%c", sec[p]);
				printf("\nProxima leitura:\n");
				size -= Qtt :: SECTOR;
			}
			else{
				cout << "\nFinal\n";
				fread(sec, size, 1, fp);
				for(int k = 0; k < size; k++)
					cout << sec[k];
				cout <<  endl;
				offset += size ;
				size = 0;				
			}

			this->cylinder[cylinder].insert(sec);
			count %= 4;
			if(count == 0){
				this->used ++;	// Incrementa apenas quando LOTA um CLUSTER
				full_cylinder = this->cylinder[cylinder%CYLINDERS].g_full();
			}
			
			if(this->cylinder[cylinder].g_full() == true){	// Se lotou um cilindro faz as rotinas necessarias
				this->set_used.insert(cylinder);
				this->set_full();
			}
			memset(sec, 512, '0');
			

//			memset(sec, 512, '0');
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
		cout << "count antes do ajuste : " << count << endl;
		for(; count != 0; count%=4){	// Gravou menos de 4 setores contiguos; ou seja,
			memset(sec, 512, '0');				// nao gravou um cluster inteiro
			this->cylinder[cylinder].insert(sec);			
			if(count == 3){
				cout << "Opa! Cluster ocupado\n";
				this->used++;
			}
			count++;
		}

		//...
		cout << "count depois do ajuste : " << count << endl;
		cout << "clusters usados depois : " << this->used << endl;
		fclose(fp);
	}
	return cylinder;
}

// public

bool HardDrive :: set_full(){
	if(this->set_used.size() == CYLINDERS)
		this->full = true;	
	return this->full;
}

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
	cout << "RRetornou da funcao insret_file2\n";
	if( ret == CYLINDERS ){
		view :: lotado_HD();
		return false;
	}
	else
		view :: inseriu_cilindro(ret);
	return true;
}
