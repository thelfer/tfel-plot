/*!
 * \file   ImportMTestCurveDialog.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   26 janv. 2016
 */

#include"TFEL/Plot/ImportMTestResultsDialog.hxx"

namespace tfel {

  namespace plot {

    ImportMTestResultsDialog::ImportMTestResultsDialog(Graph& graph,
                                                       QWidget* const p)
        : ImportLicosCurveDialog(graph, p) {
    }  // end of ImportMTestResultsDialog

    QString ImportMTestResultsDialog::getFileDescription() const {
      return "MTest results";
    }

    QStringList ImportMTestResultsDialog::getFileExtensions() const {
      return QStringList() << "res";
    }

    ImportMTestResultsDialog::~ImportMTestResultsDialog() = default;
    
  } // end of namespace plot
  
} // end of namespace tfel
