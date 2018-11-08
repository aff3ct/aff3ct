import os
import sys
import pathlib
import argparse
import subprocess

import aff3ct_help_reader as ahr

parser = argparse.ArgumentParser(prog='aff3ct-command-conversion', formatter_class=argparse.ArgumentDefaultsHelpFormatter)
parser.add_argument('--build', action='store', dest='buildPath', type=str, default="build/", help='Build path to aff3ct.')
parser.add_argument('--dest', action='store', dest='destPath', default="doc/sphinx/source/simulator/usage/parameters", type=str, help='Destination path.')

args = parser.parse_args()

indent = "   "

def read_help(sim, code, prec = "32"):
	PathOrigin = os.getcwd()
	os.chdir(args.buildPath)

	# run the tested simulator
	listArgsAFFECT = ["./bin/aff3ct", "-H", "-C", code, "--sim-type", sim, "-p", prec];

	try:
		processAFFECT = subprocess.Popen(listArgsAFFECT, stdout=subprocess.PIPE, stderr=subprocess.PIPE)
		(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()
	except KeyboardInterrupt:
		os.kill(processAFFECT.pid, signal.SIGINT)
		(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()


	returnCode = processAFFECT.returncode

	errOutput = stderrAFFECT.decode(encoding='UTF-8')
	stdOutput = stdoutAFFECT.decode(encoding='UTF-8').split("\n")

	os.chdir(PathOrigin)

	return stdOutput;

def getLongestTag(tags):
	tmp = tags.split(',')

	for t in tmp:
		t.strip()

	tag = tmp[0]
	if len(tmp) > 1:
		if len(tmp[0]) < len(tmp[1]):
			tag = tmp[1]
	return tag

def getArgReference(refHeader, refBody):
	return ".. _" + refHeader + "-" + getLongestTag(refBody).strip('-') + ":\n\n"

def makeTableLine(col1_len, col2_len, horiz = "-", vert = "+"):
	text = vert
	for i in range(col1_len):
		text += horiz
	text += vert
	for i in range(col2_len):
		text += horiz
	text += vert + "\n"

	return text

def addSpaces(text, totalLength):
	if len(text) >= totalLength:
		return text

	for i in range(totalLength - len(text)):
		text += " "

	return text

def bubbleSort(aTagList):
    for passnum in range(len(aTagList)-1,0,-1):
        for i in range(passnum):
            if aTagList[i][0].strip('-').lower() > aTagList[i+1][0].strip('-').lower():
                temp = aTagList[i]
                aTagList[i] = aTagList[i+1]
                aTagList[i+1] = temp

def sortTags(moduleMap):
	reqList = []
	for tag in moduleMap:
		if tag == "name":
			continue
		if moduleMap[tag]["group"] == "required":
			reqList.append([tag, moduleMap[tag]])

	stdList = []
	for tag in moduleMap:
		if tag == "name":
			continue
		if moduleMap[tag]["group"] == "":
			stdList.append([tag, moduleMap[tag]])

	advList = []
	for tag in moduleMap:
		if tag == "name":
			continue
		if moduleMap[tag]["group"] == "advanced":
			advList.append([tag, moduleMap[tag]])


	bubbleSort(reqList)
	bubbleSort(stdList)
	bubbleSort(advList)

	tagList = reqList + stdList + advList

	return tagList

def write_module(moduleMap, path, reftag):
	file = open(path, 'w')

	text  = ".. _" + reftag + "-" + moduleMap["name"].replace(' ', '-').lower() + ":\n\n"
	text += moduleMap["name"] + "\n"

	for x in range(len(moduleMap["name"])):
		text += "-"

	text += "\n\n"

	file.write(text)


	for Arg in sortTags(moduleMap):
		tag = Arg[0]

		group    = Arg[1]["group"   ]
		argtype  = Arg[1]["argtype" ]
		limits   = Arg[1]["limits"  ]
		# required = Arg[1]["required"]
		# default  = Arg[1]["default" ]
		# needs    = Arg[1]["needs"   ]
		# excludes = Arg[1]["excludes"]
		info     = Arg[1]["info"    ]

		text  = getArgReference(reftag, tag)

		title = "``" + tag + "``"

		if group == "required" :
			title += " |image_required_argument|"

		elif group == "advanced" :
			title += " |image_advanced_argument|"

		text += title + "\n"
		for t in range(len(title)):
			text += '"'

		text +="\n\n"

		if argtype != "FLAG":
			value = argtype

			text += indent + ":Type: " + argtype + "\n"


		allowed_values_table = []

		if limits != "":

			__limits = ""
			pos = limits.find("{");
			if pos != -1:
				__limits  = ":Allowed values:";
				allowed_values_table = limits[pos+1:-1].split('|')
				for i in range(len(allowed_values_table)):
					allowed_values_table[i] = allowed_values_table[i].strip()

				for t in allowed_values_table:
					__limits += " ``" + t + "``"

			elif argtype == "folder" or argtype == "path" or argtype == "file":
				__limits = ":Rights: " + limits[1:-1]

			else:
				if limits == "positive":
					limits = "[0;+inf["
				elif limits == "positive, non-zero":
					limits = "]0;+inf["

				if __limits != "":
					__limits = ":Range: " + toLatex(limits)

			if __limits != "":
				text += indent + __limits + "\n"


		# if default != "":
		# 	text += indent + ":Default: " + default + "\n"


		# if len(needs):
		# 	text += indent + ":Needs: "
		# 	for n in needs:
		# 		text += "``" + n + "`` "
		# 	text += "\n"

		# if len(excludes):
		# 	text += indent + ":Excludes: "
		# 	for e in excludes:
		# 		text += "``" + e + "`` "
		# 	text += "\n"

		if argtype != "FLAG":
			exampleValue = "TODO"
			# if default == "":
			if len(allowed_values_table):
				exampleValue = allowed_values_table[0]
			elif argtype == "text":
				exampleValue = '"TODO CHECK VALUE"'
			elif argtype == "integer":
				exampleValue = "1"
			elif argtype == "real number":
				exampleValue = "1.0"
			elif argtype == "folder" or argtype == "path":
				exampleValue = "example/path/to/the/right/place/"
			elif argtype == "file":
				exampleValue = "example/path/to/the/right/file"

			text += indent + ":Examples: ``" + getLongestTag(tag) + " " + exampleValue + "``\n"

		text += "\n"

		info = info[0].upper() + info[1:]
		info = info.strip(" ").strip(".") + "."

		text += info.replace("--", "\\\\-\\\\-") + "\n\n"


		if len(allowed_values_table):
			text += "Description of the allowed values:\n\n"

			valueHead = " Value "
			descrHead = " Description "

			longestValue = len(valueHead)
			for v in allowed_values_table:
				l = len(v) + 6
				if l > longestValue :
					longestValue = l

			descrSubstitution  = getLongestTag(tag).strip('-') + "_descr_"
			longestDescription = len(descrHead)
			for v in allowed_values_table:
				l = len(v) + len(descrSubstitution) + 4
				if l > longestDescription :
					longestDescription = l

			text += makeTableLine(longestValue, longestDescription, "-", "+")
			text +=( "|" + addSpaces(valueHead, longestValue) +
			        "|" + addSpaces(descrHead, longestDescription) +
			        "|\n")
			text += makeTableLine(longestValue, longestDescription, "=", "+")

			for v in allowed_values_table:
				text +=("|" + addSpaces(" ``" + v + "`` ", longestValue) +
				        "|" + addSpaces(" |" + descrSubstitution + v.lower() + "| ", longestDescription) +
				        "|\n")
				text += makeTableLine(longestValue, longestDescription, "-", "+")

			text += "\n"
			for v in allowed_values_table:
				text += ".. |" + descrSubstitution + v.lower() + "| replace:: TODO VALUE " + v + "\n"

			text += "\n\n"



		file.write(text)

	file.close()

def write_codec_file(codecPath, codeName, hasPct):
	file = open(codecPath, 'w')

	text  = ".. _codec-" + codeName.lower() + ":\n\n"

	title = "Codec " + codeName
	text += title + "\n"
	for t in range(len(title)):
		text += '*'

	text += "\n\n"

	text += ".. toctree::\n"
	text += indent + ":maxdepth: 2\n"
	text += indent + ":caption: Codec " + codeName + " Contents\n\n"
	text += indent + "enc.rst\n"
	text += indent + "dec.rst\n"

	if hasPct:
		text += indent + "pct.rst\n"

	file.write(text)




if __name__ == "__main__":
	######## BFER simulation
	stdOutput = read_help("BFERI", "LDPC", "16");
	helpMap = ahr.help_to_map(stdOutput)


	####### write modules
	destPath = args.destPath
	if destPath[-1] != "/":
		destPath += "/"

	if not os.path.exists(destPath):
		os.makedirs(destPath)


	write_module(helpMap["Other"      ], destPath + "global.rst", "global");
	write_module(helpMap["Simulation" ], destPath + "sim.rst"   , "sim");
	write_module(helpMap["Source"     ], destPath + "src.rst"   , "src");
	write_module(helpMap["CRC"        ], destPath + "crc.rst"   , "crc");
	write_module(helpMap["Modem"      ], destPath + "mdm.rst"   , "mdm");
	write_module(helpMap["Channel"    ], destPath + "chn.rst"   , "chn");
	write_module(helpMap["Monitor"    ], destPath + "mnt.rst"   , "mnt");
	write_module(helpMap["Terminal"   ], destPath + "ter.rst"   , "ter");
	write_module(helpMap["Interleaver"], destPath + "itl.rst"   , "itl");
	write_module(helpMap["Quantizer"  ], destPath + "qnt.rst"   , "qnt");

	codesList = ["BCH", "LDPC", "POLAR", "RA", "REP", "RS", "RSC", "RSC_DB", "TURBO", "TURBO_DB", "TURBO_PROD", "UNCODED"]

	for c in codesList:
		stdOutput = read_help("BFER", c, "32");
		helpMap = ahr.help_to_map(stdOutput)


		codecPath = destPath + "cdc_" + c.lower() + "/"

		if not os.path.exists(codecPath):
			os.makedirs(codecPath)

		write_module(helpMap["Encoder"], codecPath + "enc.rst", "enc-" + c.lower());
		write_module(helpMap["Decoder"], codecPath + "dec.rst", "dec-" + c.lower());

		hasPct = False
		try:
			write_module(helpMap["Puncturer"], codecPath + "pct.rst", "pct-" + c.lower());
			hasPct = True
		except KeyError:
			pass

		write_codec_file(codecPath + "cdc.rst", c, hasPct)


	os.chdir(destPath)