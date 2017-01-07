###############################################################################
#
# Implement Dijkstra search algorithm
#
# input undirect graph file (json format) is like
# {
# "45": 10, "12": 8, "67": 1, "68": 6,
# "78":  7, "25": 4, "01": 4, "07": 8,
# "17": 11, "23": 7, "56": 2, "28": 2,
# "35": 14, "34": 9
# }
# 1. Each key is a string, just two characters, stands for a path between two vertices
# 2. Each value is an integer, stands for the length of the path.
#
###############################################################################
import argparse
import json
import sys

infinite = 99999

# demo for dump a undirect graph to a file
def store_graph():
    G = {
            "AB" : 6,
            "AC" : 3,
            "BC" : 2,
            "BD" : 5,
            "CD" : 3,
            "CE" : 4,
            "DE" : 2,
            "DF" : 3,
            "EF" : 5
            }
    with open(u'undirect_graph.json', 'w') as f:
        json.dump(G, f)

def dijkstra_search(G, closed_dict):
    open_list = {}

    while len(closed_dict):
        (curr_key, curr_value) = min(closed_dict.items(), key = lambda x: x[1][0])
        closed_dict.pop(curr_key)
        print "insert ", curr_key, curr_value
        open_list[curr_key] = curr_value
        for (k,v) in closed_dict.items():
            for key_in_graph in (k+curr_key, curr_key+k):
                if G.has_key(key_in_graph):
                    if closed_dict[k][0] > (curr_value[0] + G[key_in_graph]):
                        closed_dict[k] = [curr_value[0] + G[key_in_graph], ] + curr_value[1:] + [k,]
    for (k,v) in open_list.items():
        print k, " : ", v

def parse_args():
    parser = argparse.ArgumentParser(description="""Dijkstra shortest path algorithm.
input a json file that describes a undirect graph and the source vertex""")
    parser.add_argument(u'input_file', metavar='"file name"', type=str, help=u'input json file for graph')
    parser.add_argument(u'source_vertex', metavar='"source vertex"',type=str, help=u'should be a vertex in the graph above')

    arg = parser.parse_known_args(sys.argv[1:])
    l = parser.parse_args()
    return (l.input_file, l.source_vertex)

def init(filename, source_vertex):
    print "input file: ", filename
    print "source vertext: ", source_vertex
    with open(filename, 'r') as f:
        G = json.load(f)

    closed_dict = {}
    for k in G.keys():
        assert len(k) == 2
        for vertex in k:
            if vertex not in closed_dict.keys():
                closed_dict[vertex] = [infinite,]
    if source_vertex in closed_dict.keys():
        closed_dict[source_vertex] = [0, source_vertex]
        print closed_dict
        return (G, closed_dict)
    else:
        print "source vertex %s is NOT in graph!" % source_vertex
        sys.exit(1)

def main():
    (filename, source_vertex) = parse_args()
    (graph, closed_dict) = init(filename, source_vertex)
    dijkstra_search(graph, closed_dict)

if __name__ == u'__main__':
    main()

