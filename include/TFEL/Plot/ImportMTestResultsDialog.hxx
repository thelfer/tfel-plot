/*!
 * \file  ImportMTestResultsDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 26 janvier 2016
 */

#ifndef LIB_TFEL_PLOT_IMPORTMTESTRESULTSDIALOG_H_
#define LIB_TFEL_PLOT_IMPORTMTESTRESULTSDIALOG_H_

#include <TFEL/Plot/Config.hxx>
#include <TFEL/Plot/ImportLicosCurveDialog.hxx>

namespace tfel {

  namespace plot {

    struct TFELPLOT_VISIBILITY_EXPORT ImportMTestResultsDialog
        : public ImportLicosCurveDialog {
      ImportMTestResultsDialog(Graph&, QWidget* const = nullptr);

      ~ImportMTestResultsDialog() override;

     protected:
      QString getFileDescription() const override;

      QStringList getFileExtensions() const override;

    };  // end of ImportMTestResultsDialog

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_IMPORTMTESTRESULTSDIALOG_H */
