#!/usr/bin/env python

import argparse
import os
import subprocess
import sys

from build import SRC_DIR, BUILD_DIR, build

SCRIPT_PATH = os.path.dirname(os.path.realpath(__file__))


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
    parser = argparse.ArgumentParser(formatter_class=argparse.ArgumentDefaultsHelpFormatter,
                                     description='Ground STRIPS planning tasks.')
    parser.add_argument('-i', '--instance', required=True, help="The path to the problem instance file.")
    parser.add_argument('-d', '--domain', default=None,
                        help="(Optional) The path to the problem domain file. If none is  provided, "
                             "the system will try to automatically deduce it from the instance filename.")
    parser.add_argument('-t', '--lp-output', default="reachable.lp",
                        help="File where the reachability logic program will be written.")
    parser.add_argument('-m', '--method', default="fd", help="Grounding method.")
    parser.add_argument('-b', '--build', action='store_true',
                        help="Build the project before running it (Default: False).")

    args = parser.parse_args()
    if args.domain is None:
        args.domain = find_domain_filename(args.instance)
        if args.domain is None:
            raise RuntimeError('Could not find domain filename that matches instance file ', args.domain)

    return args


def main(args):
    domain_file = args.domain
    instance_file = args.instance
    if not os.path.isfile(domain_file):
        sys.stderr.write("Error: Domain file does not exist.\n")
        sys.exit()
    if not os.path.isfile(instance_file):
        sys.stderr.write("Error: Instance file does not exist.\n")
        sys.exit()

    if args.build:
        build()

    generate_lp(args.domain, args.instance, args.lp_output)

    ground_lp(args.lp_output, args.method)

    subprocess.check_call([os.path.join(BUILD_DIR, 'grounder'), args.lp_output, args.method])


def generate_lp(domain, instance, lp_output):
    with open(lp_output, 'w') as f:
        subprocess.call([os.path.join(SRC_DIR, 'translate', 'pddl_to_prolog.py'), domain, instance], stdout=f)


def ground_lp(filename, method):
    subprocess.check_call([os.path.join(BUILD_DIR, 'grounder'), filename, method])


if __name__ == '__main__':
    main(parse_arguments())

