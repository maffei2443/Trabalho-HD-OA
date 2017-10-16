#include <iostream>

using namespace std;

class Foo{
public:
	int oi;
	void set(const int& o){oi = o;}
};

int main(){
	int a = 99;
	Foo foo;
	foo.set(a);
	a = 100;
	cout << foo.oi << endl;
	return 0;
}