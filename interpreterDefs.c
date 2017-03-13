#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "interpreterDefs.h"

void printBits(const size_t size, void const * const ptr){
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
		//left
		case 8:
			--pos.x;
			break;
		default:
			return 1;
	}
	if(pos.x >= size.x || pos.y >= size.y){
		return 2;
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
	int moveError = move();
	get_data(datum);	//no errors will be returned here (yet)
	
	//printf("x: %d\ty: %d\t dir:%d\tnib: %d\tbyte: %d\n", pos.x, pos.y, dir, datum->nibble, datum->stored);
	
	if(moveError == 1)
		printf("\n\nError!\nTried to move in an invalid direction!\n\tDirection: %d\n\tPos: %d %d", dir, pos.x, pos.y);
	else if(moveError == 2)
		printf("\n\nError!\nTried to move out of bounds!\n\tDirection: %d\n\tPos: %d %d", dir, pos.x, pos.y);
	if(depth > 1000)
		printf("\n\nError!\nMaximum function depth exceeded!\nYou may have started a function before a loop and didn't end it.\n\tDirection: %d\n\tPos: %d %d", dir, pos.x, pos.y);
	
	switch(datum->nibble){
	//pixels that don't require additional data
		//end program
		case 0:
			return -1;
		//continue
		case 2:
			if(depth <= 1) return 0; else return run(datum);
		//change direction
		case 3:
			dir = datum->stored;
			if(depth <= 1) return 0; else return run(datum);
	//pixels that require additional data
		//if statement
		case 6:
		{
			++depth;
			struct PixData ext1;
			struct PixData ext2;
			run(&ext1);
			run(&ext2);
			
			if(memcmp(vars[ext1.stored].dataAddr, vars[ext2.stored].dataAddr, min(vars[ext1.stored].size, vars[ext2.stored].size)) > 0){
				dir = (datum->stored>>4) & 15;
			}
			else{
				dir = datum->stored & 15;
			}
		}
			if(depth <= 1) return 0; else return run(datum);
		//do math with a variable
		case 4:
		{
			++depth;
			struct PixData addTo, getFrom;
			run(&addTo);
			run(&getFrom);
			//add to
			if(datum->stored == 1){
				varAdd(&vars[addTo.stored], &vars[getFrom.stored]);
			}
			//subtract from
			if(datum->stored == 2){
				varSub(&vars[addTo.stored], &vars[getFrom.stored]);
			}
		}
			if(depth <= 1) return 0; else return run(datum);
		//print a variable
		case 7:
		{
			++depth;
			struct PixData ext;
			run(&ext);
			
			if(datum->stored == 1)
				printf("%.*s", vars[ext.stored].size, vars[ext.stored].dataAddr);
			else
				for(unsigned int i = 0; i < vars[ext.stored].size; i++)
					printf("%u ", vars[ext.stored].dataAddr[i]);
		}
			if(depth <= 1) return 0; else return run(datum);
		//define a variable
		case 9:
		{
			++depth;
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
		}
			if(depth <= 1) return 0; else return run(datum);
		//get input
		case 8:
		{
			++depth;
			struct PixData ext;
			run(&ext);
			unsigned long long buf;
			if(datum->stored == 0){
				scanf("%lld", &buf);
				for(unsigned int i = 0; i < vars[ext.stored].size; ++i){
					if(!buf) break;	//if ext == 0;
					vars[ext.stored].dataAddr[vars[ext.stored].size - (i+1)] = buf&255;
					buf>>=8;
				}
			}
			else if(datum->stored == 1){
				char c;
				for(unsigned int i = 0; i < vars[ext.stored].size; ++i){
					c = getchar();
					if(c == EOF){
						vars[ext.stored].dataAddr[i] = '\0';
						break;
					}
					else
						vars[ext.stored].dataAddr[i] = c;
				}
			}
		}
			if(depth <= 1) return 0; else return run(datum);
	//pixels that return data
		//constant value
		case 10:
			return 1;
		//reference a variable
		case 11:
			return 1;
		default:
			printf("\n\nError!\nnot a valid tile\n");
	}
	
	return 2;
}

int clearVars(){
	for(unsigned int i; i < 256; ++i)
		if(vars[i].size)
			free(vars[i].dataAddr);
	return 0;
}

int varAdd(struct Var* target, struct Var* value){
	unsigned char carry;
	int temp;
	for(unsigned int i = 1; i <= target->size; ++i){
		//add last carry
		temp = target->dataAddr[target->size-i] + carry;
		target->dataAddr[target->size-i] = temp;
		carry = 0;
		if(temp != target->dataAddr[target->size-i])			//if overflow:
			carry = 1;

		//add value[i]
		if(i <= value->size){
			temp = target->dataAddr[target->size-i] + value->dataAddr[value->size-i];
			target->dataAddr[target->size-i] = temp;
			if(temp != target->dataAddr[target->size-i])		//if overflow:
				carry = 1;
		}
	}
	return 0;
}
int varSub(struct Var* target, struct Var* value){
	unsigned char carry;
	int temp;
	for(unsigned int i = 1; i <= target->size; ++i){
		//add last carry
		temp = target->dataAddr[target->size-i] - carry;
		target->dataAddr[target->size-i] = temp;
		carry = 0;
		if(temp != target->dataAddr[target->size-i])			//if overflow:
			carry = 1;

		//add value[i]
		if(i <= value->size){
			temp = target->dataAddr[target->size-i] - value->dataAddr[value->size-i];
			target->dataAddr[target->size-i] = temp;
			if(temp != target->dataAddr[target->size-i])		//if overflow:
				carry = 1;
		}
	}
	return 0;
}