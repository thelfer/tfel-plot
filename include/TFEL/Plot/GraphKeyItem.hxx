/*! 
 * \file  GraphKeyItem.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GRAPHKEYITEM_H_
#define LIB_TFEL_PLOT_GRAPHKEYITEM_H_ 

#include<QtWidgets/QGraphicsTextItem>

namespace tfel
{

  namespace plot
  {

    class Curve;

    class Graph;
    
    class GraphKeyItem
      : public QGraphicsTextItem
    {
      
      Q_OBJECT

    public:

      GraphKeyItem(const QString&,
		   Curve *const,
		   Graph *const,
		   QGraphicsItem * = nullptr);
						     
    public slots:

      void textChanged();

      void graphFontChanged(const QFont&);

    private:

      Curve *const curve;
      
    }; // end of class GraphKeyItem

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHKEYITEM_H */

