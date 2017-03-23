#include "graph.hpp"
#include <stack>
#include <queue>

using namespace std;

shared_ptr<Vertex> Graph::getVertex(std::string name) {
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

void print(shared_ptr<Vertex> v) {
  cout << " name: " << v->getName() << endl;
}

// DFS for the graph with a function.
template<typename F>
static void doDFS(Graph g, shared_ptr<Vertex> start, F fun) {
  stack<shared_ptr<Vertex>> s;
  s.push(start);

  do
    {
      // First get all the neighbors
      shared_ptr<Vertex> n = s.top();
      // Print the name of the node
      fun(n); // XXX: Look this is a lambda (functor in C++ parlance)
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
template<void (*fun)(shared_ptr<Vertex>)>
static void topological_sort(Graph g) {
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
      fun(el); // XXX: Look this is a function pointer from the
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

// TODO: shortest path and MST together

// TODO: Independent sets (opposite of cliques).

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
  // Here lambda is most likely not inlined.
  doDFS(g, g.getVertex("a"), [](shared_ptr<Vertex> x) -> void {cout << x->getName() << endl;});

  // make the graph directed.
  g.setDirected(true);

  // Do topological sort. 
  cout  << "Topological sort" << endl;
  // Here print should be inlined -- passing print as a function pointer
  // at compile time as a template argument.
  topological_sort<print>(g);

  return 0;
}


