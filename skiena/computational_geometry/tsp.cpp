#include <iostream>
#include <cstdlib>
#include <cmath>
#include <array>


using namespace std;

// This is the geometrical representation and solution of the traveling
// salesman problem in N-dimensional space.


#define N 3 			// 3 dimensional space.
#define M 15			// The number of points to visit.


// Euclidian distance between two points in the distance matrix. For now
// we are recomputing the distance everytime, but we can keep a square
// matrix to just lookup the distance between the two points the
// N-dimensional space.
float euclidian_dist(unsigned r1, unsigned r2, const int* matrix) {
  float ret = 0.0f;
  for (unsigned i=0; i < N; i++)
    ret += powf(float(matrix[N*r1 + i]) - float(matrix[N*r2 + i]), 2.0f);
  ret = sqrt(ret);
  return ret;
}


int main(void)
{
  array<unsigned, M> res;
  unique_ptr<int[]> smatrix = make_unique<int[]>(sizeof(int)*M*N);
  srand(time(nullptr));
  // Initialize the array with some elements.
  for (unsigned i = 0; i < M; ++i)
    for (unsigned j = 0; j < N; ++j)
      // Generate a random number for the element.
      smatrix[N*i+j] = rand()%100;

  // Now we use monte-carlo with generation of permutations.
  // First let us make a random permutation

  array<unsigned, M> perm;

  for (unsigned i = 0; i < M; i++)
    perm[i] = i;

  res = perm;
  // Now make an initial cost of this path.
  float cost = 0.0f;
  for (unsigned i=0; i< M-1; ++i)
    cost += euclidian_dist(i, i+1, smatrix.get());

  cost += euclidian_dist(M-1, 0, smatrix.get()); // The cost of coming
						 // back to the origin.

  cout << "first cost:" << cost << "\n";
  // Now we do for all permutations of this iterator.
  while (next_permutation(perm.begin(), perm.end())) {
    float tcost = 0.0f;
    for (auto it = perm.begin(); it < perm.end()-1; ++it)
      tcost += euclidian_dist(*it, *(it+1), smatrix.get());

    // The coming bacl to origin cost.
    tcost += euclidian_dist(*(perm.end()-1), *(perm.begin()), smatrix.get());

    if (tcost < cost) {
      cost = tcost;
      res = perm;
      cout << "new minimum cost:" << cost << "\n";
    }
  }


  // Result.
  cout << "final cost:" << cost << "\n";
  cout << "final path:" << "\n";
  for(auto r: res)
    cout << r << "\n";
  cout << "\n";
  return 0;
}
