#ifndef GROUNDER_GROUNDERS_NEW_GROUNDER_H_
#define GROUNDER_GROUNDERS_NEW_GROUNDER_H_

#include "fast_downward_grounder.h"

#include "../utils/scc.h"

#include <deque>
#include <map>
#include <unordered_map>
#include <vector>

class NewGrounder : public FastDownwardGrounder  {
    typedef std::deque<int> Bucket;

    std::vector<std::vector<int>> rule_delete_component;
    std::unordered_map<int, int> map_pred_to_scc;

    std::map<int, Bucket> q;
    int q_size;

    void dump_sccs(const LogicProgram &lp, const SCC &scc) const;

    void get_useful_rules_per_component(const LogicProgram &lp, const SCC &sccs);

    void do_insertion(int key, int fact_index) {
        q[key].push_back(fact_index);
        ++q_size;
    }

    std::pair<int, int> remove_min() {
        assert(q_size > 0);
        auto it = q.begin();
        int key = it->first;
        Bucket &bucket = it->second;
        assert(!bucket.empty());
        int fact_index = bucket.front();
        bucket.pop_front();
        if (bucket.empty())
            q.erase(it);
        --q_size;
        return std::make_pair(key, fact_index);
    }

public:
    NewGrounder() : q_size(0) {};

    int ground(LogicProgram &lp) override;
};

#endif //GROUNDER_GROUNDERS_NEW_GROUNDER_H_
