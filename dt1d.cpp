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
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

int getdir (string dir, vector<string> &files)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    while ((dirp = readdir(dp)) != NULL) {
        files.push_back(string(dirp->d_name));
    }
    closedir(dp);
    return 0;
}

int main(int argc, char **argv) {
    int mode = 0; //0 for 2-D and 1 for 1-D
    if(mode==0) //2-D
    {
        if (argc != 3) {
            fprintf(stderr, "usage: %s input(pbm) output(pgm)\n", argv[0]);
            return 1;
        }

        char *input_name = argv[1];
        char *output_name = argv[2];

      
        image<uchar> *input = loadPGM(input_name);
        image<float> *out = dt(input);

        for (int y = 0; y < out->height(); y++) 
        {
            for (int x = 0; x < out->width(); x++) 
            {
                imRef(out, x, y) = sqrt(imRef(out, x, y));
            }
        }

    // convert to grayscale
        image<uchar> *gray = imageFLOATtoUCHAR(out);
        savePGM(gray, output_name);

    }
    else
    {
        
        
        ofstream myfile;
        
    
        int n = 30;

        float A[30] = {0};
        for(int i=0;i<n;i++) A[i] = 200;
        
        A[3] = 4;
        A[8] = 9;
        A[17] = 4;
        A[21] = 30;
        A[27] = 15;
        A[22] = 12;
        A[10] = 5;
        A[26] = 8;
        //cout << ";

        float *res = dt(A,30);
        
        //myfile.open ("example.txt");
        myfile.open ("example.txt", std::ios_base::app);
        for(int i=0;i<n;i++) myfile << A[i] << " " ;
        myfile << "\n";
        for(int i=0;i<n;i++) myfile << res[i] << " " ;

        myfile << "\n";

        
        myfile.close();
        }



}
