#!/usr/bin/env python


def mc_clique(graph):
    # This is the dictionary to look up the vertex from an index
    ret = []
    vd = {i: v for i, v in enumerate(graph.vertices())}
    for i in range(2**len(list(graph.vertices()))-1, 2, -1):
        bnum = list('{0:04b}'.format(i))
        if len([x for x in bnum if x > '0']) >= 2:
            # Now collect the vertices that you need
            to_check = [vd[i] for i, v in enumerate(bnum) if v == '1']
            # Now check
            rr = []
            for i, v in enumerate(to_check):
                if v != to_check[-1]:
                    vn = [graph.opposite(v, e)
                          for e in graph.incidentEdges(v)]
                    rr += [all([x in vn for x in to_check[i+1:]])]
            if all(rr):
                ret += [x. name for x in to_check]
                break

    return ret


def main():
    # Example of an Undirected Graph
    vs = [('a', 1), ('b', 2), ('c', 3), ('d', 4)]
    es = [('a', 'b', 1), ('a', 'c', 1), ('a', 'd', 1),
          ('c', 'd', 2), ('d', 'b', 1)]
    # es += [('c', 'b', 100)]
    g = G(vs, es)
    return(mc_clique(g))


if __name__ == '__main__':
    import sys
    sys.path.append('../Graph')
    from Graph import Graph as G
    print(main())
