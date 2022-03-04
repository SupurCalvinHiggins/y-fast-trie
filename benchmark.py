import os
import glob
import subprocess

try:
	from termcolor import colored
except:
	print("*** WARNING ***")
	print("Termcolor module not installed")
	colored = lambda x : x

if __name__ == "__main__":
	for fname in glob.glob("**/*.benchmark.cpp"):
		print(colored("*** CURRENT FILE ***", "green"))
		print(fname)
		print(colored("*** COMPILING ***", "green"))
		subprocess.run(["g++", fname, "-std=c++17", "-lbenchmark", "-lpthread", "-o", "benchmark"])
		if os.path.exists("benchmark"):
			print(colored("*** RUNNING ***", "green"))
			subprocess.run(["./benchmark"])
		else:
			print(colored("*** WARNING ***", "yellow"))
			print(f"{fname} failed to compile")
	
	if os.path.exists("benchmark"):
		os.remove("benchmark")