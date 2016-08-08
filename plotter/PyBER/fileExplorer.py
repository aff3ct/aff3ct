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
import libs.reader
import pyqtgraph as pg
from PyQt4 import QtGui
from PyQt4 import QtCore
from pyqtgraph.dockarea import *

class AdvTreeView(QtGui.QTreeView):
    wBER      = []
    wFER      = []
    wBEFE     = []
    wThr      = []
    wLege     = []
    fsWatcher = []

    dataSNR  = []
    dataBER  = []
    dataFER  = []
    dataBEFE = []
    dataThr  = []
    dataLege = []

    #               1  2  3  4  5  6  7  8  9  10  11  12  13  14  15, 16
    colors       = [0, 1, 2, 4, 5, 6, 7, 8, 9, 10, 12, 13, 14, 15, 16, 17]
    lastSNR      = []
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

    def updateData(self, path):
        if path in self.paths:
            pathId = self.getPathId(path)

            self.dataSNR[pathId], self.dataBER[pathId], self.dataFER[pathId], self.dataBEFE[pathId], self.dataThr[pathId], self.dataLege[pathId] = libs.reader.dataReader(path)

    def plotCurve(self, pathId, dataSNR, dataBER, dataFER, dataBEFE, dataThr):
        icolor = self.colors[pathId % len(self.colors)]
        pen = pg.mkPen(color=(icolor,8), width=2, style=QtCore.Qt.CustomDashLine)
        pen.setDashPattern(self.dashPatterns[pathId % len(self.dashPatterns)])

        self.wBER. plot(x=dataSNR, y=dataBER,  pen=pen, symbol='x', name='BER plot'  )
        self.wFER. plot(x=dataSNR, y=dataFER,  pen=pen, symbol='x', name='FER plot'  )
        self.wBEFE.plot(x=dataSNR, y=dataBEFE, pen=pen, symbol='x', name='BE/FE plot')
        self.wThr. plot(x=dataSNR, y=dataThr,  pen=pen, symbol='x', name='T/P plot'  )

    def updateCurve(self, path):
        if path in self.paths:
            pathId = self.getPathId(path)
            
            if self.dataSNR[pathId]:
                if self.dataSNR[pathId][len(self.dataSNR[pathId]) -1] > self.lastSNR[pathId]:
                    dataSNR  = self.dataSNR [pathId]
                    dataBER  = self.dataBER [pathId]
                    dataFER  = self.dataFER [pathId]
                    dataThr  = self.dataThr [pathId]
                    dataBEFE = self.dataBEFE[pathId]

                    nPop = 0
                    for i in range(len(dataSNR)):
                        if self.lastSNR[pathId] >= dataSNR[i]:
                            nPop = i

                    for i in range(nPop):
                        dataSNR .pop(0)
                        dataBER .pop(0)
                        dataFER .pop(0)
                        dataBEFE.pop(0)
                        dataThr .pop(0)

                    self.plotCurve(pathId, dataSNR, dataBER, dataFER, dataBEFE, dataThr)
                    self.lastSNR[pathId] = self.dataSNR[pathId][len(self.dataSNR[pathId]) -1]
                elif self.dataSNR[pathId][len(self.dataSNR[pathId]) -1] < self.lastSNR[pathId]:
                    self.updateCurves()
                    self.lastSNR[pathId] = self.dataSNR[pathId][len(self.dataSNR[pathId]) -1]

    def updateCurves(self):
        self.wBER .clearPlots()
        self.wFER .clearPlots()
        self.wBEFE.clearPlots()
        self.wThr .clearPlots()
        
        for pathId in range(len(self.paths)):
            self.plotCurve(pathId, self.dataSNR[pathId], self.dataBER[pathId], self.dataFER[pathId], self.dataBEFE[pathId], self.dataThr[pathId])

    def updateDataAndCurve(self, path):
        self.updateData(path)
        self.updateCurve(path)

    def updateLegends(self):
        self.wLege.clear()

        for pathId in range(len(self.paths)):
            icolor = self.colors[pathId % len(self.colors)]
            path   = self.paths[pathId]

            # for filename in self.paths:
            pen = pg.mkPen(color=(icolor,8), width=2, style=QtCore.Qt.CustomDashLine)
            pen.setDashPattern(self.dashPatterns[pathId % len(self.dashPatterns)])

            legendArea = DockArea()
            dCurve     = Dock("", size=(250,600))
            dInfo      = Dock("", size=(250,900))

            legendArea.addDock(dCurve, 'top'   )
            legendArea.addDock(dInfo,  'bottom')

            layoutLegend = QtGui.QFormLayout()
            for entry in self.dataLege[pathId]:
                if len(entry) == 2 and entry[1]:
                    if entry[0] == "Run command":
                        runCmd = QtGui.QLineEdit(str(entry[1]))
                        runCmd.setReadOnly(True)
                        layoutLegend.addRow("<b>" + entry[0] + "</b>: ", runCmd)
                    else:
                        layoutLegend.addRow("<b>" + entry[0] + "</b>: ", QtGui.QLabel(entry[1]))
            wCur = QtGui.QWidget();
            wCur.setLayout(layoutLegend)

            sCur = QtGui.QScrollArea()
            sCur.setWidget(wCur)
            sCur.setWidgetResizable(True)
            dInfo.addWidget(sCur)

            wLegend = pg.PlotWidget(title="BER preview")
            wLegend.setLogMode(False, True)
            wLegend.plot(x=self.dataSNR[pathId], y=self.dataBER[pathId], pen=pen, name='BER plot')
            wLegend.showGrid(False, False)
            dCurve.addWidget(wLegend)

            self.wLege.addTab(legendArea, 'Curve ' + str(pathId +1))


    def selectionChanged(self, selected, deselected):
        super().selectionChanged(selected, deselected)
        newPaths = [ self.model().filePath(index) for index in self.selectedIndexes()
                        if not self.model().isDir(index)] # TODO: remove this restriction

        self.dataSNR  = [0 for x in range(len(newPaths))]
        self.dataBER  = [0 for x in range(len(newPaths))]
        self.dataFER  = [0 for x in range(len(newPaths))]
        self.dataBEFE = [0 for x in range(len(newPaths))]
        self.dataThr  = [0 for x in range(len(newPaths))]
        self.dataLege = [0 for x in range(len(newPaths))]
        self.lastSNR  = [0 for x in range(len(newPaths))]

        pathsToRemove = []
        for p in self.paths:
            if p not in newPaths:
                pathsToRemove.append(p)

        for p in pathsToRemove:
            pId = self.getPathId(p)
            self.paths.pop(pId)

        pathsToAdd = []
        for p in newPaths:
            if p not in self.paths:
                pathsToAdd.append(p)

        for p in pathsToAdd:
            self.paths.append(p)

        if len(pathsToRemove) > 0:
            self.fsWatcher.removePaths(pathsToRemove)
        if len(pathsToAdd) > 0:
            self.fsWatcher.addPaths(pathsToAdd)
        self.fsWatcher.fileChanged.connect(self.updateDataAndCurve)

        for path in self.paths:
            self.updateData(path)

        for pathId in range(len(self.paths)):
            self.lastSNR[pathId] = self.dataSNR[pathId][len(self.dataSNR[pathId]) -1]

        self.updateCurves()
        self.updateLegends()

def generatePannel():
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