#!/usr/bin/env python


def clique(graph):
    ret = []
    v = {v: len(list(graph.incidentEdges(v))) for v in graph.vertices()}
    sorted_vertices = sorted(v, key=v.__getitem__)
    for v in sorted_vertices:
        # Get the neighbors
        neighbors = [graph.opposite(v, e)
                     for e in graph.incidentEdges(v)]
        # Now check
        rr = [v.name]
        for i, n in enumerate(neighbors):
            nn = [graph.opposite(n, e) for e in graph.incidentEdges(n)]
            if n != neighbors[-1]:
                if all([k in nn for k in neighbors[i+1:]]):
                    rr += [n.name]
            else:
                rr += [n.name]
        ret += [rr]
    return ret


def main():
    # Example of an Undirected Graph
    vs = [('a', 1), ('b', 2), ('c', 3), ('d', 4)]
    es = [('a', 'b', 1), ('a', 'c', 1), ('a', 'd', 1),
          ('c', 'd', 2), ('d', 'b', 1)]
    es += [('c', 'b', 100)]
    g = G(vs, es)
    return(clique(g))


if __name__ == '__main__':
    import sys
    sys.path.append('../Graph')
    from Graph import Graph as G
    print(main())
