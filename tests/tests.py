#!/usr/bin/python3

# =============================================================================
# ================================================================== PARAMETERS

PathTests     = "data"     # can be overrided by arg1 from the cmd line
PathResults   = "results"  # can be overrided by arg2 from the cmd line
PathBuild     = "../build"
Sensibility   = 1.0
Nthreads      = 8
RecursiveScan = True
MaxFE         = 100
WeakRate      = 0.8        # 0 < WeakRate < 1
Retry         = 0          # unimplemented for now

# ================================================================== PARAMETERS
# =============================================================================

# =============================================================================
# ==================================================================== PACKAGES

import os
import sys
import math
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

def printLine(simuType, codeType, moduType, N, K, snrMin, snrMax, prec, decType, decImplem, decSIMD, testId, state, separator):
	print(repr(simuType   ).rjust( 9), end="  ",           flush=True)
	print(repr(codeType   ).rjust(11), end="  ",           flush=True)
	print(repr(moduType   ).rjust(19), end="  ",           flush=True)
	print(repr(N          ).rjust( 6), end="  ",           flush=True)
	print(repr(K          ).rjust( 6), end="  ",           flush=True)
	print(repr(snrMin     ).rjust( 7), end="  ",           flush=True)
	print(repr(snrMax     ).rjust( 7), end="  ",           flush=True)
	print(repr(prec       ).rjust( 4), end="  ",           flush=True)
	print(repr(decType    ).rjust(14), end="  ",           flush=True)
	print(repr(decImplem  ).rjust(14), end="  ",           flush=True)
	print(repr(decSIMD    ).rjust( 8), end="  ",           flush=True)
	print(repr(testId     ).rjust( 7), end="  ",           flush=True)
	print(str (state      ).rjust(13), end=str(separator), flush=True)

# =================================================================== FUNCTIONS
# =============================================================================

# =============================================================================
# ======================================================================== MAIN

if len(sys.argv) >= 2:
	PathTests = sys.argv[1]

if len(sys.argv) >= 3:
	PathResults = sys.argv[2]

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

# print the legend
print(str("SIMU_TYPE"  ).rjust( 9), end="  ", flush=True)
print(str("CODE_TYPE"  ).rjust(11), end="  ", flush=True)
print(str("MODU_TYPE"  ).rjust(19), end="  ", flush=True)
print(str("N"          ).rjust( 6), end="  ", flush=True)
print(str("K"          ).rjust( 6), end="  ", flush=True)
print(str("SNR-MIN"    ).rjust( 7), end="  ", flush=True)
print(str("SNR-MAX"    ).rjust( 7), end="  ", flush=True)
print(str("PREC"       ).rjust( 4), end="  ", flush=True)
print(str("DEC-TYPE"   ).rjust(14), end="  ", flush=True)
print(str("DEC-IMPLEM" ).rjust(14), end="  ", flush=True)
print(str("DEC-SIMD"   ).rjust( 8), end="  ", flush=True)
print(str("TEST-ID"    ).rjust( 7), end="  ", flush=True)
print(str("TEST-RESULT").rjust(13), end="\n", flush=True)

print(str("---------"  ).rjust( 9), end="  ", flush=True)
print(str("---------"  ).rjust(11), end="  ", flush=True)
print(str("---------"  ).rjust(19), end="  ", flush=True)
print(str("-"          ).rjust( 6), end="  ", flush=True)
print(str("-"          ).rjust( 6), end="  ", flush=True)
print(str("-------"    ).rjust( 7), end="  ", flush=True)
print(str("-------"    ).rjust( 7), end="  ", flush=True)
print(str("----"       ).rjust( 4), end="  ", flush=True)
print(str("--------"   ).rjust(14), end="  ", flush=True)
print(str("----------" ).rjust(14), end="  ", flush=True)
print(str("--------"   ).rjust( 8), end="  ", flush=True)
print(str("-------"    ).rjust( 7), end="  ", flush=True)
print(str("-----------").rjust(13), end="\n", flush=True)

