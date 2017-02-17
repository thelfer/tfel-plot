/*!
 * \file   plot.cxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   21 jan 2008
 */

#include<QtCore/QtDebug>
#ifdef TFEL_QT4
#include<QtGui/QApplication>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QApplication>
#endif /* TFEL_QT5 */

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

      virtual bool notify(QObject *rec, QEvent *ev) override
      {
	using namespace std;
	try {
           return QApplication::notify(rec, ev);
	}
	catch (exception& e) {
	  qDebug() << e.what();
	  return false;
	}
	catch (...) {
	  qDebug() << "TPlotApplication : unknown exception!";
	  abort();
	}
      }
    }; // end of struct TPlotApplication

  } // end of namespace plot

} // end of namespace tfel

int main(int argc,
	 char** argv)
{
  using namespace std;
  using namespace tfel::plot;
  Q_INIT_RESOURCE(TFELPlotResources);
  TPlotApplication a(argc, argv);
  try{
    TPlot tplot(argc,argv);
    if(tplot.graphicalOutput()){
      tplot.show();
      tplot.treatPendingInputs();
      return TPlotApplication::exec();
    }
  } catch (exception& e) {
    qDebug() << e.what();
    return EXIT_FAILURE;
  }
  catch (...) {
    qDebug() << "TPlotApplication : unknown exception!";
    return EXIT_FAILURE;
  }
  return EXIT_SUCCESS;
}
