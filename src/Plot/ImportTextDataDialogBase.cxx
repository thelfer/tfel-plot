/*! 
 * \file  ImportTextDataDialogBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juin 2012
 */

#include<sstream>
#include<stdexcept>

#ifdef TFEL_QT4
#include<QtGui/QLabel>
#include<QtGui/QFileDialog>
#include<QtGui/QMessageBox>
#include<QtGui/QGridLayout>
#include<QtGui/QVBoxLayout>
#include<QtGui/QHBoxLayout>
#include<QtGui/QPushButton>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QLabel>
#include<QtWidgets/QFileDialog>
#include<QtWidgets/QMessageBox>
#include<QtWidgets/QGridLayout>
#include<QtWidgets/QVBoxLayout>
#include<QtWidgets/QHBoxLayout>
#include<QtWidgets/QPushButton>
#endif /* TFEL_QT5 */

#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/DataCurve.hxx"
#include"TFEL/Plot/ImportTextDataDialogBase.hxx"

namespace tfel
{

  namespace plot
  {

    unsigned short
    ImportTextDataDialogBase::convertToUnsignedShort(const QString& qvalue)
    {
      using namespace std;
      const string& value = qvalue.toStdString();
      string::const_iterator p;
      istringstream converter(value);
      for(p=value.begin();p!=value.end();++p){
	if(!isdigit(*p)){
	  throw(runtime_error("ImportTextDataDialogBase::convertToUnsignedShort : invalid entry"));
	}
      }
      unsigned short u;
      converter >> u;
      if(!converter&&(!converter.eof())){
	string msg("ImportTextDataDialogBase::convertToUnsignedShort : ");
	msg += "not read value from token '"+value+"'.\n";
	throw(runtime_error(msg));
      }
      return u;
    } // end of ImportTextDataDialogBase::convertToUnsignedShort

    bool
    ImportTextDataDialogBase::isUnsignedShort(const QString& qs)
    {
      using namespace std;
      const string& s = qs.toStdString();
      string::const_iterator p;
      for(p=s.begin();p!=s.end();++p){
	if(!isdigit(*p)){
	  return false;
	}
      }
      return true;
    } // end of ImportTextDataDialogBase::isUnsignedShort

    ImportTextDataDialogBase::ImportTextDataDialogBase(Graph& graph,
						       QWidget * const p)
      : CurveConfigurationDialogBase(p),
	g(graph),
	a(new QLineEdit),
	o(new QLineEdit),
	la(new QComboBox),
	lo(new QComboBox),
	userDefinedKey(false),
	afirst(true),
	ofirst(true),
	ok(true)
    {
      this->setAttribute(Qt::WA_DeleteOnClose);
    } // end of TextDataTreeWidget

#ifdef TFEL_QT4
    bool
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
    int
#endif /* TFEL_QT5 */
    ImportTextDataDialogBase::exec()
    {
      const QString& d = this->getFileDescription();
      const QStringList& e = this->getFileExtensions();
      QString desc;
      desc = d;
      if(!e.empty()){
	desc += "(";
	for(QStringList::const_iterator pe=e.begin();pe!=e.end();){
	  desc += "*."+*pe;
	  if(++pe!=e.end()){
	    desc += " ";
	  }
	}
	desc += ")";
      }
      this->file = QFileDialog::getOpenFileName(this,
						tr("Open %1 File").arg(d),
						"",desc);
      if(this->file.isEmpty()){
	this->ok = false;
	return this->ok;
      }
      this->loadFile();
      if(this->ok){
	this->createGUI();
      }
      return this->ok;
    }

    bool
    ImportTextDataDialogBase::exec(const QString& f)
    {
      this->file = f;
      if(this->file.isEmpty()){
	this->ok = false;
	return this->ok;
      }
      this->loadFile();
      if(this->ok){
	this->createGUI();
      }
      return this->ok;
    } // end of ImportTextDataDialogBase

