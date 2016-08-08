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

def getVal(line, idColumn):
    # there are two different file formats, classic (lines begni with SNR) and new (lines begin with SNR value directly)
    # indices of the different parameter change, and the following array is used to convert classic indices to new
    convert_to_v1 = [0, 4, 5, 10, 0, 3, 2]
    convert_to_v2 = [0, 4, 5, 7 , 0, 3, 2]

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
        return float(line[idColumn])
    # new
    else:
        line = line.replace("||", "|")
        line = line.split('|')

        try:
            float(line[0])  # in cases where the first item of the line is neither "SNR = " nor a float
        except ValueError:
            return float(-999.0)

        if(len(line) == 14):
            return float(line[convert_to_v1[idColumn]])
        else:
            return float(line[convert_to_v2[idColumn]])

def dataReader(filename):
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
    dataLege = []

    dataLege.append(["File name", str(filename)])
    for line in lines:
        if line.startswith("#"):
            if len(line) > 3 and line[0] == '#' and line[2] == '*':
                entry = line.replace("# * ", "").replace("\n", "").split(" = ")
                dataLege.append(entry)

        else:
            snr = getVal(line, 0)
            if snr == -999.0:
                continue # line is ignored

            dataSNR.append(snr)
            dataBER.append(getVal(line, 1))
            dataFER.append(getVal(line, 2))
            dataThr.append(getVal(line, 3))

            be = getVal(line, 6)
            fe = getVal(line, 5)

            if fe == 0:
                dataBEFE.append(0.0)
            else :
                dataBEFE.append(be/fe)

    # get the command to to run to reproduce this trace
    if lines and "Run command:" in lines[0]:
        dataLege.append(["Run command", str(lines[1].strip())])
    else:
        dataLege.append(["Run command", ""])

    return dataSNR, dataBER, dataFER, dataBEFE, dataThr, dataLege