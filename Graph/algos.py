import Graph as G
import sys
sys.path.append('../Simple')
from SQ import Stack, Queue
# Set the path to be searched


def search(sq, graph=None):
    while not sq.isEmpty():
        v = sq.pop()
        print('visiting: ', v.name)
        v.visited = True
        # Get all its neighbors
        ns = [graph.opposite(v, e)
              for e in graph.incidentEdges(v)]
        ns = [v for v in ns if v not in sq and not v.visited]
        [sq.push(n) for n in ns]


def dijkstra(graph, v):
    """The shortest path algorithm on undirected graph

    """
    # Pre processing
    for vs in graph.vertices():
        vs.dw = None            # The initial weights

    v.dw = 0                    # The initial weight of the start vertex

    ret = []                    # The returned queue

    # This will be used for processing
    pq = Queue()
    pq.push((v, None))   # Add the first one: (vertex, in-edge)

    while not pq.isEmpty():
        (v, ie) = pq.pop()
        ret += [(v, ie)]

        ns = [(graph.opposite(v, e), e)
              for e in graph.incidentEdges(v)]
        ns = [(n, e) for (n, e) in ns if n not in [h for (h, k) in ret]]

        # Now do edge relaxation for the neighbors
        for (n, e) in ns:
            w = v.dw + int(e.label)
            if n.dw is None:
                n.dw = w
            elif w < n.dw:
                n.dw = w

        ns = [(n, w) for (n, w) in ns if n not in [h for (h, t) in pq]]

        # Add it to the pq
        [pq.push((n, nie)) for (n, nie) in ns]

        # sort the the ns according to their dws
        def vcmp(x):
            (v, e) = x
            return v.dw

        pq.sort(vcmp)

    return ret


def topological_sort(g):
    # First do some pre-processing
    if not g.directed:
        return []

    # Initialize the in-degree attribute
    for v in g.vertices():
        v.ind = 0

    # Now make the in-degree attr correct
    # O(n*n)!!
    dests = [e.dest for e in g.edges()]
    for v in g.vertices():
        for d in dests:
            if v.name == d.name:
                v.ind += 1

    s = Stack()
    [s.push(v) for v in g.vertices() if v.ind == 0]

    ret = []
    while not s.isEmpty():
        v = s.pop()
        ret += [v.name]
        ns = [g.opposite(v, e) for e in g.incidentEdges(v)]
        for n in ns:
            n.ind -= 1
            if n.ind == 0:
                s.push(n)

    return ret


if __name__ == '__main__':
    # Example of an Undirected Graph
    vs = [('a', 1), ('b', 2), ('c', 3), ('d', 4),
          ('e', 5), ('f', 6), ('g', 5)]
    es = [('a', 'b', 1), ('b', 'f', 1), ('a', 'f', 1),
          ('a', 'e', 2), ('e', 'f', 1), ('b', 'c', 2),
          ('c', 'g', 2), ('g', 'd', 3), ('c', 'd', 1)]
    graph = G.Graph(vs, es)
    s = Stack()
    s.push(graph.vdict['a'])

    # This is DFS
    print('DFS')
    search(s, graph)

    # This is BFS
    q = Queue()
    for v in graph.vertices():
        v.visited = False

    q.push(graph.vdict['a'])
    print('BFS')
    search(q, graph)

    # Dijkstra and Prim's MST
    spanning_tree = dijkstra(graph, graph.vdict['a'])
    print('Prim\'s MST')
    for (v, e) in spanning_tree:
        print(v.name, ',', v.dw)

    # Now do the topological sort for a directed graph
    es1 = [('a', 'b', 1), ('f', 'b', 1), ('a', 'f', 1),
           ('a', 'e', 2), ('e', 'f', 1), ('b', 'c', 2),
           ('c', 'g', 2), ('g', 'd', 3), ('c', 'd', 1)]

    digraph = G.Graph(vs, es1, True)
    tslist = topological_sort(digraph)
    print(tslist)
