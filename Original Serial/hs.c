/* Hierarchical Search - Reloaded 2014 */
/* Minas Dasygenis 2014 */
/* Code splitted to files and Makefile, Supporting Video Sequence */

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

#include "globals.h"

void subsampling4(uint8 *array, uint8 *array4)
{

  int oi,oj,ok,ol;
  int temp;	//warning, if CHAR overflow happens!

  //Subsamp4();
     for(oi=0;oi<N/4;oi++)
       for(oj=0;oj<M/4;oj++)
         {
         temp=0;
         for(ok=0;ok<4;ok++)
          for(ol=0;ol<4;ol++)
          {
         //Fix out of bounds
         if ( (4*oi+ok)<N && (4*oj+ol)<M)
          //{ temp+=previous[4*oi+ok][4*oj+ol];}
          { temp+=array[(4*oi+ok)*M + 4*oj+ol];}
          }
         //previous4[oi][oj]=temp/4;
         //convert to 1dimensional for continuous malloc usage
         array4[oi*M/4 + oj]=temp/4;
          }

}





void subsampling2(uint8 *array, uint8 *array2)
{
  int oi,oj,ok,ol;
  int temp;	//warning, if CHAR overflow happens!

 //Subsamp2();
     for(oi=0;oi<N/2;oi++)
       for(oj=0;oj<M/2;oj++)
         {
         temp=0;
         for(ok=0;ok<2;ok++)
          for(ol=0;ol<2;ol++)
          {
          //Fix out of bounds
         if( (2*oi+ok)<N && (2*oj+ol)<M)
          //{ temp+=current[(2*oi+ok)][2*oj+ol]; }
          { temp+=array[(2*oi+ok)*M + 2*oj+ol]; }
          }
         //current2[oi][oj]=temp/4;
         //convert to 1dimensional for continuous malloc usage
         array2[oi*M/2 + oj]=temp/4;
          }

}


void hs_motion_estimation2(uint8 *current,uint8 *previous,int *vectors_x,int *vectors_y) 
{
 int x=0,y=0,x2=0,y2=0,x1=0,y1=0,i=0,j=0,k=0,l=0,p1=0,p2=0,dist=0;
 int min,min2,min4; 
 

subsampling2(current,current2);
subsampling2(previous,previous2);
subsampling4(current,current4);
subsampling4(previous,previous4);


 for(x=0;x<N_B;x++)           /* For all blocks in the current frame */
  for(y=0;y<M_B;y++)
  {
   min=255*B*B;
   min2=255*(B/2)*(B/2);
   min4=255*(B/4)*(B/4); 
   for(i=-p/4;i<p/4+1;i++)    /* Motion Estimation at level 2 */
    for(j=-p/4;j<p/4+1;j++)
    {
     dist=0;
     for(k=0;k<B/4;k++)       /* For all pixels in the block */
      for(l=0;l<B/4;l++)
      {
       //p1=current4[(B/4)*x+k][(B/4)*y+l];
		//convert to 1dimensional for continuous malloc usage
	   if ( ((B/4)*x+k)<N/4 && ((B/4)*y+l)<M/4 )
       { p1=current4[((B/4)*x+k)*M/4 + (B/4)*y+l]; }
       
       if(((B/4)*x+i+k)<0 || ((B/4)*x+i+k)>((N/4)-1) || ((B/4)*y+j+l)<0 || ((B/4)*y+j+l)>((M/4)-1)) 
       p2=0;
       else 
	   //p2=previous4[(B/4)*x+i+k][(B/4)*y+j+l];
		//convert to 1dimensional for continuous malloc usage
       p2=previous4[( (B/4)*x+i+k) *M/4 + ((B/4)*y+j+l)]; 
       
       dist+=abs(p1-p2);
      } 
      
      if(dist<min4)
      {
       min4=dist;
       x2=i;
       y2=j;
      }
    }
    
   for(i=-1;i<2;i++)    /* Motion Estimation at level 1 */
    for(j=-1;j<2;j++)
    {
     dist=0;
     for(k=0;k<B/2;k++)       /* For all pixels in the block */
      for(l=0;l<B/2;l++)
      {
       //p1=current2[(B/2)*x+k][(B/2)*y+l];
		//convert to 1dimensional for continuous malloc usage
	   if ( ((B/2)*x+k)<N/2 && ((B/2)*y+l)<M/2)
       { p1=current2[((B/2)*x+k)*M/2 + ((B/2)*y+l)]; }
      
       if(((B/2)*x+2*x2+i+k)<0 || ((B/2)*x+2*x2+i+k)>((N/2)-1) || ((B/2)*y+2*y2+j+l)<0 || ((B/2)*y+2*y2+j+l)>((M/2)-1)) 
       p2=0;
       else 
       //p2=previous2[(B/2)*x+2*x2+i+k][(B/2)*y+2*y2+j+l];
		//convert to 1dimensional for continuous malloc usage
       p2=previous2[((B/2)*x+2*x2+i+k)*M/2 + ((B/2)*y+2*y2+j+l)];
      
       dist+=abs(p1-p2);
      }
      
      if(dist<min2)
      {
       min2=dist;
       x1=2*x2+i;
       y1=2*y2+j;
      }
    }
    
  for(i=-1;i<2;i++)    /* Motion Estimation at level 0 */
    for(j=-1;j<2;j++)
    {
     dist=0;
     for(k=0;k<B;k++)       /* For all pixels in the block */
      for(l=0;l<B;l++)
      {
       //p1=current[B*x+k][B*y+l];
       p1=current[(B*x+k)*M + B*y+l];
      
       if((B*x+2*x1+i+k)<0 || (B*x+2*x1+i+k)>(N-1) || (B*y+2*y1+j+l)<0 || (B*y+2*y1+j+l)>(M-1))
       p2=0;
       else
       //p2=previous[B*x+2*x1+i+k][B*y+2*y1+j+l];  
       p2=previous[(B*x+2*x1+i+k)*M + B*y+2*y1+j+l];  
       
       dist+=abs(p1-p2);
      }
      
      if(dist<min)
      {
       min=dist;
       vectors_x[x*M_B +y]=2*x1+i;
       vectors_y[x*M_B + y]=2*y1+j;
      }
    }     
 }                      

}
