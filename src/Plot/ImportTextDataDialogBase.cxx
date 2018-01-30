/*! 
 * \file  ImportTextDataDialogBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juin 2012
 */

#include<sstream>
#include<stdexcept>

#include<QtWidgets/QLabel>
#include<QtWidgets/QFileDialog>
#include<QtWidgets/QMessageBox>
#include<QtWidgets/QGridLayout>
#include<QtWidgets/QVBoxLayout>
#include<QtWidgets/QHBoxLayout>
#include<QtWidgets/QPushButton>
#include"TFEL/Raise.hxx"
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
      const auto& value = qvalue.toStdString();
      for(const auto& c : value){
	tfel::raise_if(!isdigit(c),
		       "ImportTextDataDialogBase::convertToUnsignedShort:"
		       " invalid entry");
      }
      unsigned short u;
      std::istringstream converter(value);
      converter >> u;
      tfel::raise_if(!converter&&(!converter.eof()),
		     "ImportTextDataDialogBase::convertToUnsignedShort: "
		     "not read value from token '"+value+"'.");
      return u;
    } // end of ImportTextDataDialogBase::convertToUnsignedShort

    bool ImportTextDataDialogBase::isUnsignedShort(const QString& qs)
    {
      for(const auto c : qs.toStdString()){
	if(!isdigit(c)){
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

    int ImportTextDataDialogBase::exec()
    {
      const auto& d = this->getFileDescription();
      const auto& e = this->getFileExtensions();
      QString desc;
      desc = d;
      if(!e.empty()){
	desc += "(";
	for(auto pe=e.begin();pe!=e.end();){
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

    bool ImportTextDataDialogBase::exec(const QString& f)
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

    void ImportTextDataDialogBase::createGUI()
    {
      using namespace std;
      // get the layout created by the curve configuration dialog
      auto *vl = new QVBoxLayout;
      auto *hl = new QHBoxLayout;
      auto *gl = new QGridLayout;
      const auto l = this->getCurveKeys();
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
      QObject::connect(&g,&Graph::destroyed,
		       this,&ImportTextDataDialogBase::close);
      QObject::connect(cb,&QPushButton::pressed,
		       this,&ImportTextDataDialogBase::close);
      QObject::connect(ib,&QPushButton::pressed,
		       this,&ImportTextDataDialogBase::import);
      QObject::connect(ib2,&QPushButton::pressed,
		       this,&ImportTextDataDialogBase::importAndClose);
      QObject::connect(this->a,&QLineEdit::textChanged,
		       this,&ImportTextDataDialogBase::abscissaChanged);
      QObject::connect(this->o,&QLineEdit::textChanged,
		       this,&ImportTextDataDialogBase::ordinateChanged);
      QObject::connect(this->la,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
		       this,&ImportTextDataDialogBase::currentAbscissaIndexChanged);
      QObject::connect(this->lo,static_cast<void (QComboBox::*)(int)>(&QComboBox::activated),
		       this,&ImportTextDataDialogBase::currentOrdinateIndexChanged);
      QObject::connect(this->ke,&QLineEdit::textChanged,
		       this,&ImportTextDataDialogBase::keyChanged);
      vl->addLayout(this->cl);
      vl->addLayout(gl);
      vl->addLayout(hl);
      this->setLayout(vl);
      this->setWindowTitle(this->file);
    }

    void ImportTextDataDialogBase::import()
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
	  c = shared_ptr<Curve>(new DataCurve(this->file,"",cx,cy));
	} else if((!bcx)&&bcy){
	  c = shared_ptr<Curve>(new DataCurve(this->file,"",xl,cy));
	} else if(bcx&&(!bcy)){
	  c = shared_ptr<Curve>(new DataCurve(this->file,"",cx,yl));
	} else {
	  c = shared_ptr<Curve>(new DataCurve(this->file,"",xl,yl));
	}
	auto nc = this->cd->selectedColor();
	if(nc.isValid()){
	  c->setColor(nc,false);
	}
	auto s = Curve::intToCurveStyle(this->scb->currentIndex());
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

    void ImportTextDataDialogBase::importAndClose()
    {
      this->import();
      this->close();
    } // end of ImportTextDataDialogBase::close

    void ImportTextDataDialogBase::abscissaChanged(const QString&)
    {
      this->afirst = false;
    } // end of ImportTextDataDialogBase::abscissaChanged

    void ImportTextDataDialogBase::ordinateChanged(const QString&)
    {
      this->ofirst = false;
    } // end of ImportTextDataDialogBase::ordinateChanged

    void ImportTextDataDialogBase::currentAbscissaIndexChanged(int i)
    {
      QObject::disconnect(this->a,&QLineEdit::textChanged,
			  this,&ImportTextDataDialogBase::abscissaChanged);
      if(this->afirst){
	this->a->setText("$"+QString::number(i+1));
      } else {
	this->a->setText(this->a->text()+"$"+QString::number(i+1));
      }
      QObject::connect(this->a,&QLineEdit::textChanged,
		       this,&ImportTextDataDialogBase::abscissaChanged);
    } // end of ImportTextDataDialogBase::currentAbscissaIndexChanged

    void ImportTextDataDialogBase::currentOrdinateIndexChanged(int i)
    {
      QObject::disconnect(this->o,&QLineEdit::textChanged,
			  this,&ImportTextDataDialogBase::ordinateChanged);
      if(this->ofirst){
	this->o->setText("$"+QString::number(i+1));
      } else {
	this->o->setText(this->o->text()+"$"+QString::number(i+1));
      }
      QObject::connect(this->o,&QLineEdit::textChanged,
		       this,&ImportTextDataDialogBase::ordinateChanged);
      if(!this->userDefinedKey){
	QObject::disconnect(this->ke,&QLineEdit::textChanged,
			    			 this,&ImportTextDataDialogBase::keyChanged);
	this->ke->setText(this->legends[i]);
	QObject::connect(this->ke,&QLineEdit::textChanged,
			 this,&ImportTextDataDialogBase::keyChanged);
      }
    } // end of ImportTextDataDialogBase::currentOrdinateIndexChanged

    void ImportTextDataDialogBase::keyChanged(const QString&)
    {
      this->userDefinedKey = true;
    } // end of ImportTextDataDialogBase::keyChanged

    void ImportTextDataDialogBase::loadFile()
    {
      try{
	this->data = std::make_shared<TextDataReader>();
	this->data->extractData(this->file);
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

    void ImportTextDataDialogBase::fails(const QString& msg)
    {
      QMessageBox::information(this,tr("Text Data"),msg);
      this->ok = false;
    } // end of ImportTextDataDialogBase::fails()

  } // end of namespace plot
  
} // end of namespace tfel

