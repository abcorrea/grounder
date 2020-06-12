#include "scc.h"

using namespace std;

SCC::SCC(const DirectedGraph &g) {
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

void SCC::connect(int v, const DirectedGraph &g) {
    index[v] = idx;
    low_link[v] = idx;
    idx++;
    s.push(v);
    on_stack[v] = true;

    for (const Edge &e : g.get_edges_from_node(v)) {
        int w = e.to;
        if (index[w] == UNDEFINED) {
            connect(w, g);
            low_link[v] = min(low_link[v], low_link[w]);
        }
        else if (on_stack[w] == true) {
            low_link[v] = min(low_link[v], index[w]);
        }
    }

    if (low_link[v] == index[v]) {
        cerr << "Connected component #" << scc.size() + 1 << ": ";
        Component c;
        int w;
        do {
            w = s.top();
            s.pop();
            on_stack[w] = false;
            c.push_back(w);
            cerr << w << ", ";
        }  while (w != v);
        scc.insert(c);
        cerr << endl;
    }
}

void SCC::clear_up() {
    index.clear();
    low_link.clear();
    on_stack.clear();

    index.shrink_to_fit();
    low_link.shrink_to_fit();
    on_stack.shrink_to_fit();
}
