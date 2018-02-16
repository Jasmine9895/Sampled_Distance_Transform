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
  if (argc != 3) {
    fprintf(stderr, "usage: %s input(pbm) output(pgm)\n", argv[0]);
    return 1;
  }

  char *input_name = argv[1];
  char *output_name = argv[2];

  // load input
  //image<uchar> *input = loadPBM(input_name);

    string dir = string("./img/");
    vector<string> files = vector<string>();

    getdir(dir,files);

    for (unsigned int i = 0;i < 5;i++)//change 5 to files.size() 
    {
        cout << files[i] << endl;
    
    }

    string a = "Before loading";
    cout <<  files[1] << endl;
    string in_name = "img/" + files[40];
    cout << in_name << "\n";
    image<uchar> *input = loadPGM(input_name);
    // compute dt
    cout << "Error in loading\n";
    auto start_wall_clock = std::chrono::steady_clock::now();
    image<float> *out = dt(input);
    auto finish_wall_clock = std::chrono::steady_clock::now();
    std::cout << "Wall clock: " << (finish_wall_clock - start_wall_clock) / std::chrono::microseconds(1) << " microseconds\n";
    //printf("Time Elapsed: %d\n ", timer2-timer);
    // take square roots
    for (int y = 0; y < out->height(); y++) {
      for (int x = 0; x < out->width(); x++) {
        imRef(out, x, y) = sqrt(imRef(out, x, y));
      }
    }

    // convert to grayscale
    image<uchar> *gray = imageFLOATtoUCHAR(out);

    // save output
   string out_name = "result_images/" + files[3] + "out.pgm";
    savePGM(gray, out_name.c_str());

    delete input;
    delete out;
    delete gray;
  
}
