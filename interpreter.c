//to build: gcc -Wall -Wno-pointer-sign -std=c99 -lm -o "%e" "%f"

#include <stdio.h>
#include <stdint.h>
#include <string.h>
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
int min(int a, int b) {
  return a < b ? a : b;
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
	unsigned char* dataAddr;
};

int move();		//move cursor in direction dir, with direction- and bounds-checking
int get_data(struct PixData*);
int run(struct PixData*);
int clearVars();

int dir = 0;
unsigned char* data;	//pixel data from the image
struct Coord pos;		//current position of the cursor
struct Coord size;		//maximum size of the image
struct Var vars[256];	//array of variables for the program

int main(int argc, char* argv[]){
	int n;
	if(argc <= 1){
		printf("no filename given. Exiting...");
		return -1;
	}
	else data = stbi_load(argv[1], &(size.x), &(size.y), &n, 4);
	if(n < 4){
		printf("Not enough channels in image! is there an alpha channel?\n");
		return 1;
	}
	if(data != NULL){
		//find starting position
		int startsFound = 0;
		for(unsigned int i = 0; i < size.x*size.y; ++i){
			if(data[i*4]&data[i*4+1]&data[i*4+2]&data[i*4+3] == 255){
				if(startsFound) printf("Error!\nStart has already been found! second start found at %d,%d", i%size.x, i/size.x);
				else{
					++startsFound;
					pos.x = i%size.x;
					pos.y = i/size.x;
				}
			}
		}
		dir = 2;
		struct PixData datum;
		run(&datum);
		
	}
	clearVars();
	stbi_image_free(data);
	return 0;
}

int move(){
	switch(dir){
		//up
		case 1:
			--pos.y;
			break;
		//right
		case 2:
			++pos.x;
			break;
		//down
		case 4:
			++pos.y;
			break;
		//right
		case 8:
			--pos.x;
			break;
		default:
			printf("not a valid direction");
			return 1;
	}
	if(pos.x >= size.x || pos.y >= size.y){
		printf("\n\nError!\nCursor out of bounds!\n");
		return 1;
	}
	return 0;
}

int get_data(struct PixData* datum){
	int memPos = (size.x*pos.y+pos.x)*4;	//index of the pixel in data
	datum->nibble = ((data[memPos]&7)<< 1) + ((data[memPos+1]>>2)&1);		//get first 4 bits of data
	datum->stored = ((data[memPos+1]&3)<< 6) + ((data[memPos+2]&7)<< 3) + (data[memPos+3]&7);	//get next byte of data
	return 0;
}

int run(struct PixData* datum){
	move();
	get_data(datum);
		
	switch(datum->nibble){
	//pixels that don't require additional data
		//end program
		case 0:
		{
			return 0;
		}
		//change direction
		case 3:
		{
			dir = datum->stored;
			return run(datum);
		}
	//pixels that require additional data
		//if statement
		case 6:
		{
			struct PixData ext[2];
			run(ext);
			run(&(ext[2]));
			if(memcmp(vars[ext[0].stored].dataAddr, vars[ext[1].stored].dataAddr, min(vars[ext[0].stored].size, vars[ext[0].stored].size)) > 0)
				dir = datum->stored & 15;
			else
				dir = (datum->stored>>4) & 15;
			return run(datum);
		}
		//do math with a variable - TODO
		case 4:
		{
			break;
		}
		//print a variable
		case 7:
		{
			
			struct PixData ext;
			run(&ext);
			
			if(datum->stored == 1)
				printf("%.*s", vars[ext.stored].size, vars[ext.stored].dataAddr);
			else
				for(unsigned int i = 0; i < vars[ext.stored].size; ++i)
					printf("%u ", vars[ext.stored].dataAddr[i]);
			return run(datum);
		}
		//define a variable
		case 9:
		{
			unsigned char saveTarg;
			struct PixData ext;
			
			//get variable to assign to
			run(&ext);
			saveTarg = ext.stored;
			
			//clear memory of saveTarg if already created
			if(vars[saveTarg].size)
				free(vars[saveTarg].dataAddr);
			
			//malloc saveTarg
			vars[saveTarg].size = datum->stored;
			vars[saveTarg].dataAddr = malloc(vars[saveTarg].size);
			
			
			//set *dataAddr to 0
			for(unsigned int i = 0; i < vars[saveTarg].size; ++i)
				vars[saveTarg].dataAddr[i] = 0;
			
			for(unsigned int i = 0; i < vars[saveTarg].size; ++i){
				run(&ext);
				if(ext.nibble == 11){	//if given a variable
					for(unsigned int j = 0; j < min(vars[saveTarg].size, vars[ext.stored].size); ++i);
					break;
				}
				else if(ext.nibble == 10){
					vars[saveTarg].dataAddr[i] = ext.stored;
				}
			}
			
			return run(datum);
		}
		//get input - TODO
		case 8:
		{
			break;
		}
	//pixels that return data
		//constant value
		case 10:
		{
			return 0;
		}
		//reference a variable
		case 11:
		{
			return 0;
		}
		default:
			printf("\n\nError!\nnot a valid tile\n");
	}
	
	return -1;
}

int clearVars(){
	for(unsigned int i; i < 256; ++i)
		if(vars[i].size)
			free(vars[i].dataAddr);
	return 0;
}
