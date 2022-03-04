import os
import glob
import subprocess

try:
	from termcolor import colored
except:
	print("*** WARNING ***")
	print("termcolor module not installed")
	print("try running 'pip3 install termcolor'")
	colored = lambda x : x

if __name__ == "__main__":
	for fname in glob.glob("**/*.test.cpp"):
		print(colored("*** CURRENT FILE ***", "green"))
		print(fname)
		print(colored("*** COMPILING ***", "green"))
		subprocess.run(["g++", fname, "-std=c++17", "-lgtest", "-lgtest_main", "-lpthread", "-o", "test"])
		if os.path.exists("test"):
			print(colored("*** RUNNING ***", "green"))
			subprocess.run(["./test"])
		else:
			print(colored("*** WARNING ***", "yellow"))
			print(f"{fname} failed to compile")
