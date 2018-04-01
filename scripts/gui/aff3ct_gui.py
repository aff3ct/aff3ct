# import os
import sys
import time
import os
import subprocess
from PyQt4.QtCore import *
from PyQt4.QtGui  import *
from gui_argument import *


class mainTab(argumentTab):
	layoutNumberRows    = 10
	layoutNumberColumns = 4

	def __init__(self, aff3ctRoot, aff3ctBinary, simUpdateCallFunction, runCallFunction, grpName):
		super(mainTab, self).__init__(grpName)

		self.simUpdateCallFunction = simUpdateCallFunction
		self.runCallFunction = runCallFunction

		self.aff3ctRoot   = aff3ctRoot
		self.aff3ctBinary = aff3ctBinary

		self.addAff3ctRootRow(self.aff3ctRoot)
		self.addAff3ctBinaryRow(self.aff3ctBinary)
		self.addAff3ctRunRow()

		self.updateLayout()

	def updateLayout(self):
		self.clearLayout()

		for a in range(len(self.argList)-1, -1, -1):
			if not self.argList[a].getUpdated() :
				self.argList[a].delete()
				del self.argList[a]

		self.layout.addWidget(self.labelAff3ctRoot,   self.currentLayoutRow, 0, 1, 1)
		self.layout.addWidget(self.textboxAff3ctRoot, self.currentLayoutRow, 1, 1, self.layoutNumberColumns-2)
		self.layout.addWidget(self.btnAff3ctRoot,     self.currentLayoutRow, self.layoutNumberColumns-1,1,1)
		self.currentLayoutRow += 1

		self.layout.addWidget(self.labelAff3ctBinary,   self.currentLayoutRow, 0, 1, 1)
		self.layout.addWidget(self.textboxAff3ctBinary, self.currentLayoutRow, 1, 1, self.layoutNumberColumns-2)
		self.layout.addWidget(self.btnAff3ctBinary,     self.currentLayoutRow, self.layoutNumberColumns-1,1,1)
		self.currentLayoutRow += 1

		self.layout.addWidget(self.btnAff3ctRun,    self.currentLayoutRow, 0, 1, 1)
		self.layout.addWidget(self.btnAff3ctUpdate, self.currentLayoutRow, self.layoutNumberColumns-1, 1, 1)
		self.currentLayoutRow += 1

		for a in self.argList:
			a.addToLayout(self.layout, self.currentLayoutRow)
			self.currentLayoutRow += 1

	def addAff3ctRootRow(self, defaultPath):
		# Create label
		self.labelAff3ctRoot = QLabel()
		self.labelAff3ctRoot.setText("AFF3CT root")
		self.labelAff3ctRoot.setToolTip('Root directory from where will be executed AFF3CT')

		# Create textbox
		self.textboxAff3ctRoot = QLineEdit()
		self.textboxAff3ctRoot.setText(defaultPath)
		self.textboxAff3ctRoot.setToolTip('Set path to AFF3CT root directory')
		self.textboxAff3ctRoot.textChanged.connect(self.textChanged)

		# create AFF3CT path button
		self.btnAff3ctRoot = QPushButton("Browse")
		self.btnAff3ctRoot.setToolTip('Find AFF3CT root directory')
		self.btnAff3ctRoot.clicked.connect(self.fileBrowseAff3ctRoot)

	def addAff3ctBinaryRow(self, defaultPath):
		# Create label
		self.labelAff3ctBinary = QLabel()
		self.labelAff3ctBinary.setText("AFF3CT binary")

		# Create textbox
		self.textboxAff3ctBinary = QLineEdit()
		self.textboxAff3ctBinary.setText(defaultPath)
		self.textboxAff3ctBinary.setToolTip('Set path to AFF3CT binary')
		self.textboxAff3ctBinary.textChanged.connect(self.textChanged)

		# create AFF3CT path button
		self.btnAff3ctBinary = QPushButton("Browse")
		self.btnAff3ctBinary.setToolTip('Find AFF3CT binary')
		self.btnAff3ctBinary.clicked.connect(self.fileBrowseAff3ctBinary)

	def addAff3ctRunRow(self):
		# create AFF3CT run button
		self.btnAff3ctRun = QPushButton("Run (copy to clipboard)")
		self.btnAff3ctRun.setToolTip('Launch AFF3CT!')
		self.btnAff3ctRun.clicked.connect(self.runCallFunction)

		# create AFF3CT update button
		self.btnAff3ctUpdate = QPushButton("Update")
		self.btnAff3ctUpdate.setToolTip('Update the argument lists')
		self.btnAff3ctUpdate.clicked.connect(self.simUpdateCallFunction)

	def getAff3ctBinary(self):
		return self.textboxAff3ctBinary.displayText()

	def getAff3ctRoot(self):
		return self.textboxAff3ctRoot.displayText()

	def fileBrowseAff3ctBinary(self):
		gotRoot    = QDir(self.getAff3ctRoot())
		chosenPath = QFileDialog.getOpenFileName(win, 'Open File')
		self.textboxAff3ctBinary.setText(gotRoot.relativeFilePath(chosenPath))

	def fileBrowseAff3ctRoot(self):
		self.textboxAff3ctRoot.setText(QFileDialog.getExistingDirectory(win, 'Open Folder'))

	def addArgument(self, argDecl, argDoc):
		# if not argDecl.find("--sim-cde-type") == -1 or not argDecl.find("--sim-type") == -1 :
		# 	super(mainTab, self).addArgument(argDecl, argDoc, self.simUpdateCallFunction)
		# else:
			super(mainTab, self).addArgument(argDecl, argDoc)

	def textChanged(self):
		self.aff3ctRoot   = self.getAff3ctRoot()
		self.aff3ctBinary = self.getAff3ctBinary()
		self.simUpdateCallFunction()


