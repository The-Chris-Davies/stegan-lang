//to build: gcc -Wall -std=c99 -lm -o "%e" "%f"

#include <stdio.h>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


struct Coord{
	unsigned int x;
	unsigned int y;
};
struct PixData{
	unsigned nibble : 4;	//	1/2 byte
	uint8_t value;
};

int move();		//move cursor in direction dir, with direction- and bounds-checking
int get_data(struct PixData*);
int run(struct PixData*);
int helpMessage(int);


int dir = 0;
unsigned char* data;
struct Coord pos;
struct Coord size;

int main(){
	int n;
	data = stbi_load("test3.png", &(size.x), &(size.y), &n, 4);
	printf("%d\n", n);
	if(n < 4){
		printf("Not enough channels in image! is there an alpha channel?\n");
		return 1;
	}
	if(data != NULL){
		printf("it worked?!\n");
		//find starting position
		int startsFound = 0;
		for(unsigned int i = 0; i < size.x*size.y; ++i){
			if(data[i*4]&data[i*4+1]&data[i*4+2]&data[i*4+3] == 255){
				if(startsFound) printf("ERROR: start has already been found! second start found at %d,%d", i%size.x, i/size.x);
				else{
					printf("\nstart found at %d, %d\n", i%size.x, i/size.x);
					++startsFound;
					pos.x = i%size.x;
					pos.y = i/pos.x;
				}
			}
		}
	}
	stbi_image_free(data);
	return 0;
}


int move(){
	switch(dir){
		case 1:
			pos
			break;
		
		
	}
	return 0;
}

int get_data(struct PixData* datum){
	return 0;
}

int run(struct PixData* datum){
	move();
	get_data(datum);
	return 0;
}

int helpMessage(int code){
	switch(code)
	{
	case 0:
		printf("no error found");
		break;
	default:
		printf("could not find error message for that code");
		return -1;
	}
	return code;
}
