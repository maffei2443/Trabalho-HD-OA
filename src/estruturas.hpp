#ifndef _ESTRUTURAS_HPP_
#define _ESTRUTURAS_HPP_

#include <vector>
#include <string>
#include <set>

using namespace std;
#ifndef ui
	#define ui 	unsigned int
#endif
class FatEnt;
class FatList;
class FatTable;
class Qtt;
class Sector;
class Cluster;
class Track;
class Cylinder;
class HardDrive;
class Time;
class ClusterFake;

// Specify size of Objects those store data in erms os BYTES.
class Qtt{
private:
	Qtt(){}
public:	
	constexpr static ui SECTOR = 512;
	constexpr static ui CLUSTER = 4 * SECTOR;
	constexpr static ui TRACK = 15 * CLUSTER;
	constexpr static ui CYLINDER = 5 * TRACK;
};

// Classe que vai ajudar a gravar o ultimo setor no disco.

class Fatlist{
private:
	const static ui MAX_NAME = 100;		// 
	string name;
	ui first_sector;
public:
	inline string g_file_name(){return name;}
	inline ui g_first_sector(){return first_sector;}
	
	inline void s_name(const string& neo){if(neo.size() < MAX_NAME)	name = neo;}		// TODO: adicionar aviso de que nao foi possivela insercao E a causa.
	Fatlist(){}
	Fatlist(string const& file, ui const& first);
	~Fatlist(){}
};

class FatEnt{		// Entrada para cada setor do HD
public:
	bool used;
	bool eof;
	ui next;		// Soh tem de ser valido se eof == false
	///////////////////
	FatEnt(): used(false), eof(true), next(-1){}
	FatEnt(bool usedd, bool eoff, ui nextt): used(usedd), eof(eoff), next(nextt){}
	~FatEnt(){};
};


class FatTable{
private:
public:
	vector <Fatlist> fatlist;
	vector <FatEnt> fatent;
	FatTable() : fatlist(0), fatent(0){}
	~FatTable(){fatlist.resize(0); fatent.resize(0);}
	ui insert(const string& name, const ui& size);
};

// Unidades basicas de armazenamento (num HD orientado a setores)
class Sector {
private:
	int last_valid;
	bool eof;
	ui used;						// Obs: sera usado como bool, mas poedria indicar o primeiro espaco vago no setor.
	int next;
	const static ui MAX = 512;		/// 512 bytes por setor
	unsigned char byte_s[MAX];	//[512];
public:
	Sector(): eof(false),next(-1), used(false){}
	const inline unsigned char * g_byte_s(){return byte_s;}
	bool insert_last(const char*,  const ui&, int);	//  cluster,  posicao do setor, last valid position
	bool insert(const char*, const ui&, const ui&);	//  cluster, next cluster, nº do setor
};

// 1 cluster contem 4 setores.
class Cluster{
private:
	ui eof;		// Offset que indica o final do arquivo dentrodo cluster
	bool used;							// usar tbm como bool !
	int next;
	const static ui MAX = 4;
	vector<Sector> sector;		

public:
	Cluster() : used(0){sector.resize(this->MAX);}

	inline Sector g_sector(const ui& i)const{return sector[i%MAX];}
	inline vector<Sector> g_sectors()const{return sector;}
	inline bool g_used(){return used;}

	ui insert_last(const char*, const ui&);
	ui insert(const char*, const ui&);
};

class Track{
private:
	bool full;
	ui used;
	set<ui> set_used;
	const static ui MAX_SECTOR = 60;	// 60 setores por trilha
	const static ui MAX_CLUSTERS = 4;	// 15 clusters por trilha
	const static ui CLUSTERS = MAX_SECTOR / MAX_CLUSTERS;
	vector<Cluster> cluster;
public:

	Track(): full(false), used(0) {cluster.resize(CLUSTERS);}

	inline Cluster g_cluster(const ui& i)const{return cluster[i%this->CLUSTERS];}
	inline vector<Cluster> g_clusters()const{return cluster;}
	inline bool g_full(){return full;}
	bool s_full();
//	void s_cluster(const ui& i, const Sector& neo){cluster[i%MAX_SECTOR] = neo;}
	ui insert_last(const char*, const ui&);
	ui insert(const char*, const ui&);
	constexpr static ui g_CLUSTERS(){return CLUSTERS;}
};

class Cylinder{
private:
	bool full;					// @ se false, ainda tem espaco no cilindro
	ui used;
	set<ui> set_used;
	const static ui MAX = 5;	// 5 trilhas por cilindro
	const static ui MAX_CLUSTERS = MAX * Track :: g_CLUSTERS();	// 5 trilhas por cilindro
	vector<Track> track;
	bool s_full();
public:

	Cylinder():full(false), used(0){track.resize(MAX);}

	inline Track g_track(const ui& i)const{return track[i%MAX];}
	inline vector<Track> g_tracks()const{return track;}
	inline bool g_full(){return full;}
	
	ui insert_last(const char*, const ui&);	// Setor recebi que deve ser inserido na primeira trilha
	ui insert(const char*, const ui&);	// Setor recebi que deve ser inserido na primeira trilha
	constexpr static ui g_CLUSTERS(){return MAX_CLUSTERS;}
};

class HardDrive{							// Convencao: primeiro cilindro a ser preenchido eh o mais externo
private:
 	bool full;
 	ui used;						// Clusters usados.
	static const ui CYLINDERS = 10;	// 10 trilhas por superficie
	
	FatTable fat;

 	const static ui MAX_CLUSTERS = CYLINDERS * Cylinder :: g_CLUSTERS();

 	set<ui> set_used;

 	vector <Cylinder> cylinder;

	ui insert_file2( const char file[100], ui& size, const ui, const ui);	// Primeiro 'ui' eh o cilindro de insercao
																				// Segundo 'ui'	 eh o offset ja percorrido do arquivo.
public:
	
	static const ui g_n_cylinders(){return CYLINDERS;}

	HardDrive(): full(false), used(0){cylinder.resize(CYLINDERS);}
	~HardDrive(){cylinder.resize(0);}
	inline const Cylinder g_cylinder(const ui& i){return cylinder[i%CYLINDERS];}
	ui set_full();
//	inline vector<Cylinder> g_cylinders(){return cylinder;}

	bool insert_file();
	constexpr static ui g_CLUSTERS(){return MAX_CLUSTERS;}

};









// Final das Estruturas basicas



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