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

    QVector<QString>
    ImportAlcyoneCurveDialog::getCurveKeys()
    {
      const QStringList pr = this->data->getPreamble();
      if(pr.size()!=1u){
	return {};
      }
      QVector<QString> r;
      const auto l = pr[0].split(QRegExp("\\s"));
      for(const auto& s: l){
	r.append(s);
      }
      return r;
    }

    QString
    ImportAlcyoneCurveDialog::getFileDescription() const
    {
      return "Alcyone Curve";
    }
    
    QStringList
    ImportAlcyoneCurveDialog::getFileExtensions() const
    {
      return QStringList() << "res";
    }

    ImportAlcyoneCurveDialog::~ImportAlcyoneCurveDialog() = default;
    
  } // end of namespace plot
  
} // end of namespace tfel
