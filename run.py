###############################################################################
# @file run.py                                                                #
# @author Calvin Higgins (calvin_higgins2@uri.edu)                            #
# @version 1.0                                                                #
# @date 2022-04-24                                                            #
############################################################################### 


import os
import glob
import argparse
import subprocess


###############################################################################
# Set up some pretty printing tools.                                          #
###############################################################################


colored = lambda x : x


def ok(string):
    '''Prints a ok string.'''
    print(colored(string, 'green'))


def warn(string):
    '''Prints a warn string.'''
    print(colored(string, 'yellow'))


def error(string):
    '''Prints a error string.'''
    print(colored(string, 'red'))


###############################################################################
# Attempt to enable terminal colors.                                          #
###############################################################################


try:
    from termcolor import colored
except:
    warn('*** WARNING ***')
    print('termcolor module not installed')
    ok('*** INSTALLING TERMCOLOR ***')
    subprocess.run(['pip3', 'install', 'termcolor'])
    try:
        from termcolor import colored
        ok('*** INSTALLED TERMCOLOR ***')
    except:
        warn('*** WARNING ***')
        print('failed to install termcolor')
        print('try running "pip3 install termcolor"')
        colored = lambda x : x


###############################################################################
# Attempt to enable performance mode.                                         #
###############################################################################


def enable_performance_mode():
    '''Attempts to enable performance mode.'''
    ok('*** ENABLING PERFORMANCE MODE ***')
    try:
        proc = subprocess.run(['sudo', 'cpupower', 'frequency-set', '-g', 'performance'])
        error_code = proc.returncode
    except Exception as e:
        print(e)
        error_code = 1

    if error_code != 0:
        warn("*** WARNING ***")
        print('failed to enable performance mode')
        print('try running with sudo and ensure cpupower is installed')
    else:
        ok('*** ENABLED PERFORMANCE MODE ***')


###############################################################################
# Set up some basic file operations.                                          #
###############################################################################


def assert_path_exists(path):
    '''Tests if a file path exists.'''
    if os.path.exists(path):
        return path
    raise ValueError(f'invalid file path {path}')


def is_test(fname):
    '''Checks if a file is a test.'''
    return '.test.cpp' in fname or os.path.basename(fname) == 'test.cpp'


def is_benchmark(fname):
    '''Checks if a file is a benchmark.'''
    return '.benchmark.cpp' in fname


def expand_directories(fnames):
    '''Expands directories into cpp files.'''
    new_fnames = []
    for fname in fnames:
        if os.path.isdir(fname):
            search_path = os.path.join(fname, '**', '*.cpp')
            search_results = glob.glob(search_path, recursive=True)
            new_fnames.extend(search_results)
        else:
            new_fnames.append(fname)
    return new_fnames


def only_valid_fnames(fnames):
    '''Filter out duplicates and only keep benchmarks and tests.'''
    new_fnames = []
    for fname in fnames:
        if fname in new_fnames: continue
        if is_benchmark(fname) or is_test(fname):
            new_fnames.append(fname)
    return new_fnames


def only_unique_tests(fnames):
    '''Filter out individual tests that are covered by test.cpp.'''
    new_fnames = []
    test_suite_directories = []

    for fname in fnames:
        directory, basename = os.path.split(fname)
        if basename == 'test.cpp':
            new_fnames.append(fname)
            test_suite_directories.append(directory)
    
    for fname in fnames:
        directory, basename = os.path.split(fname)
        if directory not in test_suite_directories:
            new_fnames.append(fname)
    
    return new_fnames


###############################################################################
# Set up some compiler commands.                                              #
###############################################################################


def try_compile_test(fname):
    '''Try to compile the file as a test.'''
    if not is_test(fname): return
    subprocess.run(['g++', fname, '-std=c++17', '-lgtest', '-lgtest_main', 
                    '-lpthread', '-o', 'exec'])


def try_compile_benchmark(fname):
    '''Try to compile the file as a benchmark.'''
    if not is_benchmark(fname): return
    subprocess.run(['g++', fname, '-std=c++17', '-O3', '-lbenchmark', 
                    '-lpthread', '-o', 'exec']) 


def did_compile():
    '''Tests if the file compiled.'''
    return os.path.exists('exec')


def clean_up():
    '''Cleans up compiled files if they exist.'''
    if did_compile():
        os.remove('exec')


def execute():
    '''Executes compiled files and cleans them up. Returns a status code.'''
    proc = subprocess.run([f'./exec'])
    clean_up()
    return proc.returncode


###############################################################################
# Parse the input files and execute them.                                     #
###############################################################################


if __name__ == '__main__':

    # Attempt to enable performance mode.
    enable_performance_mode()

    # Get input files and directories from the user.
    parser = argparse.ArgumentParser(description='Run test suites.')
    parser.add_argument('paths', nargs='+', type=assert_path_exists)
    args = parser.parse_args()

    # Clean up old files.
    clean_up()

    # Compute unique valid files.
    fnames = expand_directories(args.paths)
    fnames = only_valid_fnames(fnames)
    fnames = only_unique_tests(fnames)

    # Display them.
    ok('*** FILES ***')
    for fname in fnames:
        print(fname)
    
    # Run them.
    for fname in fnames:

        # Display the current file.
        ok('*** CURRENT FILE ***')
        print(fname)

        # Compile the current file.
        ok('*** COMPILING ***')
        try_compile_test(fname)
        try_compile_benchmark(fname)

        # Run the current file.
        if did_compile():
            ok('*** RUNNING ***')
            error_code = execute()
            if error_code != 0:
                warn('*** WARNING ***')
                print(f'program crashed with error code {error_code}')
        else:
            warn('*** WARNING ***')
            print(f'{fname} failed to compile')
