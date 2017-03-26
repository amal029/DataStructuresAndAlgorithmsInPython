#include <iostream>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <array>
#include <stack>
#include <list>

using namespace std;

#define N 2			// Dimensions
#define M 10 			// Number of points.

float dot_product(array<int, N> p, array<int, N> x){
  float ret = 0.0f;
  array<int, N> prod;
  for (unsigned i=0; i < N; i++)
    prod[i] = p[i]*x[i];
  for(auto x: prod)
    ret += x;
  return ret;
}

float cross_product(array<int, N> p1, array<int, N> p2, array<int, N> p3){
  return (p2[0]-p2[0])*(p3[1]-p1[1]) - (p2[1]-p1[1])*(p3[0]-p1[0]);
}


int main(void)
{
  array<array<int, N>, M> points;

  // Now populate the array with random numbers.
  srand(time(nullptr));
  for (unsigned i = 0; i < M; i++)
    for (unsigned j = 0; j < N; j++)
      points[i][j] = rand()%100 + 1;


  // Now find the lowest y-coord point.
  auto f = [](auto x, auto y){return x[N-1] < y[N-1];};
  auto ret = min_element(points.begin()+1, points.end(), f);
  if (ret == end(points))
    throw "Could not find the lowest Y-coordinate!";

  array<int, N> p = *ret; 	// The lowest point.
  *ret = points[0];
  points[0] = p;

  // Now sort the points according to slope between point and p, with
  // respect to the x-axis -- inplace sort.
  sort(points.begin()+1, points.end(), [p](auto x1, auto x2)
       {return dot_product(x1, p) < dot_product(x2, p);});

  stack<array<int, N>, list<array<int, N>>> s;
  s.push(points[0]);
  s.push(points[1]);

  // This scan finds all the points on the boundary of the convex hull,
  // but it is quite complex.
  for (unsigned i = 2; i < M; ++i) {
    // cross product < 0 means right turn, we only want left turning
    // points.
    auto B = s.top();
    s.pop();

    auto A = s.top();

    auto v = cross_product (A, B, points[i]) ;
    // cout << "cp: " << v << "\n";

    if (v > 0) {
      // cout << "pushing" << "\n";
      s.push(B);
      s.push(points[i]);
    }

    if (i < M-1 && s.empty()) {
      exit(1);
    }
    
  }

  // The result is now in points array.
  while (!s.empty()) {
    for(auto x: s.top())
      cout << x << " ";
    cout <<  "\n";
    s.pop();
  }
  return 0;
}

