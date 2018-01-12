#!/usr/bin/python3

# =============================================================================
# ==================================================================== PACKAGES

import os
import sys
import math
import pathlib
import argparse
import subprocess

# ==================================================================== PACKAGES
# =============================================================================

# =============================================================================
# ================================================================== PARAMETERS

parser = argparse.ArgumentParser(
    prog='aff3ct_tests',
    formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('--refs-path',      action='store', dest='refsPath',      type=str,   default="refs",     help='Path to the references to re-simulate.')
parser.add_argument('--results-path',   action='store', dest='resultsPath',   type=str,   default="results",  help='Path to the simulated results.')
parser.add_argument('--build-path',     action='store', dest='buildPath',     type=str,   default="../build", help='Path to the AFF3CT build.')
parser.add_argument('--start-id',       action='store', dest='startId',       type=int,   default=0,          help='Starting id to avoid computing results one again.')                                     # choices=xrange(0,   +inf)
parser.add_argument('--sensibility',    action='store', dest='sensibility',   type=float, default=1.0,        help='Sensibility to verify a SNR point.')                                                    # choices=xrange(0.0, +inf) 
parser.add_argument('--n-threads',      action='store', dest='nThreads',      type=int,   default=0,          help='Number of threads to use in the simulation (0 = all available).')                       # choices=xrange(0,   +ing)
parser.add_argument('--recursive-scan', action='store', dest='recursiveScan', type=bool,  default=True,       help='If enabled, scan the path of refs recursively.')
parser.add_argument('--max-fe',         action='store', dest='maxFE',         type=int,   default=100,        help='Maximum number of frames errors to simulate per SNR point.')                            # choices=xrange(0,   +inf)
parser.add_argument('--weak-rate',      action='store', dest='weakRate',      type=float, default=0.8,        help='Rate of valid SNR points to passed a test.')                                            # choices=xrange(0.0, 1.0 )
parser.add_argument('--max-snr-time',   action='store', dest='maxSNRTime',    type=int,   default=600,        help='The maximum amount of time to spend to compute a SNR point in seconds (0 = illimited)') # choices=xrange(0,   +inf)

# ================================================================== PARAMETERS
# =============================================================================

# =============================================================================
# =================================================================== FUNCTIONS

def recursivelyGetFilenames(currentPath, fileNames):
	if not os.path.exists(currentPath.replace(args.refsPath, args.resultsPath)):
		os.makedirs(currentPath.replace(args.refsPath, args.resultsPath))

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
			fileNames.append(currentPath.replace(args.refsPath + "/", "") + "/" + f)

# -----

# =================================================================== FUNCTIONS
# =============================================================================

# =============================================================================
# ======================================================================== MAIN

#parser.print_help()
args = parser.parse_args()

print('# AFF3CT tests')
print('# ------------')
print('#')
print('# Parameters:')
print('# refs path      =', args.refsPath     )
print('# results path   =', args.resultsPath  )
print('# build path     =', args.buildPath    )
print('# start id       =', args.startId      )
print('# sensibility    =', args.sensibility  )
print('# n threads      =', args.nThreads     )
print('# recursive scan =', args.recursiveScan)
print('# max fe         =', args.maxFE        )
print('# weak rate      =', args.weakRate     )
print('# max snr time   =', args.maxSNRTime   )
print('#')

PathOrigin = os.getcwd()

# get the filenames to test
fileNames = []
fileNamesTmp = os.listdir(args.refsPath)
for f in fileNamesTmp:
	if not os.path.isdir(args.refsPath + "/" + f):
		fileNames.append(f)
	else:
		if args.recursiveScan:
			recursivelyGetFilenames(args.refsPath + "/" + f, fileNames)

print("# Starting the test script...")

nErrors = 0
testId = 0
for fn in fileNames:

	if pathlib.Path(fn).suffix != ".txt" and pathlib.Path(fn).suffix != ".perf" and pathlib.Path(fn).suffix != ".data" and pathlib.Path(fn).suffix != ".dat":
		continue

	if testId < args.startId:
		testId = testId + 1
		continue

	print("Test n°" + str(testId) + " - " + fn, end="", flush=True);

	# open the file in read mode (from the fileName "fn" and the path)
	f = open(args.refsPath + "/" + fn, 'r')

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
	if args.maxFE:
		argsAFFECT.append("-e")
		argsAFFECT.append(str(args.maxFE))
	argsAFFECT.append("-t")
	argsAFFECT.append(str(args.nThreads))
	argsAFFECT.append("--sim-no-colors")
	if args.maxSNRTime:
		argsAFFECT.append("--sim-stop-time")
		argsAFFECT.append(str(args.maxSNRTime))

	os.chdir(args.buildPath)
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

		fRes = open(args.resultsPath + "/" + fn, 'w+')

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
			if absoluteNumDiff > args.sensibility:
				fRes.write(outputAFFECTLines[idx] + "WRONG! FER=" + ref[6][0:8] + "\n")
			else:
				valid = valid + 1
				fRes.write(outputAFFECTLines[idx] + "\n")

			cur_fe = int(simuCur[idx][4])

			idx = idx + 1

			if cur_fe < args.maxFE:
				break

		if valid == idx:
			print(" - STRONG PASSED.", end="\n");
		elif idx != 0 and float(valid) / float(idx) >= args.weakRate:
			print(" - WEAK PASSED.", end="\n");
		else:
			print(" - FAILED.", end="\n");
			nErrors = nErrors +1

		fRes.write("# End of the simulation.\n")
		fRes.close();

	testId = testId + 1

sys.exit(nErrors);

# ======================================================================== MAIN
# =============================================================================
