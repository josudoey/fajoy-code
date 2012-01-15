#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
using namespace std;

class myIpRange {
public:
	unsigned int minIp;
	unsigned int maxIp;
	char c1[4], c2[4], c3[4], c4[4];
	unsigned char i1, i2, i3, i4;
	bool deny;
	myIpRange() {
		minIp = 0x00000000;
		maxIp = 0x00000000;
	}

	void parseRange(char * ip_str) {
		memset(&c1[0],0,sizeof(c1));
		memset(&c2[0],0,sizeof(c2));
		memset(&c3[0],0,sizeof(c3));
		memset(&c4[0],0,sizeof(c4));
		char c=0;

		sscanf(ip_str, "%[+-]%[0-9\*].%3[0-9\*].%3[0-9\*].%3[0-9\*]",&c, c1, c2, c3, c4);
		if (c=='-'){
			deny=true;
		}else{
			deny=false;
		}
		minIp = 0x00000000;
		maxIp = 0x00000000;
		i1 =getMin(c1);
		i2 =getMin(c2);
		i3 =getMin(c3);
		i4 =getMin(c4);
		minIp|=i1<<24;
		minIp|=i2<<16;
		minIp|=i3<<8;
		minIp|=i4;
		i1 =getMax(c1);
		i2 =getMax(c2);
		i3 =getMax(c3);
		i4 =getMax(c4);
		maxIp|=i1<<24;
		maxIp|=i2<<16;
		maxIp|=i3<<8;
		maxIp|=i4;
	}
	bool isConatins(int ip){
		return minIp<=ip&&maxIp>=ip;
	}
	bool isDeny(int ip){
		if(deny)
			return minIp<=ip&&maxIp>=ip;
		else
			return false;
	}
private:
	unsigned char getMin(char *c) {
		char cc=c[0];
		if (cc=='*'){
			return 0;
		}
		else{
			return (unsigned char) (atoi(c) & 0xff);
		}
	}
	unsigned char getMax(char *c) {
		char cc=c[0];
		if (cc=='*')
			return 255;
		else
			return (unsigned char) (atoi(c) &0xff);
	}

};
int main() {
	char ip[] = "+192.168.137.*";
	//unsigned int ip_i = inet_addr(ip);
	myIpRange tmp = myIpRange();
	tmp.parseRange(ip);
	printf("tmp minIp=%u maxIp=%u\n",tmp.minIp,tmp.maxIp);
	myIpRange rules[100];
	memset(rules,0,sizeof(rules));
	FILE *fp=fopen("socks.conf","r");
	char buf[255];
	memset(buf,0,sizeof(buf));
	int len=0;
	int i=0;
	while(len=fgets(buf, sizeof(buf), fp) != NULL){
		rules[i]=myIpRange();
		rules[i].parseRange(buf);
		//printf("%s",buf);
		i++;
		memset(buf,0,sizeof(buf));
	}
	fclose(fp);

	for(i=0;i<5;i++){
		printf("minIp=%u maxIp=%u isContains=%d\n",rules[i].minIp,rules[i].maxIp,rules[i].isConatins(tmp.minIp));
		printf("d=%d isDelay=%d\n",rules[i].deny,rules[i].isDeny(tmp.minIp));

	}

	//socks.conf
	fflush(stdout);
	return 0;
}
