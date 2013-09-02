#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <cmath>
#include <cassert>
#include "TimeStamp.hh"
#include "StatVector.hh"
using namespace std;

//R must be "%xmmi" 0<=i<=15
#define zeroxmm(R)				\
  asm volatile("xorps %" R ", %" R "\n\t":::R);

//R1 and R2 must be "%xmmi" 0<=i<=15 
#define addxmm(R1, R2)					\
  asm volatile("addpd %" R1 ", %" R2 "\n\t":::R1, R2);

//R1 and R2 must be "%xmmi" 0<=i<=15 
#define mulxmm(R1, R2)					\
  asm volatile("mulpd %" R1 ", %" R2 "\n\t":::R1, R2);

//R = "%xmmi" 0<=i<=15
//a = double * (16 byte aligned)
#define loadxmm(a, R)					\
  asm volatile("movaps %0, %" R "\n\t"::"m"(*(a)):R);	

//R = "%xmmi" 0<=i<=15xmm
//a = double * (16 byte aligned)
#define storexmm(R, a)					\
  asm volatile("movaps %" R ", %0 \n\t":"=m"(*(a))::R);	


// a[0..2] += nitns*b[0..2]
//(expect 3 clock ticks per iteration)
void addreg(double *a, double *b, long int nitns){
  long int  i;
  loadxmm(a, "%xmm0");
  loadxmm(b, "%xmm1");
  for(i=0; i < nitns; i++)
    addxmm("%xmm0","%xmm1");
  storexmm("%xmm1", a);
}

// a[0..2] += nitns*b[0..2]
//(expect 3 clock ticks per iteration)
void add3reg(double *a, double *b, long int nitns){
  long int  i;
  loadxmm(a, "%xmm0");
  loadxmm(b, "%xmm1");
  loadxmm(a+2, "%xmm2");
  loadxmm(b+2, "%xmm3");
  loadxmm(a+4, "%xmm4");
  loadxmm(b+4, "%xmm5");
  for(i=0; i < nitns; i++){
    addxmm("%xmm0","%xmm1");
    addxmm("%xmm2","%xmm3");
    addxmm("%xmm4","%xmm5");
  }
  storexmm("%xmm1", a);
  storexmm("%xmm3", a+2);
  storexmm("%xmm5", a+4);
}

void runaddreg(){
  __declspec(align(16)) double a[6]={0};
  __declspec(align(16)) double b[6]={0};
  a[0] = 0;
  a[1] = 1;
  b[0] = 1;
  b[1] = 2;
  long int nitns = 1000*1000*1000l;
  TimeStamp clk;
  clk.tic();
  addreg(a, b, nitns);
  double cycles = clk.toc();
  cout<<"cycles per itn = "<<cycles/nitns<<endl;
  cout<<"a = "<<a[0]<<"  "<<a[1]<<endl;
  clk.tic();
  add3reg(a, b, nitns);
  cycles = clk.toc();
  cout<<"cycles per itn = "<<cycles/nitns<<endl;
  cout<<"a = "<<a[0]<<"  "<<a[1]<<endl;
}


void fivecyclesA(long int nitns){
  long int  i;
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  zeroxmm("%xmm7");
  zeroxmm("%xmm8");
  zeroxmm("%xmm9");
  zeroxmm("%xmm10");
  zeroxmm("%xmm11");
  zeroxmm("%xmm12");
  zeroxmm("%xmm13");
  zeroxmm("%xmm14");
  zeroxmm("%xmm15");
  for(i=0; i < nitns; i++){
    addxmm("%xmm8", "%xmm0");
    addxmm("%xmm9", "%xmm1");
    addxmm("%xmm10", "%xmm2");
    mulxmm("%xmm11", "%xmm3");
    mulxmm("%xmm12", "%xmm4");
    mulxmm("%xmm13", "%xmm5");
    mulxmm("%xmm14", "%xmm6");
    mulxmm("%xmm15", "%xmm7");
  }
}

