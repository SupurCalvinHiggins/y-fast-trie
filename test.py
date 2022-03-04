import os
import glob
import argparse
import subprocess

try:
	from termcolor import colored
except:
	print("*** WARNING ***")
	print("termcolor module not installed")
	print("try running 'pip3 install termcolor'")
	colored = lambda x : x

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Run test suites.")
	parser.add_argument("file", nargs="*")
	fnames = parser.parse_args().file

	if not fnames:
		fnames = glob.glob("**/*.test.cpp")

	print(colored("*** FILES ***", "green"))
	for fname in fnames:
		print(fname)

	for fname in fnames:
		print(colored("*** CURRENT FILE ***", "green"))
		print(fname)
		print(colored("*** COMPILING ***", "green"))
		subprocess.run(["g++", fname, "-std=c++17", "-lgtest", "-lgtest_main", "-lpthread", "-o", "test"])
		if os.path.exists("test"):
			print(colored("*** RUNNING ***", "green"))
			subprocess.run(["./test"])
			os.remove("test")
		else:
			print(colored("*** WARNING ***", "yellow"))
			print(f"{fname} failed to compile")
