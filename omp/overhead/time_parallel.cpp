/*
 * Copyright Divakar Viswanath, 2009-2014
 */

/*     
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of version 2 of the GNU General Public License as 
 * published by the Free Software Foundation.

 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 */

#include "../../utils/utils.hh"
#include "../../utils/TimeStamp.hh"
#include "../../utils/StatVector.hh"
#include "parallel.hh"
#include <omp.h>
#include <fstream>

void time_parallel(int nthreads, std::ofstream& ofile){
	int count = 1000*1000*100;
	StatVector stats(2*count);
	for(int i=0; i < count; i++){
		parallelA(nthreads, stats);
		parallelB(nthreads, stats);
	}
	
	ofile<<"***********************************************"<<std::endl;
	ofile<<"************* nthreads = "<<nthreads<<"***************"
	     <<std::endl;
	ofile<<"***********************************************"<<std::endl;

	ofile<<"first 20: "<<std::endl;
	for(int i=0; i < 20; i++)
		ofile<<stats(i)<<std::endl;
	
	/*
	 * stats data is sorted here
	 */
	stats.print();
   
	ofile<<"worst 20: "<<std::endl;
	for(int i=1; i < 20; i++)
		ofile<<stats(2*count-i)<<std::endl;

	double median = stats.median();
	int i = 1;
	while(stats(2*count-i)>1.1*median)
		i++;
	ofile<<"fraction of trials greater than 10% of median = "
	     <<(i-1.0)/count/2<<std::endl;
}

int main(){
	verify_dir("DBG");
	std::ofstream ofile("DBG/time_parallel.txt");
	for(int nthrd = 1; nthrd <= 12; nthrd++)
		time_parallel(nthrd, ofile);
	
}