#!/usr/bin/env bash

valgrind --tool=callgrind \
  ./builds/release/grounder/grounder ./reachable.lp fd
