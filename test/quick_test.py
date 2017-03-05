
import sys
import os
from subprocess import call

CUR_DIR = os.path.dirname(os.path.abspath(__file__))
RWL_LEXER_EXECUTABLE = os.path.abspath(os.path.join(CUR_DIR, '../rwl_lexer'))

LONG_PROGRAM_FILE = os.path.abspath(os.path.join(CUR_DIR, 'LONG_PROGRAM.rwl'))

def basic_test():
    call('echo "22 33l if IF ELSE 44 btrne_77" | {}'.format(RWL_LEXER_EXECUTABLE), shell=True)

def larger_test():
    call('cat {} | {}'.format(LONG_PROGRAM_FILE, RWL_LEXER_EXECUTABLE), shell=True)


if __name__=='__main__':
    basic_test()
    larger_test()