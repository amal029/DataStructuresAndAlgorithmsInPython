#!/usr/bin/env python
import sys
sys.path.append('../Graph')
import Graph as G


def minVertexDFS(availColors, source, graph):
    if source.color is None:
        nsc = [graph.opposite(source, e).color
               for e in graph.incidentEdges(source)]
        nsc = [n for n in nsc if n is not None]
        mc = [x for x in availColors if x not in nsc]
        if mc == []:
            # Add a new color to availColors
            availColors += ['C'+str(len(availColors))]
        source.color = availColors[-1]
    else:
        return
    for n in [graph.opposite(source, e)
              for e in graph.incidentEdges(source)]:
        minVertexDFS(availColors, n, graph)


def main(s, graph):
    availColors = []
    for v in graph.vertices():
        v.color = None
    minVertexDFS(availColors, s, graph)
    return len(availColors)


if __name__ == '__main__':
    # Example of an Undirected Graph
    vs = [('a', 1), ('b', 2), ('c', 3), ('d', 4)]
    es = [('a', 'b', 1), ('a', 'c', 1), ('a', 'd', 1),
          ('c', 'd', 2), ('d', 'b', 1)]
    graph = G.Graph(vs, es)
    print(main(graph.vdict['a'], graph))

    es += [('c', 'b', 4)]
    graph = G.Graph(vs, es)
    print(main(graph.vdict['a'], graph))
