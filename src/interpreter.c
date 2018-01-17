//main file for stegan interpreter.
#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "interpreterDefs.h"


int main(int argc, char* argv[]){
	int n;
	if(argc <= 1){
		printf("\n\nError:\nno filename given. Exiting...\n");
		return -1;
	}
	else data = stbi_load(argv[1], (int *)&(size.x), (int *)&(size.y), &n, 4);
	if(n < 4){
		printf("Not enough channels in image! is there an alpha channel?\n");
		return 1;
	}
	if(data != NULL){
		//find starting position
		struct PixData start;
		struct Coord startPos;
		int startDir;
		for(unsigned int x = 0; x < size.x; x++)
			for(unsigned int y = 0; y < size.y; y++){
				pos.x = x;
				pos.y = y;
				get_data(&start);
				if(start.nibble == 1){
					startPos.x = pos.x;
					startPos.y = pos.y;
					startDir = start.stored;
				}
			}
		pos.x = startPos.x;
		pos.y = startPos.y;
		dir = startDir;
		//printf("x:%d, y:%d\tdir:%d\n", pos.x, pos.y, dir);
		struct PixData datum;
		int error;
		while(1){
			depth = 0;
			error = run(&datum);
			if(error == -1)		//if end pixel was encountered
				break;
			else if(error == 1)
				printf("\n\nError!\nFloating data pixel at Pos: %d %d", pos.x, pos.y);
			else if(error == 2)
				printf("\n\nError!\nUnrecognized data pixel at Pos: %d %d", pos.x, pos.y);
		}
	}
	clearVars();
	stbi_image_free(data);
	return 0;
}
