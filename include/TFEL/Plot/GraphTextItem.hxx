/*!
 * \file  GraphTextItem.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 28 mai 2012
 */

#ifndef LIB_TFEL_PLOT_GRAPHTEXTITEM_H_
#define LIB_TFEL_PLOT_GRAPHTEXTITEM_H_

#include <QtWidgets/QGraphicsTextItem>

namespace tfel {

  namespace plot {

    struct Graph;

    struct GraphTextItem : QGraphicsTextItem {
      GraphTextItem(Graph *const, void (Graph::*)(const QString &, const bool));

      GraphTextItem(const QString &,
                    Graph *const,
                    void (Graph::*)(const QString &, const bool));

     public slots:

      void textChanged();

      void graphFontChanged(const QFont &);

     private:
      Graph *const graph;

      //! update label method
      void (Graph::*um)(const QString &, const bool);

      Q_OBJECT

    };  // end of struct GraphTextItem

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHTEXTITEM_H */
