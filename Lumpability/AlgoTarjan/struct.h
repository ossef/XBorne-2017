#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h> 

#define epsilon 1.000000000000000E-011
#define eq(a, b) (fabs(a-b) < epsilon)
#define inf(a, b) (eq(a, b) ? 0 : (a < b))
#define inf_eq(a, b) (eq(a, b) ? 1 : (a < b))
#define dif(a, b) ((version == 2) ? inf(a, b) : inf(b, a))
#define dif_eq(a, b) ((version == 2) ? inf_eq(a, b) : inf_eq(b, a))
#define dif_eps(a, b, e) ((version == 2) ? inf(a, b+e) : inf(b, a+e))

double eps;			//epsilon
int n;				//number of nodes
int nb;				//total number of blocks
int nbU;			//total number of blocks in U

char cwd[150];  		//current directory
char name[150];
char model[150];
char method[150];
char part[250];
int version;			//the method used

//input markov chain
double ** mkv_w;		//weight to successors
int * mkv_sz;			//number of successors
int ** mkv_suc;			//successors

//aggregated markov chain
int * agr_sz;			//number of successors
int ** agr_suc;			//successors
double ** agr_w;		//weight to successors 
double ** agr_w_min;		//minimun weight to successors in block
double ** agr_w_max;		//maximum weight to successors in block

struct bits 			//coding bits for efficiency
{
	unsigned int b : 1;	
};

int * bl;			//block ids
struct bits * spl;		//block to be split
struct bits * U_or_V;		//0 if block belongs to U, 1 if it belongs to V 
double * w;			//sum of weights
int * num;    			//number of nodes in each block

int * rdr;			//reordering of nodes
int * inv;			//inverse reordering of nodes
struct bits ** bit; 		//binary representation of states

typedef struct node 
{
	int i;			//a node in the block
	int num;		//number of nodes in the block
	double min, max; 
	struct node * l, * r;	
} btree;



	
