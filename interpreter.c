//to build: gcc -Wall -Wno-pointer-sign -std=c99 -lm -o "%e" "%f"

#include <stdio.h>
#include <stdint.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


void printBits(size_t const size, void const * const ptr){
    unsigned char *b = (unsigned char*) ptr;
    unsigned char byte;
    int i, j;

    for (i=size-1;i>=0;i--)
    {
        for (j=7;j>=0;j--)
        {
            byte = (b[i] >> j) & 1;
            printf("%u", byte);
        }
    }
    puts("");
}

struct Coord{
	unsigned int x;
	unsigned int y;
};
struct PixData{
	unsigned char stored;
	unsigned int nibble : 4;	//	1/2 byte
};
struct Var{
	unsigned int size;
	unsigned char* data_addr;
};

int move();		//move cursor in direction dir, with direction- and bounds-checking
int get_data(struct PixData*);
int run();

int dir = 0;
unsigned char* data;	//pixel data from the image
struct Coord pos;		//current position of the cursor
struct Coord size;		//maximum size of the image
struct Var vars[256];	//array of variables for the program

int main(){
	int n;
	data = stbi_load("test5.png", &(size.x), &(size.y), &n, 4);
	printf("%d\n", n);
	if(n < 4){
		printf("Not enough channels in image! is there an alpha channel?\n");
		return 1;
	}
	if(data != NULL){
		//find starting position
		int startsFound = 0;
		for(unsigned int i = 0; i < size.x*size.y; i++){
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
		pos.x = 2;
		pos.y = 3;
		struct PixData testDat;
		get_data(&testDat);
		printf("%d\n", testDat.nibble);
		printf("%d\n", testDat.stored);
	}
	stbi_image_free(data);
	return 0;
}

int move(){
	switch(dir){
		case 1:
			--pos.y;
			break;
		case 2:
			++pos.x;
			break;
		case 3:
			++pos.y;
			break;
		case 4:
			--pos.x;
			break;
		default:
			printf("not a valid direction");
			return 1;
	}
	if(pos.x >= size.x || pos.y >= size.y){
		printf("Cursor out of bounds!");
		return 1;
	}
	return 0;
}

int get_data(struct PixData* datum){
	int memPos = (size.x*pos.y+pos.x)*4;	//index of the pixel in data
	(*datum).nibble = ((data[memPos]&7)<< 1) + ((data[memPos+1]>>2)&1);		//get first 4 bits of data
	(*datum).stored = ((data[memPos+1]&4)<< 6) + ((data[memPos+2]&7)<< 3) + (data[memPos+3]&7);	//get next byte of data
	return 0;
}

int run(){
	struct PixData datum;
	move();
	get_data(&datum);
	
	switch(datum.nibble){	//flow control only
		//end program
		case 2:
			return 0;
		//change direction
		case 3:
			dir = datum.stored;
			break;
		//if statement - TODO
		case 6:
			break;
	}
	
	return 0;
}
