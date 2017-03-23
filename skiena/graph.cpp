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

  vector<shared_ptr<Edge>> gedges;
  for (auto it = edges.begin(); it != edges.end(); it++)
    this->edges.push_back(make_shared<Edge>(get<2>(*it),
				       Graph::getVertex(get<0>(*it)),
				       Graph::getVertex(get<1>(*it))));
  // Now make the alist for the vertices
  for (auto it = this->vertices.begin(); it != this->vertices.end(); it++) {
    vector<shared_ptr<Edge>> ie;
    for (auto eit = this->edges.begin(); eit != this->edges.end(); eit++) {
      if ((*it)->getName() == (*eit)->second()->getName()
	  || (*it)->getName() == (*eit)->first()->getName())
	ie.push_back(*eit);
	// (*it)->getIEdges().push_back(*eit);
    }
    (*it)->setIEdges(ie);
    // cout << "name: " << (*it)->getName() << " " << (*it)->getIEdges().size() << endl;
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

void print(string name) {
  cout << " name: " << name << endl;
}

// DFS for the graph with a function.
template <typename... A>
static void doDFS(Graph g, shared_ptr<Vertex> start, void (*fun)(A...)) {
  stack<shared_ptr<Vertex>> s;
  s.push(start);

  do
    {
      // First get all the neighbors
      shared_ptr<Vertex> n = s.top();
      // Print the name of the node
      fun(n->getName());
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
template<typename... A>
static void topological_sort(Graph g, void (*fun) (A...)) {
  // First make the degree for the vertex.
  queue<shared_ptr<Vertex>> start_vertices;
  vector<shared_ptr<Vertex>> mvertices = g.getVertices();
  for (auto it = mvertices.begin(); it != mvertices.end(); ++it) {
    vector<shared_ptr<Edge>> ves = (*it)->getIEdges();
    for (auto eit = ves.begin(); eit != ves.end(); ++eit) {
      // Increment your degree if you are the second in the incidentedge
      if ((*eit)->second()->getName() == (*it)->getName())
	(*it)->degree += 1;
    }
    if ((*it)->degree == 0) start_vertices.push(g.getVertex((*it)->getName()));
  }
  do
    {
      // Process vertices
      shared_ptr<Vertex> el = start_vertices.front();
      start_vertices.pop(); 	// C++'s horrible API!
      fun(el->getName());
      // We can call the function on the damn element.


      // Get all its neighbors and if their degree is 0, then push
      // them onto the queue.
      vector<shared_ptr<Vertex>> eln = el->neighbors();
      for (auto it = eln.begin(); it != eln.end(); ++it) {
	(*it)->degree -= 1;
	if ((*it)->degree == 0) start_vertices.push(*it);
      }
    } while (!start_vertices.empty());

  // Now we can start going through and doing the actual topological sort.
}

// TODO: shortest path and MST together

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

  // // Just checking the ref-count.
  // for (auto it = mv.begin(); it != mv.end(); ++it) {
  //   cout << (*it).use_count() << endl;
  // }
  // Print the graph using dfs and a lambda
  cout  << "DFS" << endl;
  doDFS<string>(g, g.getVertex("a"), &print);

  // Do topological sort, if it works.
  cout  << "Topological sort" << endl;
  topological_sort(g, print);


  // // Just checking the ref-count.
  // for (auto it = mv.begin(); it != mv.end(); ++it) {
  //   cout << (*it).use_count() << endl;
  // }

  return 0;
}


