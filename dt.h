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

/* distance transform */

#ifndef DT_H
#define DT_H

#include <algorithm>
#include "image.h"
#include <time.h>
#include <vector>

#define INF 900 //was 1E20 !!!TODO 50 for debugging
using namespace std;
/* dt of 1d function using squared distance */
static float *dt(float *f, int n) {
	float *d0 = new float[n]; //Values of distance for q starting from d1
	float *d1 = new float[n]; //Values of distance for q starting from d2 
	int old_method =2;


	if(old_method == 1)
	{
		float *d = new float[n]; //Values of distance for q starting from d2 
		int *v = new int[n];
		float *z = new float[n+1];
		int k = 0;
		v[0] = 0;
		z[0] = -INF;
		z[1] = +INF;
		for (int q = 1; q <= n-1; q++) {
			float s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
			while (s <= z[k]) {
				k--;
				s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
			}
			k++;
			v[k] = q;
			z[k] = s;
			z[k+1] = +INF;
		}

		k = 0;
		for (int q = 0; q <= n-1; q++) {
			while (z[k+1] < q)
				k++;
			d[q] = square(q-v[k]) + f[v[k]]; 
		}
		//TODO- expand z to align with q to understand how it is affecting clearly
		//Also plot v to see impact
		printf("Values of v:\n");
		for(int tk=0;tk<n;tk++) printf("%f ",v[tk]);
		printf("\n");
		printf("Values of z:\n");
		std::ofstream outfile;
		outfile.open("example.txt");
		
		int max=50;
		printf("Printing z[tk] values");
		
		for(int tk=1;tk<n+1;tk++) 
		{
			printf("%f ",z[tk]);
			if(z[tk]>max) z[tk] = max;
			if(z[tk]< -max) z[tk] = -max;
			outfile<<float(z[tk]) << " "; 
			
		}
		outfile << "\n";
		for(int tk=0;tk<n;tk++) 
		{
			printf("%f ",v[tk]);
			if(z[tk]>max) v[tk] = max;
			if(z[tk]< -max) v[tk] = -max;
			outfile<<float(v[tk]) << " "; 
			
		}

		outfile << "\n";
		outfile.close();
		printf("\n");
		delete [] v;
		delete [] z;
		return d;
	}
	else if(old_method ==0)
	{
		int Num_par = 2;
		int Num_elements = n/Num_par;
		vector<vector<float> > d_ForAll(Num_par,vector<float>(n,0.0));
		
		std::ofstream outfile;
		outfile.open("example.txt");
		
		for(int np=0;np<Num_par;np++)
		{
			float *d = new float[Num_elements]; //Values of distance for q starting from d2 
			int *v = new int[Num_elements];
			float *z = new float[Num_elements+1];
			int k = 0;
			v[0] = 0;
			z[0] = -INF;
			z[1] = +INF;
			for (int q = np; q <= n-1; q=q+Num_par)  //q=q+Num_par as we want to alternate in the number of processors
			{
				float s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
				while (s <= z[k]) {
					k--;
					s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
				}
				k++;
				v[k] = q;
				z[k] = s;
				z[k+1] = +INF;
			}

			k = 0;
			printf("Values for np = %d: ",np);
			for (int q = 0; q <= n-1; q++) {
				while (z[k+1] < q)
					k++;
				d_ForAll[np][q] = square(q-v[k]) + f[v[k]];
				outfile << d_ForAll[np][q] << " ";
				printf("%f ",d_ForAll[np][q]);
			}
			outfile << "\n";
			printf("\n");
			delete [] v;
			delete [] z;

		}
		outfile.close();
		float *d = new float[n];
		for(int q=0;q<n;q++)
		{
			int temp_min = d_ForAll[0][q];
			for(int np=1;np<Num_par;np++)
			{ 
				if(d_ForAll[np][q] < temp_min) temp_min = d_ForAll[np][q];

			}
			d[q] = temp_min;
		}
		return d;

	}
	else
	{
		//This is just to test things out
		int Num_par = 2;
		int Num_elements = n/Num_par;
		vector<vector<float> > d_ForAll(Num_par,vector<float>(n,INF));
		
		std::ofstream outfile;
		outfile.open("example.txt");
		
		for(int np=0;np<Num_par;np++)
		{
			float *d = new float[n]; //Values of distance for q starting from d2 
			int *v = new int[n];
			float *z = new float[n];
			int k = 0;
			v[0] = 0;
			z[0] = -INF;
			z[1] = +INF;
			for (int q = 1+np; q <= n-1; q=q+Num_par)  //q=q+Num_par as we want to alternate in the number of processors
			{
				float s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
				while (s <= z[k]) {
					k--;
					s  = ((f[q]+square(q))-(f[v[k]]+square(v[k])))/(2*q-2*v[k]);
				}
				k++;
				v[k] = q;
				z[k] = s;
				z[k+1] = +INF;
			}

			k = 0;
			printf("Values for np = %d: ",np);
			outfile << d_ForAll[np][0]<< " "; //TODO:Remove this and below- used for debugging
			outfile << d_ForAll[np][1]<< " "; //TODO : Now do a dry run to figure out what's wrong
			for (int q = 2; q <= n-1; q++) 
			{
				while (z[k+1] < q)
					k++;
				d_ForAll[np][q] = square(q-v[k]) + f[v[k]];
				outfile << d_ForAll[np][q] << " ";
				printf("%f ",d_ForAll[np][q]);
			}
			outfile << "\n";
			printf("\n");
			delete [] v;
			delete [] z;

		}
		outfile.close();
		float *d = new float[n];
		for(int q=0;q<n;q++)
		{
			int temp_min = d_ForAll[0][q];
			for(int np=1;np<Num_par;np++)
			{ 
				if(d_ForAll[np][q] < temp_min) temp_min = d_ForAll[np][q];

			}
			d[q] = temp_min;
		}
		return d;


	}
}

/* dt of 2d function using squared distance */
static void dt(image<float> *im) {
	int width = im->width();
	int height = im->height();
	float *f = new float[std::max(width,height)];

	// transform along columns
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			f[y] = imRef(im, x, y);
		}
		float *d = dt(f, height);
		for (int y = 0; y < height; y++) {
			imRef(im, x, y) = d[y];
		}
		delete [] d;
	}

	// transform along rows
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			f[x] = imRef(im, x, y);
		}
		float *d = dt(f, width);
		for (int x = 0; x < width; x++) {
			imRef(im, x, y) = d[x];
		}
		delete [] d;
	}

	delete f;
}


/* dt of binary image using squared distance */
static image<float> *dt(image<uchar> *im, uchar on = 1) {
	int width = im->width();
	int height = im->height();

	image<float> *out = new image<float>(width, height, false);
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			if (imRef(im, x, y) == on)
	imRef(out, x, y) = 0;
			else
	imRef(out, x, y) = INF;
		}
	}

	dt(out);
	return out;
}

#endif
