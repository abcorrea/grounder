#! /usr/bin/env python

import re
import sys

from lab.parser import Parser

PATTERNS = [
    ('atoms', r'(\d+) atoms', int),
    ('total_time', r'Total time: (.+)s', float),
]

class OurParser(Parser):
    def __init__(self):
        Parser.__init__(self)

        for name, pattern, typ in PATTERNS:
            self.add_pattern(name, pattern, type=typ)


def main():
    parser = OurParser()
    parser.parse()

main()
