#ifndef MANTIDQTCUSTOMINTERFACES_DIAGNOSTICS_H_
#define MANTIDQTCUSTOMINTERFACES_DIAGNOSTICS_H_

//----------------------
// Includes
//----------------------
#include "MantidQtCustomInterfaces/ui_Diagnostics.h"
#include "MantidQtCustomInterfaces/ExcitationsDiagResults.h"
#include "MantidKernel/PropertyWithValue.h"
#include "MantidAPI/AnalysisDataService.h"
#include "MantidAPI/IAlgorithm.h"
#include "MantidQtAPI/UserSubWindow.h"
#include <map>
#include <vector>
#include <string>
#include <QString>

namespace MantidQt
{
namespace CustomInterfaces
{
class Diagnostics : public MantidQt::API::UserSubWindow
{
  Q_OBJECT

public:
  /// there has to be a default constructor but you can call this with a pointer to the thing that will take ownership of it
  Diagnostics(QWidget *parent = 0);

public slots:
  void childFormDied();

private:
  //The form generated by Qt Designer
  Ui::Diagnostics m_uiForm;
  // structures to hold the values entered by the use

  // to store default values
  static const char defHighAbsolute[];   ///< default value for reject above this number of counts
  static const char defLowAbsolute[];    ///< default value for reject at or below this number of counts
  static const char defSignificanceTest[];///< default value for the number of errorbars from the median number of counts needs to be before the detector can be rejectedn in a median test
  static const char defHighMedian[];     ///< default value for the maximum number of times the median value the total number of counts can be without being rejected
  static const char defLowMedian[];      ///< default value for the minimum number of times the median value the total number of counts can be without being rejected
  static const char defVariation[];      ///< default value for the maximum allowed variation between white beam vanadium runs over the median
  static const char defBackground[];     ///< default value for the maximum allowed variation between white beam vanadium runs over the median

  /// a pointer to the results dialog box
  ExcitationsDiagResults *m_dispDialog;
  /// we set this to true if python is running to say that we shouldn't start running more python
  bool m_busy;
  /// Pointers to instances of all the algorithms used. From these the properties are taken and used for validation
  std::vector<Mantid::API::IAlgorithm_sptr> m_algorDummies;
  /// Initialize the layout
  virtual void initLayout();
  bool parseInput();
  void clearData();
  void loadAlgorDummies();
  void readTheDialog();
  void placeValidatorLabels();
  ExcitationsDiagResults::TestSummary runWhite1();
  ExcitationsDiagResults::TestSummary runWhite2(
    const ExcitationsDiagResults::TestSummary &lastResults );
  ExcitationsDiagResults::TestSummary runBack(
    const ExcitationsDiagResults::TestSummary &test1,
    const ExcitationsDiagResults::TestSummary &test2);
  /// read in the parameters that users have entered on the form
  QString constructScript() const;
  QString constructScript(
    const ExcitationsDiagResults::TestSummary &test1FoundBad) const;
  QString constructScript(
    const ExcitationsDiagResults::TestSummary &test1,
    const ExcitationsDiagResults::TestSummary &test2) const;
  void readFile(const QString &pythonFile, QString &scriptText) const;
  ExcitationsDiagResults::TestSummary readRes(QString pyhtonOut);
  ExcitationsDiagResults* raiseDialog();
  void getAlgProperties();
  void storeUserSetting(const std::string &varibleName, const QString &value);
  /// enable the run button if the results dialog has been closed and the python has stopped
  void pythonIsRunning(bool running);
private slots:
  ///run the algorithms that can be run with the data that users supplied
  void run();
  void browseClicked(const QString &buttonDis);
  void addFile();
  void removeName(QListWidgetItem *item = 0);
  /// open the wiki page for this interface in their browser
  void helpClicked();

protected:
  /// The properties associated with this algorithm
  QHash<QString, Mantid::Kernel::Property*> m_algProperties;

  /// A list of labels to use as validation markers
  QHash<QString, QLabel*> m_validators;
  /// This sets up the labels that are to be used to mark whether a property is valid.
  void createValidatorLabels();
  /// A map of property <name, value> pairs that have been taken from the dialog
  QHash<QString, QString> m_propertyValueMap;
  /// Data stored in this map may or may not be validated, it is taken from the dialog and passed to the python script generation
  std::map<std::string, QString> m_userSettingsMap;

  std::map<std::string, std::string> m_algorPropList;
  /// Set the properties that have been parsed from the dialog
  bool setPropertyValues();
  void saveInput();
  QLabel* getValidatorMarker(const QString & propname) const;
  void storePropertyValue(const QString & name, const QString & value);
};
}
}

#endif //MANTIDQTCUSTOMINTERFACES_DIAGNOSTICS_H_
