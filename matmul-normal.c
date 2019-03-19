#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

void getWalltime(double *wct){
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}

int main(){
  float *a,*b,*c;
  unsigned int i,j,k;
  double ts,te,mflops;
  //allocation
  a = (float *)malloc(N*N*sizeof(float));
  if(a==NULL) exit(1);
  b = (float *)malloc(N*N*sizeof(float));
  if(b==NULL){free(a); exit(1);}
  c = (float *)malloc(N*N*sizeof(float));
  if(c==NULL){free(a);free(b);exit(1);}
  //end allocation
  for(i=0;i<N*N;i++){
    a[i] = (1+i)*0.1;
    b[i] = (i+1)*0.5;
  }

  getWalltime(&ts);
  float sum=0;
  for(i=0;i<N;i++){
    for(j=0;j<N;j++){
      for(k=0;k<N;k++){
        sum+=a[k*N+i]*b[j*N+k];
      }
      c[i*N+j] = sum;
      sum = 0;
    }
  }
  getWalltime(&te);

  mflops = (unsigned long)N*N*N*3.0/(te-ts)*1e6;
  free(a);free(b); free(c);

  printf("Mflops/s = %f\n",mflops);
  return 0;
}
