#ifndef _ESTRUTURAS_HPP_
#define _ESTRUTURAS_HPP_

#include <vector>
#include <string>
using namespace std;
#define ui unsigned int

// Unidades basicas de armazenamento (num HD orientado a setores)
class Sector {
private:
	bool full;
	const static ui MAX = 512;		/// 512 bytes por setor
public:
	Sector(): full(false){}
	unsigned char byte_s[MAX];	//[512];
	inline unsigned char * g_byte_s(){return byte_s;}
	
};

// 1 cluster contem 4 setores.
class Cluster{
private:
	bool full;
	const static ui MAX = 4;
	ui qtd;
	vector<Sector> sector;		
public:
	Cluster() : qtd(0), full(false){sector.resize(MAX);}

	inline Sector g_sector(const ui& i)const{return sector[i%MAX];}
	inline vector<Sector> g_sectors()const{return sector;}

	void s_sectors(vector<Sector> sec){
		ui i=0;
		for(vector<Sector>::iterator it = sec.begin(); it != sec.end() && i < 4; it++, i++)
			sector[i] = sec[i];		
	}
	void insert_sec(const Sector& sec){
		if(qtd < MAX){
			sector[qtd] = sec;
			qtd++;
		}
	}

	void insert_cluster(const Cluster& clus){
		sector = clus.g_sectors();
	}
};

class Track{
private:
public:
	const static ui MAX_SECTOR = 60;	// 60 setores por trilha; nesse trabalho, 4 clusters poranto por trilha
	const ui SIZE_CLUSTER;				// In terms of sectors
	vector<Sector> sector;

	Track(): SIZE_CLUSTER(15){sector.resize(MAX_SECTOR);}
	Track(const ui& tam_sector): SIZE_CLUSTER(tam_sector%MAX_SECTOR){sector.resize(MAX_SECTOR);}

	inline Sector g_sector(const ui& i)const{return sector[i%MAX_SECTOR];}
	inline vector<Sector> g_sectors()const{return sector;}

	void s_sector(const ui& i, const Sector& neo){sector[i%MAX_SECTOR] = neo;}

};

class Cylinder{
private:
	const static ui MAX = 5;	// 5 trilhas por cilindro
	bool full;					// @ se false, ainda tem espaco no cilindro
public:
	vector<Track> track;

	Cylinder():full(false){track.resize(MAX);}

	inline Track g_track(const ui& i)const{return track[i%MAX];}
	inline vector<Track> g_tracks()const{return track;}
	inline bool g_full(){return full;}
};


// Final das Estruturas basicas
class Fatlist{
private:
	const static ui MAX_NAME = 100;		// 
	string name;
	ui first_sector;
public:
	inline string g_file_name(){return name;}
	inline ui g_first_sector(){return first_sector;}
	
	inline void s_name(const string& neo){if(neo.size() < MAX_NAME)	name = neo;}		// TODO: adicionar aviso de que nao foi possivela insercao E a causa.
	Fatlist(){name.reserve(100);}
	Fatlist(string const& file, ui const& first);
};

class FatEnt{		// Entrada para cada setor do HD
public:
	bool used;
	bool eof;
	ui next;		// Soh tem de ser valido se eof == false
	///////////////////
	FatEnt(): used(false), eof(true), next(-1){}
	FatEnt(bool usedd, bool eoff, ui nextt): used(usedd), eof(eoff), next(nextt){}
	~FatEnt();
};


class FatTable{
public:
	vector <Fatlist> fatlist;
	vector <FatEnt> fatent;
	FatTable() : fatlist(0), fatent(0){}
	void insert(const string& name, const ui& size);
};

class HardDrive{							// Convencao: primeiro cilindro a ser preenchido eh o mais externo
	static const ui TRACK_SURFACE = 10;	// 10 trilhas por superficie
 	bool full;
 	vector <Cylinder> cylinder;
 	ui insert_file(const ui& cylinder, const string& file);
private:
	ui insert_file2(const ui& cylinder, const string&, ui);
public:
	static const int g_track_surface(){return TRACK_SURFACE;}
//	Pratos pratos[10];
	HardDrive(): full(false){cylinder.resize(TRACK_SURFACE);}
	inline Cylinder g_cylinder(const ui& i){return cylinder[i%TRACK_SURFACE];}
	inline vector<Cylinder> g_cylinders(){return cylinder;}

	bool insert_file();
};

class Time{	// Modulo passivo; apenas disponibiliza tempos (obs: tempos em 'ms')
private:
	Time(){}
public:
	static const int SEEK_MEAN = 4;
	static const int SEEK_MIN = 1;
	static const int LATENCY_MEAN = 6;
	static const int TRANSFER_TRACK = 12;
};


#endif