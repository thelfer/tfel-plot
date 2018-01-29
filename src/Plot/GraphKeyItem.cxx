/*! 
 * \file  GraphKeyItem.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 07 juin 2012
 */

#include<QtGui/QTextDocument>

#include"TFEL/Plot/Curve.hxx"
#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/GraphKeyItem.hxx"

namespace tfel
{

  namespace plot
  {
    
    GraphKeyItem::GraphKeyItem(const QString& k,
			       Curve *const c,
			       Graph *const g,
			       QGraphicsItem * p)
      : QGraphicsTextItem(p),
	curve(c)
    {
      this->setFont(g->getGraphFont());
      this->setHtml(k);
      this->setFlag(QGraphicsItem::ItemIsSelectable);
      this->setTextInteractionFlags(Qt::TextEditorInteraction);
      QObject::connect(this->document(),&QTextDocument::contentsChanged,
		       this,&GraphKeyItem::textChanged);
      QObject::connect(g,&Graph::fontChanged,
		       this,&GraphKeyItem::graphFontChanged);
    } // end of GraphKeyItem::GraphKeyItem
						     
    void GraphKeyItem::textChanged()
    {
      curve->setKey(this->toPlainText(),false);
    }

    void GraphKeyItem::graphFontChanged(const QFont& f)
    {
      this->setFont(f);
    }

  } // end of namespace plot

} // end of namespace tfel

