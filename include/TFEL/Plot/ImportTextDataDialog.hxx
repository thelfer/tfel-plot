/*! 
 * \file  ImportTextDataDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2012
 */

#ifndef LIB_TFEL_PLOT_IMPORTTEXTDATADIALOG_H_
#define LIB_TFEL_PLOT_IMPORTTEXTDATADIALOG_H_ 

#include<TFEL/Plot/Config.hxx>
#include<TFEL/Plot/ImportTextDataDialogBase.hxx>

namespace tfel
{

  namespace plot
  {

    struct TFELPLOT_VISIBILITY_EXPORT ImportTextDataDialog 
      : public ImportTextDataDialogBase
    {
      ImportTextDataDialog(Graph&, QWidget* const = nullptr);

     protected:
      virtual QStringList getCurveKeys() override;

      virtual QString getFileDescription() const override;

      virtual QStringList getFileExtensions() const override;

    }; // end of ImportTextDataDialog

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_IMPORTTEXTDATADIALOG_H */

