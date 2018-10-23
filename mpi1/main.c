// Minas Dasygenis HS- reloaded
// 2014-02
// All functions divided to individual files for better management
// 

 #include <stdio.h>
 #include <math.h>
 #include <string.h>	//for strncpy
 #include <stdlib.h>
 #include <time.h>
 #include <unistd.h>	//for close
 #include <mpi.h>
#include "globals.h"



// Cleanup function
void exit_function(void)
{
printf("***Exiting Rank***\n");
free(filename);
free(current2);
free(current4);
free(previous2);
free(previous4);
free(currentframe);
free(previousframe);
free(outputframe);
}





int main(argc,argv)
int argc;
char *argv[];
 {

/*N=1;
M=1;
B=1;
p=1;
sframe=1;
nframes=1;
previousframe=0;
currentframe=0;
outputframe=0;
current2=0;
previous2=0;
current4=0;
previous4=0;
*/


//The first step is to parse all command line parameters
	 if (argc<7) { 
printf("7 variables are required: N M B p startingframe number_frames_to_process sequence\n");
printf("Example: ./main 720 576 16 7 0 1 barb.yuv\n");
exit(-3);}

 int rank, size;
 
 MPI_Init(&argc,&argv);
 MPI_Comm_size(MPI_COMM_WORLD,&size);
 MPI_Comm_rank(MPI_COMM_WORLD,&rank);

 if (size<3)
 {
	printf("Error: Minimum number of processors: 3\n\n");
 	MPI_Finalize();
	return 0;
 }

//Register the exit function
atexit(*exit_function);


//We will use this for our filename
//256 character name are enough...(I think)
filename=(char *)malloc(256*sizeof(char));



N=atoi(argv[1]);
M=atoi(argv[2]);
B=atoi(argv[3]);
p=atoi(argv[4]);
sframe=atoi(argv[5]);
nframes=atoi(argv[6]);
strncpy(filename,argv[7],255);

//Define here the N/B and M/B variables
N_B=(int)N/B;
M_B=(int)M/B;

//Check if we have a full division
if ( N%B != 0 ) { printf("Warning: N Not fully divided. Fixing it\n");N_B++;}
if ( M%B != 0 ) { printf("Warning: M Not fully divided. Fixing it\n");M_B++;}

printf("Arguments: N=%d, M=%d, B=%d, p=%d, sframe=%d, nframes=%d, sequence=%s, N_B=%d, M_B=%d\n",N,M,B,p,sframe,nframes,filename,N_B,M_B);



 // Pointer to the file of the video sequences
     FILE *picturefp;

     int i; // i frame

     struct timespec start, end; // The time variables

  double snr=0.0;

 if (rank == 0)
 {
 //Time measurement
   clock_gettime(CLOCK_MONOTONIC, &start);
 }


picturefp=opensequence(filename);
 //Do a memory allocation for the used arrays
  current2 =(uint8 *)calloc((N/2)*(M/2)*sizeof(char),1);
  if ( current2 == NULL) {  perror("Memory cannot be allocated to current2");exit(-2); }
  previous2=(uint8 *)calloc((N/2)*(M/2)*sizeof(char),1);
  if ( previous2 == NULL) {  perror("Memory cannot be allocated to previous2");exit(-2); }
  current4 =(uint8 *)calloc((N/4)*(M/4)*sizeof(char),1);
  if ( current4 == NULL) {  perror("Memory cannot be allocated to current4");exit(-2); }
  previous4=(uint8 *)calloc((N/4)*(M/4)*sizeof(char),1);
  if ( previous4 == NULL) {  perror("Memory cannot be allocated to previous4");exit(-2); }
  currentframe =(uint8 *)calloc((N)*(M)*sizeof(char),1);
  if ( currentframe == NULL) {  perror("Memory cannot be allocated to current");exit(-2); }
  previousframe =(uint8 *)calloc((N)*(M)*sizeof(char),1);
  if ( previousframe == NULL) {  perror("Memory cannot be allocated to previous");exit(-2); }
  outputframe =(uint8 *)calloc((N)*(M)*sizeof(char),1);
  if ( outputframe == NULL) {  perror("Memory cannot be allocated to output");exit(-2); }
  motion_vectors_x= (int *)calloc((N_B)*(M_B)*sizeof(int),1);
  if ( motion_vectors_x == NULL) {  perror("Memory cannot be allocated to output");exit(-2); }
  motion_vectors_y= (int *)calloc((N_B)*(M_B)*sizeof(int),1);
  if ( motion_vectors_y == NULL) {  perror("Memory cannot be allocated to output");exit(-2); }

for(i=sframe;i<nframes+sframe;i++)
{
	readframe2(picturefp,currentframe,i,N,M);
	readframe2(picturefp,previousframe,i-1,N,M);
	hs_motion_estimation2(currentframe,previousframe,motion_vectors_x,motion_vectors_y);

	//for every frame processed, print a dot
 	if (rank == 0) printf(".");
	//Clear the buffer to print the dot
	fflush(NULL);
}


// END of check
if (rank == 0)
{
	//Time measurement
	clock_gettime(CLOCK_MONOTONIC, &end);

	const int DAS_NANO_SECONDS_IN_SEC = 1000000000;
	long timeElapsed_s = end.tv_sec - start.tv_sec;
	long timeElapsed_n = end.tv_nsec - start.tv_nsec;
	if(timeElapsed_n< 0){
		timeElapsed_n= DAS_NANO_SECONDS_IN_SEC+ timeElapsed_n;
		timeElapsed_s--;
	}

	printf("\nTime: %ld.%09ld secs \n",timeElapsed_s,timeElapsed_n);
}


 //Validation, if required
  //print_array_int(motion_vectors_x,N_B,M_B);
  //print_array_int(motion_vectors_y,N_B,M_B);
  motion_compensation2(previousframe,motion_vectors_x,motion_vectors_y,outputframe);
  snr=SNR2(currentframe,outputframe);
 if (rank == 0) writeframe2(outputframe);

 if (rank == 0)  printf("SNR = %f\n",snr);


 MPI_Finalize();
 return 0;
 }

