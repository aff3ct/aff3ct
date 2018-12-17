def __parse_argument(command, declaration, info):
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
	command[tags]["info"    ] = info


def help_to_map(stdOutput):
	helpMap = {}
	i = 2 #first line is Usage and second is empty

	infoStartSpace = "      "

	parameter = ""
	while i < len(stdOutput):
		if len(stdOutput[i]) == 0 : # empty line
			i += 1

		else:
			parPos = stdOutput[i].find(" parameter(s):")
			if parPos == -1: # then still in the same parameter type
				# add the argument
				argDecl = stdOutput[i  ]
				argInfo = stdOutput[i+1][len(infoStartSpace):]
				i += 2

				while i < len(stdOutput): # check if there is more doc on several lines
					if stdOutput[i][:len(infoStartSpace)] == infoStartSpace :
						argInfo += stdOutput[i][len(infoStartSpace):]
						i += 1
					else :
						break

				__parse_argument(helpMap[parameter], str(argDecl), str(argInfo))
			else:
				parameter = stdOutput[i][:parPos]
				helpMap[parameter] = {"name" : parameter + " parameters"}
				i += 1

				if len(parameter) == 0:
					raise

	return helpMap
