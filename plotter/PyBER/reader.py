# The MIT License (MIT)
#
# Copyright (c) 2016 PyBER
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.

import os

def getVal(line, idColumn, EsEb):
	# there are two different file formats, classic (lines begni with SNR) and new (lines begin with SNR value directly)
	# indices of the different parameter change, and the following array is used to convert classic indices to new
	convert_to_v1 = [0, 4, 5, 12, 0, 3, 2]
	convert_to_v2 = [0, 4, 5, 9 , 0, 3, 2]
	convert_to_v3 = [0, 4, 5, 6 , 0, 3, 2]

	if (EsEb == "Es"):
		convert_to_v4 = [0, 5, 6, 10, 1, 4, 3]
		convert_to_v5 = [0, 5, 6, 7 , 1, 4, 3]
	else:
		convert_to_v4 = [1, 5, 6, 10, 1, 4, 3]
		convert_to_v5 = [1, 5, 6, 7 , 1, 4, 3]

	# classic
	if line.startswith("SNR = "):
		line = line.replace("#", "")
		line = line.replace("=", "")
		line = line.replace("SNR", "")
		line = line.replace("BER", "")
		line = line.replace("FER", "")
		line = line.replace("BE/FE", "")
		line = line.replace("BPS", "")
		line = line.replace("BPS", "")
		line = line.replace("MATRICES", "")
		line = line.replace("BE", "")
		line = line.replace("FE", "")
		line = line.replace("RUNTIME", "")
		line = line.replace("ERR", "")
		line = line.replace(" ", "")
		line = line.split('|')

		val = float(line[idColumn])
	# new
	else:
		line = line.replace("||", "|")
		line = line.split('|')

		try:
			float(line[0])  # in cases where the first item of the line is neither "SNR = " nor a float
		except ValueError:
			return float(-999.0)

		if(len(line) == 14):
			val = float(line[convert_to_v1[idColumn]])
		elif(len(line) == 11):
			val = float(line[convert_to_v2[idColumn]])
		elif(len(line) == 8):
			val = float(line[convert_to_v3[idColumn]])
		elif(len(line) == 12):
			val = float(line[convert_to_v4[idColumn]])
		elif(len(line) == 9):
			val = float(line[convert_to_v5[idColumn]])

	if "inf" in str(val):
		val = float(0.0)

	return val

def dataReader(filename, EsEb):
	# read all the lines from the current file
	aFile = open(filename, "r")
	lines = []
	for line in aFile:
		lines.append(line)
	aFile.close()

	dataSNR  = []
	dataBER  = []
	dataFER  = []
	dataBEFE = []
	dataThr  = []
	dataDeta = []
	dataName = []

	for line in lines:
		if line.startswith("#"):
			if len(line) > 3 and line[0] == '#' and line[2] == '*':
				entry = line.replace("# * ", "").replace("\n", "").split(" = ")
				if len(entry) == 1:
					entry[0] = entry[0].replace("-", "")
				dataDeta.append(entry)
			elif len(line) > 7 and line[0] == '#' and line[5] == '*' and line[6] == '*':
				entry = line.replace("#    ** ", "").replace("\n", "").split(" = ")
				dataDeta.append(entry)
		else:
			snr = getVal(line, 0, EsEb)
			if snr == -999.0:
				continue # line is ignored

			dataSNR.append(snr)
			dataBER.append(getVal(line, 1, EsEb))
			dataFER.append(getVal(line, 2, EsEb))
			dataThr.append(getVal(line, 3, EsEb))

			be = getVal(line, 6, EsEb)
			fe = getVal(line, 5, EsEb)

			if fe == 0:
				dataBEFE.append(0.0)
			else :
				dataBEFE.append(be/fe)

	dataDeta.append(["Other"])
	dataDeta.append(["File name", os.path.basename(filename)])

	# get the command to to run to reproduce this trace
	if lines and "Run command:" in lines[0]:
		dataDeta.append(["Run command", str(lines[1].strip())])
	else:
		dataDeta.append(["Run command", ""])
	if lines and "Run command:" in lines[2]:
		dataDeta.append(["Run command", str(lines[3].strip())])
	else:
		dataDeta.append(["Run command", ""])

	# get the curve name (is there is one)
	if lines and "Curve name:" in lines[0]:
		dataName = str(lines[1].strip())
	if lines and "Curve name:" in lines[2]:
		dataName = str(lines[3].strip())

	return dataSNR, dataBER, dataFER, dataBEFE, dataThr, dataDeta, dataName
