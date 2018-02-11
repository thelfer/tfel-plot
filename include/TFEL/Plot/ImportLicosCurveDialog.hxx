/*! 
 * \file  ImportLicosCurveDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2012
 */

#ifndef LIB_TFEL_PLOT_IMPORTLICOSCURVEDIALOG_H_
#define LIB_TFEL_PLOT_IMPORTLICOSCURVEDIALOG_H_ 

#include<TFEL/Plot/Config.hxx>
#include<TFEL/Plot/ImportTextDataDialogBase.hxx>

namespace tfel
{

  namespace plot
  {

    struct TFELPLOT_VISIBILITY_EXPORT ImportLicosCurveDialog 
      : public ImportTextDataDialogBase
    {
      ImportLicosCurveDialog(Graph&, QWidget* const = nullptr);

      ~ImportLicosCurveDialog() override;

     protected:
      QVector<QString> getCurveKeys() override;

      QString getFileDescription() const override;

      QStringList getFileExtensions() const override;

    }; // end of ImportLicosCurveDialog

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_IMPORTLICOSCURVEDIALOG_H */

