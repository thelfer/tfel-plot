/*! 
 * \file  GraphTextItem.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 mai 2012
 */

#ifndef LIB_TFEL_PLOT_GRAPHTEXTITEM_H_
#define LIB_TFEL_PLOT_GRAPHTEXTITEM_H_ 

#ifdef TFEL_QT4
#include<QtGui/QGraphicsTextItem>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QGraphicsTextItem>
#endif /* TFEL_QT5 */

namespace tfel
{

  namespace plot
  {

    class Graph;
    
    class GraphTextItem
      : public QGraphicsTextItem
    {
      
      Q_OBJECT

    public:

      GraphTextItem(Graph *const,
		    void (Graph:: *)(const QString&,
				     const bool));

      GraphTextItem(const QString &,
		    Graph *const,
		    void (Graph:: *)(const QString&,
				     const bool));
						     
    public slots:

      void textChanged(void);

      void graphFontChanged(const QFont&);

    private:

      Graph *const graph;

      /*!
       * update label method
       */
      void (Graph:: *um)(const QString&,
			 const bool);
      
    }; // end of class GraphTextItem

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHTEXTITEM_H */

