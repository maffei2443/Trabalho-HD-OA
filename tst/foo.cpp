#include <iostream>
using namespace std;

unsigned int foo(const int& i){
	return i;
}

int main(){
	
	char  oi[] = "kkkkkkkkkkk";
	int a = -1;
	cout << a << endl;

	cout << foo(a)%60 << endl;
	
	return 0;
}
