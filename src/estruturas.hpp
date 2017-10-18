#ifndef _ESTRUTURAS_HPP_
#define _ESTRUTURAS_HPP_

#include <vector>
#include <string>
using namespace std;
#define ui unsigned int

// Unidades basicas de armazenamento (num HD orientado a setores)
class Sector {
private:
	const static ui MAX = 512;		/// 512 bytes por setor
public:
	unsigned char byte_s[MAX];	//[512];
	inline unsigned char * g_byte_s(){return byte_s;}
	
};

class Cluster{
private:
	const static ui MAX = 4;
	int qtd;
	vector<Sector> sector;		// 1 cluster contem 4 setores.
public:
	Cluster(){qtd = 0;	sector.resize(MAX);}

	inline Sector g_sector(const ui& i)const{return sector[i%MAX];}
	inline vector<Sector> g_sectors()const{return sector;}
	void s_sectors(vector<Sector> sec){
		int i=0;
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
	const static int MAX = 60;	// 60 setores por trilha
public:
	vector<Sector> sector;

	Track(){sector.resize(MAX);}

	inline Sector g_sector(const ui& i)const{return sector[i%MAX];}
	inline vector<Sector> g_sectors()const{return sector;}

	void s_sector(const ui& i, const Sector& neo){sector[i%MAX] = neo;}

};

class Cylinder{
private:
	const static int MAX = 5;	// 5 trilhas por cilindro
public:
	vector<Track> track;

	Cylinder(){track.resize(MAX);}

	inline Track g_track(const ui& i)const{return track[i%MAX];}
	inline vector<Track> g_tracks()const{return track;}
};


// Final das Estruturas basicas
class Fatlist{
private:
	const static int MAX_NAME = 100;		// 
	string name;
	ui first_sector;
public:
	inline string g_file_name(){return name;}
	inline ui g_first_sector(){return first_sector;}
	
	inline void s_name(const string& neo){if(neo.size() < MAX_NAME)	name = neo;}
	inline void s_first_sector(const ui& first){first_sector = first;}
	Fatlist(){}
	Fatlist(string const& file, ui const& first);
};

class FatEnt{		// Entrada para cada setor do HD
public:
	bool used;
	bool eof;
	ui next;		// Soh tem de ser valido se eof == false
	///////////////////
	FatEnt();
	FatEnt(bool usedd, bool eoff, ui nextt): used(usedd), eof(eoff), next(nextt){}
	~FatEnt();
};


class FatTable{
public:
	vector <Fatlist> fatlist;
	vector <FatEnt> fatent;
	FatTable() : fatlist(0), fatent(0){}
	void insert(string name, ui size);
};

class HardDrive{							// Convencao: primeiro cilindro a ser preenchido eh o mais externo
	static const int TRACK_SURFACE = 10;	// 10 trilhas por superficie
 	
 	vector <Cylinder> cylinder;
public:
//	Pratos pratos[10];
	HardDrive(){cylinder.resize(TRACK_SURFACE);}
	inline Cylinder g_cylinder(const ui& i){return cylinder[i%TRACK_SURFACE];}
	inline vector<Cylinder> g_cylinders(){return cylinder;}

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