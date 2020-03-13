#!/usr/bin/env python


import argparse
import os
import shutil
import subprocess
import sys
import tempfile
import time


def find_domain_filename(task_filename):
    """
    Find domain filename for the given task using automatic naming rules.
    """
    dirname, basename = os.path.split(task_filename)

    domain_basenames = [
        "domain.pddl",
        basename[:3] + "-domain.pddl",
        "domain_" + basename,
        "domain-" + basename,
    ]

    for domain_basename in domain_basenames:
        domain_filename = os.path.join(dirname, domain_basename)
        if os.path.exists(domain_filename):
            return domain_filename


def parse_arguments():
    parser = argparse.ArgumentParser(description='Generate models.')
    parser.add_argument('-i', '--instance', required=True, help="The path to the problem instance file.")
    parser.add_argument('-d', '--domain', default=None, help="(Optional) The path to the problem domain file. If none is "
                                                       "provided, the system will try to automatically deduce "
                                                       "it from the instance filename.")
    parser.add_argument('-t', '--lp-output', default="output.lp", help="Logical program output file.")
    parser.add_argument('-m', '--method', default="fd", help="Grounding method.")

    args = parser.parse_args()
    if args.domain is None:
        args.domain = find_domain_filename(args.instance)
        if args.domain is None:
            raise RuntimeError('Could not find domain filename that matches instance file ', args.domain)

    return args


if __name__ == '__main__':
    SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))
    args = parse_arguments()

    domain_file = args.domain
    instance_file = args.instance
    if not os.path.isfile(domain_file):
        sys.stderr.write("Error: Domain file does not exist.\n")
        sys.exit()
    if not os.path.isfile(instance_file):
        sys.stderr.write("Error: Instance file does not exist.\n")
        sys.exit()

    BUILD_DIR = SCRIPT_PATH+'/builds/release/grounder'
    if not os.path.exists(BUILD_DIR):
        os.makedirs(BUILD_DIR)

    os.chdir(SCRIPT_PATH+'/builds/release/grounder')
    subprocess.check_call(['cmake', SCRIPT_PATH+'/src/grounder/'])
    subprocess.check_call(['make'])
    os.chdir(SCRIPT_PATH)

    f = args.lp_output
    if args.lp_output is not None:
        f = open(args.lp_output, 'w')
    subprocess.call([os.getcwd()+'/src/translate/pddl_to_prolog.py', domain_file, instance_file], stdout=f)

    subprocess.check_call([SCRIPT_PATH+'/builds/release/grounder/grounder',
                           args.lp_output,
                           args.method])
