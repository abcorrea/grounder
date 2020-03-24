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


exp = Experiment('/home/blaas/work/projects/grounder/experiments/combine-with-clingo/data/htg')
exp.add_fetcher('/home/blaas/work/projects/asp-grounding-planning/experiments/clingo-exp/data/htg-eval',
                name='clingo')
exp.add_fetcher('/home/blaas/work/projects/grounder/experiments/first-experiment/data/htg-eval',
                name='new-grounder-and-fd-grounder')
exp.add_report(BaseReport(attributes=['total_time'],
                           filter=[remove_timeouts, create_same_attr]),
               outfile='htg.html')
exp.add_report(ScatterPlotReport(attributes=['total_time'],
                                 filter_algorithm=['new-grounder', 'clingo'],
                                 filter=[remove_timeouts, create_same_attr],
                                 get_category=domain_as_category,
                                 scale='symlog',
                                 format='tex'),
               outfile='htg-new-grounder-vs-clingo.tex')
exp.add_report(ScatterPlotReport(attributes=['total_time'],
                                 filter_algorithm=['new-grounder', 'fd-grounder'],
                                 filter=[remove_timeouts, create_same_attr],
                                 get_category=domain_as_category,
                                 scale='symlog',
                                 format='tex'),
               outfile='htg-new-grounder-vs-fd-grounder.tex')

exp.run_steps()