    void
    ImportTextDataDialogBase::createGUI()
    {
      using namespace std;
      // get the layout created by the curve configuration dialog
      QVBoxLayout *vl = new QVBoxLayout;
      QHBoxLayout *hl = new QHBoxLayout;
      QGridLayout *gl = new QGridLayout;
      QVector<QString> l = this->getCurveKeys();
      this->setWindowTitle(tr("Import %1").arg(this->getFileDescription()));
      if(!l.isEmpty()){
	QVector<QString>::const_iterator pl;
	int i;
	for(pl=l.begin(),i=1;pl!=l.end();++pl,++i){
	  this->legends.append(*pl);
	  this->la->addItem(QString::number(i)+" : "+*pl);
	  this->lo->addItem(QString::number(i)+" : "+*pl);
	}
	if(l.size()==1){
	  this->ke->setText(l[0]);
	  this->a->setText("$1");
	  this->o->setText("$1");
  	} else {
	  this->ke->setText(l[1]);
	  this->lo->setCurrentIndex(1);
	  this->a->setText("$1");
	  this->o->setText("$2");
	}
      } else {
	this->ke->setText(this->file);
	this->a->setText("$1");
	this->o->setText("$2");
	this->la->setDisabled(true);
	this->lo->setDisabled(true);
      }
      gl->addWidget(new QLabel("Abscissa"),0,0);
      gl->addWidget(this->a,0,1);
      gl->addWidget(this->la,0,2);
      gl->addWidget(new QLabel("Ordinate"),1,0);
      gl->addWidget(this->o,1,1);
      gl->addWidget(this->lo,1,2);
      auto *cb  = new QPushButton(QObject::tr("Cancel"),this);
      auto *ib  = new QPushButton(QObject::tr("Import"),this);
      auto *ib2 = new QPushButton(QObject::tr("Import and close"),this);
      hl->addWidget(cb);
      hl->addWidget(ib);
      hl->addWidget(ib2);
      QObject::connect(&g,SIGNAL(destroyed ()),
		       this,SLOT(close()));
      QObject::connect(cb,SIGNAL(pressed ()),
		       this,SLOT(close()));
      QObject::connect(ib,SIGNAL(pressed ()),
		       this,SLOT(import()));
      QObject::connect(ib2,SIGNAL(pressed ()),
		       this,SLOT(importAndClose()));
      QObject::connect(this->a,SIGNAL(textChanged(const QString&)),
		       this,SLOT(abscissaChanged(const QString&)));
      QObject::connect(this->o,SIGNAL(textChanged(const QString&)),
		       this,SLOT(ordinateChanged(const QString&)));
      QObject::connect(this->la,SIGNAL(activated(int)),
		       this,SLOT(currentAbscissaIndexChanged(int)));
      QObject::connect(this->lo,SIGNAL(activated(int)),
		       this,SLOT(currentOrdinateIndexChanged(int)));
      QObject::connect(this->ke,SIGNAL(textChanged(const QString&)),
		       this,SLOT(keyChanged(const QString&)));
      vl->addLayout(this->cl);
      vl->addLayout(gl);
      vl->addLayout(hl);
      this->setLayout(vl);
      this->setWindowTitle(this->file);
    }

