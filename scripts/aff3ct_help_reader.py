import argparse
import sys

def __parse_argument(command, declaration, key, info):
	argStartSpace = "    "
	len_argStartSpace = len(argStartSpace)

	group = "" # required, advanced, or nothing

	if not declaration.startswith(argStartSpace):
		group = declaration[:len_argStartSpace -1]

		if group.startswith("{R}"):
			group = "required"
		elif group.startswith("{A}"):
			group = "advanced"


	# parse the name
	posspace = declaration.find(" ", len_argStartSpace)
	posend   = posspace
	if declaration[posspace-1] == ",": # then there is a tag
		posend = declaration.find(" ", posspace+1)

	tags = declaration[len_argStartSpace:posend]

	pos = declaration.find("<", posend)
	limits = ""
	if pos != -1:
		limits = declaration[pos+1:-1]

	argtype = "FLAG"
	if limits != "":
		pos = limits.find(":")

		if pos == -1 or limits.startswith("list of"): # no limits
			argtype = limits
			limits = ""

		else:
			argtype = limits[:pos]
			limits = limits[pos+1:]

			pos = limits.find("{")
			if pos != -1: # including set
				limits = limits[pos:]



		if argtype.startswith("file") or argtype.startswith("folder") or argtype.startswith("path"):
			pos = argtype.find(" ")
			if pos != -1:
				limits  = argtype[pos+1:]
				argtype = argtype[:pos]


	command[tags] = {}

	command[tags]["group"   ] = group
	command[tags]["argtype" ] = argtype
	command[tags]["limits"  ] = limits
	# command[tags]["default" ] = default
	# command[tags]["needs"   ] = needs
	# command[tags]["excludes"] = excludes
	command[tags]["key"     ] = key
	command[tags]["info"    ] = info


def help_to_map(stdOutput):
	helpMap = {}
	i = 2 #first line is Usage and second is empty

	space = "      "
	parameter = ""
	while i < len(stdOutput):
		if len(stdOutput[i]) == 0 : # empty line
			i += 1

		else:
			parPos = stdOutput[i].find(" parameter(s):")

			if parPos == -1: # then still in the same parameter type
				# add the argument
				argDecl = stdOutput[i]
				i += 1
				argKey = stdOutput[i][len(space):]
				i += 1
				if (argKey[0] == '['):
					argKey = argKey[1:len(argKey)-2]
					argInfo = stdOutput[i][len(space):]
					i += 1
				else:
					argKey  = ""
					argInfo = argKey

				while i < len(stdOutput): # check if there is more doc on several lines
					if stdOutput[i][:len(space)] == space :
						argInfo += stdOutput[i][len(space):]
						i += 1
					else :
						break

				__parse_argument(helpMap[parameter], str(argDecl), str(argKey), str(argInfo))
			else:
				parameter = stdOutput[i][:parPos]
				helpMap[parameter] = {"name" : parameter + " parameters"}
				i += 1

				if len(parameter) == 0:
					raise

	return helpMap


def print_help_map(help_map):
	for k in help_map:
		print(k + ":")

		for a in help_map[k]:

			if type(help_map[k][a]) == 'dict':
				print("\t" + a + ":")
				for i in help_map[k][a]:
					print("\t\t" + i + ":", help_map[k][a][i])
			else:
				print("\t" + a + ":", help_map[k][a])

		print()


if __name__ == "__main__":
	parser = argparse.ArgumentParser()
	parser.add_argument('input', default=sys.stdin, type=argparse.FileType('r'), nargs='?')
	args = parser.parse_args()

	data = args.input.read()

	helpMap = help_to_map(data.split('\n'))

	print_help_map(helpMap)