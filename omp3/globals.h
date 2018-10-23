#include <stdio.h>

/* All global definitions go here */
typedef unsigned char uint8;


/* Dimesions NxM, B block size and p search area */
int M;
int N;
int B;
int p;
int nframes;	//Number of frames to check
int sframe;		//Starting Frame
char *filename;


// We define these as globals in order to use them in successive HS calls and
// not malloc and free in every iteration
uint8 *current2,*previous2,*current4,*previous4;
uint8 *currentframe,*previousframe,*outputframe;
int *motion_vectors_x,*motion_vectors_y;



//Define N_B and M_B macros
int N_B;
int M_B;


/* Define Function Prototypes */
void readframe2(FILE *fp, uint8 *framed, int framenumber, int xdimension, int ydimension);

double SNR2(uint8 *current,uint8 *output);
FILE *opensequence(char *filename);
void hs_motion_estimation2(uint8 *current,uint8 *previous,int *vectors_x,int *vectors_y);

void writeframe2(uint8 *framed);

double SNR(uint8 current[N][M],uint8 output[N][M]);
void motion_compensation2(uint8 *previous,int *vectors_x,int *vectors_y,uint8 *output);

void print_array_int(int *array,int xdim,int ydim);
