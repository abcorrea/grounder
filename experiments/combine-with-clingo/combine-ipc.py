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


def get_valid(run):
    invalid_domains = ['ged-opt14-strips',
                       'ged-sat14-strips',
                       'storage',
                       'tidybot-opt11-strips',
                       'tidybot-opt14-strips',
                       'tidybot-sat11-strips']
    if run['domain'] in invalid_domains:
        return False
    else:
        return True


exp = Experiment('/home/blaas/work/projects/grounder/experiments/combine-with-clingo/data/ipc')
exp.add_fetcher('/home/blaas/work/projects/asp-grounding-planning/experiments/clingo-exp/data/ipc-eval',
                name='clingo')
exp.add_fetcher('/home/blaas/work/projects/grounder/experiments/first-experiment/data/ipc-eval',
                name='new-grounder-and-fd-grounder')
exp.add_report(BaseReport(attributes=['total_time'],
                           filter=[remove_timeouts, create_same_attr]),
               outfile='ipc.html')
exp.add_report(ScatterPlotReport(attributes=['total_time'],
                           filter_algorithm=['new-grounder', 'clingo'],
                           filter=[remove_timeouts, create_same_attr, get_valid],
                           scale='symlog',
                           format='tex'),
               outfile='ipc-new-grounder-vs-clingo.tex')
exp.add_report(ScatterPlotReport(attributes=['total_time'],
                           filter_algorithm=['new-grounder', 'fd-grounder'],
                           filter=[remove_timeouts, create_same_attr, get_valid],
                           scale='symlog',
                           format='tex'),
               outfile='ipc-new-grounder-vs-fd-grounder.tex')



exp.run_steps()
