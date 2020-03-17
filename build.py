#!/usr/bin/env python

import argparse
import os
import subprocess

PROJECT_ROOT = os.path.dirname(os.path.realpath(__file__))
BUILD_DIR = os.path.join(PROJECT_ROOT, 'builds', 'release', 'grounder')
SRC_DIR = os.path.join(PROJECT_ROOT, 'src', 'grounder')


def parse_arguments():
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter,
                                     description='Ground STRIPS planning tasks.')
    parser.add_argument('--debug',
                        action="store_true", help="Build in debug mode.")
    args = parser.parse_args()
    return args

def get_build_dir(debug):
    if debug:
        return os.path.join(PROJECT_ROOT, 'builds', 'debug', 'grounder')
    else:
        return os.path.join(PROJECT_ROOT, 'builds', 'release', 'grounder')

def build(debug_flag):
    BUILD_DIR = get_build_dir(debug_flag)
    if debug_flag:
        BUILD_TYPE = 'Debug'
    else:
        BUILD_TYPE = 'Release'
    os.makedirs(BUILD_DIR, exist_ok=True)
    subprocess.check_call(['cmake', SRC_DIR, '-DCMAKE_BUILD_TYPE='+BUILD_TYPE], cwd=BUILD_DIR)
    subprocess.check_call(['make', '-j5'], cwd=BUILD_DIR)


if __name__ == '__main__':
    args = parse_arguments()
    build(args.debug)
