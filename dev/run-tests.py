#! /usr/bin/env python3
# -*- coding: utf-8 -*-

import os
import subprocess
import sys

from distutils.dir_util import copy_tree
from itertools import product
from shutil import copyfile

"""
This test component just checks to see if the models found
in several different domains have the expected size.

"""


# Commented instances have costs which are currently ignored
EXPECTED_GROUND_ACTIONS = {'domains/agricola/p05.pddl': 48179,
                       'domains/gripper/prob01.pddl' : 36,
                       'domains/gripper/prob20.pddl': 340,
                       'domains/organic-synthesis/p12.pddl' : 3123576,
                       'domains/organic-synthesis-split/p19.pddl': 90589}

GROUNDER_CONFIGS = ['fd', 'new']


class TestRun:
    def __init__(self, instance, config):
        self.instance = instance
        self.grounder = config

    def get_config(self):
        return self.grounder

    def __str__(self):
        return self.grounder

    def run(self):
        print ("Testing {} with {}...".format(self.instance, self.get_config()), end='', flush=True)
        output = subprocess.check_output([os.path.join('..', 'ground.py'),
                                          '-i', self.instance,
                                          '-m', self.grounder])
        return output

    def evaluate(self, output, expected_size):
        num_atoms = None
        num_actions = None
        for line in output.splitlines():
            if b' atoms' in line:
                num_atoms = int(line.split()[0])
            if b' ground actions' in line:
                num_actions = int(line.split()[-1])

        if num_actions == expected_size:
            print("[expected: {}, num_actions: {}]".format(expected_size, num_actions), end='')
            return True
        else:
            print("[expected: {}, num_actions: {}]".format(expected_size, num_actions), end='')
            return False

    def remove_lp_file(self):
        lp_file = 'reachable.lp'
        if os.path.isfile(lp_file):
            os.remove(lp_file)


if __name__ == '__main__':

    if len(sys.argv) != 1:
        print("Usage: ./run-tests.py")
        sys.exit()

    print("Running with current release build.")

    for instance, size in EXPECTED_GROUND_ACTIONS.items():
        for config in GROUNDER_CONFIGS:
            test = TestRun(instance, config)
            output = test.run()
            passed = test.evaluate(output, size)
            if passed:
                print(" PASS")
            else:
                print(" FAIL")
            test.remove_lp_file()
