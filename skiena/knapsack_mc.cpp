#include <iostream>
#include <bitset>
#include <tuple>
#include <map>
#include <random>


#define N 15 			// This is the set size
#define C 100

using namespace std;



int main(void)
{
  
  // Intialize the two maps needed to hold the size of the objects and
  // the cost of the objects.

  // Size:
  map<int, tuple<int, int>> sv;
  default_random_engine gen;
  uniform_int_distribution<int> dist(1, 100);
  uniform_int_distribution<int> dist2(2, 10);
  for (unsigned i=0; i< N; ++i)
    sv[i] = make_tuple(dist2(gen), i*dist(gen));


  // Now start doing the monte-carlo

  // First generate some solution.
  // soln: bitset, size, cost
  tuple<bitset<N>, int, int> soln(0, 0, 0);

  // Now generate all subsets in N and check.
  for (unsigned i = 1; i < (2 << (N-1)); i++) {
    int csize = C;
    int svalue = 0;
    bitset<N> vv(i);
    unsigned j = 0;
    for (; j < N; ++j) {
      if (vv[j] == 1 && (csize - get<0>(sv[j]) >= 0)) {
	csize -= get<0>(sv[j]);
	svalue += get<1>(sv[j]);
      }
      else if (csize - get<0>(sv[j]) < 0)
	break;
    }
    if (j == N) {
      // This is solution, check if it is better than the current
      // solution.
      if (get<2>(soln) < svalue) {
	soln = make_tuple(vv, (C - csize), svalue);
      }
    }
  }

  // Optimal solution is:
  cout << get<0>(soln) << " with size: " << get<1>(soln);
  cout << " and cost: " << get<2>(soln) << endl;

  return 0;
}

