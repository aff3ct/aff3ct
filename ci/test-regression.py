#!/usr/bin/env python3

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

# ==================================================================== PACKAGES
# =============================================================================

# =============================================================================
# ================================================================== PARAMETERS

parser = argparse.ArgumentParser(prog='aff3ct-test-regression', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('--refs-path',      action='store', dest='refsPath',      type=str,   default="refs",                    help='Path to the references to re-simulate.')
parser.add_argument('--results-path',   action='store', dest='resultsPath',   type=str,   default="test-regression-results", help='Path to the simulated results.')
parser.add_argument('--build-path',     action='store', dest='buildPath',     type=str,   default="build",                   help='Path to the AFF3CT build.')
parser.add_argument('--start-id',       action='store', dest='startId',       type=int,   default=1,                         help='Starting id to avoid computing results one again.')                                       # choices=xrange(1,   +inf)
parser.add_argument('--sensibility',    action='store', dest='sensibility',   type=float, default=1.0,                       help='Sensibility on the difference between new vs ref to verify a noise point.')                    # choices=xrange(1.0, +inf)
parser.add_argument('--n-threads',      action='store', dest='nThreads',      type=int,   default=0,                         help='Number of threads to use in the simulation (0 = all available).')                         # choices=xrange(0,   +ing)
parser.add_argument('--recursive-scan', action='store', dest='recursiveScan', type=bool,  default=True,                      help='If enabled, scan the path of refs recursively.')
parser.add_argument('--max-fe',         action='store', dest='maxFE',         type=int,   default=100,                       help='Maximum number of frames errors to simulate per noise point.')                            # choices=xrange(0,   +inf)
parser.add_argument('--min-fe',         action='store', dest='minFE',         type=int,   default=50,                        help='Minimum number of frames errors to take into account the simulated noise point.')         # choices=xrange(0,   +inf)
parser.add_argument('--weak-rate',      action='store', dest='weakRate',      type=float, default=0.8,                       help='Rate of valid noise points to passe a test.')                                             # choices=xrange(0.0, 1.0 )
parser.add_argument('--max-snr-time',   action='store', dest='maxSNRTime',    type=int,   default=600,                       help='The maximum amount of time to spend to compute a noise point in seconds (0 = illimited)') # choices=xrange(0,   +inf)
parser.add_argument('--verbose',        action='store', dest='verbose',       type=bool,  default=False,                     help='Enable the verbose mode.')
parser.add_argument('--mpi-np',         action='store', dest='mpinp',         type=int,   default=0,                         help='Enable MPI run with the given number of process each running on "--n-threads" threads.')
parser.add_argument('--mpi-host',       action='store', dest='mpihost',       type=str,   default="",                        help='Run MPI with the given hosts (file).')

# supported file extensions (filename suffix)
extensions     = ['.txt', '.perf', '.data', '.dat']
NoiseTypeList  = ["Eb/N0", "Es/N0", "ROP", "EP"]
NoiseUnityList = ["dB",    "dB",    "dB",  ""  ]


# ================================================================== PARAMETERS
# =============================================================================

# =============================================================================
# =================================================================== FUNCTIONS
def format_e(n):
	a = '%.2e' % n
	return a

def splitFloat(n):
	s = format_e(n).split('e')
	return [float(s[0]), int(s[1])]


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

def getLegend(line):
	line = line.replace("#", "")
	line = line.replace("||", "|")
	line = line.split('|')
	for i in range(len(line)):
		line[i] = line[i].strip()

	return line

def getVal(line):
	line = line.replace("#", "")
	line = line.replace("||", "|")
	line = line.split('|')

	valLine = []
	for i in range(len(line)):
		val = float(0.0)

		try:
			val = float(line[i])

			if "inf" in str(val):
				val = float(0.0)

		except ValueError:
			pass

		valLine.append(val)

	return valLine

def getLegendIdx(legend, colName):
	for i in range(len(legend)):
		if legend[i] == colName:
			return i
	return -1

class simuData:
	def __init__(self):
		self.Legend     = []
		self.All        = []
		self.Noise      = []
		self.BER        = []
		self.FER        = []
		self.MI         = []
		self.FE         = []
		self.RunCommand = ""
		self.CurveName  = ""
		self.NoiseType  = ""

def dataReader(aff3ctOutput):
	data = simuData()

	for line in aff3ctOutput:
		if line.startswith("#"):
			if len(line) > 20 and (line.find("FRA |") != -1 or line.find("BER |") != -1 or line.find("FER |") != -1):
				data.Legend = getLegend(line)

		else:
			if len(data.Legend) != 0:
				d = getVal(line)
				if len(d) == len(data.Legend):
					data.All.append(d)


	data.All = np.array(data.All).transpose()

	# get the command to to run to reproduce this trace
	if len(aff3ctOutput) >= 2 and "Run command:" in aff3ctOutput[0]:
		data.RunCommand = str(aff3ctOutput[1].strip())
	elif len(aff3ctOutput) >= 4 and "Run command:" in aff3ctOutput[2]:
		data.RunCommand = str(aff3ctOutput[3].strip())
	else:
		data.RunCommand = ""

	# get the curve name (if there is one)
	if len(lines) >= 2 and "Curve name:" in lines[0]:
		data.CurveName = str(lines[1].strip())
	elif len(lines) >= 4 and "Curve name:" in lines[2]:
		data.CurveName = str(lines[3].strip())
	else:
		data.CurveName = ""


	# find the type of noise used in this simulation
	idx = -1
	for n in NoiseTypeList:
		idx = getLegendIdx(data.Legend, n)
		if idx != -1:
			data.NoiseType = n
			break

	if len(data.All) and idx != -1 :
		# set noise range
		data.Noise = data.All[idx]

		# set BER
		idx = getLegendIdx(data.Legend, "BER")
		if idx == -1:
			data.BER = [0 for x in range(len(data.Noise))]
		else:
			data.BER = data.All[idx]

		# set FER
		idx = getLegendIdx(data.Legend, "FER")
		if idx == -1:
			data.FER = [0 for x in range(len(data.Noise))]
		else:
			data.FER = data.All[idx]

		# set MI
		idx = getLegendIdx(data.Legend, "MI")
		if idx == -1:
			data.MI = []
		else:
			data.MI = data.All[idx]

		# set FE
		idx = getLegendIdx(data.Legend, "FE")
		if idx == -1:
			data.FE = []
		else:
			data.FE = data.All[idx]

		# set ESN0
		idx = getLegendIdx(data.Legend, "Es/N0")
		if idx == -1 or data.NoiseType == "Es/N0":
			data.ESN0 = []
		else:
			data.ESN0 = data.All[idx]

	return data

def splitAsCommand(runCommand):
	# split the run command
	argsList = [""]
	idx = 0

	new = 0
	found_dashes = 0
	for s in runCommand:
		if found_dashes == 0:
			if s == ' ':
				if new == 0:
					argsList.append("")
					idx = idx + 1
					new = 1

			elif s == '\"':
				if new == 0:
					argsList.append("")
					idx = idx + 1
					new = 1
				found_dashes = 1

			else:
				argsList[idx] += s
				new = 0

		else: # between dashes
			if s == '\"':
				argsList.append("")
				idx = idx + 1
				found_dashes = 0

			else:
				argsList[idx] += s

	del argsList[idx]

	return argsList

class tableStats:
	def __init__(self, tableCur, tableRef, sensibility, name, nData = 0):
		if len(tableCur) > len(tableRef) : # can't have less ref than checked points
			raise RuntimeError

		self.tableCur        = tableCur
		self.tableRef        = tableRef
		self.name            = name
		self.sensibility     = sensibility
		self.minSensibility  = float('inf')
		self.maxSensibility  = 0
		self.sumSensibility  = 0
		self.avgSensibility  = 0
		self.rateSensibility = 0
		self.valid           = 0

		if nData == 0 or nData > len(tableCur) :
			self.nData       = len(tableCur)
		else :
			self.nData       = nData
		self.errorsList      = []
		self.errorsPos       = []
		self.errorsMessage   = []

		self.makeStats()

	def makeStats(self):
		for i in range(self.nData):
			c = splitFloat(self.tableCur[i])
			r = splitFloat(self.tableRef[i])


			# compute the distance between the two points normalized to the unity with the lowest power
			if c[1] < r[1]:
				pw = c[1]
			else:
				pw = r[1]

			diff = math.fabs(self.tableCur[i] - self.tableRef[i]) * 10**(-pw)

			if diff > self.sensibility:
				self.errorsPos    .append(i)
				self.errorsList   .append([self.tableCur[i], self.tableRef[i]])
				self.errorsMessage.append(format_e(self.tableRef[i]))
			else:
				self.valid += 1
				self.errorsMessage.append("")

			self.sumSensibility += diff
			self.minSensibility  = min(self.minSensibility, diff)
			self.maxSensibility  = max(self.maxSensibility, diff)

		if self.nData != 0:
			self.avgSensibility = self.sumSensibility / float(self.nData)
		else:
			self.avgSensibility = 0

		if self.sensibility != 0:
			self.rateSensibility = (self.avgSensibility / self.sensibility) * 100
		else:
			self.rateSensibility = float(0)


	def errorMessage(self, idx):
		if len(self.errorsMessage) > idx and len(self.errorsMessage[idx]):
			return self.name + "=" + self.errorsMessage[idx]
		else:
			return ""

	def hasError(self):
		return self.valid != self.nData

	def passRate(self):
		if self.nData == 0:
			raise RuntimeError

		return float(self.valid) / float(self.nData)

class compStats:
	def __init__(self, dataCur, dataRef, sensibility, asked_n_fe):
		if not isinstance(dataCur, simuData) or not isinstance(dataRef, simuData) :
			raise TypeError

		self.nValidData = len(dataCur.FE)
		for d in range(len(dataCur.FE)) :
			if dataCur.FE[d] < asked_n_fe :
				self.nValidData = d


		self.dataCur  = dataCur
		self.dataRef  = dataRef
		self.dataList = []
		self.dataList.append(tableStats(dataCur.Noise, dataRef.Noise,         0.0, dataRef.NoiseType, self.nValidData))
		self.dataList.append(tableStats(dataCur.ESN0,  dataRef.ESN0,          0.0, "Es/N0"          , self.nValidData))
		self.dataList.append(tableStats(dataCur.FER,   dataRef.FER,   sensibility, "FER"            , self.nValidData))
		self.dataList.append(tableStats(dataCur.BER,   dataRef.BER,   sensibility, "BER"            , self.nValidData))
		self.dataList.append(tableStats(dataCur.MI,    dataRef.MI,    sensibility, "MI"             , self.nValidData))

	def errorMessage(self, idx):
		message = ""
		for d in self.dataList:
			dm = d.errorMessage(idx)
			if len(dm):
				message += dm + " "

		if len(message):
			return "WRONG! " + message
		else:
			return ""

	def getResumeTable(self):
		message = ""
		if len(self.dataCur.Noise) == 0:
			return message

		# get noise unity
		unity = ""
		idx = NoiseTypeList.index(self.dataRef.NoiseType)
		if idx != -1:
			unity = NoiseUnityList[idx]
		if len(unity):
			unity = " " + unity

		message += "---- Details: Noise type = " + self.dataRef.NoiseType
		message += ", first noise point = " + format_e(self.dataRef.Noise[0]) + unity
		message += ", last noise point = " + format_e(self.dataRef.Noise[len(self.dataRef.Noise) -1]) + unity + "\n"
		message += "---- -------|-------------------------|--------------|-------------------------------- \n"
		message += "----   name |     ranges [first,last] | valid points | sensibility [avg,min,max,rate]  \n"
		message += "---- -------|-------------------------|--------------|-------------------------------- \n"

		for d in self.dataList:
			if len(d.tableCur):
				message += "----" + d.name.rjust(7) + " "
				message += "| ["  + (format_e(d.tableCur[0])).rjust(10) + "," + (format_e(d.tableCur[len(d.tableCur) -1])).rjust(10) + "] "
				message += "| "   + (str(d.valid) + "/" + str(d.nData)).rjust(12) + " "
				message += "|  [ " + ("%.2f" % d.avgSensibility + ", %.2f" % d.minSensibility + ", %.2f" % d.maxSensibility + ", " + ("%.2f" % d.rateSensibility).rjust(6) + "%") + " ] "
				message += "\n"

		if self.hasError():
			message += "---- -------|------------------------------------------------------------------------- \n"
			message += "----   name | errors list {noise: new -> ref}                                          \n"
			message += "---- -------|------------------------------------------------------------------------- \n"
			for d in self.dataList:
				if d.hasError():
					message += "----" + d.name.rjust(7) + " | "
					el = 0
					for e in d.errorsList:
						if el != 0 and (el % 2) == 0 :
							message += "\n----        | "

						message += "{" + format_e(self.dataCur.Noise[d.errorsPos[el]]) + ": " + format_e(e[0]) + " -> " + format_e(e[1]) + "}"

						el += 1
						if el < len(d.errorsList):
							message += ", "
					message += "\n"

		return message

	def hasError(self):
		hasError = False
		for d in self.dataList:
			hasError = hasError or d.hasError()
		return hasError

	def passRate(self):
		passRate = float(0)

		nGoodList = 0
		for d in self.dataList:
			try:
				passRate += d.passRate()
				nGoodList += 1
			except RuntimeError:
				pass

		if nGoodList:
			return passRate / float(nGoodList)
		else:
			return passRate

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
print('# sensibility    =', args.sensibility  )
print('# n threads      =', args.nThreads     )
print('# recursive scan =', args.recursiveScan)
print('# max fe         =', args.maxFE        )
print('# weak rate      =', args.weakRate     )
print('# max snr time   =', args.maxSNRTime   )
print('# verbose        =', args.verbose      )
print('# MPI nbr proc   =', args.mpinp        )
print('# MPI host file  =', args.mpihost      )
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
	print("# (II) Starting the test script...")
else:
	print("# (WW) There is no simulation to replay.")



argsAFFECTcommand = []

if args.mpinp > 0 or args.mpihost != "":
	argsAFFECTcommand += ["mpirun", "--map-by", "socket"]

	if args.mpinp > 0:
		argsAFFECTcommand += ["-np", str(args.mpinp)]

	if args.mpihost != "":
		argsAFFECTcommand += ["--hostfile", str(args.mpihost)]


failIds = []
nErrors = 0
testId = 0


for fn in fileNames:
	if testId < args.startId -1:
		testId = testId + 1
		continue

	print("Test n°" + str(testId+1) + " / " + str(len(fileNames)) +
	      " - " + fn, end="", flush=True);

	# open the file in read mode (from the fileName "fn" and the path)
	f = open(args.refsPath + "/" + fn, 'r')

	# read all the lines from the reference file f
	lines = []
	for line in f:
		lines.append(line)

	f.close()

	# parse the reference file
	simuRef = dataReader(lines)


	# get the command line to run
	argsAFFECT = argsAFFECTcommand[:]
	argsAFFECT += splitAsCommand(simuRef.RunCommand)

	argsAFFECT += ["--ter-freq", "0", "-t", str(args.nThreads), "--sim-no-colors"]
	if args.maxFE:
		argsAFFECT += ["-e", str(args.maxFE)]

	if args.maxSNRTime:
		argsAFFECT += ["--sim-stop-time", str(args.maxSNRTime)]


	noiseVals = ""
	for n in range(len(simuRef.Noise)):
		if n != 0:
			noiseVals += ",";
		noiseVals += str(simuRef.Noise[n])


	argsAFFECT += ["-R", noiseVals]

	if simuRef.NoiseType == "Eb/N0":
		argsAFFECT += ["-E", "EBN0"]


	# run the tested simulator
	os.chdir(args.buildPath)
	startTime = time.time()
	try:
		processAFFECT = subprocess.Popen(argsAFFECT, stdout=subprocess.PIPE,
		                                             stderr=subprocess.PIPE)
		(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()
	except KeyboardInterrupt:
		os.kill(processAFFECT.pid, signal.SIGINT)
		(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()


	returnCode = processAFFECT.returncode
	elapsedTime = time.time() - startTime

	errAndWarnMessages = stderrAFFECT.decode(encoding='UTF-8')


	# compare ref and new results to check if the simulator is still OK
	os.chdir(PathOrigin)

	# get the results
	stdOutput = stdoutAFFECT.decode(encoding='UTF-8').split("\n")
	simuCur = dataReader(stdOutput)

	# result file
	fRes = open(args.resultsPath + "/" + fn, 'w+')

	if returnCode:
		print(" - ABORTED.", end="\n");
		if errAndWarnMessages:
			print("---- Error message(s):", end="\n");
			print(errAndWarnMessages)
		nErrors += 1
		failIds.append(testId +1)

		for i in range(len(stdOutput)):
			fRes.write(stdOutput[i] + "\n")
		for i in range(len(errAndWarnMessages)):
			fRes.write(errAndWarnMessages[i] + "\n")

	elif simuCur.NoiseType != simuRef.NoiseType:
		nErrors += 1
		failIds.append(testId +1)

		print(" - NOISE TYPE MISMATCH: " + simuRef.NoiseType + " vs " + simuCur.NoiseType + ".", end="\n");

		for i in range(len(stdOutput)):
			fRes.write(stdOutput[i] + "\n")
		for i in range(len(errAndWarnMessages)):
			fRes.write(errAndWarnMessages[i] + "\n")

	else:
		# parse the results to validate (or not) the BER/FER/MI performance
		comp = compStats(simuCur, simuRef, args.sensibility, args.minFE)

		# print the header
		fRes.write("Run command:\n" + simuCur.RunCommand + "\n")
		fRes.write("Curve name:\n"  + simuCur.CurveName  + "\n")

		# print the parameters directly and add to the wrong data the ref values
		idxNoise = 0
		i = 0
		while( i < len(stdOutput)):
			l = stdOutput[i]
			if l.startswith("#"):
				if "# End of the simulation." not in l:
					fRes.write(l + "\n")

			elif l == "Run command:" or l == "Curve name:":
				i += 1 # ignore the following line

			else:
				em = comp.errorMessage(idxNoise)
				fRes.write(l + em + "\n")
				idxNoise += 1

			i += 1 # to next line


		fRes.flush()

		# validate (or not) the BER/FER/MI performance
		print(" - %.2f" %elapsedTime, "sec", end="")

		passRate = comp.passRate()

		if passRate == float(1):
			print(" - STRONG PASSED.", end="\n");
		elif passRate >= args.weakRate:
			print(" - WEAK PASSED (rate = %.2f" %passRate, ").", end="\n");
		else:
			print(" - FAILED (rate = %.2f" %passRate, ").", end="\n");
			nErrors = nErrors +1
			failIds.append(testId +1)

		if args.verbose:
			print (comp.getResumeTable())

	if not returnCode and errAndWarnMessages:
		print("---- Warning message(s):", end="\n");
		print(errAndWarnMessages)
		fRes.write(errAndWarnMessages)

	fRes.write("# End of the simulation.\n")
	fRes.close();

	testId = testId + 1


	if elapsedTime < 0.5:
		break;



if len(fileNames) - (args.startId -1) > 0:
	if nErrors == 0:
		print("\n# (II) All the tests PASSED !", end="\n");
	else:
		print("\n# (II) Some tests FAILED: ", end="")
		f = 0
		for failId in failIds:
			print("n°", end="")
			print(str(failId), end="")
			if f == len(failIds) -1:
				print(".", end="\n")
			else:
				print(", ", end="")
			f = f + 1

sys.exit(nErrors);

# ======================================================================== MAIN
# =============================================================================
