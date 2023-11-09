#ifndef BLOCK
#define BLOCK

enum blocktype { Z, S, T, O, I, L, J, X };

//blocks
typedef struct {
	enum blocktype type;
} block;

#endif