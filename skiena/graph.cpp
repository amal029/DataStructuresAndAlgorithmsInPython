#include "graph.hpp"
#include <stack>
#include <queue>
#include <algorithm>
#include <bitset>

#define N 32

using namespace std;

shared_ptr<Vertex> Graph::getVertex(std::string name) const {
  auto it = this->vertices.begin();
  for (; it != this->vertices.end(); it++) {
    if ((*it)->getName() == name) break;
  }
  return (it != this->vertices.end()) ? *it : nullptr;

}

Graph::Graph(vector<tuple<string, int>> vertices,
	     vector<tuple<string, string, int>> edges,
	     bool directed=false) {

  this->directed = directed; 	// Is the graph directed?

  // Build the vertices
  for (auto it = vertices.begin(); it != vertices.end(); it++)
    this->vertices.push_back(make_shared<Vertex>(get<1>(*it), get<0>(*it)));

  for (auto it = edges.begin(); it != edges.end(); it++)
    this->edges.push_back(make_shared<Edge>(get<2>(*it),
					    Graph::getVertex(get<0>(*it)),
					    Graph::getVertex(get<1>(*it))));
  // Now make the adjacency list for the vertices
  for (auto it = this->vertices.begin(); it != this->vertices.end(); it++) {
    vector<shared_ptr<Edge>> ie;
    for (auto eit = this->edges.begin(); eit != this->edges.end(); eit++) {
      if ((*it)->getName() == (*eit)->second()->getName()
	  || (*it)->getName() == (*eit)->first()->getName())
	ie.push_back(*eit);
    }
    (*it)->setIEdges(ie);
  }
}

shared_ptr<Vertex> Edge::opposite(string v) {
  if (v == this->first()->getName())
    return this->second();
  else return this->first();
}

vector<shared_ptr<Vertex>> Vertex::neighbors() {
  vector<shared_ptr<Vertex>> ret;
  for (auto it = this->incidentEdges.begin(); it != this->incidentEdges.end(); it++) {
    ret.push_back((*it)->opposite(this->getName()));
  }
  return ret;
}

void print(Vertex* v) {
  cout << " name: " << v->getName() << endl;
}

// DFS for the graph with a function.
template<typename F>
static void doDFS(Graph& g, shared_ptr<Vertex> start, F fun) {
  stack<shared_ptr<Vertex>> s;
  s.push(start);

  do
    {
      // First get all the neighbors
      shared_ptr<Vertex> n = s.top();
      // Print the name of the node
      fun(n.get()); // XXX: Look this is a lambda (functor in C++ parlance)
      // from the argument.
      s.pop(); 			// Very bad stack interface of C++!
      n->setVisited(true);
      vector<shared_ptr<Vertex>> vns = n->neighbors();
      for (auto it = vns.begin(); it != vns.end(); it++) {
	if (!(*it)->getVisited()) {
	  s.push(*it);
	  (*it)->setVisited(true);
	}
      }
    } while (!s.empty());

  g.resetVisited();
}

// Topological sort in a directed graph
template<void (*fun)(Vertex*)>
static void topological_sort(const Graph& g) {
  if (!g.getDirected()) return;

  // First make the degree for the vertex.
  queue<shared_ptr<Vertex>> start_vertices;
  vector<shared_ptr<Vertex>> mvertices = g.getVertices();
  for (auto it = mvertices.begin(); it != mvertices.end(); ++it) {
    vector<shared_ptr<Edge>> ves = (*it)->getIEdges();
    for (auto eit = ves.begin(); eit != ves.end(); ++eit) {
      // Increment your degree if you are the second in the incident-edge
      if ((*eit)->second()->getName() == (*it)->getName())
	(*it)->degree += 1;
    }
    if ((*it)->degree == 0) start_vertices.push(g.getVertex((*it)->getName()));
  }
  // Now we can start going through and doing the actual topological sort.
  do
    {
      // Process vertices
      shared_ptr<Vertex> el = start_vertices.front();
      start_vertices.pop(); 	// C++'s horrible API!
      fun(el.get()); // XXX: Look this is a function pointer from the
      // template argument.

      // Get all its neighbors and if their degree is 0, then push
      // them onto the queue.
      vector<shared_ptr<Vertex>> eln = el->neighbors();
      for (auto it = eln.begin(); it != eln.end(); ++it) {
	(*it)->degree -= 1;
	if ((*it)->degree == 0) start_vertices.push(*it);
      }
    } while (!start_vertices.empty());

}

vector<shared_ptr<Vertex>> mst (Vertex* p, vector<shared_ptr<Vertex>> gvs) {
  vector<shared_ptr<Vertex>> ret;
  // XXX: Don't forget to remove the parent from the gvs
  auto it = gvs.begin();
  vector<decltype(it)> vits;
  for (; it != gvs.end(); ++it){
    if ((*it)->dist.second->getName() == p->getName()) {
      vits.push_back(it);
      ret.push_back(*it);
    }
  }

  // Now delete the indices from gvs.
  for (auto it = vits.begin(); it != vits.end(); ++it)
    gvs.erase(*it);

  return ret;
}