class aff3ctGui(QTabWidget):
	def __init__(self, arg):
		super(QTabWidget, self).__init__()

		# create our window
		self.setWindowTitle('AFF3CT GUI')
		self.resize(500, 300)

		aff3ctRoot   = "../aff3ct/build";
		aff3ctBinary = "bin/aff3ct";

		# add main tab
		self.addTab(mainTab(aff3ctRoot, aff3ctBinary, self.updateSimu, self.runAff3ct, "Simulation"), "Simulation")
		self.updateSimu()

	def getCommand(self):
		command = ""
		for t in range(self.count()):
			command += self.widget(t).getCommand() + " "

		# command.replace("  ", " ")
		return command

	def getMainTab(self):
		return self.widget(0)

	def getTabIndex(self, grpName):
		for t in range(self.count()):
			if self.widget(t).getGrpName() == grpName :
				return t

		return -1

	def moveToAff3ctRoot(self):
		try :
			os.chdir(self.getMainTab().getAff3ctRoot())
		except OSError:
			pass # certainly already in the right folder but exception is throw when the path is given relatively

	def runAff3ct(self):
		self.moveToAff3ctRoot()
		startTime = time.time()
		command = "./" + self.getMainTab().getAff3ctBinary() + " " + self.getCommand()

		command = str(command) # Qstring to normal string
		command = " ".join(command.split()) #remove white spaces
		print "Command: " + command

		elapsedTime = time.time() - startTime

		# copy to clipboard
		cb = app.clipboard()
		cb.clear(mode=cb.Clipboard)
		cb.setText(command, mode=cb.Clipboard)

	def updateSimu(self):
		self.moveToAff3ctRoot()
		command = self.getMainTab().getAff3ctBinary() + " " + self.getCommand() + " -h --sim-no-colors"

		command = str(command) # Qstring to normal string
		command = " ".join(command.split()) #remove white spaces

		print "Command: " + command

		argsAFFECT = command.split(" ")
		print "Command split : "
		print(argsAFFECT)

		try:
			processAFFECT = subprocess.Popen(argsAFFECT, stdout=subprocess.PIPE,
			                                             stderr=subprocess.PIPE)
			(stdoutAFFECT, stderrAFFECT) = processAFFECT.communicate()
		except OSError:
			return

		returnCode = processAFFECT.returncode

		stdOutput = stdoutAFFECT.decode(encoding='UTF-8').split("\n")
		errOutput = stderrAFFECT.decode(encoding='UTF-8')

		print "stdOutput : "
		print stdOutput
		print "errOutput : "
		print errOutput

		self.setUpdatesEnabled(False)
		self.setUpdated(False)
		self.parseAff3ctHelp(stdOutput, errOutput)
		self.clearNotUpdatedTabs()
		self.setUpdatesEnabled(True)

	def parseAff3ctHelp(self, stdOutput, errOutput):
		i = 2 #first line is Usage and second is empty

		docStartSpace = "      "

		parameter = ""
		idxTab = 0
		while i < len(stdOutput):
			if len(stdOutput[i]) == 0 : # empty line
				i += 1

			else:
				parPos = stdOutput[i].find(" parameter(s):")
				if parPos == -1: # then still in the same parameter type
					# add the argument
					argDecl = stdOutput[i  ]
					argDoc  = stdOutput[i+1][len(docStartSpace):]
					i += 2

					while i < len(stdOutput): # check if there is more doc on several lines
						if stdOutput[i][:len(docStartSpace)] == docStartSpace :
							argDoc += stdOutput[i][len(docStartSpace):]
							i += 1
						else :
							break

					self.widget(idxTab).addArgument(str(argDecl), str(argDoc))
				else:
					parameter = stdOutput[i][:parPos]
					idxTab = self.getTabIndex(parameter)
					if idxTab == -1 : # then tab does not exist yet
						self.addTab(argumentTab(parameter), parameter)
						idxTab = self.getTabIndex(parameter)

					i += 1

	def clearNotUpdatedTabs(self):
		for t in range(self.count()-1, -1, -1):
			if not self.widget(t).getUpdated() :
				self.removeTab(t)
			else:
				self.widget(t).updateLayout()

	def setUpdated(self, val):
		for t in range(self.count()-1, -1, -1):
			self.widget(t).setUpdated(val)


########################################################################### MAIN
if __name__ == '__main__':
	app = QApplication(sys.argv)
	win = aff3ctGui("coucou")
	win.show()
	sys.exit(app.exec_())