void fivecyclesB(long int nitns){
  long int  i;
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  zeroxmm("%xmm7");
  zeroxmm("%xmm8");
  for(i=0; i < nitns; i++){
    addxmm("%xmm8", "%xmm0");
    addxmm("%xmm8", "%xmm1");
    addxmm("%xmm8", "%xmm2");
    mulxmm("%xmm8", "%xmm3");
    mulxmm("%xmm8", "%xmm4");
    mulxmm("%xmm8", "%xmm5");
    mulxmm("%xmm8", "%xmm6");
    mulxmm("%xmm8", "%xmm7");
  }
}


void runfivecycles(){
  long int nitns = (long)1000*1000*1000;
  TimeStamp clk;
  clk.tic();
  fivecyclesA(nitns);
  double cycles = clk.toc();
  cout<<"fivecyclesA: number of cycles per iteration = "<<cycles/nitns<<endl;
  clk.tic();
  fivecyclesB(nitns);
  cycles = clk.toc();
  cout<<"fivecyclesB: number of cycles per iteration = "<<cycles/nitns<<endl;
}

void peakA(long int nitns){
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  zeroxmm("%xmm7");
  zeroxmm("%xmm8");
  zeroxmm("%xmm9");
  zeroxmm("%xmm10");
  zeroxmm("%xmm11");
  zeroxmm("%xmm12");
  zeroxmm("%xmm13");
  zeroxmm("%xmm14");
  zeroxmm("%xmm15");
  for(long int i=0; i < nitns; i++){
    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");
  }
}

void peakA_unroll10(long int nitns){
  long int  i;
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  zeroxmm("%xmm7");
  zeroxmm("%xmm8");
  zeroxmm("%xmm9");
  zeroxmm("%xmm10");
  zeroxmm("%xmm11");
  zeroxmm("%xmm12");
  zeroxmm("%xmm13");
  zeroxmm("%xmm14");
  zeroxmm("%xmm15");
  for(i=0; i < nitns/10; i++){
    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");
    
    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");

    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");
    
    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");

    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");
    
    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");

    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");
    
    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");

    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");
    
    addxmm("%xmm0", "%xmm1");
    mulxmm("%xmm1", "%xmm2");
    addxmm("%xmm3", "%xmm4");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm6", "%xmm7");
    mulxmm("%xmm7", "%xmm8");
    addxmm("%xmm9", "%xmm10");
    mulxmm("%xmm10", "%xmm11");
    addxmm("%xmm12", "%xmm13");
    mulxmm("%xmm13", "%xmm14");
  }
}

void peakB(long int nitns){
  long int  i;
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  zeroxmm("%xmm7");
  zeroxmm("%xmm8");
  zeroxmm("%xmm9");
  zeroxmm("%xmm10");
  zeroxmm("%xmm11");
  zeroxmm("%xmm12");
  zeroxmm("%xmm13");
  zeroxmm("%xmm14");
  zeroxmm("%xmm15");
  for(i=0; i < nitns; i++){
    mulxmm("%xmm0", "%xmm1");
    addxmm("%xmm1", "%xmm10");
    mulxmm("%xmm2", "%xmm3");
    addxmm("%xmm1", "%xmm11");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm5", "%xmm12");
    mulxmm("%xmm6", "%xmm7");
    addxmm("%xmm5", "%xmm13");
    mulxmm("%xmm8", "%xmm9");
    addxmm("%xmm9", "%xmm10");

    mulxmm("%xmm0", "%xmm1");
    addxmm("%xmm9", "%xmm11"); 
    mulxmm("%xmm2", "%xmm3");
    addxmm("%xmm3", "%xmm12");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm3", "%xmm13");
    mulxmm("%xmm6", "%xmm7");
    addxmm("%xmm7", "%xmm10");
    mulxmm("%xmm8", "%xmm9");
    addxmm("%xmm7", "%xmm11");

    mulxmm("%xmm0", "%xmm1");
    addxmm("%xmm1", "%xmm12");
    mulxmm("%xmm2", "%xmm3");
    addxmm("%xmm1", "%xmm13");
    mulxmm("%xmm4", "%xmm5");
    addxmm("%xmm5", "%xmm10");
    mulxmm("%xmm6", "%xmm7");
    addxmm("%xmm5", "%xmm11");
    mulxmm("%xmm8", "%xmm9");
    addxmm("%xmm9", "%xmm12");
  }
}

