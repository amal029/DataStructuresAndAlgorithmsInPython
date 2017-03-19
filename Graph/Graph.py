class Graph:
    def __init__(self, vlist=[], elist=[], direct=False):
        """This is the main graph class

        """
        self.directed = direct
        self.vdict = {n: Vertex(n, l,
                                self.getNeighbors(n, elist, self.directed))
                      for (n, l) in vlist}
        self.edict = {(s, d): Edge(self.vdict[s], self.vdict[d], w)
                      for (s, d, w) in elist}

    @staticmethod
    def getNeighbors(v, elist, directed):
        if not directed:
            ret = [(s, d) for (s, d, w) in elist if (s == v or d == v)]
        else:
            ret = [(s, d) for (s, d, w) in elist if s == v]
        return ret

    def vertices(self):
        return iter(self.vdict.values())

    def edges(self):
        return iter(self.edict.values())

    def incidentEdges(self, v):
        return iter([self.edict[(s, d)]
                     for (s, d) in self.vdict[v.name].neighbors])

    def __opposites(self, v):
        """This method gives all the neighbour vertices of the vertex v.

        """
        return [self.vdict[d] if d != v.name else self.vdict[s]
                for (s, d) in self.vdict[v.name].neighbors]

    def opposite(self, v, e):
        sd = [e.source.name, e.dest.name]
        if v.name in sd:
            return [self.vdict[x] for x in sd if x != v.name][0]
        else:
            raise RuntimeError('Edge: ', e.name,
                               'is not incident on vertex: ', v.name)

    def endVertices(self, e):
        return [e.source, e.dest]

    def areAdjacent(self, v, w):
        return (w in self.__opposites(v))


class Vertex:
    """This class represents the vertice of the graph.

    """
    def __init__(self, name, label=None, neighbors=[]):
        self.visited = False
        self.label = label
        self.neighbors = neighbors
        self.name = name


class Edge:
    """This class represents the edges of the graph
    """
    def __init__(self, source, dest, label=None):
        self.visited = False
        self.label = label
        self.source = source
        self.dest = dest
