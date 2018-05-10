/*!
 * \file   tplot-gui.cxx
 * \brief
 * \author Helfer Thomas
 * \date   21 jan 2008
 */

<<<<<<< HEAD
#include<QtCore/QtDebug>
#include<QtWidgets/QMessageBox>
#include<QtWidgets/QApplication>
#include"TFEL/Plot/InitRessources.hxx"
#include"TFEL/Plot/TPlot.hxx"

namespace tfel
{

  namespace plot
  {

    struct TPlotApplication
      :public QApplication 
    {
      TPlotApplication(int &c, char **v)
	: QApplication(c, v)
      {}

      bool notify(QObject *rec, QEvent *ev) override
      {
	try {
           return QApplication::notify(rec, ev);
	}
	catch (std::exception& e) {
	  //	  QMessageBox::critical(nullptr,"TPlotApplication",e.what());
	  return false;
	}
	catch (...) {
	  //	  QMessageBox::critical(this,"TPlotApplication","unknown exception");
	  abort();
	}
=======
#include <QtCore/QtDebug>
#include <QtWidgets/QMessageBox>
#include <QtWidgets/QApplication>
#include "TFEL/Plot/TPlot.hxx"

namespace tfel {

  namespace plot {

    struct TPlotApplication : public QApplication {
      TPlotApplication(int &c, char **v) : QApplication(c, v) {}

      bool notify(QObject *rec, QEvent *ev) override {
        try {
          return QApplication::notify(rec, ev);
        } catch (std::exception &e) {
          QMessageBox::critical(nullptr, "TPlotApplication", e.what());
          return false;
        } catch (...) {
          //	  QMessageBox::critical(this,"TPlotApplication","unknown
          //exception");
          abort();
        }
>>>>>>> b30f13705f2f5dbecf436d7743f410b99de6ef2d
      }
    };  // end of struct TPlotApplication

  }  // end of namespace plot

}  // end of namespace tfel

<<<<<<< HEAD
int main(int argc,char** argv)
{
  tfel::plot::initRessources();
=======
int main(int argc, char **argv) {
  Q_INIT_RESOURCE(TFELPlotResources);
>>>>>>> b30f13705f2f5dbecf436d7743f410b99de6ef2d
  tfel::plot::TPlotApplication a(argc, argv);
  try {
    tfel::plot::TPlot tplot(argc, argv);
    if (tplot.graphicalOutput()) {
      if (!tplot.isCLIModeEnabled()) {
        tplot.show();
      }
      tplot.treatPendingInputs();
      return tfel::plot::TPlotApplication::exec();
    }
  } catch (std::exception &e) {
    qDebug() << e.what();
    return EXIT_FAILURE;
  } catch (...) {
    qDebug() << "TPlotApplication : unknown exception!";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
