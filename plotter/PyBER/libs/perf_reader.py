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
    a = perf_be_reader(filename)
    b = perf_fe_reader(filename)
    c = np.divide(a, b)
    return c

def buildLegendLayout(filename):
    iPrec      = -999
    iThreads   = -999
    iCode      = ""
    iDomain    = ""
    iInfoBits  = -999 
    iFraSize   = -999
    iSNRMin    = -999.0
    iSNRMax    = -999.0
    iSNRStep   = -999.0
    iMaxFE     = -999
    iFraGen    = ""
    iCRC       = ""
    iFrozenB   = ""
    iFBSigma   = ""
    iInterlea  = ""
    iEncoSys   = ""
    iEncoBuff  = ""
    iMod       = ""
    iChannel   = ""
    iDemod     = ""
    iDemodMax  = ""
    iQuantizer = ""
    iFixedP    = ""
    iDecAlgo   = ""
    iDecImplem = ""
    iDecSIMD   = ""
    iDecIte    = -999
    iDecL      = -999
    iDecSF     = ""
    iDecMAP    = ""

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

    # get the main infos from the lines
    for l in lines:
        if len(l) > 3 and l[0] == '#' and l[2] == '*':
            if "Code type" in l:
                tmp = l.replace("codes", "").replace(" ", "").strip().split("=")
                iCode = tmp[1]
            elif "Modulation type" in l:
                tmp = l.replace(" ", "").replace("\n", "").split("=")
                iMod = tmp[1]
            elif "Number of information bits" in l:
                tmp = l.replace(" ", "").split("=")
                iInfoBits = int(tmp[1])
            elif "Codeword length" in l:
                tmp = l.replace(" ", "").split("=")
                tmp2 = tmp[1].split("+")
                iFraSize = int(tmp2[0])
            elif "SNR min" in l:
                tmp = l.replace("dB", "").replace(" ", "").split("=")
                iSNRMin = float(tmp[1])
            elif "SNR max" in l:
                tmp = l.replace("dB", "").replace(" ", "").split("=")
                iSNRMax = float(tmp[1])
            elif "Max frame error count" in l:
                tmp = l.replace(" ", "").split("=")
                iMaxFE = int(tmp[1])
            elif "Type of bits" in l:
                if "long long" in l:
                    iPrec = 64
                elif "int" in l:
                    iPrec = 32
                elif "short" in l:
                    iPrec = 16
                elif "signed char" in l:
                    iPrec = 8
            elif "Decoding algorithm" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDecAlgo = tmp[1]
            elif "Decoding implementation" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDecImplem = tmp[1]
            elif "Decoder SIMD strategy" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDecSIMD = tmp[1]
            elif "Multi-threading" in l:
                tmp = l.replace("thread(s)", "").replace(" ", "").split("=")
                iThreads = int(tmp[1])
            elif "Domain" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDomain = tmp[1]
            elif "SNR step" in l:
                tmp = l.replace("dB", "").replace(" ", "").split("=")
                iSNRStep = float(tmp[1])
            elif "Codewords generation method" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iFraGen = tmp[1]
            elif "CRC" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iCRC = tmp[1]
            elif "Systematic encoding" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iEncoSys = tmp[1]
            elif "Channel type" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iChannel = tmp[1]
            elif "Demodulation max type" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDemodMax = tmp[1]
            elif "Demodulation" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDemod = tmp[1]
            elif "Quantizer type" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iQuantizer = tmp[1]
            elif "Fixed-point representation" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iFixedP = tmp[1]
            elif "Decoding iterations" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDecIte = int(tmp[1])
            elif "Number of lists in the SCL" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDecL = int(tmp[1])
            elif "Frozen bits generation method" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iFrozenB = tmp[1]
            elif "Sigma for code generation" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iFBSigma = tmp[1]
            elif "Buffered encoding" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iEncoBuff = tmp[1]
            elif "Interleaver" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iInterlea = tmp[1]
            elif "Scaling factor" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDecSF = tmp[1]
            elif "Decoder MAP implementation" in l:
                tmp = l.replace(" ", "").strip().split("=")
                iDecMAP = tmp[1]
    f.close()

    layout = QtGui.QFormLayout()
    layout.addRow("<b>File name</b>: ", QtGui.QLabel(os.path.basename(filename)))

    if iPrec     != -999:   layout.addRow("<b>Precision</b>: "         , QtGui.QLabel(str(iPrec     ) + "-bit"))
    if iThreads  != -999:   layout.addRow("<b>Threads num.</b>: "      , QtGui.QLabel(str(iThreads  )         ))
    if iCode:               layout.addRow("<b>Code type</b>: "         , QtGui.QLabel(str(iCode     )         ))
    if iDomain:             layout.addRow("<b>Domain: </b>: "          , QtGui.QLabel(str(iDomain   )         ))
    if iInfoBits != -999:   layout.addRow("<b>Info. bits (K): </b>: "  , QtGui.QLabel(str(iInfoBits )         ))
    if iFraSize  != -999:   layout.addRow("<b>Frame size (N): </b>: "  , QtGui.QLabel(str(iFraSize  )         ))
    if iSNRMin   != -999.0: layout.addRow("<b>SNR min: </b>: "         , QtGui.QLabel(str(iSNRMin   ) + " dB" ))
    if iSNRMax   != -999.0: layout.addRow("<b>SNR max: </b>: "         , QtGui.QLabel(str(iSNRMax   ) + " dB" ))
    if iSNRStep  != -999.0: layout.addRow("<b>SNR step: </b>: "        , QtGui.QLabel(str(iSNRStep  ) + " dB" ))
    if iMaxFE    != -999:   layout.addRow("<b>Max frame errors </b>: " , QtGui.QLabel(str(iMaxFE    )         ))
    if iFraGen:             layout.addRow("<b>Frame gen. </b>: "       , QtGui.QLabel(str(iFraGen   )         ))
    if iCRC:                layout.addRow("<b>CRC </b>: "              , QtGui.QLabel(str(iCRC      )         ))
    if iFrozenB:            layout.addRow("<b>FrozenBits gen. </b>: "  , QtGui.QLabel(str(iFrozenB  )         ))
    if iFBSigma:            layout.addRow("<b>Sigma for FB. </b>: "    , QtGui.QLabel(str(iFBSigma  )         ))
    if iInterlea:           layout.addRow("<b>Interleaver </b>: "      , QtGui.QLabel(str(iInterlea )         ))
    if iEncoSys:            layout.addRow("<b>Encoder sys. </b>: "     , QtGui.QLabel(str(iEncoSys  )         ))
    if iEncoBuff:           layout.addRow("<b>Encoder buffered </b>: " , QtGui.QLabel(str(iEncoBuff )         ))
    if iMod:                layout.addRow("<b>Modulation: </b>: "      , QtGui.QLabel(str(iMod      )         ))
    if iChannel:            layout.addRow("<b>Channel: </b>: "         , QtGui.QLabel(str(iChannel  )         ))
    if iDemod:              layout.addRow("<b>Demodulation </b>: "     , QtGui.QLabel(str(iDemod    )         ))
    if iDemodMax:           layout.addRow("<b>Demodulation max</b>: "  , QtGui.QLabel(str(iDemodMax )         ))
    if iQuantizer:          layout.addRow("<b>Quantizer</b>: "         , QtGui.QLabel(str(iQuantizer)         ))
    if iFixedP:             layout.addRow("<b>Fixed-point </b>: "      , QtGui.QLabel(str(iFixedP   )         ))
    if iDecAlgo:            layout.addRow("<b>Decoder algo. </b>: "    , QtGui.QLabel(str(iDecAlgo  )         ))
    if iDecImplem:          layout.addRow("<b>Decoder implem. </b>: "  , QtGui.QLabel(str(iDecImplem)         ))
    if iDecSIMD:            layout.addRow("<b>Decoder SIMD. </b>: "    , QtGui.QLabel(str(iDecSIMD  )         ))
    if iDecIte   != -999:   layout.addRow("<b>Decoder num. ite. </b>: ", QtGui.QLabel(str(iDecIte   )         ))
    if iDecL     != -999:   layout.addRow("<b>Decoder lists (L)</b>: " , QtGui.QLabel(str(iDecL     )         ))
    if iDecSF:              layout.addRow("<b>Decoder SF</b>: "        , QtGui.QLabel(str(iDecSF    )         ))
    if iDecMAP:             layout.addRow("<b>Decoder MAP</b>: "       , QtGui.QLabel(str(iDecMAP   )         ))
    if runCommand:          layout.addRow("<b>Run command</b>: "       , runCmd                                   )
 
    return layout