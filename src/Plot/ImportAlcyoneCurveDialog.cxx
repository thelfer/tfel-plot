/*! 
 * \file  ImportAlcyoneCurveDialoag.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 juin 2012
 */

#include"TFEL/Plot/ImportAlcyoneCurveDialog.hxx"

namespace tfel
{

  namespace plot
  {

    ImportAlcyoneCurveDialog::ImportAlcyoneCurveDialog(Graph& graph,
						       QWidget * const p)
      : ImportTextDataDialogBase(graph,p)
    {} // end of ImportAlcyoneCurveDialog

    QVector<QString> ImportAlcyoneCurveDialog::getCurveKeys(){
      return QVector<QString>::fromList(this->data->getLegends());
    }

    QString ImportAlcyoneCurveDialog::getFileDescription() const{
      return "Alcyone Curve";
    }
    
    QStringList ImportAlcyoneCurveDialog::getFileExtensions() const{
      return QStringList() << "res";
    }

    ImportAlcyoneCurveDialog::~ImportAlcyoneCurveDialog() = default;
    
  } // end of namespace plot
  
} // end of namespace tfel
