#include "Graph.h"
#include <stack>

using namespace std;

shared_ptr<Vertex> Graph::getVertex(std::string name) {
  auto it = this->vertices.begin();
  for (; it != this->vertices.end(); it++) {
    if (it->getName() == name) break;
  }
  return (it != this->vertices.end()) ? make_shared<Vertex>(*it) : NULL;

}

Graph::Graph(vector<tuple<string, int>> vertices,
	     vector<tuple<string, string, int>> edges,
	     bool directed=false) {

  this->directed = directed; 	// Is the graph directed?

  // Build the vertices
  for (auto it = vertices.begin(); it != vertices.end(); it++)
    this->vertices.push_back(Vertex(get<1>(*it), get<0>(*it)));

  vector<Edge> gedges;
  for (auto it = edges.begin(); it != edges.end(); it++)
    gedges.push_back(Edge(get<2>(*it),
			  Graph::getVertex(get<0>(*it)),
			  Graph::getVertex(get<1>(*it))));
  this->edges = gedges;

  // Now make the alist for the vertices
  for (auto it = this->vertices.begin(); it != this->vertices.end(); it++) {
    vector<shared_ptr<Edge>> iEdges;
    for (auto eit = this->edges.begin(); eit != this->edges.end(); eit++) {
      if (it->getName() == eit->second()->getName()
	  || it->getName() == eit->first()->getName())
	iEdges.push_back(make_shared<Edge>(*eit));	
    }
    it->setIEdges(iEdges);
  }
}

shared_ptr<Vertex> Edge::opposite(Vertex* v) {
  if (v->getName() == this->first()->getName())
    return this->second();
  else return this->first();
}

vector<shared_ptr<Vertex>> Vertex::neighbors() {
  vector<shared_ptr<Vertex>> ret;
  for (auto it = this->incidentEdges.begin(); it != this->incidentEdges.end(); it++) {
    ret.push_back((*it)->opposite(this));
  }
  return ret;
}

void print(string name) {
  cout << name << endl;
}

// DFS for the graph with a function.
template <typename T, typename... A>
static void doDFS(Graph g, shared_ptr<Vertex> start, T (*fun)(A... args)) {
  stack<shared_ptr<Vertex>> s;
  s.push(start);

  do
    {
      // First get all the neighbors
      shared_ptr<Vertex> n = s.top();
      // Print the name of the node
      fun(n->getName());
      s.pop(); 			// Very bad stack interface of C++!
      vector<shared_ptr<Vertex>> ns = n->neighbors();
      for (auto it = ns.begin(); it != ns.end(); it++) {
	if (!(*it)->getVisited()) {
	  s.push(*it);
	  (*it)->setVisited(true);
	}
      }
    } while (!s.empty());

  g.resetVisited();
}

// TODO: Topological sort in a directed graph

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

  // Print the graph using dfs and a lambda
  doDFS<void, string>(g, g.getVertex("a"), &print);

  return 0;
}


