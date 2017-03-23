#include <iostream>
#include <vector>
#include <tuple>

class Edge;

class Vertex
{
public:
  Vertex(int weight, std::string name) : weight(weight), name(name) {}
  virtual ~Vertex(){std::cout << "deleting vertex: " << name << std::endl;}
  std::string getName() const {return this->name;}
  inline std::vector<std::shared_ptr<Edge>>getIEdges() const {return this->incidentEdges;}
  inline void setIEdges(std::vector<std::shared_ptr<Edge>> e) {this->incidentEdges  = e;}
  std::vector<std::shared_ptr<Vertex>> neighbors();
  inline bool getVisited() const {return visited;}
  inline void setVisited(bool v) {visited = v;}
  unsigned degree = 0;
  
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
    return std::get<0>(this->vertices);
  }
  inline std::shared_ptr<Vertex> second() const {
    return std::get<1>(this->vertices);
  }
  std::shared_ptr<Vertex> opposite(std::string) ;
  virtual ~Edge(){std::cout << "deleting edge!" << std::endl;}

protected:
  std::tuple<std::shared_ptr<Vertex>, std::shared_ptr<Vertex>> vertices;
  int weight;
};


class Graph
{
public:
  Graph(std::vector<std::tuple<std::string, int>>,
	std::vector<std::tuple<std::string, std::string, int>>,
	bool directed);
  std::shared_ptr<Vertex> getVertex(std::string);
  inline std::vector<std::shared_ptr<Vertex>> getVertices() const {return this->vertices;}
  inline std::vector<std::shared_ptr<Edge>> getEdges() const {return this->edges;}
  inline void resetVisited() {
    // Reset all the visited bools.
    for (auto it = vertices.begin(); it != vertices.end(); it++)
      (*it)->setVisited(false);
  }
  virtual ~Graph(){}

private:
  bool directed;
  std::vector<std::shared_ptr<Vertex>> vertices;
  std::vector<std::shared_ptr<Edge>> edges;
};
