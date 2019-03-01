#!/usr/bin/env python3

import argparse
import sys
import re
import subprocess
import os
import glob
import copy

import aff3ct_help_parser as ahp

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
	# ahp.print_help_map(help_map)
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
	helpMap = ahp.help_to_map(std)

	codesList = helpMap["Simulation"]["--sim-cde-type, -C"]["limits"] [1:-1].split("|")
	simList   = helpMap["Simulation"]["--sim-type"        ]["limits"] [1:-1].split("|")

	# try to run all codes ans simu to get their helps
	aff3ct_keys = []
	for c in codesList:
		for s in simList:
			args_list = [aff3ct_path, "-C", c, "-H", "-k", "--sim-type", s, "-p", "8"]
			std, err  = run_aff3ct(args_list)

			helpMap = ahp.help_to_map(std)

			aff3ct_helpmap_to_keys_list(helpMap, aff3ct_keys)

	return aff3ct_keys

def get_doc_keys(doc_path):

	doc_keys = []
	for filename in glob.iglob(doc_path + '**/*.rst', recursive=True):
		pattern = re.compile("\|(factory::[^ ]*)\|")
		for i, line in enumerate(open(filename)):
			for match in re.finditer(pattern, line):
				doc_keys.append(match.group(1))

	# remove duplicates
	doc_keys = list(set(doc_keys))

	return doc_keys

def display_keys(keys):

	for e in keys:
		print ("  - [" + e + "]")

	if len(keys) == 0:
		print ("  The keys list is empty.")

def check_keys(keys_file, aff3ct_path, doc_path):

	list_keys   = get_keys(keys_file)
	aff3ct_keys = get_aff3ct_help_keys(aff3ct_path)
	doc_keys    = get_doc_keys(doc_path)

	list_keys.sort()
	aff3ct_keys.sort()
	doc_keys.sort()

	aff3ct_keys_save = copy.deepcopy(aff3ct_keys)

	not_in_aff3ct_keys = []
	for k in list_keys:
		try:
			idx = aff3ct_keys.index(k)
			del aff3ct_keys[idx]
		except Exception as e:
			not_in_aff3ct_keys.append(k)

	not_in_doc_keys = []
	for k in aff3ct_keys_save:
		try:
			idx = doc_keys.index(k)
			del doc_keys[idx]
		except Exception as e:
			not_in_doc_keys.append(k)

	# manages special key exceptions
	exceptions_not_in_doc_keys = ["factory::Frozenbits_generator::parameters::p+pb-path"]
	exceptions_doc_keys = ["factory::BFER::parameters::p+mpi-comm-freq", "factory::Launcher::parameters::except-a2l"]
	for e in exceptions_not_in_doc_keys:
		if e in not_in_doc_keys: not_in_doc_keys.remove(e)
	for e in exceptions_doc_keys:
		if e in doc_keys: doc_keys.remove(e)

	print("Keys used in the AFF3CT help but not defined in the strings database (undocumented keys):")
	display_keys(aff3ct_keys)
	print()
	print("Keys used in the AFF3CT doc but not used in the AFF3CT help:")
	display_keys(doc_keys)
	print()
	print("Keys used in the AFF3CT help but not used in the AFF3CT doc:")
	display_keys(not_in_doc_keys)
	print()
	print("Keys defined in the strings database but not used in the AFF3CT help or in the AFF3CT doc:")
	display_keys(not_in_aff3ct_keys)
	print()

	nDiff = len(aff3ct_keys) + len(doc_keys) + len(not_in_doc_keys)

	return nDiff;

if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('--keys',   action='store', dest='keys_file',   type=str, default='doc/sphinx/strings.rst')
	parser.add_argument('--aff3ct', action='store', dest='aff3ct_path', type=str, default='build/bin/aff3ct')
	parser.add_argument('--doc',    action='store', dest='doc_path',    type=str, default='doc/sphinx/source/user/simulation/parameters/')

	args = parser.parse_args()

	nDiff = check_keys(args.keys_file, args.aff3ct_path, args.doc_path)

	sys.exit(nDiff);