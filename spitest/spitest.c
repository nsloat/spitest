#include "wiringPiSPI.h"
#include "wiringPi.h"
#include "stdio.h"
#include "stdlib.h"

int spiWrite(unsigned int tosend, int bytecount) {
	unsigned char buf[bytecount];
	int i;
	for(i = 0; i < bytecount; i++) {
		buf[i] = 0xff & (tosend>>(i*8));
	}
	printf("writing 0x%.2x%.2x\n",buf[1],buf[0]);
	
	return wiringPiSPIDataRW(0,buf,bytecount);
}

int main(int argc, char *argv[]) {
	wiringPiSetup();
	
	pinMode(0,OUTPUT);
	digitalWrite(0,1);
	
	unsigned char bytes[2] = {0x5f,0x61};
	int speed = 500000;
	unsigned int scroll = 0x1;
	
	if(argc > 1) {bytes[0] = 0xff&(atoi(argv[argc-1])<<4);}
	
	wiringPiSPISetup(0,speed);
	wiringPiSPISetup(1,speed);
	
		digitalWrite(0,0);
	wiringPiSPIDataRW(0,bytes,2);
	delayMicroseconds(500);
		digitalWrite(0,1);

	wiringPiSPIDataRW(1,bytes,2);
	printf("read 0x%.2x 0x%.2x\n", bytes[0]&0xff, bytes[1]&0xff);
	delayMicroseconds(500);

	int bytes2send = 2;
	int valread = 0;
	while(1) {

		digitalWrite(0,0);
		spiWrite(scroll, bytes2send);
		digitalWrite(0,1);
delayMicroseconds(550);
		wiringPiSPIDataRW(1,bytes,2);
		
		delayMicroseconds(500);
		valread = ((bytes[1] & 0xff)<<8) | (bytes[0] & 0xff);
		printf("write 0x%.4x\nread  0x%.4x\n", scroll&0xffff, valread&0xffff);
		delayMicroseconds(1000000);
		scroll = scroll<<1;
	}
	
	return 0;
}
