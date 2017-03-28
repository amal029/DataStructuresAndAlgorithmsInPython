#include <iostream>
#include "graph.hpp"
#include <queue>
#include <vector>
#include <algorithm>
#include <iterator>

using namespace std;


vector<int> ec_main(Vertex* start)
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
      // Processing the incidentedges.
      vector<shared_ptr<Edge>> nce;
      for (auto x: v->getIEdges()){
	if (!x->colored)
	  nce.push_back(x);
	else
	  nc.push_back(x->color);
	Vertex* n = x->opposite(v->getName()).get();
	if (!n->getVisited()){
	  s.push(n);
	  n->setVisited(true);
	}
      }

      set_difference(result.begin(), result.end(),
		     nc.begin(), nc.end(),
		     inserter(diff, diff.begin()));

      // Be careful here, because we can have many edges to color.
      for (unsigned i = 0; i < nce.size(); i++) {
	if (!diff.empty())
	  nce[i]->color = diff[i];
	else {
	  result.push_back(color++);
	  nce[i]->color = *(result.end()-1);
	}
	nce[i]->colored = true;
      }
      // Now erase things.
      nc.clear();
      diff.clear();

    } while (!s.empty());


  return result;
}

