#ifndef GROUNDER_GROUNDERS_NEW_GROUNDER_H_
#define GROUNDER_GROUNDERS_NEW_GROUNDER_H_

#include "fast_downward_grounder.h"

#include "../utils/scc.h"

#include <unordered_map>
#include <vector>

class NewGrounder : public FastDownwardGrounder  {
    std::vector<std::vector<int>> rule_delete_component;
    std::unordered_map<int, int> map_pred_to_scc;

    void dump_sccs(const LogicProgram &lp, const SCC &scc) const;

    void compuete_rule_delete_component(const LogicProgram &lp, const SCC &sccs);

public:
    NewGrounder() {};

    int ground(LogicProgram &lp) override;
};

#endif //GROUNDER_GROUNDERS_NEW_GROUNDER_H_
