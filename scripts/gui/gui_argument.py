import os
from PyQt5.QtCore import *
from PyQt5.QtGui import *
from PyQt5.QtWidgets import *

class aff3ctArgument(QObject):
	layoutNumberRows    = 1
	layoutNumberColumns = 4
	argStartSpace = "    "

	def __init__(self, argDecl, argDoc, updateFunction = "__noupdate__"):
		super(aff3ctArgument, self).__init__()
		self.argDecl        = argDecl
		self.argDoc         = argDoc
		self.updateCall     = updateFunction != "__noupdate__";
		self.updateFunction = updateFunction;

		self.special = ""
		self.tag     = ""
		self.name    = ""
		self.range   = ""
		self.updated = True

		self.parse()

	def parse(self):

		if self.argDecl[:len(self.argStartSpace)] == self.argStartSpace:
			self.special = ""
		else:
			self.special = self.argDecl[:len(self.argStartSpace)-1]

		# parse the name
		startpos = len(self.argStartSpace)
		posspace = self.argDecl.find(" ", startpos)
		if self.argDecl[posspace-1] == ",": # then there is a tag
			posend = self.argDecl.find(" ", posspace+1)
		else:
			posend = posspace

		if posend == posspace: # no tag
			self.tag   = ""
			self.name  = self.argDecl[startpos:posend]
		else:
			self.name = self.argDecl[startpos:posspace-1]
			self.tag  = self.argDecl[posspace+1:posend]

		pos = self.argDecl.find("<", posend)
		if pos == -1:
			self.range = ""
		else:
			self.range = self.argDecl[pos+1:-1]

		# add check box with the argument label
		self.cb = QCheckBox(self.getLabel())
		self.cb.setToolTip(self.argDoc) # add the doc as tip
		if self.updateCall:
			self.cb.stateChanged.connect(self.updateFunction)

		if self.special == "{R}":
			self.cb.setChecked(True)

		# create the argument setter
		if self.range == "": # no range
			self.setter = "NONE"
		else:
			pos = self.range.find("including set")
			if pos != -1: # then a combo box !
				self.setter = "COMBO"

				# Create combobox
				self.combo = QComboBox()
				spos = self.range.find("={", pos )
				epos = self.range.rfind("}", spos)
				self.combo.addItems(self.range[spos + 2:epos].split("|"))
				self.combo.setToolTip(self.range[pos:])
				if self.updateCall:
					self.combo.currentIndexChanged.connect(self.updateFunction)

			else:
				self.setter = "LINEEDIT"

				self.lineEdit = QLineEdit()
				self.lineEdit.setToolTip(self.range)

				if self.range.startswith("integer"):
					self.lineEdit.setValidator(QIntValidator())

				elif self.range.startswith("real number"):
					self.lineEdit.setValidator(QDoubleValidator())

				elif self.range.startswith("text"):
					pass

				elif self.range.startswith("file"):
					self.setter += "_BUTTON"
					self.fb = QPushButton("Browse")
					self.fb.clicked.connect(self.fileBrowse)

				elif self.range.startswith("folder"):
					self.setter += "_BUTTON"
					self.fb = QPushButton("Browse")
					self.fb.clicked.connect(self.folderBrowse)


	def addToLayout(self, GridLayout, row):
		GridLayout.addWidget(self.cb, row, 0, 1, 1)
		if self.setter == "COMBO":
			GridLayout.addWidget(self.combo, row, 1, 1, 1)
		elif self.setter == "LINEEDIT":
			GridLayout.addWidget(self.lineEdit, row, 1, 1, self.layoutNumberColumns-2)
		elif self.setter == "LINEEDIT_BUTTON":
			GridLayout.addWidget(self.lineEdit, row, 1, 1, self.layoutNumberColumns-2)
			GridLayout.addWidget(self.fb, row, self.layoutNumberColumns -1, 1, 1)

	def getCommand(self):
		command = "";
		if self.cb.isChecked():
			command += self.name + " "

			if self.setter == "COMBO":
				command += self.combo.currentText()
			elif self.setter.startswith("LINEEDIT"):
				command += self.lineEdit.displayText()

		return command

	def getName(self):
		return self.name

	def getLabel(self):
		if len(self.special):
			label = self.special + " "
		else:
			label = self.argStartSpace

		label += self.name
		if not len(self.tag) == 0:
			label += ", " + self.tag

		return label

	def compare(self, otherArg):
		return self.argDecl == otherArg.argDecl and self.argDoc == otherArg.argDoc

	def setUpdated(self, val):
		self.updated = val

	def getUpdated(self):
		return self.updated

	def delete(self):
		# else it is not hidden automatically
		self.cb.hide()

		if self.setter == "COMBO":
			self.combo.hide()
		elif self.setter == "LINEEDIT":
			self.lineEdit.hide()
		elif self.setter == "LINEEDIT_BUTTON":
			self.lineEdit.hide()
			self.fb.hide()

		self.deleteLater()

	def fileBrowse(self):
		gotRoot    = QDir(os.getcwd())
		chosenPath = QFileDialog.getOpenFileName(QWidget(), 'Open File')
		self.lineEdit.setText(gotRoot.relativeFilePath(chosenPath))

	def folderBrowse(self):
		self.lineEdit.setText(QFileDialog.getExistingDirectory(win, 'Open Folder'))


class argumentTab(QWidget):

	def __init__(self, grpName):
		super(QWidget, self).__init__()
		self.grpName = grpName

		self.argList = []

		# layout of the tab
		self.layout = QGridLayout()
		self.setLayout(self.layout)
		self.currentLayoutRow = 0
		self.updated = False

	def getGrpName(self):
		return self.grpName

	def setUpdated(self, val):
		self.updated = val

		for a in self.argList:
			a.setUpdated(val)

	def getUpdated(self):
		return self.updated

	def clearLayout(self):
		while self.layout.count():
			child = self.layout.takeAt(0)
			# if child.widget():
			# 	child.widget().deleteLater()

		self.currentLayoutRow = 0

	def updateLayout(self):
		self.clearLayout()

		for a in range(len(self.argList)-1, -1, -1):
			if not self.argList[a].getUpdated() :
				self.argList[a].delete()
				del self.argList[a]

		for a in self.argList:
			a.addToLayout(self.layout, self.currentLayoutRow)
			self.currentLayoutRow += 1

	def addArgument(self, argDecl, argDoc, updateFunction = "__noupdate__"):
		self.updated = True

		#add to the arg list
		newArg = aff3ctArgument(argDecl, argDoc, updateFunction)

		#try to find if there is not already another one in the list
		for a in self.argList:
			if a.compare(newArg):
				a.setUpdated(True)
				return

		self.argList.append(newArg)

		#and add to the layout
		self.argList[-1].addToLayout(self.layout, self.currentLayoutRow)
		self.currentLayoutRow += 1

	def getCommand(self):
		command = "";
		for a in self.argList:
			command += a.getCommand() + " "
		return command