testId = 0
for fn in fileNames:

	# default caracteristics of the current test
	simuType  = "UNDEF"
	codeType  = "UNDEF"
	moduType  = "UNDEF"
	N         = 0
	K         = 0
	snrMin    = 0.0
	snrMax    = 0.0
	prec      = 0
	decSIMD   = "UNDEF"
	decType   = "UNDEF"
	decImplem = "UNDEF"
	maxFE     = 100

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
		if l != "" and l[0] == '#':
			if "# * " in l:
				currentSection = l.replace("#", "").replace("*", "").replace("-", "").replace(" ", "").replace("\n", "")
			else:
				if "#    ** " in l:
					if ("Simulation" in currentSection) and ("Type     " in l):
						tmp = l.replace(" ", "").replace("\n", "").split("=")
						simuType = tmp[1]
					elif ("Simulation" in currentSection) and ("SNR min" in l):
						tmp = l.replace("dB", "").replace(" ", "").split("=")
						snrMin = float(tmp[1])
					elif ("Simulation" in currentSection) and ("SNR max" in l):
						tmp = l.replace("dB", "").replace(" ", "").split("=")
						snrMax = float(tmp[1])
					elif ("Simulation" in currentSection) and ("Type of bits" in l):
						if "long long" in l:
							prec = 64
						elif "int" in l:
							prec = 32
						elif "short" in l:
							prec = 16
						elif "signed char" in l:
							prec = 8
					elif ("Code" in currentSection) and ("Type" in l):
						tmp = l.replace("codes", "").replace(" ", "").strip().split("=")
						codeType = tmp[1]
					elif ("Code" in currentSection) and ("Info. bits" in l):
						tmp = l.replace(" ", "").split("=")
						K = int(tmp[1])
					elif ("Code" in currentSection) and ("Codeword size" in l):
						tmp = l.replace(" ", "").split("=")
						tmp2 = tmp[1].split("+")
						N = int(tmp2[0])
					elif ("Modulator" in currentSection) and ("Type" in l):
						tmp = l.replace(" ", "").replace("\n", "").split("=")
						moduType = tmp[1]
					elif ("Decoder" in currentSection) and ("Type" in l):
						tmp = l.replace(" ", "").strip().split("=")
						decType = tmp[1]
					elif ("Decoder" in currentSection) and ("Implementation" in l):
						tmp = l.replace(" ", "").strip().split("=")
						decImplem = tmp[1]
					elif ("Decoder" in currentSection) and ("SIMD strategy" in l):
						tmp = l.replace(" ", "").strip().split("=")
						decSIMD = tmp[1]
					elif ("Monitor" in currentSection) and ("Frame error count" in l):
						tmp = l.replace(" ", "").split("=")
						maxFE = int(tmp[1])

		# avoid the first lines and the comments
		if idx > 6 and l.replace(" ", "") != "" and l.replace(" ", "") != "\n" and l[0] != '#':
			simuRef.append(l.strip().replace("||", "|").replace(" ", "").split("|"))	
		idx = idx +1

	f.close()

	# run the tests
	printLine(simuType, codeType, moduType, N, K, snrMin, snrMax, prec, decType, decImplem, decSIMD, testId, "RUNNING", "\r")

	argsAFFECT = []
	argsAFFECT = runCommand.split(" ")

	argsAFFECT.append("--term-freq")
	argsAFFECT.append("0")
	argsAFFECT.append("--mnt-max-fe")
	argsAFFECT.append(str(MaxFE))
	argsAFFECT.append("--sim-threads")
	argsAFFECT.append(str(Nthreads))

	os.chdir(PathBuild)
	processAFFECT = subprocess.Popen(argsAFFECT, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
	(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()

	# begin to write the results into a file
	os.chdir(PathOrigin)

	fRes = open(PathResults + "/" + fn + "." + str(testId), 'w+')
	fRes.write("Run command: \n")
	fRes.write(runCommand + "\n")
	fRes.write("Trace: \n")

	# parse the results
	stdOutput = stdoutAFFECT.decode(encoding='UTF-8').split("\n")
	outputAFFECTLines = []
	simuCur = []
	for l in stdOutput:
		if l != "" and l[0] != '#': # avoid the first lines and the comments
			outputAFFECTLines.append(l)
			simuCur.append(l.strip().replace("||", "|").replace(" ", "").split("|"))
		elif l != "" and l[0] == '#' and "# End of the simulation." not in l:
			fRes.write(l + "\n")

	fRes.flush()

	# validate (or not) the BER/FER performance
	valid = 0;
	idx = 0
	for ref in simuRef:
		numRef = float(ref[5][0:4])
		powerRef = int(ref[5][6:8])
		try:
			numCur = float(simuCur[idx][5][0:4])
			powerCur = int(simuCur[idx][5][6:8])	
		except IndexError: # no such line
			break

		if powerRef - powerCur != 0:
			if powerRef > powerCur:
				numCur = numCur * 10**(math.fabs(powerRef - powerCur))
			else:
				numRef = numRef * 10**(math.fabs(powerRef - powerCur))

		absoluteNumDiff = math.fabs(numRef - numCur)
		if absoluteNumDiff > Sensibility:
			fRes.write(outputAFFECTLines[idx] + "WRONG! FER=" + ref[5][0:8] + "\n")
		else:
			valid = valid + 1
			fRes.write(outputAFFECTLines[idx] + "\n")

		idx = idx + 1

	if valid == idx:
		printLine(simuType, codeType, moduType, N, K, snrMin, snrMax, prec, decType, decImplem, decSIMD, testId, "STRONG PASSED", "\n")
	elif idx != 0 and float(valid) / float(idx) >= WeakRate:
		printLine(simuType, codeType, moduType, N, K, snrMin, snrMax, prec, decType, decImplem, decSIMD, testId, "WEAK PASSED", "\n")
	else:
		printLine(simuType, codeType, moduType, N, K, snrMin, snrMax, prec, decType, decImplem, decSIMD, testId, "FAILED", "\n")

	fRes.write("# End of the simulation.\n")
	fRes.close();

	testId = testId + 1

# ======================================================================== MAIN
# =============================================================================