void peakC(double *a, long int nitns){
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  for(long int i=0; i < nitns; i++){
    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    
    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");
  }
}

void peakC_unroll4(double *a, long int nitns){
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  for(long int i=0; i < nitns/4; i++){
    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    
    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    
    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    
    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    
    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");
  }
}

void peakD(double *a, long int nitns){
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  for(long int i=0; i < nitns; i++){
    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0"); 
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);
  }
}

void peakD_unroll8(double *a, long int nitns){
  zeroxmm("%xmm0");
  zeroxmm("%xmm1");
  zeroxmm("%xmm2");
  zeroxmm("%xmm3");
  zeroxmm("%xmm4");
  zeroxmm("%xmm5");
  zeroxmm("%xmm6");
  for(long int i=0; i < nitns/8; i++){
    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0"); 
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);

    /** 2 ***/

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0"); 
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);

    /*** 4 ***/

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0"); 
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0"); 
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);

    loadxmm(a, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm2");
    storexmm("%xmm2", a+100);

    loadxmm(a+12, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm3");
    storexmm("%xmm3", a+200);

    loadxmm(a+24, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm4");
    storexmm("%xmm4", a+300);

    loadxmm(a+36, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm5");
    storexmm("%xmm5", a+400);

    loadxmm(a+48, "%xmm0");
    mulxmm("%xmm1", "%xmm0");
    addxmm("%xmm0", "%xmm6");
    storexmm("%xmm6", a+500);
    
  }
}

void runpeak(){
  __declspec(align(16)) double a[1024]={0};
  long int nitns = (long)1000*1000*1000;
  TimeStamp clk;
  double cycles;
  
  clk.tic();
  peakA(nitns);
  cycles = clk.toc();
  cout<<"peakA: number of cycles per iteration = "<<cycles/nitns<<endl;
  
  clk.tic();
  peakA_unroll10(nitns);
  cycles = clk.toc();
  cout<<"peakA_unroll10: number of cycles per iteration = "<<cycles/nitns<<endl;

  clk.tic();
  peakB(nitns);
  cycles = clk.toc();
  cout<<"peakB: number of cycles per iteration = "<<cycles/nitns<<endl;

  clk.tic();
  peakC(a, nitns);
  cycles = clk.toc();
  cout<<"peakC: number of cycles per iteration = "<<cycles/nitns<<endl;

  clk.tic();
  peakC_unroll4(a, nitns);
  cycles = clk.toc();
  cout<<"peakC_unroll4: number of cycles per iteration = "<<cycles/nitns<<endl;

  clk.tic();
  peakD(a, nitns);
  cycles = clk.toc();
  cout<<"peakD: number of cycles per iteration = "<<cycles/nitns<<endl;
  
  clk.tic();
  peakD_unroll8(a, nitns);
  cycles = clk.toc();
  cout<<"peakD_unroll8: number of cycles per iteration = "<<cycles/nitns<<endl;
}

void runmacro(){
  __declspec(align(16)) double a[2]={1,2};
  __declspec(align(16)) double b[2]={-1,-2};
  loadxmm(a, "%xmm0");
  loadxmm(b, "%xmm1");
  addxmm("%xmm1", "%xmm0");
  storexmm("%xmm0", a);
  printf("%f %f \n", a[0], a[1]);
}

int main(){
  //runaddreg();
  //runaddarray();
  //runfivecycles();
  runpeak();
  //runmacro();
}