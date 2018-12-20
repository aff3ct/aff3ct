import argparse
import sys
import re
import subprocess
import os

import aff3ct_help_reader as ahr


# read all the lines from the given file and set them in a list of string lines with striped \n \r
def readFileInTable(filename):
	aFile = open(filename, "r")
	lines = []
	for line in aFile:
		line = re.sub('\r','',line.rstrip('\n'))
		if len(line) > 0:
			lines.append(line)
	aFile.close()

	return lines;

def get_keys(filename):
	lines = readFileInTable(filename)

	list_keys = []

	for l in lines:
		if l.startswith(".. |"):
			start_pos = 4
			end_pos = l.find("|", start_pos)
			list_keys.append(l[start_pos:end_pos])

	return list_keys

def run_aff3ct(args_list):
	try:
		processAFFECT = subprocess.Popen(args_list, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()
	except KeyboardInterrupt:
		os.kill(processAFFECT.pid, signal.SIGINT)
		(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()

	err = stderrAFFECT.decode(encoding='UTF-8')
	std = stdoutAFFECT.decode(encoding='UTF-8').split("\n")

	return std, err


def aff3ct_helpmap_to_keys_list(help_map, aff3ct_keys): # fill aff3ct_keys from help_map
	# ahr.print_help_map(help_map)
	for m in help_map: # module
		for a in help_map[m]: # argument
			if type(help_map[m][a]) is dict:
				key = help_map[m][a]["key"]

				if key != "":
					try:
						aff3ct_keys.index(key)
					except Exception as e:
						aff3ct_keys.append(key)
					else:
						pass


def get_aff3ct_help_keys(aff3ct_path):

	# get the available codes and simulation types
	args_list = [aff3ct_path, "-h"]
	std, err  = run_aff3ct(args_list)
	helpMap = ahr.help_to_map(std)

	codesList = helpMap["Simulation"]["--sim-cde-type, -C"]["limits"] [1:-1].split("|")
	simList   = helpMap["Simulation"]["--sim-type"        ]["limits"] [1:-1].split("|")


	# try to run all codes ans simu to get their helps
	aff3ct_keys = []
	for c in codesList:
		for s in simList:
			args_list = [aff3ct_path, "-C", c, "-H", "-k", "--sim-type", s]
			std, err  = run_aff3ct(args_list)

			helpMap = ahr.help_to_map(std)

			aff3ct_helpmap_to_keys_list(helpMap, aff3ct_keys)

	return aff3ct_keys


def check_keys(keys_file, aff3ct_path):

	list_keys   = get_keys(keys_file)
	aff3ct_keys = get_aff3ct_help_keys(aff3ct_path)

	not_in_aff3ct_keys = []

	for k in list_keys:
		try:
			idx = aff3ct_keys.index(k)
			del aff3ct_keys[idx]
		except Exception as e:
			not_in_aff3ct_keys.append(k)

	print("keys left in aff3ct help:")
	print(aff3ct_keys)
	print()
	print("keys not in aff3ct help:")
	print(not_in_aff3ct_keys)
	print()

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('--keys',   action='store', dest='keys_file',   type=str, default='doc/sphinx/strings.rst')
	parser.add_argument('--aff3ct', action='store', dest='aff3ct_path', type=str, default='build/bin/aff3ct')

	args = parser.parse_args()

	check_keys(args.keys_file, args.aff3ct_path)