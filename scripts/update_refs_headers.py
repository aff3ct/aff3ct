
# =============================================================================
# ==================================================================== PACKAGES

import os
import signal
import sys
import math
import time
import pathlib
import argparse
import subprocess
import numpy as np

sys.path.insert(0, os.path.abspath(os.path.join(os.path.dirname(__file__), "../refs")))
import aff3ct_refs_reader as arr

# ==================================================================== PACKAGES
# =============================================================================

# =============================================================================
# ================================================================== PARAMETERS

parser = argparse.ArgumentParser(prog='aff3ct-test-regression', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('--refs-path',      action='store', dest='refsPath',      type=str,   default="refs",                    help='Path to the references to re-simulate.')
parser.add_argument('--results-path',   action='store', dest='resultsPath',   type=str,   default="test-regression-results", help='Path to the simulated results.')
parser.add_argument('--build-path',     action='store', dest='buildPath',     type=str,   default="build",                   help='Path to the AFF3CT build.')
parser.add_argument('--start-id',       action='store', dest='startId',       type=int,   default=1,                         help='Starting id to avoid computing results one again.')                                       # choices=xrange(1,   +inf)
parser.add_argument('--recursive-scan', action='store', dest='recursiveScan', type=bool,  default=True,                      help='If enabled, scan the path of refs recursively.')
parser.add_argument('--verbose',        action='store', dest='verbose',       type=bool,  default=False,                     help='Enable the verbose mode.')

extensions = ['.txt', '.perf', '.data', '.dat']
# ================================================================== PARAMETERS
# =============================================================================

# =============================================================================
# =================================================================== FUNCTIONS

def getFileNames(currentPath, fileNames):
	if os.path.isdir(currentPath):
		if not os.path.exists(currentPath.replace(args.refsPath, args.resultsPath)):
			os.makedirs(currentPath.replace(args.refsPath, args.resultsPath))

		files = os.listdir(currentPath)
		for f in files:
			if "~" in f:
				continue
			newCurrentPath = currentPath + "/" + f
			if os.path.isdir(newCurrentPath):
				if args.recursiveScan:
					getFileNames(newCurrentPath, fileNames)
			else:
				getFileNames(newCurrentPath, fileNames)
	elif os.path.isfile(currentPath):
		if pathlib.Path(currentPath).suffix in extensions:
			if args.refsPath == currentPath:
				basename = os.path.basename(args.refsPath)
				dirname = args.refsPath.replace(basename, '')
				args.refsPath = dirname
				fileNames.append(basename)
			else:
				shortenPath = currentPath.replace(args.refsPath + "/", "")
				shortenPath = shortenPath.replace(args.refsPath,       "")
				fileNames.append(shortenPath)
	else:
		print("# (WW) The path '", currentPath, "' does not exist.")

# =================================================================== FUNCTIONS
# =============================================================================

# =============================================================================
# ======================================================================== MAIN

#parser.print_help()
args = parser.parse_args()

if args.startId <= 0:
	args.startId = 1

print('# AFF3CT tests')
print('# ------------')
print('#')
print('# Parameters:')
print('# refs path      =', args.refsPath     )
print('# results path   =', args.resultsPath  )
print('# build path     =', args.buildPath    )
print('# start id       =', args.startId      )
print('# recursive scan =', args.recursiveScan)
print('# verbose        =', args.verbose      )
print('#')


PathOrigin = os.getcwd()

if os.path.isfile(args.resultsPath):
	print("# (EE) The results path should not be an existing file.")
	sys.exit(1);

# auto create the result folder if it does not exist
if not os.path.exists(args.resultsPath):
	os.makedirs(args.resultsPath)


# get the filenames to test
fileNames = []
getFileNames(args.refsPath, fileNames)

if (len(fileNames) - (args.startId -1) > 0) :
	print("# (II) Starting the update refs header script...")
else:
	print("# (WW) There is no reference to update.")



failIds = []
nErrors = 0
refId   = 0

for fn in fileNames:
	if refId < args.startId -1:
		refId += 1
		continue

	print("Ref n°" + str(refId+1) + " / " + str(len(fileNames)) +
	      " - " + fn, end="", flush=True);

	# parse the reference file
	refFile = arr.readFileInTable(args.refsPath + "/" + fn)
	refData = arr.aff3ctRefsReader(refFile)

	refCommand = refData.getSplitCommand()
	refCommand += ["-t", "1", "--sim-max-frame", "1"]


	# run the simulator
	os.chdir(args.buildPath)
	startTime = time.time()
	try:
		processAFFECT = subprocess.Popen(refCommand, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()
	except KeyboardInterrupt:
		os.kill(processAFFECT.pid, signal.SIGINT)
		(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()

	returnCode = processAFFECT.returncode
	errAndWarnMessages = stderrAFFECT.decode(encoding='UTF-8')


	# update ref with new header
	os.chdir(PathOrigin)

	# get the results
	newFile = stdoutAFFECT.decode(encoding='UTF-8').split("\n")
	newData = arr.aff3ctRefsReader(newFile)


	# result file
	fRes = open(args.resultsPath + "/" + fn, 'w+')

	if returnCode:
		print(" - ERROR.", end="\n");
		if errAndWarnMessages:
			print("---- Error message(s):", end="\n");
			print(errAndWarnMessages)
		nErrors += 1
		failIds.append(refId +1)


		for i in range(len(newFile)):
			fRes.write(newFile[i] + "\n")
		for i in range(len(errAndWarnMessages)):
			fRes.write(errAndWarnMessages[i] + "\n")

	else:
		fRes.write(refData.getMetadataAsString())

		i = 0
		while( i < len(newFile)):
			l = newFile[i]
			if l.startswith("# The simulation is running..."):
				break
			fRes.write(l + "\n")
			i += 1 # to next line

		i = 0
		dump = False
		while( i < len(refFile)):
			l = refFile[i]
			if dump:
				fRes.write(l + "\n")
			elif l.startswith("# The simulation is running..."):
				dump = True
			i += 1 # to next line

		fRes.flush()
		print(" - OK.", end="\n");

	refId += 1


if len(fileNames) - (args.startId -1) > 0:
	print("\n# " + str(refId-nErrors) + " refs updated, " + str(nErrors) + " updates failed.", end="\n")

	if nErrors == 0:
		print("\n# (II) All refs UPDATED !", end="");
	else:
		print("\n# (II) FAILED updates : ", end="")
		f = 0
		for failId in failIds:
			print("n°", end="")
			print(str(failId), end="")
			if f == len(failIds) -1:
				print(".", end="")
			else:
				print(", ", end="")
			f = f + 1

	print("\n")

