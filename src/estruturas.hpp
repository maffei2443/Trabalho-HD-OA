#ifndef _ESTRUTURAS_HPP_
#define _ESTRUTURAS_HPP_

#include <vector>
#include <string>
#include <set>
#include <iostream>
///#include <iostream>
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

/// Specify size of Objects those store data in erms os BYTES.
ui which_cylinder(ui cluster);
ui which_track(ui cluster);
ui which_cluster(ui cluster);

class Qtt{
private:
	Qtt(){}
public:	
	constexpr static ui SECTOR = 512;
	constexpr static ui CLUSTER = 4 * SECTOR;
	constexpr static ui TRACK = 15 * CLUSTER;
	constexpr static ui CYLINDER = 5 * TRACK;
	constexpr static ui HARDDRIVE = 10 * CYLINDER;
};

/// Classe que vai ajudar a gravar o ultimo setor no disco.

class Fatlist{
private:
	bool full;
	const static ui MAX_NAME = 100;		/// 
	string name;
	ui first_sector;
public:
	string g_file_name(){return name;}
	ui g_first_cluster(){return first_sector;}
	
	inline void s_name(const string& neo){if(neo.size() < MAX_NAME)	name = neo;}		/// TODO: adicionar aviso de que nao foi possivela insercao E a causa.
	Fatlist(): full(false){}
	Fatlist(string const& file, ui const& first);
	~Fatlist(){}
};

class FatEnt{		/// Entrada para cada next do HD
public:
	bool used;
	bool eof;
	ui next;		/// Soh tem de ser valido se eof == false
	////////////////////////////
	FatEnt(): used(false), eof(true), next(-1){}
	FatEnt(bool usedd, bool eoff, ui nextt): used(usedd), eof(eoff), next(nextt){}
	~FatEnt(){};
};


class FatTable{
///	vector<bool>usad;	/// Resulta em segfault :( . 
private:
	bool full;
	ui usado;
	ui alloc_first();
	ui alloc_nxt(ui, ui, ui);
	set <ui> fatlist_reuse;
	set<ui> used;
	set<ui> unused;
	bool clusters[750];
public:
	vector <Fatlist> fatlist;
	FatEnt fatent[750];
	FatTable();
	~FatTable(){fatlist.resize(0);}/// fatent.resize(0);}
	ui insert(const char* name, const ui& size);
	inline bool g_full(){return full;}
	vector<ui> alloc_space(const string&, ui);
	bool free_space(const string&);
	inline ui g_unused(){return unused.size();}
	vector<ui> clusters_file(const string &);
	vector<ui> g_clusters_list(const string&);
	void show();
};

/// Unidades basicas de armazenamento (num HD orientado a setores)
class Sector {
private:
	int last_valid;
	bool eof;
	ui used;						/// Obs: sera usado como bool, mas poedria indicar o primeiro espaco vago no setor.
	int next;
	const static ui MAX = 512;		//// 512 bytes por setor
	char byte_s[MAX];	///[512];
public:
	Sector(): eof(false),next(-1), used(false){/*cout << this->used << endl;*/}
	inline const char * g_byte_s(){return byte_s;}
	inline bool g_eof(){return eof;}
	inline int g_last_valid(){return last_valid;}
	ui insert_last(const string&,  const ui&, int);	///  cluster,  posicao do setor, last valid position
	ui insert(const char*, const ui&, const ui&, string);	///  cluster, next cluster, nº do setor, se eh o ultimo ou nao
};

/// 1 cluster contem 4 setores.
class Cluster{
private:
	ui eof;		/// Offset que indica o final do arquivo dentro do cluster
	bool used;							/// usar tbm como bool !
	int next;
	const static ui MAX = 4;
	vector<Sector> sector;		
public:
	Cluster() : used(0){sector.resize(this->MAX);}

	inline Sector g_sector(const ui& i)const{return sector[i%MAX];}
	inline vector<Sector> g_sectors()const{return sector;}
	inline bool g_used(){return used;}
	inline ui g_eof(){return eof;}

	ui insert(const char*, const ui&, string);
	char* g_cluster_content();
};

class Track{
private:
	bool full;
	ui used;
	const static ui MAX_SECTOR = 60;	/// 60 setores por trilha
	const static ui MAX_CLUSTERS = 4;	/// 15 clusters por trilha
	const static ui CLUSTERS = MAX_SECTOR / MAX_CLUSTERS;
	vector<Cluster> cluster;
public:

	Track(): full(false), used(0) {cluster.resize(CLUSTERS);}

	inline Cluster g_cluster(const ui& i)const{return cluster[i%this->CLUSTERS];}
	inline vector<Cluster> g_clusters()const{return cluster;}
	inline bool g_full(){return full;}
	bool s_full();
	ui insert(const char*, ui, const ui&, string);
	constexpr static ui g_CLUSTERS(){return CLUSTERS;}
	char* g_cluster_content(ui);
};

class Cylinder{
private:
	bool full;					/// @ se false, ainda tem espaco no cilindro
	ui used;
	const static ui MAX = 5;	/// 5 trilhas por cilindro
	const static ui MAX_CLUSTERS = MAX * Track :: g_CLUSTERS();	/// 5 trilhas por cilindro
	vector<Track> track;
	vector<ui> full_tracks;
	bool s_full();
public:

	Cylinder():full(false), used(0), full_tracks(MAX,0){track.resize(MAX);}
	inline Track g_track(const ui& i)const{return track[i%MAX];}
	inline vector<Track> g_tracks()const{return track;}
	inline bool g_full(){return full;}
	ui g_cluster(ui);

	ui insert(const char*, ui ,const ui&, string);	/// Posicao do cluster(relativa), posicao absoluta do PROXIMO cluster.
	char* g_cluster_content(ui);		/// TODO: implementar isso

	inline constexpr static ui g_CLUSTERS(){return MAX_CLUSTERS;}
};

class HardDrive{							/// Convencao: primeiro cilindro a ser preenchido eh o mais externo
private:
 	bool full;
 	ui used;						/// Clusters usados.
	static const ui CYLINDERS = 10;	/// 10 trilhas por superficie	
	FatTable fat;
 	const static ui MAX_CLUSTERS = CYLINDERS * Cylinder :: g_CLUSTERS();
 	vector <Cylinder> cylinder;
	ui insert_file(const string&, ui);	/// Primeiro 'ui' eh o cilindro de insercao
	bool set_full();
public:
	
	static const ui g_n_cylinders(){return CYLINDERS;}
	HardDrive(): full(false), used(0){cylinder.resize(CYLINDERS);}
	~HardDrive(){cylinder.resize(0);}
	inline Cylinder g_cylinder(const ui& i){return cylinder[i%CYLINDERS];}
	inline bool g_full(){return full;}
	ui insert_file();			/// Funcao QUE INTERESSA ao usuario que vai gusrdar um arquivo.
	bool remove_file();
	void show_file();
	void show_FAT();
	constexpr static ui g_CLUSTERS(){return MAX_CLUSTERS;}
};

/// Final das Estruturas basicas

class Time{	/// Modulo passivo; apenas disponibiliza tempos (obs: tempos em 'ms')
private:
	Time(){}
public:
	static const int SEEK_MEAN = 4;
	static const int SEEK_MIN = 1;
	static const int LATENCY_MEAN = 6;
	static const int TRANSFER_TRACK = 12;
	static constexpr double TRANSFER_CLUSTER = TRANSFER_TRACK/15.0;
};



#endif