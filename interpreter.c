//to build: gcc -Wall -std=c99 -lm -o "%e" "%f"

#include <stdio.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


int main(){
	int x, y, n;
	unsigned char *data = stbi_load("test.png", &x, &y, &n, 3);
	if(*data != NULL){
		for(unsigned int i = 0; i < x*y*n; i += 3)
			printf("%d\t%d\t%d\n", data[i], data[i+1], data[i+2]);
	}
	 stbi_image_free("test.png");
	return 0;
}
