import os
import glob
import argparse
import subprocess

try:
    from termcolor import colored
except:
    print("*** WARNING ***")
    print("termcolor module not installed")
    print("*** INSTALLING TERMCOLOR ***")
    subprocess.run(["pip3", "install", "termcolor"])
    try:
        from termcolor import colored
    except:
        print("*** WARNING ***")
        print("failed to install termcolor")
        print("try running 'pip3 install termcolor'")
        colored = lambda x : x

def assert_valid_path(path):
    '''Tests if a file path exists.'''
    if os.path.exists(path):
        return path
    raise ValueError(f"invalid file path {path}")

if __name__ == "__main__":
    # parse arguments
    parser = argparse.ArgumentParser(description="Run test suites.")
    parser.add_argument("mode", choices=["test", "benchmark"])
    parser.add_argument("files", nargs="*", type=assert_valid_path)
    args = parser.parse_args()

    mode = args.mode
    fnames = args.files

    # glob all files if none are specified
    if not fnames:
        glob_regex = f"**/*.{mode}.cpp"
        fnames = glob.glob(glob_regex)

    # display all of the files
    print(colored("*** FILES ***", "green"))
    for fname in fnames:
        print(fname)

    for fname in fnames:
        # display the file
        print(colored("*** CURRENT FILE ***", "green"))
        print(fname)

        # compile the file
        print(colored("*** COMPILING ***", "green"))
        if mode == "test":
            subprocess.run(["g++", fname, "-std=c++17", "-lgtest", "-lgtest_main", "-lpthread", "-o", mode])
        elif mode == "benchmark":
            subprocess.run(["g++", fname, "-std=c++17", "-O3", "-lbenchmark", "-lpthread", "-o", mode]) 

        # run the file
        if os.path.exists(mode):
            print(colored("*** RUNNING ***", "green"))
            subprocess.run([f"./{mode}"])
            os.remove(mode)
        else:
            print(colored("*** WARNING ***", "yellow"))
            print(f"{fname} failed to compile")
