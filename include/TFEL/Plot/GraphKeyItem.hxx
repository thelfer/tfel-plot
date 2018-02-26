/*!
 * \file  GraphKeyItem.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GRAPHKEYITEM_H_
#define LIB_TFEL_PLOT_GRAPHKEYITEM_H_

#include <QtWidgets/QGraphicsTextItem>

namespace tfel {

  namespace plot {

    struct Curve;

    struct Graph;

    struct GraphKeyItem : public QGraphicsTextItem {

      GraphKeyItem(const QString &,
                   Curve *const,
                   Graph *const,
                   QGraphicsItem * = nullptr);

     public slots:

      void textChanged();

      void graphFontChanged(const QFont &);

     private:
      Curve *const curve;
      Q_OBJECT
    };  // end of struct GraphKeyItem

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHKEYITEM_H */
