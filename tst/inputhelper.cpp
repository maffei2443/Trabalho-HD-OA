#include <bits/stdc++.h>
using namespace std;

int main(){
	FILE *fp3 = fopen("hack", "w+");
	for(int i = 16; i < 201; i++){
		FILE *fp = fopen("in", "r");
		fprintf(fp3, "1\n%s\n", string("in" + to_string(i)).c_str() );
		string n("in" + to_string(i));
		FILE * fp2 = fopen(n.c_str(), "w+");
		
		char* line;
		string linha;
		ifstream x("in", ifstream :: binary);
		cout << linha << endl;
		getline(x,linha);
		while(fscanf(fp, "%s", line) > 0){
			fprintf(fp2, "%s", linha.c_str());		
			getline(x,linha);
		}

		fclose(fp);
		fclose(fp2);
	}
	fclose(fp3);
}