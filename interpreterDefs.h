#ifndef INTERPRETER_DEFS
#define INTERPRETER_DEFS

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

void printBits(size_t const, void const * const);
int min(int, int);
int move();		//move cursor in direction dir, with direction- and bounds-checking
int get_data(struct PixData*);		//get data at cursor location
int run(struct PixData*);			//moves and gets data, automatically
int clearVars();					//
int varAdd(struct Var*, struct Var*);
int varSub(struct Var*, struct Var*);

int dir;
unsigned char* data;	//pixel data from the image
struct Coord pos;		//current position of the cursor
struct Coord size;		//maximum size of the image
struct Var vars[256];	//array of variables for the program
long long depth;
#endif
