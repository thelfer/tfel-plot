/*! 
 * \file  TPlot.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 30 mai 2013
 */

#include<cassert>
#include<sstream>

#include<QtCore/QProcess>
#include<QtCore/QMimeData>
#include<QtCore/QUrl>
#include<QtCore/QBuffer>

#ifdef TFEL_QT4
#include<QtGui/QAction>
#include<QtGui/QMenuBar>
#include<QtGui/QMessageBox>
#include<QtGui/QFileDialog>
#include<QtGui/QPrintDialog>
#include<QtGui/QDockWidget>
#include<QtGui/QFontDialog>
#include<QtGui/QRadioButton>
#include<QtGui/QButtonGroup>
#include<QtGui/QVBoxLayout>
#include<QtGui/QDialogButtonBox>
#include<QtGui/QApplication>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QAction>
#include<QtWidgets/QMenuBar>
#include<QtWidgets/QMessageBox>
#include<QtWidgets/QFileDialog>
#include<QtWidgets/QDockWidget>
#include<QtWidgets/QFontDialog>
#include<QtWidgets/QRadioButton>
#include<QtWidgets/QButtonGroup>
#include<QtWidgets/QVBoxLayout>
#include<QtWidgets/QDialogButtonBox>
#include<QtWidgets/QApplication>
#include<QtPrintSupport/QPrintDialog>
#endif /* TFEL_QT5 */
#include<QtGui/QKeyEvent>
#include<QtGui/QClipboard>
#include<QtGui/QImageReader>

#include"TFEL/Plot/GraphConfigurationDialog.hxx"
#include"TFEL/Plot/DataCurve.hxx"
#include"TFEL/Plot/EvaluatedCurve.hxx"
#include"TFEL/Plot/ThemeManager.hxx"
#include"TFEL/Plot/ThemeSelectionAction.hxx"
#include"TFEL/Plot/CurveConfigurationAction.hxx"
#include"TFEL/Plot/TextDataReader.hxx"
#include"TFEL/Plot/ImportTextDataDialog.hxx"
#include"TFEL/Plot/ImportLicosCurveDialog.hxx"
#include"TFEL/Plot/ImportAlcyoneCurveDialog.hxx"
#include"TFEL/Plot/ImportLicosResultsDialog.hxx"
#include"TFEL/Plot/ImportMTestResultsDialog.hxx"

#include"TFEL/Plot/TPlot.hxx"

namespace tfel
{

  namespace plot
  {

    TPlot::CurveOptions::CurveOptions()
      : hasTitle(false),
	hasThemeLineStyle(false),
	themestyle(0),
	hasStyle(false),
	style(Curve::SOLIDLINE),
	hasColor(false),
	axis(Graph::XY)
    {} // end of TPlot::CurveOptions::CurveOptions

    TPlot::CurveOptions&
    TPlot::Input::getCurveOptions()
    {
      if(this->is<TPlot::Data>()){
	return this->get<TPlot::Data>();
      } else if(this->is<TPlot::Function>()){
	return this->get<TPlot::Function>();
      }
      throw(std::runtime_error("TPlot::Input::getCurveOptions: "
			       "input has no curve options"));
    }

    unsigned short TPlot::convertToUnsignedShort(const QString& qvalue)
    {
      const auto value = qvalue.toStdString();
      for(const auto& c : value){
	if(!std::isdigit(c)){
	  throw(std::runtime_error("TPlot::convertToUnsignedShort: invalid entry"));
	}
      }
      std::istringstream converter(value);
      unsigned short u;
      converter >> u;
      if(!converter&&(!converter.eof())){
	throw(std::runtime_error("TPlot::convertToUnsignedShort: "
				 "not read value from token '"+value+"'.\n"));
      }
      return u;
    } // end of TPlot::convertToUnsignedShort

