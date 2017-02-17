/*! 
 * \file  GraphConfigurationDialog.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_GRAPHCONFIGURATIONDIALOG_H_
#define _LIB_TFEL_PLOT_GRAPHCONFIGURATIONDIALOG_H_ 

#ifdef TFEL_QT4
#include<QtGui/QDialog>
#include<QtGui/QLineEdit>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QDialog>
#include<QtWidgets/QLineEdit>
#endif /* TFEL_QT5 */

#include<TFEL/Plot/Config.hxx>

namespace tfel
{

  namespace plot
  {

    //! forward declaration
    class  Graph;
    
    /*!
     * class handling graph configuration
     */
    class TFELPLOT_VISIBILITY_EXPORT GraphConfigurationDialog
      : public QDialog
    {
      Q_OBJECT
    public:
      GraphConfigurationDialog(Graph&);
    protected slots:
      void xMinRangeChanged(const QString&);
      void xMaxRangeChanged(const QString&);
      void graphXRangeChanged(const qreal, const qreal);
      void x2MinRangeChanged(const QString&);
      void x2MaxRangeChanged(const QString&);
      void graphX2RangeChanged(const qreal, const qreal);
      void yMinRangeChanged(const QString&);
      void yMaxRangeChanged(const QString&);
      void graphYRangeChanged(const qreal, const qreal);
      void y2MinRangeChanged(const QString&);
      void y2MaxRangeChanged(const QString&);
      void graphY2RangeChanged(const qreal, const qreal);
      void xLabelChanged(const QString&);
      void graphXLabelChanged(const QString&);
      void yLabelChanged(const QString&);
      void graphYLabelChanged(const QString&);
      void x2LabelChanged(const QString&);
      void graphX2LabelChanged(const QString&);
      void y2LabelChanged(const QString&);
      void graphY2LabelChanged(const QString&);
      void upperTitleChanged(const QString&);
      void graphUpperTitleChanged(const QString&);
      void downTitleChanged(const QString&);
      void graphDownTitleChanged(const QString&);
      void leftTitleChanged(const QString&);
      void graphLeftTitleChanged(const QString&);
      void rightTitleChanged(const QString&);
      void graphRightTitleChanged(const QString&);
    private:
      QWidget*
      createRangesPage(void);
      QWidget*
      createLabelsPage(void);
      QWidget*
      createTitlesPage(void);
      Graph&    graph;
      QLineEdit *xmin;
      QLineEdit *xmax;
      QLineEdit *ymin;
      QLineEdit *ymax;
      QLineEdit *x2min;
      QLineEdit *x2max;
      QLineEdit *y2min;
      QLineEdit *y2max;
      QLineEdit *xle;
      QLineEdit *x2le;
      QLineEdit *yle;
      QLineEdit *y2le;
      QLineEdit *utle;
      QLineEdit *dtle;
      QLineEdit *ltle;
      QLineEdit *rtle;
    }; // end of GraphConfigurationDialog
    
  } // end of namespace plot
  
} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_GRAPHCONFIGURATIONDIALOG_H */

