/*
  Solution by Nursultan Jubatyrov.
  This program uses average values of triangle images from database and simply iterate to find the best patch.
  How to compile:
  g++ -o imageMosaicTriangle  imageMosaicTriangle.cpp -I/usr/X11R6/include -L/usr/X11R6/lib -lX11
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
CImg <int> d[2010];
int n, rr, N, imgSize;
double dist1, dist2, dist3, dist, r1, g1, b1, tmp, pre;
int cnt[1000];
char c[21];
string s;

struct point {

  double r, g, b;

} points[2010][4], trgt, m[1010][1010][4];

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

double calcError(point a, point b) {
  
  return (a.r - b.r) * (a.r - b.r) + (a.g - b.g) * (a.g - b.g) + (a.b - b.b) * (a.g - b.b);
}

int main() {

  srand(time(0));

  puts("Please enter the number of images and the size of the patch");
  scanf("%d %d", &N, &imgSize);
  puts("Wait until images are downloaded...");

  for(int i = 0; i < N; i++) {

      getName(i + 1);
      CImg <int> tmp(c);
      tmp.move_to(d[i]);
      d[i].resize(imgSize, imgSize);
      d[i].save(c);

      r1 = b1 = g1 = 0;
      for(int h = 0; h < imgSize; h++)
        for(int l = 0; l < imgSize - h; l++) {
          r1 += d[i](h, l, 0);
          g1 += d[i](h, l, 1);
          b1 += d[i](h, l, 2);
        }
      points[i][0].r = r1 / (imgSize * imgSize);
      points[i][0].g = g1 / (imgSize * imgSize);
      points[i][0].b = b1 / (imgSize * imgSize); 

      r1 = b1 = g1 = 0;
      for(int h = 0; h < imgSize; h++)
        for(int l = imgSize - h; l < imgSize; l++) {
          r1 += d[i](h, l, 0);
          g1 += d[i](h, l, 1);
          b1 += d[i](h, l, 2);
        }
      points[i][1].r = r1 / (imgSize * imgSize);
      points[i][1].g = g1 / (imgSize * imgSize);
      points[i][1].b = b1 / (imgSize * imgSize); 

      r1 = b1 = g1 = 0;
      for(int h = 0; h < imgSize; h++)
        for(int l = 0; l < h; l++) {
          r1 += d[i](h, l, 0);
          g1 += d[i](h, l, 1);
          b1 += d[i](h, l, 2);
        }
      points[i][2].r = r1 / (imgSize * imgSize);
      points[i][2].g = g1 / (imgSize * imgSize);
      points[i][2].b = b1 / (imgSize * imgSize); 

      r1 = b1 = g1 = 0;
      for(int h = 0; h < imgSize; h++)
        for(int l = h; l < imgSize; l++) {
          r1 += d[i](h, l, 0);
          g1 += d[i](h, l, 1);
          b1 += d[i](h, l, 2);
        }
      points[i][3].r = r1 / (imgSize * imgSize);
      points[i][3].g = g1 / (imgSize * imgSize);
      points[i][3].b = b1 / (imgSize * imgSize); 
    }

  for(int i = 0; i < target.height() / imgSize; i++) {
      for(int j = 0; j < target.width() / imgSize; j++) {
         
      r1 = b1 = g1 = 0;

      for(int h = 0; h < imgSize; h++)
        for(int l = 0; l < imgSize - h; l++) {
          r1 += target(j * imgSize + l, i * imgSize + h, 0);
          g1 += target(j * imgSize + l, i * imgSize + h, 1);
          b1 += target(j * imgSize + l, i * imgSize + h, 2);
        }
      
      m[j][i][0].r = r1 / (imgSize * imgSize);
      m[j][i][0].g = g1 / (imgSize * imgSize);
      m[j][i][0].b = b1 / (imgSize * imgSize);

      r1 = b1 = g1 = 0;
      for(int h = 0; h < imgSize; h++)
        for(int l = imgSize - h; l < imgSize; l++) {
          r1 += target(j * imgSize + l, i * imgSize + h, 0);
          g1 += target(j * imgSize + l, i * imgSize + h, 1);
          b1 += target(j * imgSize + l, i * imgSize + h, 2);
        }
      
      m[j][i][1].r = r1 / (imgSize * imgSize);
      m[j][i][1].g = g1 / (imgSize * imgSize);
      m[j][i][1].b = b1 / (imgSize * imgSize);

      r1 = b1 = g1 = 0;

      for(int h = 0; h < imgSize; h++)
        for(int l = 0; l < h; l++) {
          r1 += target(j * imgSize + l, i * imgSize + h, 0);
          g1 += target(j * imgSize + l, i * imgSize + h, 1);
          b1 += target(j * imgSize + l, i * imgSize + h, 2);
        }
      
      m[j][i][2].r = r1 / (imgSize * imgSize);
      m[j][i][2].g = g1 / (imgSize * imgSize);
      m[j][i][2].b = b1 / (imgSize * imgSize);

      r1 = b1 = g1 = 0;

      for(int h = 0; h < imgSize; h++)
        for(int l = h; l < imgSize; l++) {
          r1 += target(j * imgSize + l, i * imgSize + h, 0);
          g1 += target(j * imgSize + l, i * imgSize + h, 1);
          b1 += target(j * imgSize + l, i * imgSize + h, 2);
        }
      
      m[j][i][3].r = r1 / (imgSize * imgSize);
      m[j][i][3].g = g1 / (imgSize * imgSize);
      m[j][i][3].b = b1 / (imgSize * imgSize);
      }
    }

    CImg < int > result(target.width(), target.height(), 1, 3);
    
    for(int i = 0; i < target.height() / imgSize; i++) {

      for(int j = 0; j < target.width() / imgSize; j++) {
        
          rr = -1;
          int k = rand() % 2;
          if(k & 1) {
              for(int h = 0; h < N; h++) {
                tmp = calcError(points[h][0], m[j][i][0]);

                if(rr == -1 || tmp < dist) {
                  dist = tmp;
                  rr = h; 
                }
              }
              
            for(int h = 0; h < imgSize; h++)
              for(int l = 0; l < imgSize - h; l++) {
                result(j * imgSize + l, i * imgSize + h, 0) = d[rr](l, h, 0);
                result(j * imgSize + l, i * imgSize + h, 1) = d[rr](l, h, 1);
                result(j * imgSize + l, i * imgSize + h, 2) = d[rr](l, h, 2);
              }
              rr = -1;
              for(int h = 0; h < N; h++) {
                tmp = calcError(points[h][1], m[j][i][1]);

                if(rr == -1 || tmp < dist) {
                  dist = tmp;
                  rr = h; 
                }
              }
              
            for(int h = 0; h < imgSize; h++)
              for(int l = imgSize - h; l < imgSize; l++) {
                result(j * imgSize + l, i * imgSize + h, 0) = d[rr](l, h, 0);
                result(j * imgSize + l, i * imgSize + h, 1) = d[rr](l, h, 1);
                result(j * imgSize + l, i * imgSize + h, 2) = d[rr](l, h, 2);
              }
          }
          else {
              for(int h = 0; h < N; h++) {
                tmp = calcError(points[h][2], m[j][i][2]);

                if(rr == -1 || tmp < dist) {
                  dist = tmp;
                  rr = h; 
                }
              }
              
            for(int h = 0; h < imgSize; h++)
              for(int l = 0; l < h; l++) {
                result(j * imgSize + l, i * imgSize + h, 0) = d[rr](l, h, 0);
                result(j * imgSize + l, i * imgSize + h, 1) = d[rr](l, h, 1);
                result(j * imgSize + l, i * imgSize + h, 2) = d[rr](l, h, 2);
              }
              rr = -1;
              for(int h = 0; h < N; h++) {
                tmp = calcError(points[h][3], m[j][i][3]);

                if(rr == -1 || tmp < dist) {
                  dist = tmp;
                  rr = h; 
                }
              }
              
            for(int h = 0; h < imgSize; h++)
              for(int l = h; l < imgSize; l++) {
                result(j * imgSize + l, i * imgSize + h, 0) = d[rr](l, h, 0);
                result(j * imgSize + l, i * imgSize + h, 1) = d[rr](l, h, 1);
                result(j * imgSize + l, i * imgSize + h, 2) = d[rr](l, h, 2);
              }

          }
        
      } 

    }
    result.display();
    result.save("imageMosaicTriangle.jpg");
  return 0;
}