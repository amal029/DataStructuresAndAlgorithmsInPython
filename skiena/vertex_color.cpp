#include <iostream>
#include "graph.hpp"
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;


vector<int> vc_main(Vertex* start)
{

  queue<Vertex*> s;
  s.push(start);
  vector<int> result;

  vector<int> nc;
  vector<int> diff;
  int color = 0;
  do
    {
      Vertex* v = s.front();
      v->setVisited(true);
      s.pop();
      for (auto x: v->neighbors()){
	if (x->colored)
	  nc.push_back(x->color);
	if (!x->getVisited()){
	  s.push(x.get());
	  x->setVisited(true);
	}
      }
      set_difference(result.begin(), result.end(),
		     nc.begin(), nc.end(),
		     inserter(diff, diff.begin()));
      // Now if diff is not empty, then pick one.
      if (!diff.empty()) {
	v->color = *diff.begin();
      }
      else{
	result.push_back(color);
	v->color = *(result.end()-1);
	color += 1;
      }
      v->colored = true;
      // Now erase things.
      nc.clear();
      diff.clear();

    } while (!s.empty());


  return result;
}

