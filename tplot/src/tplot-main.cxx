/*!
 * \file   tplot.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   2/03/2017
 */

#include<cstring>
#include<iostream>
#ifdef TPLOT_ENABLE_CLI
#include<QtCore/QThread>
#endif /* TPLOT_ENABLE_CLI */
#include<QtCore/QProcess>
#include<QtCore/QStringList>
#include<QtCore/QTemporaryFile>
#include<QtCore/QCoreApplication>
#ifdef TPLOT_ENABLE_CLI
#include"TFEL/Plot/CLIHandler.hxx"
#endif /* TPLOT_ENABLE_CLI */

static QString temporary_file_name()
{
  QTemporaryFile f;
  if(!f.open()){
    std::cerr << "tplot: can't get temporary file name" << std::endl;
    ::exit(-1);
  }
  return f.fileName();
} // end of temporary_file_name

int main(int argc,char** argv)
{
  auto args = QStringList{};
  bool cli = false;
  // skipping programm name
  for(const char* const* a = argv+1;a!=argv+argc;++a){
    if(strcmp(*a,"--cli")==0){
      cli = true;
    }
    args << *a;
  }
  int status = EXIT_SUCCESS;
  if(cli==true){
#ifdef TPLOT_ENABLE_CLI
    QCoreApplication a(argc,argv);
    QProcess g;
    const auto fin  = temporary_file_name();
    const auto fout = temporary_file_name();
    args << "--input-socket="+fout;
    args << "--output-socket="+fin;
    g.start("tplot-gui",args);
    auto h  = new tfel::plot::CLIHandler(fout);
    auto th = new QThread(&a);
    h->moveToThread(th);
    //    connect(worker, SIGNAL(error(QString)), this, SLOT(errorString(QString)));
    QObject::connect(th, SIGNAL(started()), h, SLOT(process()));
    QObject::connect(h, SIGNAL(finished()), th, SLOT(quit()));
    QObject::connect(h, SIGNAL(finished()), h, SLOT(deleteLater()));
    QObject::connect(th, SIGNAL(finished()), th, SLOT(deleteLater()));
    th->start();
    return QCoreApplication::exec();
#else  /* TPLOT_ENABLE_CLI */
    status=EXIT_FAILURE;
#endif /* TPLOT_ENABLE_CLI */
  } else {
    status = QProcess::execute("tplot-gui",args);
  }
  return status;
}
