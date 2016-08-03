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
import numpy as np
from PyQt4 import QtGui

def perf_reader(filename, s_values):

    # there are two different file formats, classic (lines begni with SNR) and new (lines begin with SNR value directly)
    # indices of the different parameter change, and the following array is used to convert classic indices to new
    convert_to_v1 = np.array([0, 4, 5, 10, 0, 3, 2])
    convert_to_v2 = np.array([0, 4, 5, 7 , 0, 3, 2])

    fin = open(filename, "r")

    perf_list = []

    for line in fin:

        if line.startswith("#"):
            continue

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
            perf_list.append(float(line[s_values]))
        # new
        else:
            line = line.replace("||", "|")
            line = line.split('|')
            try:
                float(line[0])  # in cases where the first item of the line is neither "SNR = " nor a float
            except ValueError:
                continue        # line is ignored

            if(len(line) == 14):
                perf_list.append(float(line[convert_to_v1[s_values]]))
            else:
                perf_list.append(float(line[convert_to_v2[s_values]]))

    return perf_list

def perf_snr_reader(filename):
    return perf_reader(filename, 0)

def perf_ber_reader(filename):
    return perf_reader(filename, 1)

def perf_fer_reader(filename):
    return perf_reader(filename, 2)

def perf_bps_reader(filename):
    return perf_reader(filename, 3)

def perf_fe_reader(filename):
    return perf_reader(filename, 5)

def perf_be_reader(filename):
    return perf_reader(filename, 6)

def perf_befe_reader(filename):
    a = []
    b = []
    a = perf_be_reader(filename)
    b = perf_fe_reader(filename)
    c = []
    if a and b:
        for i in range(0, len(a)):
            c.append(a[i] / b[i])
    # c = np.divide(a, b)
    return c

def buildLegendLayout(filename):
    # open the file in read mode
    f = open(filename, 'r')

    # read all the lines from the current file f
    lines = []
    for line in f:
        lines.append(line)
    
    # get the command to to run to reproduce this trace
    runCommand = ""
    if "Run command:" in lines[0]:
        runCommand = lines[1].strip()
    runCmd = QtGui.QLineEdit(str(runCommand))
    runCmd.setReadOnly(True)

    entries = []

    # get the main infos from the lines
    for l in lines:
        if len(l) > 3 and l[0] == '#' and l[2] == '*':
            entry = l.replace("# * ", "").replace("\n", "").split(" = ")
            entries.append(entry)
    f.close()

    layout = QtGui.QFormLayout()
    layout.addRow("<b>File name</b>: ", QtGui.QLabel(os.path.basename(filename)))

    for entry in entries:
        layout.addRow("<b>" + entry[0] + "</b>: ", QtGui.QLabel(entry[1]))

    if runCommand:
        layout.addRow("<b>Run command</b>: ", runCmd)
 
    return layout