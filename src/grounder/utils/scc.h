#ifndef GROUNDER_UTILS_SCC_H
#define GROUNDER_UTILS_SCC_H

#include <algorithm>
#include <cassert>
#include <stack>
#include <unordered_map>
#include <unordered_set>
#include <vector>

#include <boost/functional/hash.hpp>

const int UNDEFINED = -1;

using Component = std::vector<int>;

struct Edge {
    Edge(int u, int v, int l) : from(u), to(v), label(l) {}
    int from;
    int to;
    int label;
};

class AdjacencyList {
    std::unordered_map<int, std::vector<Edge>> list;

    std::vector<Edge> empty_vector;
public:
    AdjacencyList() = default;

    void emplace_back(int i, int j, int l) {
        Edge e(i, j, l);
        auto inserted = list.insert({i, {e}});
        if (!inserted.second) {
            list[i].push_back(e);
        }
    }

    const std::vector<Edge> &operator[](int idx) const {
        auto has_idx = list.find(idx);
        if (has_idx == list.end())
            return empty_vector;
        return list.at(idx);
    }
};

class DirectedGraph {
    std::unordered_set<int> nodes;
    AdjacencyList adj_list;
    int maximum_node_index;
public:
    DirectedGraph() {
        maximum_node_index = 0;
    };

    void add_node(int i) {
        nodes.insert(i);
        maximum_node_index = std::max(maximum_node_index, i);
    }

    void add_edge(int i, int j, int label) {
        assert((nodes.find(i) != nodes.end()) and (nodes.find(j) != nodes.end()));
        adj_list.emplace_back(i, j, label);
    }

    const std::unordered_set<int> &get_nodes() const {
        return nodes;
    }

    const std::vector<Edge> get_edges_from_node(int u) const {
        return adj_list[u];
    }

    int get_max_index() const {
        return maximum_node_index;
    }
};


class SCC {
    std::vector<Component> sccs;

    std::vector<int> index;
    std::vector<int> low_link;
    std::vector<bool> on_stack;
    std::stack<int> s;

    int idx;

    void connect(int v, const DirectedGraph &g);
    void insert_new_component(int v);
    void clear_up();

public:
    SCC() = default;

    SCC(const DirectedGraph &g);

    const std::vector<Component> &get_components() const {
        return sccs;
    }

    const Component &get_component_by_index(size_t i) const {
        assert(i < sccs.size());
        return sccs[i];
    }

    size_t size() const {
        return sccs.size();
    }

};

#endif //GROUNDER_UTILS_SCC_H
