#ifndef GROUNDER_UTILS_SCC_H
#define GROUNDER_UTILS_SCC_H

#include <algorithm>
#include <cassert>
#include <iostream>
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
    std::unordered_set<Component, boost::hash<std::vector<int>>> scc;

    std::vector<int> index;
    std::vector<int> low_link;
    std::vector<bool> on_stack;
    std::stack<int> s;

    int idx;

    void connect(int v, const DirectedGraph &g) {
        index[v] = idx;
        low_link[v] = idx;
        idx++;
        s.push(v);
        on_stack[v] = true;

        for (const Edge &e : g.get_edges_from_node(v)) {
            int w = e.to;
            if (index[w] == UNDEFINED) {
                connect(w, g);
                low_link[v] = std::min(low_link[v], low_link[w]);
            }
            else if (on_stack[w] == true) {
                low_link[v] = std::min(low_link[v], index[w]);
            }
        }

        if (low_link[v] == index[v]) {
            //std::cerr << "Connected component #" << scc.size() + 1 << ": ";
            Component c;
            int w;
            do {
                w = s.top();
                s.pop();
                on_stack[w] = false;
                c.push_back(w);
                //std::cerr << w << ", ";
            }  while (w != v);
            scc.insert(c);
            //std::cerr << std::endl;
        }
    }

    void clear_up() {
        index.clear();
        low_link.clear();
        on_stack.clear();

        index.shrink_to_fit();
        low_link.shrink_to_fit();
        on_stack.shrink_to_fit();
    }

public:
    SCC() = default;

    SCC(const DirectedGraph &g) {
        int num_nodes = g.get_max_index() + 1;
        index.resize(num_nodes, UNDEFINED);
        low_link.resize(num_nodes, UNDEFINED);
        on_stack.resize(num_nodes, false);
        idx = 0;
        for (auto &v : g.get_nodes())
            if (index[v] == UNDEFINED)
                connect(v, g);

        clear_up();
    }

};

#endif //GROUNDER_UTILS_SCC_H
