/*
Copyright (C) 2006 Pedro Felzenszwalb

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
*/

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "pnmfile.h"
#include "imconv.h"
#include "dt.h"
#include <time.h>
#include <chrono>
#include<iostream>
#include<omp.h>

int main(int argc, char **argv) {
  if (argc != 3) {
    fprintf(stderr, "usage: %s input(pbm) output(pgm)\n", argv[0]);
    return 1;
  }

  char *input_name = argv[1];
  char *output_name = argv[2];

  // load input
  //image<uchar> *input = loadPBM(input_name);

  
  image<uchar> *input = loadPGM(input_name);
  // compute dt
  auto start_wall_clock = std::chrono::steady_clock::now();
  image<float> *out = dt(input);
  auto finish_wall_clock = std::chrono::steady_clock::now();
  std::cout << "Wall clock: " << (finish_wall_clock - start_wall_clock) / std::chrono::microseconds(1) << " microseconds\n";
  //printf("Time Elapsed: %d\n ", timer2-timer);
  // take square roots
 int A[5] = {10,11,12,13,14}; 
  omp_set_num_threads(8);
  #pragma omp parallel shared(A) 
{
	printf("\nThread num: %d \n",omp_get_thread_num());
	int a= 5;
	#pragma omp for
	for(int i=0;i<5;i++) 
	{
		printf("\nNum(%d) TN:(%d) A[%d] = %d \n",i,omp_get_thread_num(),i,A[i]);
		for(int j=0;j<5;j++) 
		{
			printf("\nThread num: %d \n",omp_get_thread_num());
			printf("i-BA[%d] = %d   ",i,A[i]);
			printf("j-B[%d] = %d   ",j,A[j]);
		}
		printf("\n");
	}
	
	
	printf("\n");
}

for (int y = 0; y < out->height(); y++) {
    for (int x = 0; x < out->width(); x++) {
      imRef(out, x, y) = sqrt(imRef(out, x, y));
    }
  }

  // convert to grayscale
  image<uchar> *gray = imageFLOATtoUCHAR(out);

  // save output
  savePGM(gray, output_name);

  delete input;
  delete out;
  delete gray;
}
