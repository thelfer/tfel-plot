/*! 
 * \file  ImportAlcyoneCurveDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 10 juin 2012
 */

#ifndef LIB_TFEL_PLOT_IMPORTALCYONECURVEDIALOG_H_
#define LIB_TFEL_PLOT_IMPORTALCYONECURVEDIALOG_H_ 

#include<TFEL/Plot/Config.hxx>
#include<TFEL/Plot/ImportTextDataDialogBase.hxx>

namespace tfel
{

  namespace plot
  {

    struct TFELPLOT_VISIBILITY_EXPORT ImportAlcyoneCurveDialog 
      : public ImportTextDataDialogBase
    {

      ImportAlcyoneCurveDialog(Graph&,
			       QWidget * const = nullptr);
      //! destructor
      virtual ~ImportAlcyoneCurveDialog();
      
    protected:
      
      virtual QVector<QString>
      getCurveKeys(void) override;

      virtual QString
      getFileDescription() const override;
    
      virtual QStringList
      getFileExtensions() const override;

    }; // end of ImportAlcyoneCurveDialog

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_IMPORTALCYONECURVEDIALOG_H */

