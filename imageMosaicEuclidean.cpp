/*
  Solution by Nursultan Jubatyrov.
  This program uses Euclidean distance for each pixel values of images from database and simply iterate to find the best patch.
  How to compile:
  g++ -o imageMosaicEuclidean  imageMosaicEuclidean.cpp -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
*/
#include <vector>
#include <queue>
#include <list>
#include <map>
#include <set>
#include <deque>
#include <stack>
#include <bitset>
#include <algorithm>
#include <functional>
#include <numeric>
#include <utility>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <cstring>
#include <cstdio>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include "CImg.h"

using namespace std;
using namespace cimg_library;

#define fi first
#define se second
#define mp make_pair
#define sz(x) ((int)(x).size())
#define re return
#define pb(x) push_back(x)
#define pf(x) push_front(x)
#define abs(x) ((x) < 0 ? -(x) : (x))
#define INF 2000000000
#define sqr(x) ((x) * (x))
#define all(x) x.begin(), x.end()
#define fname "a"
#define MOD 1000000007

CImg<int> target("NU.png");
CImg <int> d[2020];
int n, res, N, imgSize;
long double dist1, dist2, dist3, dist, r1, g1, b1, tmp;
int cnt[1000];
char c[21];
string s;

struct point {

  long double r, g, b;

} points[2020], trgt, m[1111][100][100]; 

void getName(int i) {

  memset(c, 0, 11);

  s = to_string(i);
  int  k = 0;
  for(int j = 0; j < sz(s); j++) {
    c[k++] = s[j];
  }
  c[k++] = '.';
  c[k++] = 'j';
  c[k++] = 'p';
  c[k++] = 'g';
}

long double calcError(int indx, int jj, int ii) {
  
  dist1 = dist2 = dist3 = 0;

  for(int i = 0; i < imgSize; i++)
    for(int j = 0; j < imgSize; j++) {

      dist1 += (m[indx][j][i].r - target(jj * imgSize + j, ii * imgSize + i, 0)) * (m[indx][j][i].r - target(jj * imgSize + j, ii * imgSize + i, 0));
      dist2 += (m[indx][j][i].g - target(jj * imgSize + j, ii * imgSize + i, 1)) * (m[indx][j][i].g - target(jj * imgSize + j, ii * imgSize + i, 1));
      dist3 += (m[indx][j][i].b - target(jj * imgSize + j, ii * imgSize + i, 2)) * (m[indx][j][i].b - target(jj * imgSize + j, ii * imgSize + i, 2));
    }

  return sqrt(dist1 * dist1 + dist2 * dist2 + dist3 * dist3);
}

int main() {
  srand(time(0));
  puts("Please enter the number of images and the size of the patch");
  scanf("%d %d", &N, &imgSize);
  puts("Wait until images are downloaded...");

  

  for(int i = 0; i < N; i++) {
      cnt[i] = 100;
      getName(i + 1);
      CImg <int> tmp(c);
      tmp.move_to(d[i]);
     
      r1 = b1 = g1 = 0;
      for(int h = 0; h < imgSize; h++)
        for(int l = 0; l < imgSize; l++) {
          m[i][h][l].r = d[i](h, l, 0);
          m[i][h][l].g = d[i](h, l, 1);
          m[i][h][l].b = d[i](h, l, 2);
        }
      
    }

    puts("Done, now a little time to get the results...");

    CImg < int > result(target.width(), target.height(), 1, 3);
    for(int i = 0; i < target.height() / imgSize; i++) 
      for(int j = 0; j < target.width() / imgSize; j++) {

          res = -1;
          for(int h = 0; h < N; h++) {
            
            tmp = calcError(h, j, i);

            if(res == -1 || tmp < dist) {
              dist = tmp;
              res = h;
            }
          }

          for(int k = 0; k < imgSize; k++)
            for(int l = 0; l < imgSize; l++) {
              result(j * imgSize + l, i * imgSize + k, 0) = d[res](l, k, 0);
              result(j * imgSize + l, i * imgSize + k, 1) = d[res](l, k, 1);
              result(j * imgSize + l, i * imgSize + k, 2) = d[res](l, k, 2);
            }
      } 
    cout << clock() / (double)CLOCKS_PER_SEC << endl;
    result.display();
    result.save("imageMosaicEuclidean.jpg");
  return 0;
}