    bool TPlot::isUnsignedShort(const QString& qs)
    {
      const auto s = qs.toStdString();
      for(const auto& c : s){
	if(!isdigit(c)){
	  return false;
	}
      }
      return true;
    } // end of TPlot::isUnsignedShort
 TPlot::TPlot(const int argc,
		 const char * const * const argv)
      : tfel::utilities::ArgumentParserBase<TPlot>(argc,argv),
	g(new Graph(this)),
	shell(new GraphShell(*g,this)),
	name(argv[0])
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b)(throw(std::runtime_error("TPlot::TPlot: "+std::string(m))));
      };
      this->registerArgumentCallBacks();
      this->parseArguments();
      this->initialize();
      this->createActions();
      this->createMainMenu();
      this->shell->setPrompt("tplot>");
      auto *dock = new QDockWidget(tr("TPlot Shell"), this);
      dock->setFeatures(dock->features()^QDockWidget::DockWidgetClosable);
      dock->setAllowedAreas(Qt::BottomDockWidgetArea);
      dock->setWidget(shell);
      this->shell->setVisible(false);
      this->addDockWidget(Qt::BottomDockWidgetArea,dock);
      this->setCentralWidget(this->g);
      // drag an drop
      this->g->setAcceptDrops(false);
      this->setAcceptDrops(true);
      // replot the graph
      this->g->replot();
      if(this->output.isEmpty()){
	this->goutput = true;
      } else {
	throw_if(this->goutput,"an input (for example a gnuplot script, a licos curve) "
		 "is not compatible with the definition of an output file");
	const auto& out = this->output.toStdString();
	auto spos = out.find_last_of('.');
	throw_if(spos==std::string::npos,"invalid output name '" + out+"'");
	const auto extension = out.substr(spos+1);
	if(extension=="pdf"){
	  this->g->exportToPDF(this->output);
#ifdef TFEL_QT4
	} else if(extension=="eps"){
	  this->g->exportToPostScript(this->output);
#endif /* TFEL_QT4 */
	} else if(extension=="svg"){
	  this->g->exportToSVG(this->output);
	} else if(extension=="png"){
	  this->g->exportToPNG(this->output);
	} else {
	  throw_if(true,"invalid output name '"+out+" "
		   "(unsupported extension "+extension+")");
	}
	this->goutput = false;
      }
      QObject::connect(g,SIGNAL(updated()),
		       this,SLOT(createCurvesMenu()));
    }

    void TPlot::dragEnterEvent(QDragEnterEvent *e)
    {
      if (e->mimeData()->hasUrls()){
	e->acceptProposedAction();
      }
    }
    
    struct ChooseImportFilter
      : public QDialog
    {
      ChooseImportFilter(QWidget *p)
	: QDialog(p)
      {
	QRadioButton* b1 = new QRadioButton(QObject::tr("Import as txt file"), this);
	QRadioButton* b2 = new QRadioButton(QObject::tr("Import as licos result file"), this);
	b1->setChecked(true);
	this->bgrp =  new QButtonGroup(this);
	this->bgrp->addButton(b1,0);
	this->bgrp->addButton(b2,1);
	QVBoxLayout *vl = new QVBoxLayout;
	vl->addWidget(b1);
	vl->addWidget(b2);
	bbox = new QDialogButtonBox(QDialogButtonBox::Ok,Qt::Horizontal,this);
	vl->addWidget(bbox);
	this->setLayout(vl);
	// signals
	connect(bbox, SIGNAL(accepted()), this, SLOT(accept()));
      }
      int getFilterId(){
	return this->bgrp->checkedId();
      }
    private:
      QButtonGroup * bgrp;
      QDialogButtonBox *bbox;
    };

    void TPlot::dropEvent(QDropEvent *e)
    {
      if (e->mimeData()->hasUrls()) {
	foreach (QUrl url, e->mimeData()->urls()) {
	  QString f = url.toLocalFile();
	  QFileInfo fi(f);
	  QString ext  = fi.completeSuffix();
	  if((ext=="txt") ||(ext=="dat")||
	     (ext=="data")||(ext=="res")){
	    ChooseImportFilter cif(this);
	    cif.exec();
	    if(cif.getFilterId()==1){
	      ImportLicosCurveDialog *d = new ImportLicosCurveDialog(*(this->g),this);
	      d->exec(f);
	      if(*d){
		d->setWindowModality(Qt::NonModal);
		d->show();
	      }
	    } else {
	      ImportTextDataDialog *d = new ImportTextDataDialog(*(this->g),this);
	      d->exec(f);
	      if(*d){
		d->setWindowModality(Qt::NonModal);
		d->show();
	      }
	    }
	  }
	}
      }
    }

    void TPlot::createActions(void)
    {
      this->epdfa = new QAction(tr("Export to pdf"), this);
      this->epdfa->setIcon(QIcon::fromTheme("document-save-as"));
      this->epdfa->setIconVisibleInMenu(true);
      this->epdfa->setStatusTip(tr("Export the graph in PDF format"));
      QObject::connect(this->epdfa, SIGNAL(triggered()),
		       this, SLOT(exportToPDF()));
      this->esvga = new QAction(tr("Export to svg"), this);
      this->esvga->setIcon(QIcon::fromTheme("document-save-as"));
      this->esvga->setIconVisibleInMenu(true);
      this->esvga->setStatusTip(tr("Export the graph in SVG format"));
      QObject::connect(this->esvga, SIGNAL(triggered()),
		       this, SLOT(exportToSVG()));
#ifdef TFEL_QT4
      this->epsa = new QAction(tr("Export to ps"), this);
      this->epsa->setStatusTip(tr("Export the graph in PostScript format"));
      this->epsa->setIcon(QIcon::fromTheme("document-save-as"));
      this->epsa->setIconVisibleInMenu(true);
      QObject::connect(this->epsa, SIGNAL(triggered()),
		       this, SLOT(exportToPostScript()));
#endif /* TFEL_QT4 */
      this->epnga = new QAction(tr("Export to png"), this);
      this->epnga->setStatusTip(tr("Export the graph in Portable Network Plot format"));
      this->epnga->setIcon(QIcon::fromTheme("document-save-as"));
      this->epnga->setIconVisibleInMenu(true);
      QObject::connect(this->epnga, SIGNAL(triggered()),
		       this, SLOT(exportToPNG()));
      this->etxta = new QAction(tr("Export to txt"), this);
      this->etxta->setIcon(QIcon::fromTheme("document-save-as"));
      this->etxta->setIconVisibleInMenu(true);
      this->etxta->setStatusTip(tr("Export the graph in text format"));
      QObject::connect(this->etxta, SIGNAL(triggered()),
		       this, SLOT(exportToTable()));
      
      this->pa = new QAction(QIcon(":/images/print.png"), tr("&Print"), this);
      this->pa->setStatusTip(tr("Print the graph"));
      this->pa->setIcon(QIcon::fromTheme("document-print"));
      this->pa->setIconVisibleInMenu(true);
      QObject::connect(this->pa, SIGNAL(triggered()), this, SLOT(print()));

      this->na = new QAction(tr("&New window"), this);
      this->na->setShortcuts(QKeySequence::New);
      this->na->setStatusTip(tr("Open a new window"));
      this->na->setIcon(QIcon::fromTheme("document-new"));
      this->na->setIconVisibleInMenu(true);
      QObject::connect(this->na, SIGNAL(triggered()), this,
		       SLOT(newWindow()));

      this->ea = new QAction(tr("E&xit"), this);
      this->ea->setShortcuts(QKeySequence::Quit);
      this->ea->setStatusTip(tr("Exit the application"));
      this->ea->setIcon(QIcon::fromTheme("window-close"));
      this->ea->setIconVisibleInMenu(true);

      QObject::connect(this->ea, SIGNAL(triggered()), this,
		       SLOT(close()));
      this->gca = new QAction(tr("&Configuration"), this);
      this->gca->setStatusTip(tr("Open the graph configuration dialog"));
      QObject::connect(this->gca, SIGNAL(triggered()),
		       this, SLOT(showGraphConfigurationDialog()));
      this->fa = new QAction(tr("&Select Font"), this);
      this->fa->setStatusTip(tr("Open the font selection dialog"));
      QObject::connect(this->fa, SIGNAL(triggered()),
		       this, SLOT(selectFontDialog()));
      this->xloga  = new QAction(tr("set logscale"),this); 
      QObject::connect(this->xloga, SIGNAL(triggered()),
		       this->g, SLOT(setXAxisLogScale()));
      this->x2loga = new QAction(tr("set logscale"),this); 
      QObject::connect(this->x2loga, SIGNAL(triggered()),
		       this->g, SLOT(setX2AxisLogScale()));
      this->yloga  = new QAction(tr("set logscale"),this); 
      QObject::connect(this->yloga, SIGNAL(triggered()),
		       this->g, SLOT(setYAxisLogScale()));
      this->y2loga = new QAction(tr("set logscale"),this); 
      QObject::connect(this->y2loga, SIGNAL(triggered()),
		       this->g, SLOT(setY2AxisLogScale()));
      this->xuloga  = new QAction(tr("unset logscale"),this); 
      QObject::connect(this->xuloga, SIGNAL(triggered()),
		       this->g, SLOT(unsetXAxisLogScale()));
      this->x2uloga = new QAction(tr("unset logscale"),this); 
      QObject::connect(this->x2uloga, SIGNAL(triggered()),
		       this->g, SLOT(unsetX2AxisLogScale()));
      this->yuloga  = new QAction(tr("unset logscale"),this); 
      QObject::connect(this->yuloga, SIGNAL(triggered()),
		       this->g, SLOT(unsetYAxisLogScale()));
      this->y2uloga = new QAction(tr("unset logscale"),this); 
      QObject::connect(this->y2uloga, SIGNAL(triggered()),
		       this->g, SLOT(unsetY2AxisLogScale()));

      this->igsa = new QAction(tr("Import Gnuplot Script"), this);
      this->igsa->setStatusTip(tr("Import a gnuplot script"));
      this->igsa->setIcon(QIcon::fromTheme("document-open"));
      this->igsa->setIconVisibleInMenu(true);

      connect(this->igsa, SIGNAL(triggered()),this,SLOT(importGnuplotScript()));
      this->itda = new QAction(tr("Import Text Data"), this);
      this->itda->setStatusTip(tr("Open a dialog box to import text data"));
      this->itda->setIcon(QIcon::fromTheme("document-open"));
      this->itda->setIconVisibleInMenu(true);
      connect(this->itda, SIGNAL(triggered()),this,SLOT(importTextData()));

      this->ilca = new QAction(tr("Import Licos Curve"), this);
      this->ilca->setStatusTip(tr("Open a dialog box to import licos curves"));
      this->ilca->setIcon(QIcon::fromTheme("document-open"));
      this->ilca->setIconVisibleInMenu(true);
      connect(this->ilca, SIGNAL(triggered()),this,SLOT(importLicosCurve()));

      this->ilra = new QAction(tr("Import Licos Results"), this);
      this->ilra->setStatusTip(tr("Open a dialog box to import licos results"));
      this->ilra->setIcon(QIcon::fromTheme("document-open"));
      this->ilra->setIconVisibleInMenu(true);
      connect(this->ilra, SIGNAL(triggered()),this,SLOT(importLicosResults()));

      this->cba = new QAction(tr("Copy to clipboard as bitmap"), this);
      this->cba->setStatusTip(tr("Copy to clipboard as bitmap"));
      this->cba->setIcon(QIcon::fromTheme("edit-copy"));
      this->cba->setIconVisibleInMenu(true);
      connect(this->cba, SIGNAL(triggered()),
	      this,SLOT(copyToClipboardAsBitmap()));

      this->csvga = new QAction(tr("Copy to clipboard as svg"), this);
      this->csvga->setStatusTip(tr("Copy to clipboard as svg"));
      this->csvga->setIcon(QIcon::fromTheme("edit-copy"));
      this->csvga->setIconVisibleInMenu(true);
      connect(this->csvga, SIGNAL(triggered()),
	      this,SLOT(copyToClipboardAsSVG()));

      this->ctablea = new QAction(tr("Copy to clipboard as plain text"), this);
      this->ctablea->setStatusTip(tr("Copy to clipboard as plain text"));
      this->ctablea->setIcon(QIcon::fromTheme("edit-copy"));
      this->ctablea->setIconVisibleInMenu(true);
      connect(this->ctablea, SIGNAL(triggered()),
	      this,SLOT(copyToClipboardAsTable()));

      this->pastea = new QAction(tr("Paste"), this);
      this->pastea->setStatusTip(tr("Paste from clipboard"));
      connect(this->pastea, SIGNAL(triggered()),
	      this,SLOT(paste()));
      this->pastea->setIcon(QIcon::fromTheme("edit-paste"));
      this->pastea->setIconVisibleInMenu(true);

      this->iia = new QAction(tr("Insert image from file"), this);
      this->iia->setStatusTip(tr("Insert image from file"));
      connect(this->iia, SIGNAL(triggered()),
	      this,SLOT(insertImageFromFile()));

      this->showShellAction = new QAction(tr("Show shell"),this);
      this->showShellAction->setStatusTip(tr("Show the tplot shell"));
      connect(this->showShellAction, SIGNAL(triggered()),
	      this,SLOT(showShell()));
      
      this->showGridAction = new QAction(tr("Show grid"),this);
      this->showGridAction->setStatusTip(tr("Show the graph grid"));
      connect(this->showGridAction, SIGNAL(triggered()),
	      this->g,SLOT(toggleGrid()));

      this->aa = new QAction(tr("&About"), this);

       this->aa->setStatusTip(tr("Show the application's About box"));
      connect(this->aa, SIGNAL(triggered()), this, SLOT(about()));
      this->aa2 = new QAction(tr("About &Qt"), this);
      this->aa2->setStatusTip(tr("Show the Qt library's About box"));
      connect(this->aa2, SIGNAL(triggered()), qApp, SLOT(aboutQt()));
    }

    void TPlot::createCurvesMenu()
    {
      auto curves = this->g->getCurves();
      this->cm->clear();
      auto i = int{0};
      auto p = curves.begin();
      for(;p!=curves.end();++p,++i){
	auto&  c = *(*p);
	auto k = c.getKey();
	if(k.isEmpty()){
	  k = tr("Curve '%1'").arg(QString::number(i));
	}
	auto *ca = new CurveConfigurationAction(&c,k, this);
	ca->setStatusTip(tr("Configure curve '%1'").arg(k));
	this->cm->addAction(ca);
      }
    } // end of TPlot::createCurvesMenu

    void TPlot::createThemesMenu(void)
    {
      const auto& thm = ThemeManager::getThemeManager();
      const auto themes = thm.getAvailableThemesNames();
      this->tm->clear();
      for(auto t : themes){
	auto *ta = new ThemeSelectionAction(t,*(this->g), this);
	ta->setStatusTip(tr("Select the '%1' theme").arg(t));
	this->tm->addAction(ta);
      }
    } // end of TPlot::createThemesMenu
   
    void TPlot::createMainMenu(void)
    {
      this->fm = this->menuBar()->addMenu(tr("&File"));
      this->fm->addAction(this->na);
      this->fm->addSeparator();
      this->dm = this->fm->addMenu(tr("&Open"));
      this->dm->setIcon(QIcon::fromTheme("document-save-as"));
      this->dm->addAction(this->igsa);
      this->dm->addAction(this->itda);
      this->dm->addAction(this->ilca);
      this->dm->addAction(this->ilra);
      this->fm->addAction(this->pa);
      this->edm = this->menuBar()->addMenu(tr("&Edit"));
      this->edm->addAction(this->cba);
      this->edm->addAction(this->csvga);
      this->edm->addAction(this->ctablea);
      this->edm->addSeparator();
      this->edm->addAction(this->pastea);
      this->edm = this->menuBar()->addMenu(tr("&Insert"));
      this->edm->addAction(this->iia);
      this->em = this->fm->addMenu("E&xport");
      this->em->addAction(this->esvga);
      this->em->addAction(this->epdfa);
#ifdef TFEL_QT4
      this->em->addAction(this->epsa);
#endif /* TFEL_QT4 */
      this->em->addAction(this->epnga);
      this->em->addAction(this->etxta);
      this->fm->addSeparator();
      this->fm->addAction(this->ea);
      this->gm = this->menuBar()->addMenu(tr("&Graph"));
      gm->addAction(this->gca);
      gm->addAction(this->fa);
      this->tm = this->gm->addMenu(tr("&Themes"));
      this->createThemesMenu();
      QMenu *xaxis  = this->gm->addMenu(tr("X  Axis"));
      QMenu *x2axis = this->gm->addMenu(tr("X2 Axis"));
      QMenu *yaxis  = this->gm->addMenu(tr("Y  Axis"));
      QMenu *y2axis = this->gm->addMenu(tr("Y2 Axis"));
      xaxis->addAction(this->xloga);
      xaxis->addAction(this->xuloga);
      x2axis->addAction(this->x2loga);
      x2axis->addAction(this->x2uloga);
      yaxis->addAction(this->yloga);
      yaxis->addAction(this->yuloga);
      y2axis->addAction(this->y2loga);
      y2axis->addAction(this->y2uloga);
      this->cm = this->menuBar()->addMenu(tr("&Curves"));
      this->createCurvesMenu();
      QObject::connect(g,SIGNAL(curvesRemoved()),
		       this,SLOT(createCurvesMenu()));
      QObject::connect(g,SIGNAL(curveRemoved(Curve *)),
		       this,SLOT(createCurvesMenu()));
      QObject::connect(g,SIGNAL(curveAdded(Curve *)),
		       this,SLOT(createCurvesMenu()));
      this->sm = this->menuBar()->addMenu(tr("&Show"));
      this->sm->addAction(this->showGridAction);
      this->sm->addAction(this->showShellAction);
      this->hm = this->menuBar()->addMenu(tr("&Help"));
      this->hm->addAction(this->aa);
      this->hm->addAction(this->aa2);
    }

    void TPlot::importLicosResults()
    {
      auto *licos = new ImportLicosResultsDialog((*this->g),this->g);
      if(*licos){
	licos->setWindowModality(Qt::NonModal);
	licos->show();
      }
    } // end of TPlot::importLicosResults

    void TPlot::importGnuplotScript()
    {
      auto f = QFileDialog::getOpenFileName(this,tr("Choose file"),
					    "",tr("Gnuplot script (*.gp *.gnuplot)"));
      if(f.isEmpty()){
	return;
      }
      this->shell->importGnuplotFile(f);
    }

    void TPlot::importTextData()
    {
      auto *d = new ImportTextDataDialog(*(this->g),this);
      d->exec();
      if(*d){
	d->setWindowModality(Qt::NonModal);
	d->show();
      }
    } // end of TPlot::importTextData

    void TPlot::importLicosCurve()
    {
      auto *d = new ImportLicosCurveDialog(*(this->g),this);
      d->exec();
      if(*d){
	d->setWindowModality(Qt::NonModal);
	d->show();
      }
    } // end of TPlot::importLicosCurve

    void TPlot::about()
    {
      QMessageBox::about(this, tr("About TPlot"),
			 tr("The <b>TPlot</b> is a simple "
			    "application around the Graph "
			    "widget."));
    }

    void TPlot::keyPressEvent(QKeyEvent *ev)
    {
      QString k = ev->text();
      if(k=="g"){
	ev->accept();
	this->g->toggleGrid(true);
      } else if(k=="a"){
	ev->accept();
	this->g->unsetXRange(false);
	this->g->unsetYRange(false);
	this->g->unsetX2Range(false);
	this->g->unsetY2Range(false);
	this->g->replot();
      } else if(k=="q"){
	ev->accept();
	this->close();
      } else if(k=="f"){
	ev->accept();
	if(this->isFullScreen()) {
	  this->setWindowState(Qt::WindowNoState);
	} else {
	  this->setWindowState(Qt::WindowFullScreen);
	}
      } else if(k=="p"){
	ev->accept();
	this->print();
      } else {
	ev->ignore();
      }
    }

    const std::vector<std::string> TPlot::tokenize(const std::string& s,
		    const char c)
    {
      std::vector<std::string> res;
      std::string::size_type b = 0u;
      std::string::size_type e = s.find_first_of(c, b);
      while (std::string::npos != e || std::string::npos != b){
	// Found a token, add it to the vector.
	res.push_back(s.substr(b, e - b));
	b = s.find_first_not_of(c, e);
	e = s.find_first_of(c, b);
      }
      return res;
    } // end of TPlot::tokenize
 TPlot::~TPlot()
    {} // end of TPlot::TPlot

    void TPlot::registerArgumentCallBacks(void)
    {
      this->registerNewCallBack("--noborder",&TPlot::treatNoBorder,
				"don't show the graph border");
      this->registerNewCallBack("--xy","-xy",&TPlot::treatXY,
				"specify axis used for drawing");
      this->registerNewCallBack("--x2y","-x2y",&TPlot::treatX2Y,
				"specify axis used for drawing");
      this->registerNewCallBack("--xy2","-xy2",&TPlot::treatXY2,
				"specify axis used for drawing");
      this->registerNewCallBack("--x2y2","-x2y2",&TPlot::treatX2Y2,
				"specify axis used for drawing");
      this->registerNewCallBack("--using","-u",&TPlot::treatUsing,
				"specify columns used for drawing",true);
      this->registerNewCallBack("--output","-o",&TPlot::treatOutput,
				"specify an output file",true);
      this->registerNewCallBack("--color","-c",&TPlot::treatColor,
				"specify color used for "
				"drawing the previous curve",true);
      this->registerNewCallBack("--with-grid",&TPlot::treatWithGrid,
				"add grid");
      this->registerNewCallBack("--xmin",&TPlot::treatXMin,
				"set minimal x value",true);
      this->registerNewCallBack("--xmax",&TPlot::treatXMax,
				"set maximal x value",true);
      this->registerNewCallBack("--ymin",&TPlot::treatYMin,
				"set minimal y value",true);
      this->registerNewCallBack("--ymax",&TPlot::treatYMax,
				"set maximal y value",true);
      this->registerNewCallBack("--x2min",&TPlot::treatX2Min,
				"set minimal x2 value",true);
      this->registerNewCallBack("--x2max",&TPlot::treatX2Max,
				"set maximal x2 value",true);
      this->registerNewCallBack("--y2min",&TPlot::treatY2Min,
				"set minimal y2 value",true);
      this->registerNewCallBack("--y2max",&TPlot::treatY2Max,
				"set maximal y2 value",true);
      this->registerNewCallBack("--theme",&TPlot::treatTheme,
				"set the graph theme",true);
      this->registerNewCallBack("--title","-t",&TPlot::treatTitle,
				"set title of the previous curve",true);
      this->registerNewCallBack("--upperTitle","-ut",&TPlot::treatUpperTitle,
				"set graph upper title",true);
      this->registerNewCallBack("--downTitle","-dt",&TPlot::treatDownTitle,
				"set graph down title",true);
      this->registerNewCallBack("--leftTitle","-lt",&TPlot::treatLeftTitle,
				"set graph left title",true);
      this->registerNewCallBack("--rightTitle","-rt",&TPlot::treatRightTitle,
				"set graph right title",true);
      this->registerNewCallBack("--x2title",&TPlot::treatUpperTitle,
				"set graph upper title",true);
      this->registerNewCallBack("--xtitle",&TPlot::treatDownTitle,
				"set graph down title",true);
      this->registerNewCallBack("--ytitle",&TPlot::treatLeftTitle,
				"set graph left title",true);
      this->registerNewCallBack("--y2title",&TPlot::treatRightTitle,
				"set graph right title",true);
      this->registerNewCallBack("--key-vertical-position","-kv",
				&TPlot::treatKeyVerticalPosition,
				"set key vertical position",true);
      this->registerNewCallBack("--key-horizontal-position","-kh",&TPlot::treatKeyHorizontalPosition,
				"set key horizontal position",true);
      this->registerNewCallBack("--upperLabel","-ul",&TPlot::treatUpperLabel,
				"set graph upper label",true);
      this->registerNewCallBack("--downLabel","-dl",&TPlot::treatDownLabel,
				"set graph down label",true);
      this->registerNewCallBack("--leftLabel","-ll",&TPlot::treatLeftLabel,
				"set graph left label",true);
      this->registerNewCallBack("--rightLabel","-rl",&TPlot::treatRightLabel,
				"set graph right label",true);
      this->registerNewCallBack("--x2label",&TPlot::treatUpperLabel,
				"set graph upper label",true);
      this->registerNewCallBack("--xlabel",&TPlot::treatDownLabel,
				"set graph down label",true);
      this->registerNewCallBack("--ylabel",&TPlot::treatLeftLabel,
				"set graph left label",true);
      this->registerNewCallBack("--y2label",&TPlot::treatRightLabel,
				"set graph right label",true);
      this->registerNewCallBack("--line-style","-ls",&TPlot::treatLineStyle,
				"set the line style (the type of line and the color is given by the theme)",true);
      this->registerNewCallBack("--with-solidline",&TPlot::treatWithSolidLine,
				"draw the preceeding curve with a solid line");
      this->registerNewCallBack("--with-dotline",&TPlot::treatWithDotLine,
				"draw the preceeding curve with a dot line");
      this->registerNewCallBack("--with-dashline",&TPlot::treatWithDashLine,
				"draw the preceeding curve with a dash line");
      this->registerNewCallBack("--with-dashdotline",&TPlot::treatWithDashDotLine,
				"draw the preceeding curve with a dash and dot line");
      this->registerNewCallBack("--with-dashdotdotline",
				&TPlot::treatWithDashDotDotLine,
				"draw the preceeding curve with a succession of dash and two dots");
      this->registerNewCallBack("--with-lineplus",
				&TPlot::treatWithLinePlus,
				"draw the preceeding curve with a solid line and plus mark");
      this->registerNewCallBack("--with-dots",&TPlot::treatWithDots,
				"draw the preceeding curve with dots");
      this->registerNewCallBack("--with-squares",&TPlot::treatWithSquares,
				"draw the preceeding curve with squares");
      this->registerNewCallBack("--with-triangles",&TPlot::treatWithTriangles,
				"draw the preceeding curve with triangles");
      this->registerNewCallBack("--with-diamonds",&TPlot::treatWithDiamonds,
				"draw the preceeding curve with diamonds");
      this->registerNewCallBack("--with-crosses",&TPlot::treatWithCrosses,
				"draw the preceeding curve with crosses");
      this->registerNewCallBack("--font-size","-fs",&TPlot::treatFontSize,
				"specify font size",true);
      this->registerNewCallBack("--font-family","-ff",&TPlot::treatFontFamily,
				"specify font family",true);
      this->registerNewCallBack("--gnuplot-instruction",
				"-gp",&TPlot::treatGnuplotInstruction,
				"add a gnuplot instruction",true);
    } // end of TPlot::registerArgumentCallBacks

    bool TPlot::graphicalOutput(void) const
    {
      return this->goutput;
    }

    void TPlot::getStringFromArgs(QString& w,
			     const std::string& method,
			     const std::string& key)
    {
      const auto& o =this->currentArgument->getOption();
      if(!w.isEmpty()){
	throw(std::runtime_error("TPlot::"+method+": "
				 "key "+key+" has already been declared"));
      }
      if(o.empty()){
	throw(std::runtime_error("TPlot::"+method+": "
				 "no argument given to the "+key+"  declaration"));
      }
      w = QString::fromUtf8(&o[0],o.size());
    } // end of TPlot::getStringFromArgs

    QString TPlot::getStringOption(const bool b)
    {
      const auto o =this->currentArgument->getOption();
      if(!b){
	if(o.empty()){
	  throw(std::runtime_error("TPlot::getStringOption: no argument given to the '"
				   +std::string{*(this->currentArgument)}+"' option"));
	}
      }
      return QString::fromUtf8(&o[0],o.size());
    }

    double TPlot::getDoubleOption()
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b)(throw(std::runtime_error("TPlot::getDoubleOption: "+std::string(m))));
      };
      const auto o =this->currentArgument->getOption();
      throw_if(o.empty(),"no argument given to the '"+
	       std::string{*(this->currentArgument)}+"' option");
      bool b;
      const auto r = QString::fromStdString(o).toDouble(&b);
      throw_if(!b,"can't convert argument given to the '"+
	       std::string{*(this->currentArgument)}+"' option to double");
      return r;
    }

    void TPlot::treatOutput(void)
    {
      this->getStringFromArgs(this->output,"treatOutput","output");
    } // end of TPlot::treatOutput

    void TPlot::treatNoBorder(void)
    {
      this->g->hideBorder(false);
    } // end of TPlot::treatNoBorder

    void TPlot::treatLineStyle()
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b)(throw(std::runtime_error("TPlot::treatLineStyle: "+std::string(m))));
      };
      throw_if(this->inputs.empty(),"no curve specified");
      QString s;
      this->getStringFromArgs(s,"treatLineStyle",
			      "gnuplot instruction");
      if(TPlot::isUnsignedShort(s)){
	const auto ls = TPlot::convertToUnsignedShort(s);
	auto& o = this->inputs.back().getCurveOptions();
	throw_if(o.hasThemeLineStyle,"theme line style already specified");
	o.hasThemeLineStyle = true;
	o.themestyle = ls;
      } else {
	throw_if(true,"can't convert argument given to the '"+
		 std::string{*(this->currentArgument)}+"' option to unsigned short");
      }
    }

    void TPlot::treatGnuplotInstruction(void)
    {
      QString i;
      this->getStringFromArgs(i,"treatGnuplotInstruction",
			      "gnuplot instruction");
      this->shell->treatNewCommand(i);
    } // end of TPlot::treatGnuplotInstruction

    void TPlot::treatTheme(void)
    {
      auto& thm = ThemeManager::getThemeManager();
      this->g->setTheme(thm.getTheme(this->getStringOption()),false);
    } // end of TPlot::treatTheme

    void TPlot::treatTitle(void)
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b)(throw(std::runtime_error("TPlot::treatTitle: "+std::string(m))));
      };
      throw_if(this->inputs.empty(),"no curve declared");
      auto& o = this->inputs.back().getCurveOptions();
      throw_if(o.hasTitle,"title already defined");
      o.hasTitle = true;
      o.title = this->getStringOption(true);
    } // end of TPlot::treatTitle

    void TPlot::treatCurveStyle(const Curve::Style s)
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b)(throw(std::runtime_error("TPlot::treatCurveStyle: "+std::string(m))));
      };
      throw_if(this->inputs.empty(),"no curve declared");
      auto& o = this->inputs.back().getCurveOptions();
      throw_if(o.hasStyle,"curve style already declared");
      o.style    = s;
      o.hasStyle = true;
    } // end of TPlot::treatCurveStyle
    
    void TPlot::treatWithSolidLine(void)
    {
      this->treatCurveStyle(Curve::SOLIDLINE);
    } // end of TPlot::treatWithSolidLine

    void TPlot::treatWithDotLine(void)
    {
      this->treatCurveStyle(Curve::DOTLINE);
    } // end of TPlot::treatWithDotLine

    void TPlot::treatWithDashLine(void)
    {
      this->treatCurveStyle(Curve::DASHLINE);
    } // end of TPlot::treatWithDashLine

    void TPlot::treatWithDashDotLine(void)
    {
      this->treatCurveStyle(Curve::DASHDOTLINE);
    } // end of TPlot::treatWithDashDotLine

    void TPlot::treatWithDashDotDotLine(void)
    {
      this->treatCurveStyle(Curve::DASHDOTDOTLINE);
    } // end of TPlot::treatWithDashDotDotLine

    void TPlot::treatWithLinePlus(void)
    {
      this->treatCurveStyle(Curve::LINEPLUS);
    } // end of TPlot::treatWithLinePlusLine

    void TPlot::treatWithCrosses(void)
    {
      this->treatCurveStyle(Curve::CROSS);
    } // end of TPlot::treatWithCrosses

    void TPlot::treatWithDiamonds(void)
    {
      this->treatCurveStyle(Curve::DIAMOND);
    } // end of TPlot::treatWithDiamonds

    void TPlot::treatWithSquares(void)
    {
      this->treatCurveStyle(Curve::SQUARE);
    } // end of TPlot::treatWithSquares

    void TPlot::treatWithTriangles(void)
    {
      this->treatCurveStyle(Curve::TRIANGLE);
    } // end of TPlot::treatWithTriangles

    void TPlot::treatWithDots(void)
    {
      this->treatCurveStyle(Curve::DOT);
    } // end of TPlot::treatWithDots

    void TPlot::treatXY(void)
    {
      if(this->inputs.empty()){
	throw(std::runtime_error("TPlot::treatXY: "
				 "no curve specified"));
      }
      auto& o = this->inputs.back().getCurveOptions();
      o.axis = Graph::XY;
    } // end of TPlot::treatXY
    
    void TPlot::treatX2Y(void)
    {
      if(this->inputs.empty()){
	throw(std::runtime_error("TPlot::treatX2Y: "
				 "no curve specified"));
      }
      auto& o = this->inputs.back().getCurveOptions();
      o.axis = Graph::X2Y;
    } // end of TPlot::treatX2Y
    
    void TPlot::treatXY2(void)
    {
      if(this->inputs.empty()){
	throw(std::runtime_error("TPlot::treatXY2: "
				 "no curve specified"));
      }
      auto& o = this->inputs.back().getCurveOptions();
      o.axis = Graph::XY2;
    } // end of TPlot::treatX2Y
    
    void TPlot::treatX2Y2(void)
    {
      if(this->inputs.empty()){
	throw(std::runtime_error("TPlot::treatX2Y2: "
				 "no curve specified"));
      }
      auto& o = this->inputs.back().getCurveOptions();
      o.axis = Graph::X2Y2;
    } // end of TPlot::treatX2Y
    
    void TPlot::treatUpperTitle(void)
    {
      this->getStringFromArgs(this->upperTitle,"treatUpperTitle","upperTitle");
    } // end of TPlot::treatUpperTitle

    void TPlot::treatDownTitle(void)
    {
      this->getStringFromArgs(this->downTitle,"treatDownTitle","downTitle");
    } // end of TPlot::treatDownTitle

    void TPlot::treatLeftTitle(void)
    {
      this->getStringFromArgs(this->leftTitle,"treatLeftTitle","leftTitle");
    } // end of TPlot::treatLeftTitle

    void TPlot::treatRightTitle(void)
    {
      this->getStringFromArgs(this->rightTitle,"treatRightTitle","rightTitle");
    } // end of TPlot::treatRightTitle

    void TPlot::treatUpperLabel(void)
    {
      this->getStringFromArgs(this->upperLabel,"treatUpperLabel","upperLabel");
    } // end of TPlot::treatUpperLabel

    void TPlot::treatDownLabel(void)
    {
      this->getStringFromArgs(this->downLabel,"treatDownLabel","downLabel");
    } // end of TPlot::treatDownLabel

    void TPlot::treatLeftLabel(void)
    {
      this->getStringFromArgs(this->leftLabel,"treatLeftLabel","leftLabel");
    } // end of TPlot::treatLeftLabel

    void TPlot::treatRightLabel(void)
    {
      this->getStringFromArgs(this->rightLabel,"treatRightLabel","rightLabel");
    } // end of TPlot::treatRightLabel

    void TPlot::treatWithGrid(void)
    {
      this->hasGrid = true;
    } // end of TPlot::treatWithGrid

    void TPlot::treatKeyHorizontalPosition(void)
    {
      this->getStringFromArgs(this->keyHorizontalPosition,"treatKeyHorizontalPosition",
			      "keyHorizontalPosition");
    } // end of TPlot::treatKeyHorizontalPosition

    void TPlot::treatKeyVerticalPosition(void)
    {
      this->getStringFromArgs(this->keyVerticalPosition,
			      "treatKeyVerticalPosition",
			      "keyVerticalPosition");
    } // end of TPlot::treatKeyVerticalPosition

    void TPlot::treatColor(void)
    {
      if(this->inputs.empty()){
	throw(std::runtime_error("TPlot::treatColor: "
				 "no curve specified"));
      }
      const auto color = this->getStringOption();
      auto& o = this->inputs.back().getCurveOptions();
      if(color=="red"){
	o.hasColor = true;
	o.color = Qt::red;
      } else if(color=="green"){
	o.hasColor = true;
	o.color = Qt::green;
      } else if(color=="blue"){
	o.hasColor = true;
	o.color = Qt::blue;
      } else if(color=="white"){
	o.hasColor = true;
	o.color = Qt::white;
      } else if(color=="black"){
	o.hasColor = true;
	o.color = Qt::black;
      } else if(color=="yellow"){
	o.hasColor = true;
	o.color = Qt::yellow;
      } else {
	QRegExp expr("rgb:(\\d+):(\\d+):(\\d+)");
	if(expr.exactMatch(color)){
	  const auto c_r = expr.cap(1);
	  const auto c_g = expr.cap(2);
	  const auto c_b = expr.cap(3);
	  o.hasColor = true;
	  o.color = QColor(c_r.toInt(),c_g.toInt(),c_b.toInt());
	} else {
	  throw(std::runtime_error("TPlot::treatColor: "
				   "unknown color '"+color.toStdString()+"'"));
	}
      }
    } // end of TPlot::treatColor

    void TPlot::treatXMin(void)
    {
      this->xmin = this->getDoubleOption();
      this->hasXMinValue = true;
    } // end of TPlot::treatXMin

    void TPlot::treatYMin(void)
    {
      this->ymin = this->getDoubleOption();
      this->hasYMinValue = true;
    } // end of TPlot::treatYMin

    void TPlot::treatXMax(void)
    {
      this->xmax = this->getDoubleOption();
      this->hasXMaxValue = true;
    } // end of TPlot::treatXMax

    void TPlot::treatYMax(void)
    {
      this->ymax = this->getDoubleOption();
      this->hasYMaxValue = true;
    } // end of TPlot::treatYMax

    void TPlot::treatX2Min(void)
    {
      this->x2min = this->getDoubleOption();
      this->hasX2MinValue = true;
    } // end of TPlot::treatX2Min

    void TPlot::treatY2Min(void)
    {
      this->y2min = this->getDoubleOption();
      this->hasY2MinValue = true;
    } // end of TPlot::treatY2Min

    void TPlot::treatX2Max(void)
    {
      this->x2max = this->getDoubleOption();
      this->hasX2MaxValue = true;
    } // end of TPlot::treatX2Max

    void TPlot::treatY2Max(void)
    {
      this->y2max = this->getDoubleOption();
      this->hasY2MaxValue = true;
    } // end of TPlot::treatY2Max

    void TPlot::treatUsing(void)
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b)(throw(std::runtime_error("TPlot::treatUsing: "+std::string(m))));
      };
      throw_if(this->inputs.empty(),"no curve specified");
      throw_if(!this->inputs.back().is<Data>(),"last input is not a data curve");
      const auto c = TPlot::tokenize(this->getStringOption().toStdString(),':');
      if(c.size()==1){
	auto& d = this->inputs.back().get<Data>();
	throw_if(!d.xvalues.isEmpty(),"using already called");
	d.xvalues = "__DataCurve__::line_number";
	d.yvalues = QString::fromStdString(c[0]);
      } else {
	throw_if(c.size()!=2,"more than two column specified");
	auto& d = this->inputs.back().get<Data>();
	throw_if(!d.xvalues.isEmpty(),"using already called");
	d.xvalues = QString::fromStdString(c[0]);
	d.yvalues = QString::fromStdString(c[1]);
      }
    } // end of TPlot::treatUsing

    void TPlot::treatUnknownArgument(void)
    {
      auto throw_if = [](const bool b, const std::string& m){
	if(b)(throw(std::runtime_error("TPlot::treatUnknownArgument: "+std::string(m))));
      };
      const auto arg = std::string{*(this->currentArgument)};
      throw_if(arg[0]=='-',"unknown argument " + arg);
      const auto a = QString::fromStdString(arg);
      if(a.startsWith("gp:")){
	this->goutput = true;
	GnuplotScript s;
	s.fileName = a.mid(3);
	throw_if(s.fileName.isEmpty(),"empty gnuplot script name");
	Input i;
	i.set(s);
	this->inputs.push_back(i);
      } else if(a.startsWith("f:")){
	Function f;
	f.f = a.mid(2);
	throw_if(f.f.isEmpty(),"empty function");
	Input i;
	i.set(f);
	this->inputs.push_back(i);
      } else if(a.startsWith("txt:")){
	this->goutput = true;
	TextDataInput d;
	d.fileName = a.mid(4);
	throw_if(d.fileName.isEmpty(),"empty gnuplot script name");
	Input i;
	i.set(d);
	this->inputs.push_back(i);
      } else if(a.startsWith("lc:")){
	this->goutput = true;
	LicosCurve c;
	c.fileName = a.mid(3);
	throw_if(c.fileName.isEmpty(),"empty gnuplot script name");
	Input i;
	i.set(c);
	this->inputs.push_back(i);
      } else if(a.startsWith("mtest:")){
	this->goutput = true;
	MTestCurve c;
	if(a.startsWith("mtest:")){
	  c.fileName = a.mid(6);
	}
	throw_if(c.fileName.isEmpty(),"empty gnuplot script name");
	Input i;
	i.set(c);
	this->inputs.push_back(i);
      } else if(a.startsWith("alcyone:")){
	this->goutput = true;
	AlcyoneCurve c;
	if(a.startsWith("alcyone:")){
	  c.fileName = a.mid(8);
	}
	throw_if(c.fileName.isEmpty(),"empty alcyone results file name");
	Input i;
	i.set(c);
	this->inputs.push_back(i);
      } else if(a.startsWith("licos:")){
	this->goutput = true;
	LicosResults r;
	r.fileName = a.mid(6);
	throw_if(r.fileName.isEmpty(),"empty licos results file name");
	Input i;
	i.set(r);
	this->inputs.push_back(i);
      } else {
	Data d;
	d.fileName = a;
	if(this->dataSources.find(d.fileName)==this->dataSources.end()){
	  this->dataSources.insert({d.fileName,std::make_shared<TextDataReader>(d.fileName)});
	}
	Input i;
	i.set(d);
	this->inputs.push_back(i);
      }
    } // end of TPlot::treatUnknownArgument(void)

    std::string TPlot::getVersionDescription(void) const
    {
      return "1.0";
    }

    std::string TPlot::getUsageDescription(void) const
    {
      return "Usage: "+this->programName+" [options] [files]";
    }

    void TPlot::initialize(void)
    {
      std::string extension;
      if(this->inputs.empty()){
	return;
      }
      if(!this->upperTitle.isEmpty()){
	this->g->setUpperTitle(this->upperTitle,false);
      }
      if(!this->downTitle.isEmpty()){
	this->g->setDownTitle(this->downTitle,false);
      }
      if(!this->leftTitle.isEmpty()){
	this->g->setLeftTitle(this->leftTitle,false);
      }
      if(!this->rightTitle.isEmpty()){
	this->g->setRightTitle(this->rightTitle,false);
      }
      if (this->keyVerticalPosition=="top"){
	this->g->setKeyVerticalPosition(Graph::TOP,false);
      } else if (this->keyVerticalPosition=="bottom"){
	this->g->setKeyVerticalPosition(Graph::BOTTOM,false);
      } else if (this->keyVerticalPosition=="center"){
	this->g->setKeyVerticalPosition(Graph::VCENTER,false);
      }
      if (this->keyHorizontalPosition=="left"){
	this->g->setKeyHorizontalPosition(Graph::LEFT,false);
      } else if (this->keyHorizontalPosition=="right"){
	this->g->setKeyHorizontalPosition(Graph::RIGHT,false);
      } else if (this->keyHorizontalPosition=="center"){
	this->g->setKeyHorizontalPosition(Graph::HCENTER,false);
      }
      if(!this->upperLabel.isEmpty()){
	this->g->setX2Label(this->upperLabel,false);
      } else {
	if(this->inputs.size()==1u){
	  if(this->inputs.front().is<Data>()){
	    const Data& d = this->inputs.front().get<Data>();
	    if(d.axis&Graph::x2axis){
	      auto p2 = this->dataSources.find(d.fileName);
	      assert(p2!=this->dataSources.end());
	      if(!d.xvalues.isEmpty()){
		if(TPlot::isUnsignedShort(d.xvalues)){
		  auto c = TPlot::convertToUnsignedShort(d.xvalues);
		  this->g->setX2Label(p2->second->getLegend(c),false);
		}
	      } else {
		this->g->setX2Label(p2->second->getLegend(1),false);
	      }
	    }
	  }
	}
      }

      if(!this->downLabel.isEmpty()){
	this->g->setXLabel(this->downLabel,false);
      } else {
	if(this->inputs.size()==1u){
	  if(this->inputs.front().is<Data>()){
	    const Data& d = this->inputs.front().get<Data>();
	    if(d.axis&Graph::xaxis){
	      auto p2 = this->dataSources.find(d.fileName);
	      assert(p2!=this->dataSources.end());
	      if(!d.xvalues.isEmpty()){
		if(TPlot::isUnsignedShort(d.xvalues)){
		  auto c = TPlot::convertToUnsignedShort(d.xvalues);
		  this->g->setXLabel(p2->second->getLegend(c),false);
		}
	      } else {
		this->g->setXLabel(p2->second->getLegend(1),false);
	      }
	    }
	  }
	}
      }

      if(!this->leftLabel.isEmpty()){
	this->g->setYLabel(this->leftLabel,false);
      } else {
	if(this->inputs.size()==1u){
	  if(this->inputs.front().is<Data>()){
	    const Data& d = this->inputs.front().get<Data>();
	    if(d.axis&Graph::yaxis){
	      auto p2 = this->dataSources.find(d.fileName);
	      if(!d.yvalues.isEmpty()){
		if(TPlot::isUnsignedShort(d.yvalues)){
		  auto c = TPlot::convertToUnsignedShort(d.yvalues);
		  this->g->setYLabel(p2->second->getLegend(c),false);
		}
	      } else {
		this->g->setYLabel(p2->second->getLegend(2),false);
	      }
	    }
	  }
	}
      }

      if(!this->rightLabel.isEmpty()){
	this->g->setY2Label(this->rightLabel,false);
      } else {
	if(this->inputs.size()==1u){
	  if(this->inputs.front().is<Data>()){
	    const Data& d = this->inputs.front().get<Data>();
	    if(d.axis&Graph::y2axis){
	      auto p2 = this->dataSources.find(d.fileName);
	      if(!d.yvalues.isEmpty()){
		if(TPlot::isUnsignedShort(d.yvalues)){
		  auto c = TPlot::convertToUnsignedShort(d.yvalues);
		  this->g->setY2Label(p2->second->getLegend(c),false);
		}
	      } else {
		this->g->setY2Label(p2->second->getLegend(2),false);
	      }
	    }
	  }
	}
      }

      if(this->hasGrid){
	this->g->showGrid(false);
      } else {
	this->g->hideGrid(false);
      }

      for(const auto& i : this->inputs){
	if(i.is<Data>()){
	  this->treatDataInput(i.get<Data>());
	} else if(i.is<Function>()){
	  this->treatFunctionInput(i.get<Function>());
	} else if(i.is<GnuplotScript>()){
	  if(!this->output.isEmpty()){
	    throw(std::runtime_error("TPlot::initialize: "
				     "gnuplot script is not compatible with "
				     "the definition of an output file"));	  
	  }
	  const auto& s = i.get<GnuplotScript>();
	  this->shell->importGnuplotFile(s.fileName);
	}
      }
      this->setRanges();
      this->g->replot();
    } // end of TPlot::initialize

    void TPlot::treatPendingInputs()
    {
      for(const auto& i : this->inputs){
	if(i.is<TextDataInput>()){
	  const auto& c = i.get<TextDataInput>();
	  auto *d = new ImportTextDataDialog(*(this->g),this);
	  d->exec(c.fileName);
	  if(*d){
	    d->setWindowModality(Qt::NonModal);
	    d->show();
	  }
	} else if(i.is<LicosCurve>()){
	  const auto& c = i.get<LicosCurve>();
	  auto *d = new ImportLicosCurveDialog(*(this->g),this);
	  d->exec(c.fileName);
	  if(*d){
	    d->setWindowModality(Qt::NonModal);
	    d->show();
	  }
	} else 	if(i.is<LicosResults>()){
	  const auto& c = i.get<LicosResults>();
	  auto *d = new ImportLicosResultsDialog(*(this->g),
						 c.fileName,this);
	  if(*d){
	    d->setWindowModality(Qt::NonModal);
	    d->show();
	    d->exec();
	  }
	} else if(i.is<MTestCurve>()){
	  const auto& c = i.get<MTestCurve>();
	  auto *d = new ImportMTestResultsDialog(*(this->g),this);
	  d->exec(c.fileName);
	  if(*d){
	    d->setWindowModality(Qt::NonModal);
	    d->show();
	  }
	} else if(i.is<AlcyoneCurve>()){
	  const auto& c = i.get<AlcyoneCurve>();
	  auto *d = new ImportAlcyoneCurveDialog(*(this->g),this);
	  d->exec(c.fileName);
	  if(*d){
	    d->setWindowModality(Qt::NonModal);
	    d->show();
	  }
	}
      }
    } // end of TPlot::treatPendingInputs

    void TPlot::treatDataInput(const TPlot::Data& d)
    {
      auto p2 = this->dataSources.find(d.fileName);
      assert(p2!=this->dataSources.end());
      DataCurve * curve;
      unsigned short cx = 1;
      unsigned short cy = 2;
      bool bcx = true;
      bool bcy = true;
      if(!d.xvalues.isEmpty()){
	if(TPlot::isUnsignedShort(d.xvalues)){
	  cx  = TPlot::convertToUnsignedShort(d.xvalues);
	} else {
	  bcx = false;
	}
      }
      if(!d.yvalues.isEmpty()){
	if(TPlot::isUnsignedShort(d.yvalues)){
	  cy  = TPlot::convertToUnsignedShort(d.yvalues);
	} else {
	  bcy = false;
	}
      }
      if(bcx&&bcy){
	curve = new DataCurve(d.fileName,cx,cy);
      } else if(bcx&&(!bcy)){
	curve = new DataCurve(d.fileName,cx,d.yvalues);
      } else if((!bcx)&&bcy){
	curve = new DataCurve(d.fileName,d.xvalues,cy);
      } else {
	curve = new DataCurve(d.fileName,d.xvalues,d.yvalues);
      }
      if(d.hasThemeLineStyle){
	curve->setThemeLineStyle(d.themestyle);
      } else {
	curve->setStyle(d.style,false);
      }
      if(d.hasColor){
	curve->setColor(d.color,false);
      }
      if(d.hasTitle){
	curve->setKey(d.title,false);
      } else {
	if(bcy){
	  unsigned short coly = 2;
	  if(!d.yvalues.isEmpty()){
	    coly = TPlot::convertToUnsignedShort(d.yvalues);
	  }
	  if(!p2->second->getLegend(coly).isEmpty()){
	    curve->setKey(p2->second->getLegend(coly),false);
	  } else {
	    std::ostringstream legend;
	    if(d.xvalues.isEmpty()){
	      legend << "\"" << d.fileName.toStdString() << "\" using " 
		     << 1 << ":" << coly;
	    } else {
	      legend << "\"" << d.fileName.toStdString() << "\" using " 
		     << d.xvalues.toStdString() << ":" << coly;
	    }
	    curve->setKey(QString::fromStdString(legend.str()),false);
	  }
	} else {
	  curve->setKey(d.yvalues,false);
	}
      }
      this->g->addCurve(std::shared_ptr<Curve>(curve),d.axis);
    }

    void TPlot::treatFunctionInput(const TPlot::Function& f)
    {
      using namespace tfel::math;
      auto ev = std::make_shared<Evaluator>(f.f.toStdString());
      std::shared_ptr<Curve> curve(new EvaluatedCurve(ev));
      if(f.hasThemeLineStyle){
	curve->setThemeLineStyle(f.themestyle);
      } else {
	curve->setStyle(f.style,false);
      }
      if(f.hasColor){
	curve->setColor(f.color,false);
      }
      if(f.hasTitle){
	curve->setKey(f.title,false);
      } else {
	curve->setKey(f.f,false);
      }
      this->g->addCurve(curve,f.axis);
    } // end of TPlot::treatFunctionInput

    void TPlot::treatFontSize(void){
      this->g->setGraphFontSize(this->getDoubleOption());
    }

    void TPlot::treatFontFamily(void){
      this->g->setGraphFontFamily(this->getStringOption());
    }

    void TPlot::paste()
    {
      const auto* m =  QApplication::clipboard()->mimeData();
      if(m->hasText()){
	TextDataReader dr;
	try{
#ifdef TFEL_QT4
	  QTextStream s(m->text().toAscii());
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
	  QTextStream s(m->text().toLatin1());
#endif /* TFEL_QT5 */
	  dr.extractData(s);
	} catch(...){
	  return;
	}
	const unsigned short c = dr.getNumberOfColumns();
	if(c>=2){
	  try{
	    QVector<double> vx = dr.getColumn(1);
	    for(unsigned short i=1;i!=c;++i){
	      QVector<double> vy = dr.getColumn(i+1);
	      auto curve = std::shared_ptr<Curve>(new DataCurve(vx,vy));
	      curve->setKey(dr.getLegend(i));
	      this->g->addCurve(curve);
	      this->g->replot();
	    }
	  } catch(...){
	    return;
	  }
	}
	return;
      }
    } // end of TPlot::paste;

    void TPlot::copyToClipboardAsBitmap()
    {
      QApplication::clipboard()->setImage(this->g->exportToQImage(),
					  QClipboard::Clipboard);
    } // end of TPlot::copyToClipboardAsBitmap

    void TPlot::copyToClipboardAsSVG()
    {
      auto * d = new QMimeData();
      d->setData("image/svg+xml",this->g->exportToSVG());
      QApplication::clipboard()->setMimeData(d,QClipboard::Clipboard);
    } // end of TPlot::copyToClipboardAsSVG

    void TPlot::copyToClipboardAsTable()
    {
      auto * d = new QMimeData();
      QString table_data(this->g->exportToTable());
      d->setData("text/plain",table_data.toUtf8());
      QApplication::clipboard()->setMimeData(d,QClipboard::Clipboard);
    } // end of TPlot::copyToClipboardAsTable

    void TPlot::insertImageFromFile(void)
    {
      QList<QByteArray> exts = QImageReader::supportedImageFormats();
      QList<QByteArray>::const_iterator p;
      QString e;
      for(p=exts.begin();p!=exts.end();){
	e += "*."+*p;
	if(++p!=exts.end()){
	  e += " ";
	}
      }
      const auto f = QFileDialog::getOpenFileName(this,tr("Open Image"),"",
					       tr("Image Files (%1)").arg(e));
      if(!f.isEmpty()){
	this->g->insertImage(QPixmap(f));
      }
    } // end of TPlot::insertImageFromFile

    void TPlot::exportToPDF()
    {
      const auto f = QFileDialog::getSaveFileName(this,tr("Choose file"),
						  "",tr("Adobe Portable Document Format (*.pdf)"));
      if(f.isEmpty()){
	return;
      }
      this->g->exportToPDF(f);
    }

    void TPlot::exportToSVG()
    {
      const auto f = QFileDialog::getSaveFileName(this,tr("Choose file"),
						  "",tr("Scalable Vector Plot Format (*.svg)"));
      if(f.isEmpty()){
	return;
      }
      this->g->exportToSVG(f);
    }

    void TPlot::exportToPNG()
    {
      const auto f = QFileDialog::getSaveFileName(this,tr("Choose file"),
						  "",tr("Portable Network Plot Format (*.png)"));
      if(f.isEmpty()){
	return;
      }
      this->g->exportToPNG(f);
    }

    void TPlot::exportToTable()
    {
      const auto f = QFileDialog::getSaveFileName(this,tr("Choose file"),
						  "",tr("Text (*.txt)"));
      if(f.isEmpty()){
	return;
      }
      this->g->exportToTable(f);
    }

