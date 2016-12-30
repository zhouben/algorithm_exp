#
# Implement Dijkstra search algorithm
#
infinite = 999
G = {
        frozenset({"A", "B"}) : 6,
        frozenset({"A", "C"}) : 3,
        frozenset({"B", "C"}) : 2,
        frozenset({"B", "D"}) : 5,
        frozenset({"C", "D"}) : 3,
        frozenset({"C", "E"}) : 4,
        frozenset({"D", "E"}) : 2,
        frozenset({"D", "F"}) : 3,
        frozenset({"E", "F"}) : 5
        }

open_list = {}
closed_dict = {"A":[0, "A"], "B":[infinite,] , "C":[infinite,], "D":[infinite,], "E":[infinite,], "F":[infinite,]}

while len(closed_dict):
    (curr_key, curr_value) = min(closed_dict.items(), key = lambda x: x[1][0])
    closed_dict.pop(curr_key)
    print "insert ", curr_key, curr_value
    open_list[curr_key] = curr_value
    for (k,v) in closed_dict.items():
        key_in_G = frozenset({k, curr_key})
        if G.has_key(key_in_G):
            if closed_dict[k][0] > (curr_value[0] + G[key_in_G]):
                closed_dict[k] = [curr_value[0] + G[key_in_G], ] + curr_value[1:] + [k,]
print open_list
