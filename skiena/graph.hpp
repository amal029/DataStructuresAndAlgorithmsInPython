#include <iostream>
#include <vector>
#include <tuple>
#include <memory>
#include <algorithm>
#include <climits>

class Edge;

class Vertex
{
public:
  Vertex(int weight, std::string name) : weight(weight), name(name) {}
  virtual ~Vertex(){std::cout << "deleting vertex: " << name << std::endl;}
  std::string getName() const {return this->name;}
  inline std::vector<std::shared_ptr<Edge>>getIEdges() const {return incidentEdges;}
  inline void setIEdges(std::vector<std::shared_ptr<Edge>> e) {incidentEdges  = e;}
  std::vector<std::shared_ptr<Vertex>> neighbors();
  inline bool getVisited() const {return visited;}
  inline void setVisited(bool v) {visited = v;}
  unsigned degree = 0;
  std::pair<unsigned, std::shared_ptr<Vertex>> dist {INT_MAX, nullptr};
  
protected:
  int weight;
  std::string name;
  std::vector<std::shared_ptr<Edge>> incidentEdges;
  bool visited;
};


class Edge
{
public:
  Edge(int weight, std::shared_ptr<Vertex> s, std::shared_ptr<Vertex> e) : weight(weight) {
    std::get<0>(this->vertices) = s;
    std::get<1>(this->vertices) = e;
  }
  inline std::shared_ptr<Vertex> first() const {
    return std::get<0>(this->vertices).lock();
  }
  inline std::shared_ptr<Vertex> second() const {
    return std::get<1>(this->vertices).lock();
  }
  std::shared_ptr<Vertex> opposite(std::string) ;
  virtual ~Edge(){std::cout << "deleting edge!" << std::endl;}

  inline unsigned getWeight() const {return weight;}

protected:
  std::tuple<std::weak_ptr<Vertex>, std::weak_ptr<Vertex>> vertices;
  unsigned weight;
};


class Graph
{
public:
  Graph(std::vector<std::tuple<std::string, int>>,
	std::vector<std::tuple<std::string, std::string, int>>,
	bool directed);
  std::shared_ptr<Vertex> getVertex(std::string) const;
  inline bool getDirected () const {return directed;}
  inline void setDirected (bool value) {directed = value;}
  inline std::vector<std::shared_ptr<Vertex>> getVertices() const {return this->vertices;}
  inline std::vector<std::shared_ptr<Edge>> getEdges() const {return this->edges;}
  inline void resetVisited() {
    // Reset all the visited bools.
    std::for_each(std::begin(vertices), std::end(vertices),
		  [](std::shared_ptr<Vertex> x) {x->setVisited(false);});

  }
  virtual ~Graph(){std::cout << "Deleting graph" << std::endl;}

private:
  bool directed;
  std::vector<std::shared_ptr<Vertex>> vertices;
  std::vector<std::shared_ptr<Edge>> edges;
};
