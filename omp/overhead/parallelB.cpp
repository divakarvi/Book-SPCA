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

#include "../../utils/TimeStamp.hh"
#include "../../utils/StatVector.hh"
#include "parallel.hh"
#include <omp.h>

extern void dummy();

void parallelB(int nthreads, StatVector& stats){
	TimeStamp clk;
	clk.tic();
#pragma omp parallel				\
	num_threads(nthreads)			\
	default(none)
	{
		dummy();
	}
	double cycles = clk.toc();
	stats.insert(cycles);
}
