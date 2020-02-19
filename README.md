Grounder
======

Current state
--
- Implementing Fast Downward grounder in C++
- Using Fast Downward translator code to parse and produce logical program
- Aiming for STRIPS domains

## Future Ideas
- Negated static atoms from preconditions
- Statistics of linearity and dependency graph of the logical programs
- Lifted symmetries during grounding (see Sievers et al. 2018)
- Exploit mutexes during grounding (see Fiser 2020)
- Check how we want to handle ADL domains
- Ground only facts and no atoms