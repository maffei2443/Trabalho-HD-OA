#include "estruturas.hpp"
#include "view.hpp"
#include <iostream>
#include <string>
using namespace std;

// class Fatlist

Fatlist :: Fatlist(string const& file, ui const& first){

	this->s_name(file);
	this->first_sector = first;
}

// class FatTable

void FatTable :: insert(const string& name, const ui& size){
	
}

// class HardDrive

// private

ui HardDrive :: insert_file2(const ui& cylinder, const string& file, ui at = 0){

	if((at % TRACK_SURFACE) == TRACK_SURFACE - 1)	return TRACK_SURFACE;	// Tentou insrecao em todos os cilindros e mesmo assim nao deu certo

	if(this->cylinder[cylinder%TRACK_SURFACE].g_full() == true)
		return this->insert_file2(cylinder, file,at+1);
	else{		
		// TODO : implementar a insercao no cilindro
		//...
		return at;
	}

}

// public

bool HardDrive :: insert_file(){
	view :: escreve_HD_virtual();
	string file;
	getline(cin, file);;
	cout << file << endl;
	bool again = true;
	ui ret = insert_file2(0, file);
	if( ret == TRACK_SURFACE ){
		cout << "HD lotado\n";
		return false;
	}
	else
		cout << "Inserido no cilindro " << ret << endl;
	return true;
}