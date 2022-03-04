import os
import glob
import argparse
import subprocess

try:
	from termcolor import colored
except:
	print("*** WARNING ***")
	print("Termcolor module not installed")
	colored = lambda x : x

if __name__ == "__main__":
	parser = argparse.ArgumentParser(description="Run benchmarks.")
	parser.add_argument("file", nargs="*")
	fnames = parser.parse_args().file

	if not fnames:
		fnames = glob.glob("**/*.benchmark.cpp")

	print(colored("*** FILES ***", "green"))
	for fname in fnames:
		print(fname)

	for fname in fnames:
		print(colored("*** CURRENT FILE ***", "green"))
		print(fname)
		print(colored("*** COMPILING ***", "green"))
		subprocess.run(["g++", fname, "-std=c++17", "-O3", "-lbenchmark", "-lpthread", "-o", "benchmark"])
		if os.path.exists("benchmark"):
			print(colored("*** RUNNING ***", "green"))
			subprocess.run(["./benchmark"])
			os.remove("benchmark")
		else:
			print(colored("*** WARNING ***", "yellow"))
			print(f"{fname} failed to compile")		