#pylint: disable=invalid-name,attribute-defined-outside-init,too-many-locals, too-many-instance-attributes,line-too-long
# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'ui/sans/trans_spreader.ui'
#
# Created: Wed Nov 16 13:57:35 2011
#      by: PyQt4 UI code generator 4.7.4
#
# WARNING! All changes made in this file will be lost!

from PyQt4 import QtCore, QtGui

class Ui_GroupBox(object):
    def setupUi(self, GroupBox):
        GroupBox.setObjectName("GroupBox")
        GroupBox.resize(802, 583)
        self.verticalLayout = QtGui.QVBoxLayout(GroupBox)
        self.verticalLayout.setObjectName("verticalLayout")
        self.gridLayout = QtGui.QGridLayout()
        self.gridLayout.setObjectName("gridLayout")
        self.label = QtGui.QLabel(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label.sizePolicy().hasHeightForWidth())
        self.label.setSizePolicy(sizePolicy)
        self.label.setMinimumSize(QtCore.QSize(205, 0))
        self.label.setObjectName("label")
        self.gridLayout.addWidget(self.label, 0, 0, 1, 1)
        self.sample_scatt_edit = QtGui.QLineEdit(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sample_scatt_edit.sizePolicy().hasHeightForWidth())
        self.sample_scatt_edit.setSizePolicy(sizePolicy)
        self.sample_scatt_edit.setMinimumSize(QtCore.QSize(300, 0))
        self.sample_scatt_edit.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.sample_scatt_edit.setObjectName("sample_scatt_edit")
        self.gridLayout.addWidget(self.sample_scatt_edit, 0, 2, 1, 1)
        self.sample_scatt_browse = QtGui.QPushButton(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sample_scatt_browse.sizePolicy().hasHeightForWidth())
        self.sample_scatt_browse.setSizePolicy(sizePolicy)
        self.sample_scatt_browse.setObjectName("sample_scatt_browse")
        self.gridLayout.addWidget(self.sample_scatt_browse, 0, 3, 1, 1)
        self.label_2 = QtGui.QLabel(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_2.sizePolicy().hasHeightForWidth())
        self.label_2.setSizePolicy(sizePolicy)
        self.label_2.setObjectName("label_2")
        self.gridLayout.addWidget(self.label_2, 1, 0, 1, 1)
        self.sample_spread_edit = QtGui.QLineEdit(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sample_spread_edit.sizePolicy().hasHeightForWidth())
        self.sample_spread_edit.setSizePolicy(sizePolicy)
        self.sample_spread_edit.setMinimumSize(QtCore.QSize(300, 0))
        self.sample_spread_edit.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.sample_spread_edit.setObjectName("sample_spread_edit")
        self.gridLayout.addWidget(self.sample_spread_edit, 1, 2, 1, 1)
        self.sample_spread_browse = QtGui.QPushButton(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.sample_spread_browse.sizePolicy().hasHeightForWidth())
        self.sample_spread_browse.setSizePolicy(sizePolicy)
        self.sample_spread_browse.setObjectName("sample_spread_browse")
        self.gridLayout.addWidget(self.sample_spread_browse, 1, 3, 1, 1)
        self.label_3 = QtGui.QLabel(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_3.sizePolicy().hasHeightForWidth())
        self.label_3.setSizePolicy(sizePolicy)
        self.label_3.setObjectName("label_3")
        self.gridLayout.addWidget(self.label_3, 2, 0, 1, 1)
        self.label_4 = QtGui.QLabel(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_4.sizePolicy().hasHeightForWidth())
        self.label_4.setSizePolicy(sizePolicy)
        self.label_4.setObjectName("label_4")
        self.gridLayout.addWidget(self.label_4, 3, 0, 1, 1)
        self.direct_scatt_edit = QtGui.QLineEdit(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.direct_scatt_edit.sizePolicy().hasHeightForWidth())
        self.direct_scatt_edit.setSizePolicy(sizePolicy)
        self.direct_scatt_edit.setMinimumSize(QtCore.QSize(300, 0))
        self.direct_scatt_edit.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.direct_scatt_edit.setObjectName("direct_scatt_edit")
        self.gridLayout.addWidget(self.direct_scatt_edit, 2, 2, 1, 1)
        self.direct_spread_edit = QtGui.QLineEdit(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.direct_spread_edit.sizePolicy().hasHeightForWidth())
        self.direct_spread_edit.setSizePolicy(sizePolicy)
        self.direct_spread_edit.setMinimumSize(QtCore.QSize(300, 0))
        self.direct_spread_edit.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.direct_spread_edit.setObjectName("direct_spread_edit")
        self.gridLayout.addWidget(self.direct_spread_edit, 3, 2, 1, 1)
        self.direct_scatt_browse = QtGui.QPushButton(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.direct_scatt_browse.sizePolicy().hasHeightForWidth())
        self.direct_scatt_browse.setSizePolicy(sizePolicy)
        self.direct_scatt_browse.setObjectName("direct_scatt_browse")
        self.gridLayout.addWidget(self.direct_scatt_browse, 2, 3, 1, 1)
        self.direct_spread_browse = QtGui.QPushButton(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.direct_spread_browse.sizePolicy().hasHeightForWidth())
        self.direct_spread_browse.setSizePolicy(sizePolicy)
        self.direct_spread_browse.setObjectName("direct_spread_browse")
        self.gridLayout.addWidget(self.direct_spread_browse, 3, 3, 1, 1)
        self.label_5 = QtGui.QLabel(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Preferred)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.label_5.sizePolicy().hasHeightForWidth())
        self.label_5.setSizePolicy(sizePolicy)
        self.label_5.setObjectName("label_5")
        self.gridLayout.addWidget(self.label_5, 4, 0, 1, 1)
        self.horizontalLayout = QtGui.QHBoxLayout()
        self.horizontalLayout.setObjectName("horizontalLayout")
        self.spreader_trans_edit = QtGui.QLineEdit(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.spreader_trans_edit.sizePolicy().hasHeightForWidth())
        self.spreader_trans_edit.setSizePolicy(sizePolicy)
        self.spreader_trans_edit.setMaximumSize(QtCore.QSize(113, 16777215))
        self.spreader_trans_edit.setObjectName("spreader_trans_edit")
        self.horizontalLayout.addWidget(self.spreader_trans_edit)
        self.label_7 = QtGui.QLabel(GroupBox)
        self.label_7.setObjectName("label_7")
        self.horizontalLayout.addWidget(self.label_7)
        self.spreader_trans_spread_edit = QtGui.QLineEdit(GroupBox)
        sizePolicy = QtGui.QSizePolicy(QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Fixed)
        sizePolicy.setHorizontalStretch(0)
        sizePolicy.setVerticalStretch(0)
        sizePolicy.setHeightForWidth(self.spreader_trans_spread_edit.sizePolicy().hasHeightForWidth())
        self.spreader_trans_spread_edit.setSizePolicy(sizePolicy)
        self.spreader_trans_spread_edit.setMaximumSize(QtCore.QSize(113, 16777215))
        self.spreader_trans_spread_edit.setObjectName("spreader_trans_spread_edit")
        self.horizontalLayout.addWidget(self.spreader_trans_spread_edit)
        spacerItem = QtGui.QSpacerItem(40, 20, QtGui.QSizePolicy.Expanding, QtGui.QSizePolicy.Minimum)
        self.horizontalLayout.addItem(spacerItem)
        self.gridLayout.addLayout(self.horizontalLayout, 4, 2, 1, 1)
        spacerItem1 = QtGui.QSpacerItem(22, 20, QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem1, 0, 5, 1, 1)
        spacerItem2 = QtGui.QSpacerItem(20, 20, QtGui.QSizePolicy.Fixed, QtGui.QSizePolicy.Minimum)
        self.gridLayout.addItem(spacerItem2, 0, 1, 1, 1)
        self.sample_scatt_plot = QtGui.QPushButton(GroupBox)
        self.sample_scatt_plot.setMinimumSize(QtCore.QSize(0, 0))
        self.sample_scatt_plot.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.sample_scatt_plot.setObjectName("sample_scatt_plot")
        self.gridLayout.addWidget(self.sample_scatt_plot, 0, 4, 1, 1)
        self.sample_spread_plot = QtGui.QPushButton(GroupBox)
        self.sample_spread_plot.setMinimumSize(QtCore.QSize(0, 0))
        self.sample_spread_plot.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.sample_spread_plot.setObjectName("sample_spread_plot")
        self.gridLayout.addWidget(self.sample_spread_plot, 1, 4, 1, 1)
        self.direct_scatt_plot = QtGui.QPushButton(GroupBox)
        self.direct_scatt_plot.setMinimumSize(QtCore.QSize(0, 0))
        self.direct_scatt_plot.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.direct_scatt_plot.setObjectName("direct_scatt_plot")
        self.gridLayout.addWidget(self.direct_scatt_plot, 2, 4, 1, 1)
        self.direct_spread_plot = QtGui.QPushButton(GroupBox)
        self.direct_spread_plot.setMinimumSize(QtCore.QSize(0, 0))
        self.direct_spread_plot.setMaximumSize(QtCore.QSize(16777215, 16777215))
        self.direct_spread_plot.setObjectName("direct_spread_plot")
        self.gridLayout.addWidget(self.direct_spread_plot, 3, 4, 1, 1)
        self.verticalLayout.addLayout(self.gridLayout)
        spacerItem3 = QtGui.QSpacerItem(20, 40, QtGui.QSizePolicy.Minimum, QtGui.QSizePolicy.Expanding)
        self.verticalLayout.addItem(spacerItem3)

        self.retranslateUi(GroupBox)
        QtCore.QMetaObject.connectSlotsByName(GroupBox)

    def retranslateUi(self, GroupBox):
        GroupBox.setWindowTitle(QtGui.QApplication.translate("GroupBox", "GroupBox", None, QtGui.QApplication.UnicodeUTF8))
        GroupBox.setTitle(QtGui.QApplication.translate("GroupBox", "Beam Spreader", None, QtGui.QApplication.UnicodeUTF8))
        self.label.setText(QtGui.QApplication.translate("GroupBox", "Sample scattering data file:", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_scatt_edit.setToolTip(QtGui.QApplication.translate("GroupBox", "Enter a valid data file path.", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_scatt_browse.setText(QtGui.QApplication.translate("GroupBox", "Browse", None, QtGui.QApplication.UnicodeUTF8))
        self.label_2.setText(QtGui.QApplication.translate("GroupBox", "Sample spreader data file:", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_spread_edit.setToolTip(QtGui.QApplication.translate("GroupBox", "Enter a valid data file path.", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_spread_browse.setText(QtGui.QApplication.translate("GroupBox", "Browse", None, QtGui.QApplication.UnicodeUTF8))
        self.label_3.setText(QtGui.QApplication.translate("GroupBox", "Direct scattering data file:", None, QtGui.QApplication.UnicodeUTF8))
        self.label_4.setText(QtGui.QApplication.translate("GroupBox", "Direct spreader data file:", None, QtGui.QApplication.UnicodeUTF8))
        self.direct_scatt_edit.setToolTip(QtGui.QApplication.translate("GroupBox", "Enter a valid data file path.", None, QtGui.QApplication.UnicodeUTF8))
        self.direct_spread_edit.setToolTip(QtGui.QApplication.translate("GroupBox", "Enter a valid data file path.", None, QtGui.QApplication.UnicodeUTF8))
        self.direct_scatt_browse.setText(QtGui.QApplication.translate("GroupBox", "Browse", None, QtGui.QApplication.UnicodeUTF8))
        self.direct_spread_browse.setText(QtGui.QApplication.translate("GroupBox", "Browse", None, QtGui.QApplication.UnicodeUTF8))
        self.label_5.setText(QtGui.QApplication.translate("GroupBox", "Beam spreader transmission:", None, QtGui.QApplication.UnicodeUTF8))
        self.spreader_trans_edit.setToolTip(QtGui.QApplication.translate("GroupBox", "Transmission of the beam spreader in %.", None, QtGui.QApplication.UnicodeUTF8))
        self.label_7.setText(QtGui.QApplication.translate("GroupBox", "+/-", None, QtGui.QApplication.UnicodeUTF8))
        self.spreader_trans_spread_edit.setToolTip(QtGui.QApplication.translate("GroupBox", "Uncertainty on the beam spreader transmission.", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_scatt_plot.setToolTip(QtGui.QApplication.translate("GroupBox", "Click to plot 2D data.", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_scatt_plot.setText(QtGui.QApplication.translate("GroupBox", "Plot", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_spread_plot.setToolTip(QtGui.QApplication.translate("GroupBox", "Click to plot 2D data.", None, QtGui.QApplication.UnicodeUTF8))
        self.sample_spread_plot.setText(QtGui.QApplication.translate("GroupBox", "Plot", None, QtGui.QApplication.UnicodeUTF8))
        self.direct_scatt_plot.setToolTip(QtGui.QApplication.translate("GroupBox", "Click to plot 2D data.", None, QtGui.QApplication.UnicodeUTF8))
        self.direct_scatt_plot.setText(QtGui.QApplication.translate("GroupBox", "Plot", None, QtGui.QApplication.UnicodeUTF8))
        self.direct_spread_plot.setToolTip(QtGui.QApplication.translate("GroupBox", "Click to plot 2D data.", None, QtGui.QApplication.UnicodeUTF8))
        self.direct_spread_plot.setText(QtGui.QApplication.translate("GroupBox", "Plot", None, QtGui.QApplication.UnicodeUTF8))

