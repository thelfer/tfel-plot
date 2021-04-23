/*!
 * \file   TPlotCLIApplication.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   16 avril 2017
 */

#include <iostream>
#include <QtCore/QDebug>
#include <QtCore/QTemporaryFile>
#include "TFEL/Plot/TPlotCLIApplication.hxx"

static QString temporary_file_name() {
  QTemporaryFile f;
  if (!f.open()) {
    std::cerr << "tplot: can't get temporary file name" << std::endl;
    ::exit(-1);
  }
  return f.fileName();
}  // end of temporary_file_name

namespace tfel {

  namespace plot {

    TPlotCLIApplication::TPlotCLIApplication(int argc, char** argv)
        : QCoreApplication(argc, argv),
          g(new QProcess(this)),
          th(new QThread(this)) {
      const auto fin = temporary_file_name();
      const auto fout = temporary_file_name();
      QStringList args;
      args << "--input-socket=" + fout;
      args << "--output-socket=" + fin;
      this->g->start("tplot-gui", args);
      this->h = new tfel::plot::CLIHandler(fout);
      this->h->connect(fin);
      this->h->moveToThread(th);
      QObject::connect(this->th, &QThread::started, this->h,
                       &CLIHandler::process);
      QObject::connect(this->h, &CLIHandler::finished, this->th,
                       &QThread::quit);
      QObject::connect(this->h, &CLIHandler::finished, this->h,
                       &CLIHandler::deleteLater);
      QObject::connect(this->th, &QTread::finished, this,
                       TPlotCLIApplication::quit);
      QObject::connect(this->th, &QThread::finished, this->th,
                       &QThread::deleteLater);
      QObject::connect(this->g, &QProcess::finished, this,
                       &TPlotCLIApplication::exit);
      this->th->start();
    }

    void TPlotCLIApplication::quit() {
      QObject::disconnect(this->g, &QProcess::finished, this,
                          &TPlotCLIApplication::exit);
      this->g->waitForFinished();
      QCoreApplication::quit();
    }  // end of TPlotCLIApplication::quit

    void TPlotCLIApplication::exit(int s) { QCoreApplication::exit(s); }

  }  // namespace plot
}  // namespace tfel
