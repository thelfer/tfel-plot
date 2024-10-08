/*!
 * \file  GraphTextItem.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 29 mai 2012
 */

#include <QtCore/QtDebug>
#include <QtWidgets/QMenu>
#include <QtWidgets/QGraphicsSceneContextMenuEvent>
#include <QtGui/QAction>
#include <QtGui/QFocusEvent>
#include <QtGui/QTextDocument>
#include "TFEL/Plot/Graph.hxx"
#include "TFEL/Plot/GraphTextItem.hxx"

namespace tfel {

  namespace plot {

    GraphTextItem::GraphTextItem(Graph *const g,
                                 void (Graph::*m)(const QString &, const bool))
        : graph(g), um(m) {
      this->setFlag(QGraphicsItem::ItemIsSelectable);
      this->setTextInteractionFlags(Qt::TextEditorInteraction);
      this->setFont(this->graph->getGraphFont());
      QObject::connect(this->document(), &QTextDocument::contentsChanged, this,
                       &GraphTextItem::textChanged);
      QObject::connect(g, &Graph::fontChanged, this,
                       &GraphTextItem::graphFontChanged);
    }

    GraphTextItem::GraphTextItem(const QString &text,
                                 Graph *const g,
                                 void (Graph::*m)(const QString &, const bool))
        : QGraphicsTextItem(), graph(g), um(m) {
      this->setHtml(text);
      this->setFlag(QGraphicsItem::ItemIsSelectable);
      this->setTextInteractionFlags(Qt::TextEditorInteraction);
      this->setFont(this->graph->getGraphFont());
      QObject::connect(this->document(), &QTextDocument::contentsChanged, this,
                       &GraphTextItem::textChanged);
      QObject::connect(g, &Graph::fontChanged, this,
                       &GraphTextItem::graphFontChanged);
    }

    void GraphTextItem::textChanged() {
      (this->graph->*um)(this->toHtml(), false);
    }  // end of GraphTextItem::textChanged

    void GraphTextItem::graphFontChanged(const QFont &f) { this->setFont(f); }

  }  // end of namespace plot

}  // end of namespace tfel
