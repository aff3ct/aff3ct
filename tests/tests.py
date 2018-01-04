#!/usr/bin/python3

# =============================================================================
# ================================================================== PARAMETERS

PathTests     = "refs"     # can be overrided by arg1 from the cmd line
PathResults   = "results"  # can be overrided by arg2 from the cmd line
StartId       = 0          # can be overrided by arg3 from the cmd line, starting id to not do previous one again

PathBuild     = "../build"
Sensibility   = 1.0
Nthreads      = 0          # if 0 then AFF3CT takes all the available threads
RecursiveScan = True
MaxFE         = 100        # 0 takes fe from the original simulation
WeakRate      = 0.8        # 0 < WeakRate < 1
MaxTimeSNR    = 600        # max time to spend per SNR (in sec), 0 = illimited

# ================================================================== PARAMETERS
# =============================================================================

# =============================================================================
# ==================================================================== PACKAGES

import os
import sys
import math
import pathlib
import subprocess

# ==================================================================== PACKAGES
# =============================================================================

# =============================================================================
# =================================================================== FUNCTIONS

def recursivelyGetFilenames(currentPath, fileNames):
	if not os.path.exists(currentPath.replace(PathTests, PathResults)):
		os.makedirs(currentPath.replace(PathTests, PathResults))

	files = os.listdir(currentPath)
	for f in files:
		if ".empty" in f:
			continue
		if "~" in f:
			continue
		if os.path.isdir(currentPath + "/" + f):
			newCurrentPath = currentPath + "/" + f
			recursivelyGetFilenames(newCurrentPath, fileNames)
		else:
			fileNames.append(currentPath.replace(PathTests + "/", "") + "/" + f)

# -----

# =================================================================== FUNCTIONS
# =============================================================================

# =============================================================================
# ======================================================================== MAIN

if len(sys.argv) >= 2:
	PathTests = sys.argv[1]

if len(sys.argv) >= 3:
	PathResults = sys.argv[2]

if len(sys.argv) >= 4:
	StartId = int(sys.argv[3])

PathOrigin = os.getcwd()

# get the filenames to test
fileNames = []
fileNamesTmp = os.listdir(PathTests)
for f in fileNamesTmp:
	if not os.path.isdir(PathTests + "/" + f):
		fileNames.append(f)
	else:
		if RecursiveScan:
			recursivelyGetFilenames(PathTests + "/" + f, fileNames)

print("Starting the test script...")

testId = 0
for fn in fileNames:

	if testId < StartId:
		testId = testId + 1
		continue

	if pathlib.Path(fn).suffix != ".txt" and pathlib.Path(fn).suffix != ".perf" and pathlib.Path(fn).suffix != ".data" and pathlib.Path(fn).suffix != ".dat":
		continue

	print("Test n°" + str(testId) + " - " + fn, end="", flush=True);

	# open the file in read mode (from the fileName "fn" and the path)
	f = open(PathTests + "/" + fn, 'r')

	# read all the lines from the current file f
	lines = []
	for line in f:
		lines.append(line)

	# get the main infos from the lines
	runCommand = lines[1].strip()

	currentSection = ""
	idx = 0
	simuRef = []
	for l in lines:
		# avoid the first lines and the comments
		if idx > 6 and l.replace(" ", "") != "" and l.replace(" ", "") != "\n" and l[0] != '#':
			simuRef.append(l.strip().replace("||", "|").replace(" ", "").split("|"))
		idx = idx +1

	f.close()

	# split the run command
	argsAFFECT = [""]
	idx = 0

	new = 0
	found_dashes = 0
	for s in runCommand:
		if found_dashes == 0:
			if s == ' ':
				if new == 0:
					argsAFFECT.append("")
					idx = idx + 1
					new = 1

			elif s == '\"':
				if new == 0:
					argsAFFECT.append("")
					idx = idx + 1
					new = 1
				found_dashes = 1

			else:
				argsAFFECT[idx] += s
				new = 0

		else: # between dashes
			if s == '\"':
				argsAFFECT.append("")
				idx = idx + 1
				found_dashes = 0

			else:
				argsAFFECT[idx] += s

	del argsAFFECT[idx]


	argsAFFECT.append("--ter-freq")
	argsAFFECT.append("0")
	if MaxFE:
		argsAFFECT.append("-e")
		argsAFFECT.append(str(MaxFE))
	argsAFFECT.append("-t")
	argsAFFECT.append(str(Nthreads))
	argsAFFECT.append("--sim-no-colors")
	if MaxTimeSNR:
		argsAFFECT.append("--sim-stop-time")
		argsAFFECT.append(str(MaxTimeSNR))

	os.chdir(PathBuild)
	processAFFECT = subprocess.Popen(argsAFFECT, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()

	err = stderrAFFECT.decode(encoding='UTF-8')
	if err:
		print(" - ABORTED.", end="\n");
		print("Error message:", end="\n");
		print(err)
	else:
		# begin to write the results into a file
		os.chdir(PathOrigin)

		fRes = open(PathResults + "/" + fn, 'w+')

		# parse the results
		stdOutput = stdoutAFFECT.decode(encoding='UTF-8').split("\n")
		outputAFFECTLines = []
		simuCur = []
		for l in stdOutput:
			if l != "" and l[0] != '#' and l[0] != "(": # avoid the first lines and the comments
				array = l.strip().replace("||", "|").replace(" ", "").split("|")
				if (len(array) == 12 or len(array) == 9):
					outputAFFECTLines.append(l)
					simuCur.append(array)
				else:
					fRes.write(l + "\n")
			elif l != "" and l[0] == '#' and "# End of the simulation." not in l:
				fRes.write(l + "\n")

		fRes.flush()

		# validate (or not) the BER/FER performance
		valid = 0;
		idx = 0
		for ref in simuRef:
			numRef = float(ref[6][0:4])
			powerRef = int(ref[6][6:8])
			try:
				numCur = float(simuCur[idx][6][0:4])
				powerCur = int(simuCur[idx][6][6:8])
			except IndexError: # no such line
				break

			if powerRef - powerCur != 0:
				if powerRef > powerCur:
					numCur = numCur * 10**(math.fabs(powerRef - powerCur))
				else:
					numRef = numRef * 10**(math.fabs(powerRef - powerCur))

			absoluteNumDiff = math.fabs(numRef - numCur)
			if absoluteNumDiff > Sensibility:
				fRes.write(outputAFFECTLines[idx] + "WRONG! FER=" + ref[6][0:8] + "\n")
			else:
				valid = valid + 1
				fRes.write(outputAFFECTLines[idx] + "\n")

			cur_fe = int(simuCur[idx][4])

			idx = idx + 1

			if cur_fe < MaxFE:
				break

		if valid == idx:
			print(" - STRONG PASSED.", end="\n");
		elif idx != 0 and float(valid) / float(idx) >= WeakRate:
			print(" - WEAK PASSED.", end="\n");
		else:
			print(" - FAILED.", end="\n");

		fRes.write("# End of the simulation.\n")
		fRes.close();

	testId = testId + 1

# ======================================================================== MAIN
# =============================================================================
