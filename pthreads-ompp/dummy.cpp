int dummy(int& x){
  return x;
}

double dummy(double& x){
  return x;
}

void dummy(){
}

void dummy(double *a, int len){
  for(int i=0; i < len; i++)
    a[i] = (i%77)*(i*1001);
}