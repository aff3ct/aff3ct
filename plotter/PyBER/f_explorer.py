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
import sys
import numpy as np
import pyqtgraph as pg
import libs.perf_reader
from PyQt4 import QtGui
from PyQt4 import QtCore
from pyqtgraph.dockarea import *

class AdvTreeView(QtGui.QTreeView):
    plot_ber  = []
    plot_fer  = []
    plot_befe = []
    plot_thr  = []
    plot_lege = []

    def selectionChanged(self, selected, deselected):
        super().selectionChanged(selected, deselected)
        paths = [ self.model().filePath(index) for index in self.selectedIndexes()
                        if not self.model().isDir(index)] # TODO: remove this restriction

        self.plot_ber.clearPlots()
        self.plot_fer.clearPlots()
        self.plot_befe.clearPlots()
        self.plot_thr.clearPlots()
        self.plot_lege.clear()

        styles = []
        styles.append(QtCore.Qt.SolidLine)
        styles.append(QtCore.Qt.DashLine)
        styles.append(QtCore.Qt.DotLine)
        styles.append(QtCore.Qt.DashDotLine)
        styles.append(QtCore.Qt.DashDotDotLine)

        space = 3
        dashPatterns = []
        dashPatterns.append([1, space, 4, space])
        dashPatterns.append([2, space, 4, space])
        dashPatterns.append([1, space, 1, space])
        dashPatterns.append([4, space, 4, space])
        dashPatterns.append([3, space, 2, space])
        dashPatterns.append([4, space, 1, space])

        icolor = 0
        for filename in paths:
            pen1 = pg.mkPen(color=(icolor,8), width=2, style=QtCore.Qt.CustomDashLine)
            pen1.setDashPattern(dashPatterns[icolor % len(dashPatterns)])
            data_snr  = libs.perf_reader.perf_snr_reader (filename)
            data_ber  = libs.perf_reader.perf_ber_reader (filename)
            data_fer  = libs.perf_reader.perf_fer_reader (filename)
            data_bps  = libs.perf_reader.perf_bps_reader (filename)
            data_befe = libs.perf_reader.perf_befe_reader(filename)
            self.plot_ber.plot (x=data_snr, y=data_ber,  pen=pen1, symbol='x', name='BER plot'  )
            self.plot_fer.plot (x=data_snr, y=data_fer,  pen=pen1, symbol='x', name='FER plot'  )
            self.plot_befe.plot(x=data_snr, y=data_befe, pen=pen1, symbol='x', name='BE/FE plot')
            self.plot_thr.plot (x=data_snr, y=data_bps,  pen=pen1, symbol='x', name='T/P plot'  )

            # Debug legend
            legendArea = DockArea()
            dCurve     = Dock("", size=(250,600))
            dInfo      = Dock("", size=(250,900))

            legendArea.addDock(dCurve, 'top'   )
            legendArea.addDock(dInfo,  'bottom')

            wCur = QtGui.QWidget();
            wCur.setLayout(libs.perf_reader.buildLegendLayout(filename))

            sCur = QtGui.QScrollArea()
            sCur.setWidget(wCur)
            sCur.setWidgetResizable(True)
            dInfo.addWidget(sCur)

            wLegend = pg.PlotWidget(title="BER preview")
            wLegend.setLogMode(False, True)
            wLegend.plot(x=data_snr, y=data_ber,  pen=pen1, name='BER plot')
            wLegend.showGrid(False, False)
            dCurve.addWidget(wLegend)

            self.plot_lege.addTab(legendArea,'Curve ' + str(icolor +1))
            
            icolor = icolor +1

def gen_panel():
    if len(sys.argv) >= 2:
        os.chdir(sys.argv[1])
    else:
        os.chdir("data/")

    model = QtGui.QFileSystemModel()
    model.setReadOnly(True)
    model.setRootPath(QtCore.QDir.currentPath())
    model.setNameFilters(['*.perf', '*.dat', '*.txt', '*.data'])
    model.setNameFilterDisables(False)

    view = AdvTreeView()
    view.setSelectionMode(QtGui.QAbstractItemView.ExtendedSelection)
    view.setModel(model)
    view.hideColumn(1);
    view.hideColumn(2);
    view.hideColumn(3);
    view.setColumnWidth(30, 1)
    view.setRootIndex(model.index(QtCore.QDir.currentPath(), 0))
    view.setAnimated(True)
    view.setIconSize(QtCore.QSize(24,24))

    return view