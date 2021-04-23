/*!
 * \file   TPlotCLIApplication.hxx
 * \brief
 * \author THOMAS HELFER
 * \date   16 avril 2017
 */

#ifndef LIB_TPLOT_TPLOTCLIAPPLICATION_HXX
#define LIB_TPLOT_TPLOTCLIAPPLICATION_HXX

#include <QtCore/QCoreApplication>
#include <QtCore/QThread>
#include <QtCore/QProcess>
#include "TFEL/Plot/CLIHandler.hxx"

namespace tfel {
  namespace plot {

    class TPlotCLIApplication : QCoreApplication {
      Q_OBJECT

     public:
      TPlotCLIApplication(int, char **);

     public slots:

      //! \brief exit.
      virtual void quit();

     private slots:

      //! \brief exit.
      virtual void exit(int);

     private:
      QProcess *g;
      QThread *th;
      tfel::plot::CLIHandler *h;
    };

  }  // end of namespace plot
}  // end of namespace tfel

#endif /* LIB_TPLOT_TPLOTCLIAPPLICATION_HXX */
