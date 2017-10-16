#ifndef _ESTRUTURAS_HPP_
#define _ESTRUTURAS_HPP_

#include <vector>
#include <string>
using namespace std;
#define ui unsigned int

// Unidades basicas de armazenamento (num HD orientado a setores)
class Sector {
private:
	const static ui MAX = 512;
public:
	unsigned char byte_s[MAX];	//[512];
	inline unsigned char * g_byte_s(){return byte_s;}
	
};

class Cluster{
private:
	const static ui MAX = 4;
	int qtd;
	vector<Sector> sector;		// Contem 4 setores.
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
	const static int MAX = 60;
public:
	vector<Sector> sector;

	Track(){sector.resize(MAX);}

	inline Sector g_sector(const ui& i)const{return sector[i%MAX];}
	inline vector<Sector> g_sectors()const{return sector;}

	void s_sector(const ui& i, const Sector& neo){sector[i%MAX] = neo;}

};

class Cylinder{
private:
	const static int MAX = 60;
public:
	vector<Track> track;

	Cylinder(){track.resize(MAX);}

	inline Track g_track(const ui& i)const{return track[i%MAX];}
	inline vector<Track> g_tracks()const{return track;}
};

// Final das Estruturas basicas
class Fatlist{
private:
	const static int MAX = 100;
	string file_name;
	ui first_sector;
public:
	inline string g_file_name(){return file_name;}
	inline ui g_first_sector(){return first_sector;}
	
	inline void s_file_name(const string& neo){if(neo.size() < MAX)	file_name = neo;}
	inline void s_first_sector(const ui& first){first_sector = first;}
};

class Fatent{
	ui used;
	ui eof;
	ui next;
public:
	inline ui g_used(){return used;}
	inline ui g_eof(){return eof;}
	inline ui g_next(){return next;}

	inline void s_used(const ui& neo){used = neo;}
	inline void s_eof(const ui& neo){eof = neo;}
	inline void s_next(const ui& neo){next = neo;}
};


class Fat_table{
public:
	vector <Fatlist> name_file;
	vector <Fatent> control;
};

class HardDrive{
	static const int TRACK_SURFACE = 10;
	const ui MAX_CYLINDER;
	vector <Cylinder> cylinder;
public:
//	Pratos pratos[10];
	HardDrive(const ui& number): MAX_CYLINDER(number){cylinder.resize(number);}
	inline Cylinder g_cylinder(const ui& i){return cylinder[i%MAX_CYLINDER];}
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