    void
    ImportTextDataDialogBase::import()
    {
      using namespace std;
      shared_ptr<Curve> c;
      const QString& xl = this->a->text();
      const QString& yl = this->o->text();
      const bool bcx = ImportTextDataDialogBase::isUnsignedShort(xl);
      const bool bcy = ImportTextDataDialogBase::isUnsignedShort(yl);
      unsigned short cx = 0;
      unsigned short cy = 0;
      try{
	if(bcx){
	  cx = ImportTextDataDialogBase::convertToUnsignedShort(xl);
	}
	if(bcy){
	  cy = ImportTextDataDialogBase::convertToUnsignedShort(yl);
	}
	if(bcx&&bcy){
	  c = shared_ptr<Curve>(new DataCurve(this->file,cx,cy));
	} else if((!bcx)&&bcy){
	  c = shared_ptr<Curve>(new DataCurve(this->file,xl,cy));
	} else if(bcx&&(!bcy)){
	  c = shared_ptr<Curve>(new DataCurve(this->file,cx,yl));
	} else {
	  c = shared_ptr<Curve>(new DataCurve(this->file,xl,yl));
	}
	QColor nc = this->cd->selectedColor();
	if(nc.isValid()){
	  c->setColor(nc,false);
	}
	Curve::Style s = Curve::intToCurveStyle(this->scb->currentIndex());
	c->setKey(this->ke->text());
	c->setStyle(s,false);
	this->g.addCurve(c);
	this->g.replot();
      } catch(exception& e){
	this->fails(e.what());
      } catch(...){
	this->fails(QObject::tr("Unknown exception"));
      }
    } // end of ImportTextDataDialogBase::import

    void
    ImportTextDataDialogBase::importAndClose()
    {
      this->import();
      this->close();
    } // end of ImportTextDataDialogBase::close

    void
    ImportTextDataDialogBase::abscissaChanged(const QString&)
    {
      this->afirst = false;
    } // end of ImportTextDataDialogBase::abscissaChanged

    void
    ImportTextDataDialogBase::ordinateChanged(const QString&)
    {
      this->ofirst = false;
    } // end of ImportTextDataDialogBase::ordinateChanged

    void
    ImportTextDataDialogBase::currentAbscissaIndexChanged(int i)
    {
      QObject::disconnect(this->a,SIGNAL(textChanged(const QString&)),
			  this,SLOT(abscissaChanged(const QString&)));
      if(this->afirst){
	this->a->setText("$"+QString::number(i+1));
      } else {
	this->a->setText(this->a->text()+"$"+QString::number(i+1));
      }
      QObject::connect(this->a,SIGNAL(textChanged(const QString&)),
		       this,SLOT(abscissaChanged(const QString&)));
    } // end of ImportTextDataDialogBase::currentAbscissaIndexChanged

    void
    ImportTextDataDialogBase::currentOrdinateIndexChanged(int i)
    {
      QObject::disconnect(this->o,SIGNAL(textChanged(const QString&)),
			  this,SLOT(ordinateChanged(const QString&)));
      if(this->ofirst){
	this->o->setText("$"+QString::number(i+1));
      } else {
	this->o->setText(this->o->text()+"$"+QString::number(i+1));
      }
      QObject::connect(this->o,SIGNAL(textChanged(const QString&)),
		       this,SLOT(ordinateChanged(const QString&)));
      if(!this->userDefinedKey){
	QObject::disconnect(this->ke,SIGNAL(textChanged(const QString&)),
			 this,SLOT(keyChanged(const QString&)));
	this->ke->setText(this->legends[i]);
	QObject::connect(this->ke,SIGNAL(textChanged(const QString&)),
			 this,SLOT(keyChanged(const QString&)));
      }
    } // end of ImportTextDataDialogBase::currentOrdinateIndexChanged

    void
    ImportTextDataDialogBase::keyChanged(const QString&)
    {
      this->userDefinedKey = true;
    } // end of ImportTextDataDialogBase::keyChanged

    void
    ImportTextDataDialogBase::loadFile()
    {
      try{
	this->data = std::make_shared<TextDataReader>(this->file);
      } catch(std::exception& e){
	QString msg("ImportTextDataDialogBase::loadFile :\n");
	msg += e.what();
	this->fails(msg);
      } catch(...){
	QString msg("ImportTextDataDialogBase::loadFile :\n");
	msg += "unknown exception";
	this->fails(msg);
      }
    }

    ImportTextDataDialogBase::operator bool () const
    {
      return this->ok;
    }

    void
    ImportTextDataDialogBase::fails(const QString& msg)
    {
      QMessageBox::information(this,tr("Text Data"),msg);
      this->ok = false;
    } // end of ImportTextDataDialogBase::fails()

  } // end of namespace plot
  
} // end of namespace tfel

