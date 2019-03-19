#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <emmintrin.h>
#include <pmmintrin.h>

void getWalltime(double *wct){
  struct timeval tp;
  gettimeofday(&tp,NULL);
  *wct = (double)(tp.tv_sec + tp.tv_usec / 1000000.0);
}

int main(){
  float *a, *b, *c , *tempSum;
  __m128 *va, *vb, *vtempSum;
  __m128 tempSumNp;
  unsigned int i,j,k;
  double ts,te,mflops;
  i = posix_memalign((void **)&a,16,N*N*sizeof(float));
  if(i!=0){exit(1);}
  i = posix_memalign((void **)&b,16,N*N*sizeof(float));
  if(i!=0){free(a);exit(1);}
  i = posix_memalign((void **)&c,16,N*N*sizeof(float));
  if(i!=0){free(a);free(b);exit(1);}
  i = posix_memalign((void **)&tempSum,16,4*sizeof(float));
  if(i!=0){free(a);free(b);free(c);exit(1);}

  for(i=0;i<N*N;i++){
    a[i] = (1+i)*0.1;
    b[i] = (i+1)*0.5;
    c[i] = 0.0;
  }

  for(i=0;i<4;i++){
    tempSum[i] = 0;
  }


  getWalltime(&ts);
  for(i=0;i<N;i++){
    va = (__m128 *)a;
    vtempSum = (__m128 *)tempSum;
    for(j=0;j<N;j++){
      vb = (__m128 *)(b+N*i);
      for(k=0;k<N;k+=4){
        *vtempSum = _mm_add_ps(*vtempSum, _mm_mul_ps(*va,*vb));
        va++;
        vb++;
      }
      tempSumNp = _mm_hadd_ps(*vtempSum,*vtempSum);
      *vtempSum = _mm_hadd_ps(tempSumNp,tempSumNp);
      c[N*j+i] = tempSum[0];
      for(int l=0;l<4;l++){
        tempSum[l] = 0;
      }
    }
  }
  getWalltime(&te);
  free(a),free(b),free(c);
  mflops = (unsigned long)N*N*N*3.0/(te-ts)*1e6;

  printf("MFlops/s = %f\n",mflops);
  return 0;
}
