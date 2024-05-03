#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "x86intrin.h"
#include "avx_mathfunc.h"
#include "macro.h"

#define SQR(x) ((x)*(x))

#define NOPS (1024)

void log_exp_check()
{
  static float x[8] = {0.2, 0.3, 0.4, 0.5, 0.6, 0.7, 0.8, 0.9};
  static float y[8];

  long long int ts0, ts1;

  ts0 = _rdtsc();
  for(int k=0;k<NOPS;k++) {
    for(int i=0;i<8;i++) {
      y[i] = log(x[i]+0.1);
      x[i] = exp(y[i]);
    }
  }
  ts1 = _rdtsc();

  long long int cycle_wo_avx = ts1-ts0;

  float x_avx[8] __attribute__((aligned(32)));

  __m256 _x = 
    _mm256_set_ps(0.9, 0.8, 0.7, 0.6, 0.5, 0.4, 0.3, 0.2);
  
  __m256 _y;

  ts0 = _rdtsc();

  for(int k=0;k<NOPS;k++) {
    _x = _mm256_add_ps(_x, _mm256_set1_ps(0.1));
    _y = log256_ps(_x);
    _x = exp256_ps(_y);
  }
  ts1 = _rdtsc();

  long long int cycle_w_avx = ts1-ts0;

  _mm256_store_ps(x_avx, _x);

  float rel_err = 0.0;
  for(int i=0;i<8;i++) {
    rel_err += SQR(fabs((x_avx[i]-x[i])/x[i]));
  }
  rel_err = sqrt(rel_err)/8.0;

  printf("# logarithmic and exponential functions\n");

  printf("Performance gain : %14.6e \n", (float)cycle_wo_avx/(float)cycle_w_avx);
  printf("Relative error   : %14.6e \%\n", rel_err*100.0);
  
}

void cbrt_check()
{
  static float y[8] = {3.0e11, 1.0e12, 3.0e12, 1.0e13, 3.0e13, 1.0e14, 3.0e14, 1.0e15};

  long long int ts0, ts1;

  ts0 = _rdtsc();
  for(int k=0;k<NOPS;k++) {
    for(int i=0;i<8;i++) {
      float x;
      x = cbrt(y[i]);
      y[i] = x*x;
    }
  }
  ts1 = _rdtsc();
  
  long long int cycle_wo_avx = ts1-ts0;


  float x_avx[8] __attribute__((aligned(32)));

  __m256 _x = 
    _mm256_set_ps(1.0e15, 3.0e14, 1.0e14, 3.0e13, 1.0e13, 3.0e12, 1.0e12, 3.0e11);
  __m256 _y, _z;
  
  ts0 = _rdtsc();
  for(int k=0;k<NOPS;k++) {
    _x = cbrt256_ps(_x);
    _x = _mm256_mul_ps(_x, _x);
  }
  ts1 = _rdtsc();

  long long int cycle_w_avx = ts1 - ts0;

  _mm256_store_ps(x_avx, _x);

  float rel_err = 0.0;
  for(int i=0;i<8;i++) {
    rel_err += SQR(fabs((x_avx[i]-y[i])/y[i]));
  }
  rel_err = sqrt(rel_err)/8.0;

  printf("# cubic root function \n");
  printf("Performance gain : %14.6e \n", (float)cycle_wo_avx/(float)cycle_w_avx);
  printf("Relative error   : %14.6e \%\n", rel_err*100.0);  
}


void sin_check()
{
  static float x[8]={2.4, 2.1, 1.8, 1.5, 1.2, 0.9, 0.6, 0.3};

  long long int ts0, ts1;

  ts0 = _rdtsc();
  for(int k=0;k<NOPS;k++) {
    for(int i=0;i<8;i++) {
      x[i] = sin(x[i]);
    }
  }
  ts1 = _rdtsc();

  long long int cycle_wo_avx = ts1-ts0;

  float x_avx[8] __attribute__((aligned(32)));

  __m256 _x = _mm256_set_ps(0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4);

  ts0 = _rdtsc();
  for(int k=0;k<NOPS;k++) {
    _x = sin256_ps(_x);
  }
  ts1 = _rdtsc();

  long long int cycle_w_avx = ts1-ts0;

  _mm256_store_ps(x_avx, _x);

  float rel_err = 0.0;
  for(int i=0;i<8;i++) {
    rel_err += SQR(fabs((x_avx[i]-x[i])/x[i]));
  }
  rel_err = sqrt(rel_err)/8.0;

  printf("# sine function \n");
  printf("Performance gain : %14.6e \n", (float)cycle_wo_avx/(float)cycle_w_avx);
  printf("Relative error   : %14.6e \%\n", rel_err*100.0);


}

void cos_check()
{
  static float x[8]={2.4, 2.1, 1.8, 1.5, 1.2, 0.9, 0.6, 0.3};
  long long int ts0, ts1;

  ts0 = _rdtsc();
  for(int k=0;k<NOPS;k++) {
    for(int i=0;i<8;i++) {
      x[i] = cos(x[i]);
    }
  }
  ts1 = _rdtsc();

  long long int cycle_wo_avx = ts1-ts0;

  float x_avx[8] __attribute__((aligned(32)));

  __m256 _x = _mm256_set_ps(0.3, 0.6, 0.9, 1.2, 1.5, 1.8, 2.1, 2.4);

  ts0 = _rdtsc();
  for(int k=0;k<NOPS;k++) {
    _x = cos256_ps(_x);
  }
  ts1 = _rdtsc();

  long long int cycle_w_avx = ts1-ts0;

  _mm256_store_ps(x_avx, _x);

  float rel_err = 0.0;
  for(int i=0;i<8;i++) {
    rel_err += SQR(fabs((x_avx[i]-x[i])/x[i]));
  }
  rel_err = sqrt(rel_err)/8.0;

  printf("# cosine function \n");
  printf("Performance gain : %14.6e \n", (float)cycle_wo_avx/(float)cycle_w_avx);
  printf("Relative error   : %14.6e \%\n", rel_err*100.0);

}

int main(int argc, char **argv) 
{
  log_exp_check();
  cbrt_check();
  sin_check();
  cos_check();
}
