#!/usr/bin/env python

import os
import subprocess

PROJECT_ROOT = os.path.dirname(os.path.realpath(__file__))
BUILD_DIR = os.path.join(PROJECT_ROOT, 'builds', 'release', 'grounder')
SRC_DIR = os.path.join(PROJECT_ROOT, 'src', 'grounder')


def build():
    os.makedirs(BUILD_DIR, exist_ok=True)
    subprocess.check_call(['cmake', SRC_DIR, '-DCMAKE_BUILD_TYPE=Release'], cwd=BUILD_DIR)
    subprocess.check_call(['make', '-j5'], cwd=BUILD_DIR)


if __name__ == '__main__':
    build()
