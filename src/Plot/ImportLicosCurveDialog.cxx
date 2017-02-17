/*! 
 * \file  ImportLicosCurveDialoag.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 juin 2012
 */

#include"TFEL/Plot/ImportLicosCurveDialog.hxx"

namespace tfel
{

  namespace plot
  {

    ImportLicosCurveDialog::ImportLicosCurveDialog(Graph& graph,
						   QWidget * const p)
      : ImportTextDataDialogBase(graph,p)
    {} // end of ImportLicosCurveDialog

    QVector<QString>
    ImportLicosCurveDialog::getCurveKeys()
    {
      using namespace std;
      QVector<QString> k;
      const auto pr = this->data->getPreamble();
      auto p        = pr.begin();
      const auto pe = pr.end();
      if(p==pe){
	return k;
      }
      if(*p!=" first column : time"){
	return k;
      }
      k.push_back("time");
      ++p;
      while(p!=pe){
	const auto& tokens = p->split(QRegExp("\\s+"),QString::SkipEmptyParts);
	if(tokens.size()<4){
	  return k;
	}
	if((tokens[1]!="column")||
	   (tokens[2]!=":")){
	  return k;
	}
	auto       pt  = tokens.begin()+3u;
	const auto pte = tokens.end();
	QString r;
	while(pt!=pte){
	  r += *pt+' ';
	  ++pt;
	}
	k.push_back(r.trimmed());
	++p;
      }
      return k;
    }

    QString
    ImportLicosCurveDialog::getFileDescription() const
    {
      return "Licos Curve";
    }
    
    QStringList
    ImportLicosCurveDialog::getFileExtensions() const
    {
      return QStringList() << "txt" << "dat" << "data" << "res";
    }

    ImportLicosCurveDialog::~ImportLicosCurveDialog() = default;
    
  } // end of namespace plot
  
} // end of namespace tfel

