/*!
 * \file   TextDataReader.cxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Nov 2007
 */

#include<sstream>
#include<stdexcept>

#include<QtCore/QFile>
#include<QtCore/QRegExp>
#include<QtCore/QTextStream>

#include"TFEL/Plot/TextDataReader.hxx"

namespace tfel
{

  namespace plot
  {

    TextDataReader::TextDataReader(const QString& file)
    {
      using namespace std;
      QFile f(file);
      if(!f.open(QIODevice::ReadOnly)){
	throw(runtime_error("TextDataReader::TextDataReader: "
			    "can't open '"+file.toStdString()+"'"));
      }
      QTextStream s(&f);
      this->extractData(s);
    }

    TextDataReader::TextDataReader()
    {}// end of TextDataReader::TextDataReader

    void
    TextDataReader::extractData(QTextStream& in)
    {
      lines.clear();
      legends.clear();
      preamble.clear();
      bool firstLine     = true;
      bool firstComments = true;
      unsigned short nbr = 1;
      while(!in.atEnd()){
	QString line = in.readLine();
	if(!line.isEmpty()){
	  if(line[0]=='#'){
	    line.remove(0,1u);
	    if(firstLine){
	      this->legends  = line.split(" ");
	    }
	    if(firstComments){
	      this->preamble.push_back(line);
	    }
	  } else {
	    QStringList tokens(line.split(QRegExp("\\s+"),QString::SkipEmptyParts));
	    if(!tokens.empty()){
	      QStringList::const_iterator p;
	      bool b = true;
	      for(p=tokens.begin();(p!=tokens.end())&&(b);++p){
		p->toDouble(&b);
	      }
	      if(b){
		Line l;
		l.nbr = nbr;
		for(p=tokens.begin();(p!=tokens.end())&&(b);++p){
		  l.values.push_back(p->toDouble());
		}
		this->lines.push_back(l);
	      } else {
		if(firstLine){
		  this->preamble.push_back(line);
		}
	      }
	      firstComments = false;
	    }
	    firstLine = false;
	  }
	}
	++nbr;
      }
    } // end of TextDataReader::TextDataReader
      
    const QStringList&
    TextDataReader::getLegends() const
    {
      return this->legends;
    } // end of TextDataReader::getLegends

    const QStringList&
    TextDataReader::getPreamble() const
    {
      return this->preamble;
    } // end of TextDataReader::getPreamble

    QString
    TextDataReader::getLegend(const unsigned short c) const
    {
      using namespace std;
      if(c==0){
	string msg("TextDataReader::getLegend : ");
	msg += "invalid column index";
	throw(runtime_error(msg));
      }
      if(c>=this->legends.size()+1){
	return "";
      }
      return this->legends[c-1];
    } // end of TextDataReader::getLegend

    unsigned short
    TextDataReader::findColumn(const QString& name) const
    {
      using namespace std;
      if(!this->legends.contains(name)){
	string msg("TextDataReader::findColumn : ");
	msg += "no column named '"+name.toStdString()+"' found'.";
	throw(runtime_error(msg));
      }
      return static_cast<unsigned short>(this->legends.indexOf(name)+1);
    } // end of TextDataReader::findColumn

    QVector<double>
    TextDataReader::getColumn(const unsigned short i) const
    {
      using namespace std;
      QVector<double> tab;
      this->getColumn(tab,i);
      return tab;
    } // end of TextDataReader::getColumn

    void
    TextDataReader::getColumn(QVector<double>& tab,
			      const unsigned short i) const
    {
      using namespace std;
      tab.reserve(this->lines.size());
      int j;
      // current line
      QVector<Line>::const_iterator line;
      // sanity check
      if(i==0u){
	string msg("TextDataReader::getColumn : ");
	msg += "column '0' requested (column numbers begins at '1').";
	throw(runtime_error(msg));
      }
      // treatment
      for(line=this->lines.begin(),j=0;line!=this->lines.end();++line,++j){
	if(line->values.size()<i){
	  ostringstream msg;
	  msg << "TextDataReader::getColumn : line '" 
	      << line->nbr << "' "
	      << "does not have '" << i << "' columns.";
	  throw(runtime_error(msg.str()));
	}
	tab.push_back(line->values[i-1]);
      }
    } // end of TextDataReader::getColumn

    QVector<TextDataReader::Line>::const_iterator
    TextDataReader::begin() const
    {
      return this->lines.begin();
    } // end of TextDataReader::begin()

    QVector<TextDataReader::Line>::const_iterator
    TextDataReader::end() const
    {
      return this->lines.end();
    } // end of TextDataReader::end()

    unsigned short
    TextDataReader::getNumberOfColumns() const
    {
      if(this->lines.isEmpty()){
	return 0u;
      }
      return static_cast<unsigned short>(this->lines[0].values.size());
    }// 

      
  } // end of namespace plot

} // end of namespace tfel

