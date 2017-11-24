#include <iostream>
using namespace std;
#define ui unsigned int
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

inline ui which_cylinder(ui cluster){
	printf("ui which_cylinder(%d) == %d\n", cluster, (2 * cluster) / ((Qtt :: HARDDRIVE / Qtt :: CYLINDER) * (Qtt :: TRACK / Qtt :: CLUSTER)));
	return (2 * cluster) / ((Qtt :: HARDDRIVE / Qtt :: CYLINDER) * (Qtt :: TRACK / Qtt :: CLUSTER));
}

ui which_track(ui cluster){
	ui aux = 0;
	ui div = 75;
	if(cluster >= div){
		aux = which_cylinder(cluster);
		printf("[which_track :: cylinder] aux == %d\n", aux);
	}
	cluster -= aux * div;
	return cluster / (Qtt :: TRACK / Qtt :: CLUSTER);	//  15
}

ui which_cluster(ui cluster){
	return ( cluster - which_cylinder(cluster) * 75 - which_track(cluster) * 15 );
}

int main(){
	for(int i = 0; i < 750; i++){
		cout << i << "<>" << which_cylinder(i) << endl;
		cout << i << "<>" << which_track(i) << endl;
		cout << i << "<>" << which_cluster(i) << endl;

	}
}