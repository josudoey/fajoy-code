#include <string>
#include <iostream>
#include <sstream>
using namespace std;

int main() {
	stringstream ss;
	ss <<"This is\0 a apple.";

	string tmp=ss.str();
	string oldstr="\0";
	string newstr="";

	int i=0;
	i=tmp.find(oldstr);
	cout<<"i="<<i<<endl;
	if(i >=0){
		tmp.replace(i,oldstr.length(),newstr);
	}
	cout <<ss.str()<<endl;
	cout <<tmp<<endl;
	cout <<endl;



	i=tmp.find(oldstr);
	cout<<"i="<<i<<endl;
	if(i >=0){
		tmp.replace(i,oldstr.length(),newstr);
	}
	cout <<ss.str()<<endl;
	cout <<tmp<<endl;
	cout <<endl;



	i=tmp.find(oldstr);
	cout<<"i="<<i<<endl;
	if(i >=0){
		tmp.replace(i,oldstr.length(),newstr);
	}
	cout <<ss.str()<<endl;
	cout <<tmp<<endl;
	cout <<endl;


	return 0;
}
