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
    plot_ber   = []
    plot_fer   = []
    plot_befe  = []
    plot_thr   = []
    plot_lege  = []
    fs_watcher = []

    colors       = [0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15]
    last_snr     = []
    paths        = []
    styles       = [QtCore.Qt.SolidLine, QtCore.Qt.DashLine, QtCore.Qt.DotLine, QtCore.Qt.DashDotLine, QtCore.Qt.DashDotDotLine]
    dashPatterns = [[1, 3, 4, 3], [2, 3, 4, 3], [1, 3, 1, 3], [4, 3, 4, 3], [3, 3, 2, 3], [4, 3, 1, 3]]

    def getPathId(self, path):
        if path in self.paths:
            curId = 0
            for p in self.paths:
                if p == path:
                    return curId
                else:
                    curId = curId +1
            return -1
        else:
            return -1

    def updateCurve(self, path):
        if path in self.paths:
            pathId = self.getPathId(path)
            icolor = self.colors[pathId] % len(self.colors)

            # for filename in self.paths:
            pen = pg.mkPen(color=(icolor,8), width=2, style=QtCore.Qt.CustomDashLine)
            pen.setDashPattern(self.dashPatterns[pathId % len(self.dashPatterns)])
            
            data_snr = []
            data_snr = libs.perf_reader.perf_snr_reader (path)
            if data_snr:
                if data_snr[len(data_snr) -1] > self.last_snr[pathId]:
                    data_ber  = libs.perf_reader.perf_ber_reader (path)
                    data_fer  = libs.perf_reader.perf_fer_reader (path)
                    data_bps  = libs.perf_reader.perf_bps_reader (path)
                    data_befe = libs.perf_reader.perf_befe_reader(path)

                    nPop = 0
                    for i in range(0, len(data_snr)):
                        if self.last_snr[pathId] >= data_snr[i]:
                            nPop = i

                    for i in range(0, nPop):
                        data_snr.pop(0)
                        data_ber.pop(0)
                        data_fer.pop(0)
                        data_befe.pop(0)
                        data_bps.pop(0)

                    self.plot_ber.plot (x=data_snr, y=data_ber,  pen=pen, symbol='x', name='BER plot'  )
                    self.plot_fer.plot (x=data_snr, y=data_fer,  pen=pen, symbol='x', name='FER plot'  )
                    self.plot_befe.plot(x=data_snr, y=data_befe, pen=pen, symbol='x', name='BE/FE plot')
                    self.plot_thr.plot (x=data_snr, y=data_bps,  pen=pen, symbol='x', name='T/P plot'  )

                    self.last_snr[pathId] = data_snr[len(data_snr) -1]

    def updateLegend(self, path):
        if path in self.paths:
            pathId = self.getPathId(path)
            icolor = self.colors[pathId] % len(self.colors)

            # for filename in self.paths:
            pen = pg.mkPen(color=(icolor,8), width=2, style=QtCore.Qt.CustomDashLine)
            pen.setDashPattern(self.dashPatterns[pathId % len(self.dashPatterns)])
            data_snr = libs.perf_reader.perf_snr_reader(path)
            data_ber = libs.perf_reader.perf_ber_reader(path)

            legendArea = DockArea()
            dCurve     = Dock("", size=(250,600))
            dInfo      = Dock("", size=(250,900))

            legendArea.addDock(dCurve, 'top'   )
            legendArea.addDock(dInfo,  'bottom')

            wCur = QtGui.QWidget();
            wCur.setLayout(libs.perf_reader.buildLegendLayout(path))

            sCur = QtGui.QScrollArea()
            sCur.setWidget(wCur)
            sCur.setWidgetResizable(True)
            dInfo.addWidget(sCur)

            wLegend = pg.PlotWidget(title="BER preview")
            wLegend.setLogMode(False, True)
            wLegend.plot(x=data_snr, y=data_ber,  pen=pen, name='BER plot')
            wLegend.showGrid(False, False)
            dCurve.addWidget(wLegend)

            self.plot_lege.addTab(legendArea, 'Curve ' + str(pathId +1))


    def selectionChanged(self, selected, deselected):
        super().selectionChanged(selected, deselected)
        new_paths = [ self.model().filePath(index) for index in self.selectedIndexes()
                        if not self.model().isDir(index)] # TODO: remove this restriction

        self.last_snr = []
        for p in new_paths:
            self.last_snr.append(-999.0)

        paths_to_remove = []
        for p in self.paths:
            if p not in new_paths:
                paths_to_remove.append(p)

        for p in paths_to_remove:
            pId = self.getPathId(p)
            self.paths.pop(pId)

        paths_to_add = []
        for p in new_paths:
            if p not in self.paths:
                paths_to_add.append(p)

        for p in paths_to_add:
            self.paths.append(p)

        if len(paths_to_remove) > 0:
            self.fs_watcher.removePaths(paths_to_remove)
        if len(paths_to_add) > 0:
            self.fs_watcher.addPaths(paths_to_add)
        self.fs_watcher.fileChanged.connect(self.updateCurve)

        self.plot_ber.clearPlots()
        self.plot_fer.clearPlots()
        self.plot_befe.clearPlots()
        self.plot_thr.clearPlots()
        self.plot_lege.clear()

        for path in self.paths:
            self.updateCurve(path)
            self.updateLegend(path)

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