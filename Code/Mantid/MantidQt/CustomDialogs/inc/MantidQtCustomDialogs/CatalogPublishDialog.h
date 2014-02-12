#ifndef MANTIDQT_CUSTOM_DIALOGS_CATALOGPUBLISHDIALOG_H
#define MANTIDQT_CUSTOM_DIALOGS_CATALOGPUBLISHDIALOG_H

#include "MantidQtAPI/AlgorithmDialog.h"
#include "ui_CatalogPublishDialog.h"

namespace MantidQt
{
  namespace CustomDialogs
  {
    /**
     This class gives specialised dialog for the CatalogPublish algorithm.

     @author Jay Rainey, ISIS Rutherford Appleton Laboratory
     @date 14/01/2014
     Copyright &copy; 2013 ISIS Rutherford Appleton Laboratory & NScD Oak Ridge National Laboratory

     This file is part of Mantid.

     Mantid is free software; you can redistribute it and/or modify
     it under the terms of the GNU General Public License as published by
     the Free Software Foundation; either version 3 of the License, or
     (at your option) any later version.

     Mantid is distributed in the hope that it will be useful,
     but WITHOUT ANY WARRANTY; without even the implied warranty of
     MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
     GNU General Public License for more details.

     You should have received a copy of the GNU General Public License
     along with this program.  If not, see <http://www.gnu.org/licenses/>.

     File change history is stored at: <https://github.com/mantidproject/mantid>.
     Code Documentation is available at: <http://doxygen.mantidproject.org>
    */
    class CatalogPublishDialog : public MantidQt::API::AlgorithmDialog
    {
      Q_OBJECT

      public:
        /// Constructor
        CatalogPublishDialog(QWidget *parent = 0);
        /// Destructor
        ~CatalogPublishDialog();

      private:
        /// Create the inital layout.
        virtual void initLayout();
        /// Populate the investigation number combo-box with investigations that the user can publish to.
        void populateUserInvestigations();

      private slots:
        /// When the "browse" button is clicked open a file browser.
        void workspaceSelected(const QString& wsName);
        /// Set the "FileName" property when a file is selected from the file browser.
        void fileSelected();

      protected:
        /// The form generated by QT Designer.
        Ui::CatalogPublishDialog m_uiForm;
    };
  }
}

#endif /* MANTIDQT_CUSTOM_DIALOGS_CATALOGPUBLISHDIALOG_H */
