#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <inttypes.h>
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
	
	//printf("x: %d\ty: %d\tdir:%d\tnib: %d\tbyte: %d\n", pos.x, pos.y, dir, datum->nibble, datum->stored);
	
	if(moveError == 1)
		printf("\n\nError!\nTried to move in an invalid direction!\n\tDirection: %d\n\tPos: %d %d\n", dir, pos.x, pos.y);
	else if(moveError == 2)
		printf("\n\nError!\nTried to move out of bounds!\n\tDirection: %d\n\tPos: %d %d\n", dir, pos.x, pos.y);
	if(depth > 1000)
		printf("\n\nError!\nMaximum function depth exceeded!\nYou may have started a function before a loop and didn't end it.\n\tDirection: %d\n\tPos: %d %d\n", dir, pos.x, pos.y);
	
	switch(datum->nibble){
	//pixels that don't require additional data
		//end program
		case 0:
			return -1;
		//continue
		case 2:
			if(depth > 0) return run(datum); else return 0;
		//change direction
		case 3:
			dir = datum->stored;
			if(depth > 0) return run(datum); else return 0;
	//pixels that require additional data
		//if statement
		case 6:
		{
			++depth;
			struct PixData ext1;
			struct PixData ext2;
			run(&ext1);
			run(&ext2);
			
			//printf("var1: %" PRId64 "\nvar2: %" PRId64 "\n", *((int64_t*)vars[ext1.stored].dataAddr), *((int64_t*)vars[ext2.stored].dataAddr));
			
			if(vars[ext1.stored].size > 0 && vars[ext2.stored].size > 0 && memcmp(vars[ext1.stored].dataAddr, vars[ext2.stored].dataAddr, min(vars[ext1.stored].size, vars[ext2.stored].size)) > 0){
				dir = (datum->stored>>4) & 15;
			}
			else if(vars[ext1.stored].size < 0 && vars[ext2.stored].size < 0 && *((int64_t*)vars[ext1.stored].dataAddr) < *((int64_t*)vars[ext2.stored].dataAddr)){
				dir = (datum->stored>>4) & 15;
			}
			else{
				dir = datum->stored & 15;
			}
			--depth;
			if(depth > 0) return run(datum); else return 0;
		}
		//do math with a variable
		case 4:
		{
			++depth;
			struct PixData addTo, getFrom;
			run(&addTo);
			run(&getFrom);
			//if both variables are ints
			if(vars[addTo.stored].size == -1 && vars[getFrom.stored].size == -1){
				//printf("\nvar1:\t%" PRId64 "\nvar2:\t%" PRId64 "\n", *((int64_t*)vars[addTo.stored].dataAddr), *((int64_t*)vars[getFrom.stored].dataAddr));
				switch(datum->stored){
					//add to
					case 1:
						*((int64_t*)vars[addTo.stored].dataAddr) += *((int64_t*)vars[getFrom.stored].dataAddr);
						break;
					//subtract from
					case 2:
						*((int64_t*)vars[addTo.stored].dataAddr) -= *((int64_t*)vars[getFrom.stored].dataAddr);
						break;
					//multiply by
					case 3:
						*((int64_t*)vars[addTo.stored].dataAddr) *= *((int64_t*)vars[getFrom.stored].dataAddr);
						break;
					//divide by
					case 4:
						*((int64_t*)vars[addTo.stored].dataAddr) /= *((int64_t*)vars[getFrom.stored].dataAddr);
						break;
					case 5:
						*((int64_t*)vars[addTo.stored].dataAddr) %= *((int64_t*)vars[getFrom.stored].dataAddr);
						break;
					default:
						printf("\n\nError!\nMath function argument is not a valid operation!\n\tDirection: %d\n\tPos: %d %d\n", dir, pos.x, pos.y);
						break;
				}
			}
			else printf("\n\nError!\nArguments given to math function are not both ints!\n\tDirection: %d\n\tPos: %d %d\n", dir, pos.x, pos.y);
			--depth;
			if(depth > 0) return run(datum); else return 0;
		}
		//string manipulation
		case 5:
		{
			++depth;
			struct PixData ext1, ext2, ext3;	//ext3 is only used in substring gets.
			run(&ext1);
			run(&ext2);
			switch(datum->stored){
				//get length of string
				case 1:
					//ext1 is the string, ext2 is the int to set
					if(vars[ext2.stored].size > 0)
						free(vars[ext1.stored].dataAddr);
					if(vars[ext2.stored].size >= 0){
						vars[ext2.stored].dataAddr = malloc(sizeof(int64_t));
						vars[ext2.stored].size = -1;
					}
					*((int64_t*)vars[ext2.stored].dataAddr) = vars[ext1.stored].size;
					break;
				//set length of string
				case 2:
					//ext1 is the string, ext2 is the new length
					if(vars[ext1.stored].size >= 0){
						vars[ext1.stored].dataAddr = realloc(vars[ext1.stored].dataAddr, *((int64_t*)vars[ext2.stored].dataAddr));
						for(unsigned int i = vars[ext1.stored].size; i < *((int64_t*)vars[ext2.stored].dataAddr); i++)
							vars[ext1.stored].size = 0;
						vars[ext1.stored].size = *((int64_t*)vars[ext2.stored].dataAddr);
					}
					else
						printf("\n\nError!\nTrying to set the length of a non-string!\nVar %d may be uninitialized or may be an int!\n\tDirection: %d\n\tPos: %d %d\n", ext1.stored, dir, pos.x, pos.y);
					break;
				//get substr
				case 3:
					//ext1 is the string, ext2 is the pos to start from, and ext3 is the string to copy to
					run(&ext3);
						if(vars[ext1.stored].size > 0 && vars[ext2.stored].size == -1 && vars[ext3.stored].size > 0)
							for(unsigned int i = 0; i < min(vars[ext3.stored].size, vars[ext1.stored].size - *((int64_t*)vars[ext2.stored].dataAddr)); ++i)
								vars[ext3.stored].dataAddr[i] = vars[ext1.stored].dataAddr[i + *((int64_t*)vars[ext2.stored].dataAddr)];
						else if(vars[ext1.stored].size > 0 && vars[ext2.stored].size == -1 && vars[ext3.stored].size < 0)
							*((int64_t*)vars[ext2.stored].dataAddr) = vars[ext1.stored].dataAddr[*((int64_t*)vars[ext2.stored].dataAddr)];
						else
							printf("\n\nError!\nA value is not the right type, or is uninitialized!\n\tDirection: %d\n\tpos: %d %d\n", dir, pos.x, pos.y);
					break;
				//set substr
				case 4:
					//ext1 is the string, ext2 is the pos to start from, and ext3 is the string to copy from
					run(&ext3);
						if(vars[ext1.stored].size > 0 && vars[ext2.stored].size == -1 && vars[ext3.stored].size > 0)
							for(unsigned int i = 0; i < min(vars[ext3.stored].size, vars[ext1.stored].size - *((int64_t*)vars[ext2.stored].dataAddr)); ++i)
								vars[ext1.stored].dataAddr[i + *((int64_t*)vars[ext2.stored].dataAddr)] = vars[ext3.stored].dataAddr[i];
						else if(vars[ext1.stored].size > 0 && vars[ext2.stored].size == -1 && vars[ext3.stored].size < 0)
							vars[ext1.stored].dataAddr[*((int64_t*)vars[ext2.stored].dataAddr)] = *((int64_t*)vars[ext2.stored].dataAddr);
						else
							printf("\n\nError!\nA value is not the right type, or is uninitialized!\n\tDirection: %d\n\tpos: %d %d\n", dir, pos.x, pos.y);
					break;
					
			}
			--depth;
			if(depth > 0) return run(datum); else return 0;
		}
		//print a variable
		case 7:
		{
			if(vars[datum->stored].size > 0)
				printf("%.*s", vars[datum->stored].size, vars[datum->stored].dataAddr);
			else if(vars[datum->stored].size < 0)
				printf("%" PRId64 "", *((int64_t*)vars[datum->stored].dataAddr));
			if(depth > 0) return run(datum); else return 0;
		}
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
			
			//if variable will be defined as an int
			if(datum->stored == 0){
				//malloc saveTarg
				vars[saveTarg].size = -1;
				vars[saveTarg].dataAddr = malloc(sizeof(int64_t));
				*(vars[saveTarg].dataAddr) = 0;
			}
			//if variable is a string
			else{
				//malloc saveTarg
				vars[saveTarg].size = datum->stored;
				vars[saveTarg].dataAddr = malloc(vars[saveTarg].size);
			
				//set *dataAddr to 0
				for(unsigned int i = 0; i < vars[saveTarg].size; ++i)
					vars[saveTarg].dataAddr[i] = 0;
			}
			
			for(unsigned int i = 0; i < (vars[saveTarg].size < 0 ? sizeof(int64_t) : vars[saveTarg].size); ++i){
				run(&ext);
				if(ext.nibble == 11){	//if given a variable
					if(vars[ext.stored].size < 0 && vars[saveTarg].size < 0)	//if both variables are ints
						*((int64_t*)vars[saveTarg].dataAddr) = *((int64_t*)vars[ext.stored].dataAddr);
					else if(vars[ext.stored].size > 0 && vars[saveTarg].size > 0)	//elif both vars are strs
						for(unsigned int j = 0; j < min(vars[saveTarg].size, vars[ext.stored].size); ++i)
							vars[saveTarg].dataAddr[j] = vars[ext.stored].dataAddr[j];
					
					break;
				}
				else if(ext.nibble == 10){
					vars[saveTarg].dataAddr[i] = ext.stored;
				}
			}
			--depth;
			if(depth > 0) return run(datum); else return 0;
		}
		//get input
		case 8:
		{
			++depth;
			struct PixData ext;
			run(&ext);
			if(vars[ext.stored].size != 0){
				free(vars[ext.stored].dataAddr);
				vars[ext.stored].size = 0;
			}
			//if getting an int
			if(datum->stored == 0){
				vars[ext.stored].dataAddr = malloc(sizeof(int64_t));
				vars[ext.stored].size = -1;
				scanf("%" SCNd64 "", (int64_t*)vars[ext.stored].dataAddr);
			}
			//if getting a str of set size
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
			//if getting a str w/out set size
			else if(datum->stored == 2){
				int ch;
				size_t len = 0;
				while(EOF!=(ch=getchar()) && ch != '\n'){
					if(len==vars[ext.stored].size){
						vars[ext.stored].dataAddr = realloc(vars[ext.stored].dataAddr, sizeof(char)*(vars[ext.stored].size+=1));
					}
					vars[ext.stored].dataAddr[len++]=ch;
				}
			}
			--depth;
			if(depth > 0) return run(datum); else return 0;
		}
	//pixels that return data
		//constant value
		case 10:
			return 1;
		//reference to a variable
		case 11:
			return 1;
		default:
			printf("\n\nError!\nnot a valid tile\n\tDirection: %d\n\tPos: %d %d\n", dir, pos.x, pos.y);
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
