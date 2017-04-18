/*!
 * \file   TPlotCLIApplication.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   16 avril 2017
 */

#include<iostream>
#include<QtCore/QDebug>
#include<QtCore/QTemporaryFile>
#include"TFEL/Plot/TPlotCLIApplication.hxx"

static QString temporary_file_name()
{
  QTemporaryFile f;
  if(!f.open()){
    std::cerr << "tplot: can't get temporary file name" << std::endl;
    ::exit(-1);
  }
  return f.fileName();
} // end of temporary_file_name

namespace tfel{

  namespace plot{

    TPlotCLIApplication::TPlotCLIApplication(int argc,char** argv)
      : QCoreApplication(argc,argv),
	g(new QProcess(this)),
	th(new QThread(this))
    {
      const auto fin  = temporary_file_name();
      const auto fout = temporary_file_name();
      QStringList args;
      args << "--input-socket="+fout;
      args << "--output-socket="+fin;
      this->g->start("tplot-gui",args);
      this->h = new tfel::plot::CLIHandler(fout);
      this->h->connect(fin);
      this->h->moveToThread(th);
      QObject::connect(this->th,SIGNAL(started()),
		       this->h, SLOT(process()));
      QObject::connect(this->h, SIGNAL(finished()),
		       this->th,SLOT(quit()));
      QObject::connect(this->h, SIGNAL(finished()),
		       this->h, SLOT(deleteLater()));
      QObject::connect(this->th,SIGNAL(finished()),
		       this,SLOT(quit()));
      QObject::connect(this->th,SIGNAL(finished()),
		       this->th,SLOT(deleteLater()));
      QObject::connect(this->g, SIGNAL(finished(int)),
		       this,    SLOT(exit(int)));
      this->th->start();
    }

    void TPlotCLIApplication::quit()
    {
      QObject::disconnect(this->g, SIGNAL(finished(int)),
			  this,    SLOT(exit(int)));
      this->g->waitForFinished();
      QCoreApplication::quit();
    } // end of TPlotCLIApplication::quit
    
    void TPlotCLIApplication::exit(int s)
    {
      QCoreApplication::exit(s);
    }
    
  }
}
