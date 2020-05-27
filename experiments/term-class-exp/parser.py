#! /usr/bin/env python

import re
import sys

from lab.parser import Parser

PATTERNS = [
    ('atoms', r'(\d+) atoms', int),
    ('parsing_time', r'Parsing time: (.+)s', float),
    ('grounding_time', r'Grounding time: (.+)s', float),
    ('total_time', r'Total time: (.+)s', float),
    ('memory', r'Peak memory usage: (\d+) kB', int),
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
