#include "rules.h"

#include <algorithm>

using namespace std;

int Rule::next_index = 0;

size_t Rule::computing_matching_variables() {
    // Join rule should have only have two atoms in the conditions
    assert(conditions.size()==2);

    vector<int> new_key;
    vector<vector<int>> positions(2);

    int pos1 = 0;
    for (int c : conditions[0].get_arguments()) {
        auto it2 = find(conditions[1].get_arguments().begin(),
                        conditions[1].get_arguments().end(),
                        c);
        if (it2!=conditions[1].get_arguments().end()) {
            // Free variables match in both atoms
            int pos2 = distance(conditions[1].get_arguments().begin(), it2);
            new_key.push_back(c);
            positions[0].push_back(pos1);
            positions[1].push_back(pos2);
        }
        ++pos1;
    }
    position_of_joining_vars = positions;
    return new_key.size();
}