// XXX: shortest path and MST together
vector<Vertex*> sp_mst(const Graph& g, shared_ptr<Vertex> start) {
  // We need to use a priority queue.
  start->dist.first = 0;

  // Declare a pq with a comparator.
  auto comp = [](const shared_ptr<Vertex> e1,
		 const shared_ptr<Vertex> e2){
    return e1->dist.first > e2->dist.first;
  };
  priority_queue<shared_ptr<Vertex>,
		 vector<shared_ptr<Vertex>>,
		 decltype(comp)> pq(comp);
  pq.push(start);

  do
    {
      // First get all the neighbors
      shared_ptr<Vertex> me = pq.top();
      vector<shared_ptr<Edge>> ie = me->getIEdges();
      pq.pop(); 		// remove the top from the pq.
      // Expand the distance for each neighbor.
      for(shared_ptr<Edge> x : ie) {
	shared_ptr<Vertex> men = x->opposite(me->getName());
	unsigned xw = x->getWeight();
	if (!men->getVisited() && men->dist.first > me->dist.first + xw) {
	  men->dist.first = me->dist.first+xw;
	  men->dist.second = me;
	  pq.push(men); 	// FIXME: can add same vertex again!
	}
      }
      me->setVisited(true);
    } while (!pq.empty());

  // Build the minimum spanning tree
  vector<shared_ptr<Vertex>> gvs = g.getVertices();
  vector<Vertex*> q;
  q.push_back (start.get());

  // remove start from gvs.
  auto it = gvs.begin();
  for ( ; it != gvs.end(); ++it)
    if ((*it) == start) break;
  // remove it from gvs.
  gvs.erase(it);

  unsigned counter = 0;
  for (; counter < q.size(); ++counter) {
    auto nn = mst(q[counter], gvs);
    for (auto x: nn)
      q.push_back(x.get());
  }
  return q;
}

// XXX: Independent sets (dual of cliques).
vector<Vertex*> independent_sets (const Graph& g){
  vector<Vertex*> ret {};
  vector<shared_ptr<Vertex>> gvs = g.getVertices();
  unsigned pcount = 0;
  vector<Vertex*> fe;
  for (unsigned i = (2 << (gvs.size() - 1))-1; i >=1 ; --i) {
    bitset<N> bb(i);  		// FIXME: This is very annoying in C++
    // Now just get the length of bits that we need.
    bool possibly_return = true;
    unsigned ccount = 0;
    for (int j = gvs.size()-1; j >= 0; --j)
      if (bb[j]){
	++ccount;
	fe.push_back(gvs[j].get());
      }
    for(unsigned j=0; j < fe.size(); ++j) {
      // Now get the edge's opposite and check. 
      vector<shared_ptr<Vertex>> mns = fe[j]->neighbors();
      for (auto it = mns.begin(); it != mns.end(); ++it) {
	if (find(fe.begin(), fe.end(), (*it).get()) != fe.end()) {
	  possibly_return = false;
	  goto L;
	}
      }
    }
  L: if(possibly_return && ccount > pcount) {
      ret = fe;
      pcount = ccount;
    }
    else
      fe.clear();
  }
  return ret;
}

int main(void)
{
  // Test program.
  vector<tuple<string, int>> vertices = {make_tuple("a", 1), make_tuple("b", 2),
					 make_tuple("c", 3), make_tuple("d", 4)};
  vector<tuple<string, string, int>> edges = {make_tuple("a", "b", 5), make_tuple("a", "c", 1),
					      make_tuple("a", "d", 4), make_tuple("c", "d", 2),
					      make_tuple("d", "b", 3)};

  // This is the graph.
  Graph g(vertices, edges, false);
  vector<shared_ptr<Vertex>> mv = g.getVertices();

  // Print the graph using dfs and a std::copy_backward(std::begin(container), std::end(container), std::end(container));

  cout  << "DFS" << endl;
  // Here lambda is most likely also inlined.
  doDFS(g, g.getVertex("a"), [](Vertex* x) -> void {cout << x->getName() << endl;});

  // make the graph directed.
  g.setDirected(true);

  // Do topological sort. 
  cout  << "Topological sort" << endl;
  // Here print should be inlined -- passing print as a function pointer
  // at compile time as a template argument.
  topological_sort<print>(g);


  // Dijkstra's shortest path and minimum spanning tree together
  vector<Vertex*> tree = sp_mst(g, g.getVertex("a"));
  cout << "Done mst" << "\n";

  // Print the tree
  for (auto x : tree)
    cout << x->getName() << " ";
  cout << "\n";


  // Independent sets
  vector<Vertex*> s = independent_sets(g);
  for (auto x: s)
    cout << x->getName() << " ";
  cout << "\n";

  return 0;
}
