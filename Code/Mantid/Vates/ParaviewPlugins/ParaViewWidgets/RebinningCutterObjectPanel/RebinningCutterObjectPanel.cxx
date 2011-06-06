#include "RebinningCutterObjectPanel.h"
#include "pqObjectPanelInterface.h"
#include "pqPropertyManager.h"
#include "pqNamedWidgets.h"
#include "vtkSMStringVectorProperty.h"
#include <QLayout>
#include <QMessageBox>
#include <QCheckBox>
#include "GeometryWidget.h"
#include "MantidVatesAPI/RebinningCutterPresenter.h"
#include "MantidGeometry/MDGeometry/MDGeometryXMLParser.h"
#include "MantidVatesAPI/SynchronisingGeometryPresenter.h"

RebinningCutterObjectPanel::RebinningCutterObjectPanel(pqProxy* pxy, QWidget* p) :
pqAutoGeneratedObjectPanel(pxy, p), m_geometryXMLString(""), m_geometryWidget(NULL)
{
  //Auto generated widgets are replaced by Custom Widgets. Autogenerated ones need to be removed.
  removeAutoGeneratedWidgets();
}

/// Event handler for framework event.
void RebinningCutterObjectPanel::updateInformationAndDomains()
{
  using namespace Mantid::VATES;
  using namespace Mantid::Geometry;
  this->proxy()->UpdatePropertyInformation();

  vtkSMStringVectorProperty* inputGeometryProperty = vtkSMStringVectorProperty::SafeDownCast(
    this->proxy()->GetProperty("InputGeometryXML"));

  std::string geometryXMLString = inputGeometryProperty->GetElement(0);
  
  if(m_geometryXMLString != geometryXMLString) //Only attempt to reconstruct the geometry widget if the xml has changed.
  {
    try
    {
      MDGeometryXMLParser xmlParser(geometryXMLString);
      xmlParser.execute();
      //Empty geometry widget added to layout.
      if(m_geometryWidget != NULL)
      {
        this->layout()->removeWidget(m_geometryWidget);
        delete m_geometryWidget;
      }
      m_geometryWidget = new GeometryWidget(new SynchronisingGeometryPresenter(xmlParser), true);
      this->layout()->addWidget(m_geometryWidget);


      connect(m_geometryWidget, SIGNAL(ignoreBinChanges()), this, SLOT(ignoreBinChangesListner()));

      vtkSMProperty * appliedGeometryXML = this->proxy()->GetProperty("AppliedGeometryXML");

      //Hook up geometry change event to listener on filter.
      this->propertyManager()->registerLink(m_geometryWidget, "GeometryXML",
        SIGNAL(valueChanged()), this->proxy(), appliedGeometryXML);

      m_geometryXMLString = geometryXMLString;
    }
    catch(std::exception& ex)
    {
      UNUSED_ARG(ex);
      QMessageBox::information(NULL, "Setup Not possible.",
        "Could not interpret metadata. Are you using a rebinning source? Check field data.");
    }
  }
}

/// Direct removal of autogenerated widgets.
void RebinningCutterObjectPanel::removeAutoGeneratedWidgets()
{
  popWidget(); // Autogenerated Geometry QLineEdit
  popWidget(); // Autogenerated Geometry QLabel
}

/// Pop widgets off the layout and hide them.
void RebinningCutterObjectPanel::popWidget()
{
  unsigned int size = layout()->count();
  if(size >= 1)
  {
    //Pop the last widget off the layout and hide it.
    QLayoutItem* pLayoutItem = layout()->itemAt(size - 1);
    QWidget* pWidget = pLayoutItem->widget();
    if (NULL == pWidget)
    {
      throw std::domain_error(
        "Error ::popWidget(). Attempting to pop a non-widget object off the layout!");
    }
    else
    {
      pWidget->setHidden(true);
      this->layout()->removeItem(pLayoutItem);
    }
  }
}

/// Listener for ignore bin changes.
void RebinningCutterObjectPanel::ignoreBinChangesListner()
{
  QLayoutItem *child;
  unsigned int size = layout()->count();
  for(unsigned int i = 0; i < size; i++)
  {
    child = layout()->itemAt(i);
    QWidget* pWidget = child->widget();
    if (NULL != pWidget) // capability query of layout item.
    {
      QCheckBox* checkBox;
      if((checkBox = dynamic_cast<QCheckBox*>(pWidget)) != NULL) //capability query of widget.
      {
        // Apply Clip check box set to unchecked.
        checkBox->setChecked(false);
        break;
      }
    }
  }

}
