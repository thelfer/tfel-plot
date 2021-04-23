/*!
 * \file   tplot.cxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   2/03/2017
 */

#include <cstring>
#include <iostream>
#include <QtCore/QProcess>
#ifdef TPLOT_ENABLE_CLI
#include "TFEL/Plot/TPlotCLIApplication.hxx"
#endif /* TPLOT_ENABLE_CLI */

int main(int argc, char** argv) {
  auto args = QStringList{};
  bool cli = false;
  // skipping programm name
  for (const char* const* a = argv + 1; a != argv + argc; ++a) {
    if (strcmp(*a, "--cli") == 0) {
      cli = true;
    }
    args << *a;
  }
  int status = EXIT_SUCCESS;
  if (cli == true) {
#ifdef TPLOT_ENABLE_CLI
    tfel::plot::TPlotCLIApplication a(argc, argv);
    // QCoreApplication a(argc,argv);
    // QProcess g;
    // const auto fin  = temporary_file_name();
    // const auto fout = temporary_file_name();
    // args << "--input-socket="+fout;
    // args << "--output-socket="+fin;
    // g.start("tplot-gui",args);
    // auto h = new tfel::plot::CLIHandler(fout);
    // h->connect(fin);
    // auto th = new QThread(&a);
    // h->moveToThread(th);
    // //    connect(worker, SIGNAL(error(QString)), this,
    // SLOT(errorString(QString))); QObject::connect(th, SIGNAL(started()), h,
    // SLOT(process())); QObject::connect(h, SIGNAL(finished()), th,
    // SLOT(quit())); QObject::connect(h, SIGNAL(finished()), h,
    // SLOT(deleteLater())); QObject::connect(th, SIGNAL(finished()), th,
    // SLOT(deleteLater()));
    // QObject::connect(&g,SIGNAL(finished(int)),&a,SLOT(quit()));
    // th->start();
    return QCoreApplication::exec();
#else  /* TPLOT_ENABLE_CLI */
    status = EXIT_FAILURE;
#endif /* TPLOT_ENABLE_CLI */
  } else {
    status = QProcess::execute("tplot-gui", args);
  }
  return status;
}
