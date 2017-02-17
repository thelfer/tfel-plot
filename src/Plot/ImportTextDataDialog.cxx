/*! 
 * \file  ImportTextDataDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juin 2012
 */

#include<QtCore/QtDebug>

#include"TFEL/Plot/ImportTextDataDialog.hxx"

namespace tfel
{

  namespace plot
  {


    ImportTextDataDialog::ImportTextDataDialog(Graph& graph,
					       QWidget * const p)
      : ImportTextDataDialogBase(graph,p)
    {} // end of TextDataTreeWidget
    
    QVector<QString>
    ImportTextDataDialog::getCurveKeys()
    {
      using namespace std;
      QVector<QString> k;
      QStringList l = this->data->getLegends();
      if(!l.isEmpty()){
	k.resize(l.size());
	QVector<QString>::iterator pk;
	QStringList::const_iterator pl;
	for(pk=k.begin(),pl=l.begin();pl!=l.end();++pl,++pk){
	  *pk = *pl;
	}
      }
      return k;
    }

    QString
    ImportTextDataDialog::getFileDescription() const
    {
      return "Text Data";
    }
    
    QStringList
    ImportTextDataDialog::getFileExtensions() const
    {
      return QStringList() << "txt" << "dat" << "data" << "res";
    }

  } // end of namespace plot
  
} // end of namespace tfel

