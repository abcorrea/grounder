#! /usr/bin/env python

import os
import platform

from lab.environments import LocalEnvironment, BaselSlurmEnvironment
from lab.experiment import Experiment

from downward import suites
from downward.reports.absolute import AbsoluteReport
from downward.reports.scatter import ScatterPlotReport

from common_setup import Configuration


from pathlib import Path

'''
Experiment to test term-class branch.
'''

# Create custom report class with suitable info and error attributes.
class BaseReport(AbsoluteReport):
    INFO_ATTRIBUTES = []
    ERROR_ATTRIBUTES = [
        'domain', 'problem', 'algorithm', 'unexplained_errors', 'error', 'node']

NODE = platform.node()
REMOTE = NODE.endswith(".scicore.unibas.ch") or NODE.endswith(".cluster.bc2.ch")
BENCHMARKS_DIR = os.environ["HTG_BENCHMARKS"]
RUN_SCRIPT_DIR = str(Path(os.getcwd()).parent.parent)

if REMOTE:
    SUITE = ['genome-edit-distance',
             'genome-edit-distance-split',
             'genome-edit-distance-positional',
             'pipesworld-tankage-nosplit',
             'organic-synthesis-MIT',
             'organic-synthesis-original',
             'organic-synthesis-alkene']
    ENV = BaselSlurmEnvironment(
        partition='infai_2',
        memory_per_cpu="6G",
        extra_options='#SBATCH --cpus-per-task=3',
        setup="%s\n%s" % (
            BaselSlurmEnvironment.DEFAULT_SETUP,
            "source /infai/blaas/virtualenvs/grounder/bin/activate\n"),
        export=["PATH", "DOWNWARD_BENCHMARKS", "POWER_LIFTED_DIR"])
else:
    SUITE = ['organic-synthesis-alkene:p2.pddl']
    ENV = LocalEnvironment(processes=4)

TIME_LIMIT = 1800
MEMORY_LIMIT = 16384

ATTRIBUTES=['atoms',
            'grounding_time',
            'memory',
            'parsing_time',
            'total_time']

# Create a new experiment.
exp = Experiment(environment=ENV)

# Add custom parser for Power Lifted.
exp.add_parser('parser.py')

NEW_GROUNDER_CONFIGS = [Configuration('term-class-new-grounder', [])]
FD_CONFIGS = []

# Create one run for each instance and each configuration
for config in NEW_GROUNDER_CONFIGS:
    for task in suites.build_suite(BENCHMARKS_DIR, SUITE):
        run = exp.add_run()
        run.add_resource('domain', task.domain_file, symlink=True)
        run.add_resource('problem', task.problem_file, symlink=True)
        run.add_command('run-search',
                        [RUN_SCRIPT_DIR+'/ground.py', '-i', task.problem_file],
                        time_limit=TIME_LIMIT,
                        memory_limit=MEMORY_LIMIT)
        run.set_property('domain', task.domain)
        run.set_property('problem', task.problem)
        run.set_property('algorithm', config.name)
        run.set_property('id', [config.name, task.domain, task.problem])
for config in FD_CONFIGS:
    for task in suites.build_suite(BENCHMARKS_DIR, SUITE):
        run = exp.add_run()
        run.add_resource('domain', task.domain_file, symlink=True)
        run.add_resource('problem', task.problem_file, symlink=True)
        run.add_command('run-search',
                        [RUN_SCRIPT_DIR+'/src/translate/build_model.py', task.domain_file, task.problem_file],
                        time_limit=TIME_LIMIT,
                        memory_limit=MEMORY_LIMIT)
        run.set_property('domain', task.domain)
        run.set_property('problem', task.problem)
        run.set_property('algorithm', config.name)
        run.set_property('id', [config.name, task.domain, task.problem])

        # Add step that writes experiment files to disk.
exp.add_step('build', exp.build)

# Add step that executes all runs.
exp.add_step('start', exp.start_runs)

# Add step that collects properties from run directories and
# writes them to *-eval/properties.
exp.add_fetcher(name='fetch')

# Make a report.
exp.add_report(
    BaseReport(attributes=ATTRIBUTES),
    outfile='report.html')

# Parse the commandline and run the specified steps.
exp.run_steps()