#ifdef TFEL_QT4
    void TPlot::exportToPostScript()
    {
      const auto f = QFileDialog::getSaveFileName(this,tr("Choose file"),
						  "",tr("PostScript Format (*.ps)"));
      if(f.isEmpty()){
	return;
      }
      this->g->exportToPostScript(f);
    }
#endif /* TFEL_QT4 */

    void TPlot::print()
    {
      QPrinter printer(QPrinter::HighResolution);
      QPrintDialog printDialog(&printer,this);
      if (printDialog.exec() == QDialog::Accepted) {
	this->g->print(&printer);
      }
    }

    void TPlot::setRanges()
    {
      using namespace std;
      if(this->hasXMinValue||this->hasXMaxValue){
	double amin = this->g->getXMin();
	double amax = this->g->getXMax();
	if(this->hasXMinValue){
	  amin = this->xmin;
	}
	if(this->hasXMaxValue){
	  amax = this->xmax;
	}
	this->g->setXRange(amin,amax,false);
      }
      if(this->hasYMinValue||this->hasYMaxValue){
	auto amin = this->g->getYMin();
	auto amax = this->g->getYMax();
	if(this->hasYMinValue){
	  amin = this->ymin;
	}
	if(this->hasYMaxValue){
	  amax = this->ymax;
	}
	this->g->setYRange(amin,amax,false);
      }
      if(this->hasX2MinValue||this->hasX2MaxValue){
	auto amin = this->g->getX2Min();
	auto amax = this->g->getX2Max();
	if(this->hasX2MinValue){
	  amin = this->x2min;
	}
	if(this->hasX2MaxValue){
	  amax = this->x2max;
	}
	this->g->setX2Range(amin,amax,false);
      }
      if(this->hasY2MinValue||this->hasY2MaxValue){
	auto amin = this->g->getY2Min();
	auto amax = this->g->getY2Max();
	if(this->hasY2MinValue){
	  amin = this->y2min;
	}
	if(this->hasY2MaxValue){
	  amax = this->y2max;
	}
	this->g->setY2Range(amin,amax,false);
      }
    } // end of TPlot::setRanges

    void TPlot::showGraphConfigurationDialog()
    {
      auto& gcd = this->g->getConfigurationDialog();
      if(!gcd.isVisible()){
	gcd.show();
      }
    } // end of TPlot::showGraphConfigurationDialog

    void TPlot::selectFontDialog()
    {
      bool ok;
      auto f  = QFontDialog::getFont(&ok,this->g->getGraphFont(),this,"Select graph font");
      if(ok){
	this->g->setGraphFont(f);
      }
    } // end of TPlot::selectFontDialog
  
    void TPlot::showShell(void)
    {
      this->shell->setHidden(!this->shell->isHidden());
    }

    void TPlot::newWindow(void)
    {
      QProcess::startDetached(this->name);
    } // end of TPlot::newWindow

  } // end of namespace plot

} // end of namespace tfel


