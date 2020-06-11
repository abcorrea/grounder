#ifndef GROUNDER_UTILS_SCC_H
#define GROUNDER_UTILS_SCC_H

#include <cassert>
#include <unordered_set>
#include <vector>

#include <boost/functional/hash.hpp>

using Component = std::vector<int>;

class SCC {
    std::unordered_set<Component, boost::hash<std::vector<int>>> scc;

public:
    SCC() = default;

};


class DirectedGraph {
    std::unordered_set<int> nodes;
    std::vector<std::pair<int,int>> edges;

public:
    DirectedGraph() = default;

    void add_node(int i) {
        nodes.insert(i);
    }

    void add_edge(int i, int j) {
        assert((nodes.find(i) != nodes.end()) and (nodes.find(j) != nodes.end()));
        edges.emplace_back(i, j);
    }

};

#endif //GROUNDER_UTILS_SCC_H
