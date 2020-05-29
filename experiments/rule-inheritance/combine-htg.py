#! /usr/bin/env python

import os
import platform

from lab.environments import LocalEnvironment, BaselSlurmEnvironment
from lab.experiment import Experiment

from downward import suites
from downward.reports.absolute import AbsoluteReport
from downward.reports.scatter import ScatterPlotReport

from common_setup import Configuration

# Create custom report class with suitable info and error attributes.
class BaseReport(AbsoluteReport):
    INFO_ATTRIBUTES = []
    ERROR_ATTRIBUTES = [
        'domain', 'problem', 'algorithm', 'unexplained_errors', 'error', 'node']


def remove_timeouts(run):
    if 'total_time' in run:
        if run['total_time'] > 1780:
            run['ground'] = 0
            run['total_time'] = None
    return run

def create_same_attr(run):
    if 'translator_time_computing_model' in run:
        run['total_time'] = run['translator_time_computing_model']
    return run


def domain_as_category(run1, run2):
    # run2['domain'] has the same value, because we always
    # compare two runs of the same problem.
    return run1['domain']


exp = Experiment('data/combined-htg')
exp.add_fetcher('data/htg-eval', name='term-class')
exp.add_fetcher('/home/blaas/work/projects/grounder/experiments/first-experiment/data/htg-eval',
                name='master')

exp.add_report(BaseReport(attributes=['total_time'],
                           filter=[remove_timeouts, create_same_attr]),
               outfile='htg.html')

exp.run_steps()
