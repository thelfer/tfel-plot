/*! 
 * \file  ImportLicosResultsDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juin 2012
 */

#include<QtXml/QDomDocument>
#ifdef TFEL_QT4
#include<QtGui/QFileDialog>
#include<QtGui/QMessageBox>
#include<QtGui/QVBoxLayout>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QFileDialog>
#include<QtWidgets/QMessageBox>
#include<QtWidgets/QVBoxLayout>
#endif /* TFEL_QT5 */

#include"TFEL/Plot/ImportLicosCurveDialog.hxx"
#include"TFEL/Plot/ImportLicosResultsDialog.hxx"

namespace tfel
{

  namespace plot
  {

    struct LicosCurveTreeWidgetItem
      : public QTreeWidgetItem
    {
      
      LicosCurveTreeWidgetItem(const QString&,
			       LicosResultsTreeWidget *);

      LicosCurveTreeWidgetItem(const QString&,
			       QTreeWidgetItem *);

      virtual QString
      getFile() const;
      
      virtual ~LicosCurveTreeWidgetItem();

    protected:

      const QString file;

    }; 

    LicosCurveTreeWidgetItem::LicosCurveTreeWidgetItem(const QString& f,
						       LicosResultsTreeWidget *p)
      : QTreeWidgetItem(p),
	file(f)
    {}

    LicosCurveTreeWidgetItem::LicosCurveTreeWidgetItem(const QString& f,
						       QTreeWidgetItem *p)
      : QTreeWidgetItem(p),
	file(f)
    {}
    
    QString
    LicosCurveTreeWidgetItem::getFile() const
    {
      return this->file;
    }

    LicosCurveTreeWidgetItem::~LicosCurveTreeWidgetItem() = default;

    LicosResultsTreeWidget::LicosResultsTreeWidget(Graph& g,
						   QWidget *p)
      : QTreeWidget(p),
	graph(g)
    {
      QObject::connect(this,SIGNAL(clicked(const QModelIndex &)),
		       this,SLOT(itemSelected(const QModelIndex &)));
    } // end of LicosResultsTreeWidget::LicosResultsTreeWidget

    void
    LicosResultsTreeWidget::itemSelected(const QModelIndex& i)
    {
      QTreeWidgetItem * tw = this->itemFromIndex(i);
      LicosCurveTreeWidgetItem *c = dynamic_cast<LicosCurveTreeWidgetItem *>(tw);
      if(c!=nullptr){
	auto d = new ImportLicosCurveDialog(this->graph,this);
	d->exec(c->getFile());
	if(*d){
	  d->setWindowModality(Qt::NonModal);
	  d->show();
	}
      }
    } // end of LicosResultsTreeWidget::itemSelected
    // QList<QTreeWidgetItem *> items = this->tree->selectedItems();
    // QList<QTreeWidgetItem *>::const_iterator p;
    // for(p!=items.begin();p!=items.end();++p){
    // 	qDebug() << "ImportLicosResultsDialog::itemSelected : " << *p;
    //	LicosCurveTreeWidgetItem *c = static_cast<LicosCurveTreeWidgetItem *>(*p);
    //	qDebug() << "ImportLicosResultsDialog::itemSelected : " << *p;
    // if(c!=0){
    //   qDebug() << "ImportLicosResultsDialog::itemSelected : " << c->getFile();
    // }
    //}
    // QString file = fw->data(0,Qt::DisplayRole).toString();
      // if(!file.isEmpty()){
      // 	qDebug() << file;
      // }

    ImportLicosResultsDialog::ImportLicosResultsDialog(Graph& g,
						       QWidget * const p)
      : QDialog(p),
	tree(new LicosResultsTreeWidget(g)),
	ok(true)
    {
      const QString& d = tr("Licos Results");
      const QString& e = "res";
      const QString& f = QFileDialog::getOpenFileName(this,
						      tr("Open %1 File").arg(d),
						      "",
						      d+" (*."+e+")");
      if(f.isEmpty()){
	this->ok = false;
	return;
      }
      this->setAttribute(Qt::WA_DeleteOnClose);
      auto *l = new QVBoxLayout;
      l->addWidget(this->tree);
      this->loadFile(f);
      this->setLayout(l);
      this->tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    } // end of LicosResultsTreeWidget
    
    ImportLicosResultsDialog::ImportLicosResultsDialog(Graph& g,
						       const QString& f,
						       QWidget * const p)
      : QDialog(p),
	tree(new LicosResultsTreeWidget(g)),
	ok(true)
    {
      this->setAttribute(Qt::WA_DeleteOnClose);
      QVBoxLayout *l = new QVBoxLayout;
      l->addWidget(this->tree);
      this->loadFile(f);
      this->setLayout(l);
      this->tree->setSelectionMode(QAbstractItemView::ExtendedSelection);
    } // end of ImportLicosResultsDialog

    struct DataFiles
    {
      QString     type;
      QStringList curves;
    };

    void
    ImportLicosResultsDialog::loadFile(const QString& f)
    {
      QString errorStr;
      int errorLine;
      int errorColumn;
      QDomDocument doc;
      QFile file(f);
      QMap<QString,QStringList>                global_outputs;
      QMap<QString,QMap<QString,QStringList> > local_outputs;
      if (!file.open(QIODevice::ReadOnly)){
	this->fails(tr("Can't open file '%1'").arg(f));
	return;
      }
      if (!doc.setContent(&file, true, &errorStr, &errorLine,
			  &errorColumn)) {
	this->fails(tr("Parse error at line %1, column %2:\n%3")
		    .arg(errorLine)
		    .arg(errorColumn)
		    .arg(errorStr));
        return;
      }
      file.close();
      QDomElement docElem = doc.documentElement();
      if(docElem.tagName()!="licos_results"){
	this->fails(tr("Invalid root element tag name '%1'")
		    .arg(docElem.tagName()));
	return;
      }
      QDomNode n = docElem.firstChild();
      while(!n.isNull()) {
	if(!n.isComment()){
	  if(!n.isElement()){
	    this->fails(tr("Invalid XML node type"));
	    return;
	  }
	  QDomElement elt = n.toElement();
	  if(elt.tagName()=="submesh"){
	    if(!elt.hasAttributes()){
	      this->fails(tr("'submesh' node has no "
			     "'name' attribute"));
	      return;
	    }
	    QDomNamedNodeMap attr = elt.attributes();
	    if((attr.count()!=1)||
	       (!attr.contains ("name"))){
	      this->fails(tr("'submesh' tag has  more than "
			     "one attributes or an attributes "
			     "different than 'name'"));
	      return;
	    }
	    QString s = elt.attribute("name");
	    if(!this->getOutputFiles(local_outputs[s],elt)){
	      return;
	    }
	  } else if(elt.tagName()=="global"){
	    if(!this->getOutputFiles(global_outputs,elt)){
	      return;
	    }
	  } else {
	    this->fails(tr("invalid node '%1'").arg(elt.tagName()));
	    return;
	  }
	}
	n = n.nextSiblingElement();
      }
      // building the view
      QList<QTreeWidgetItem *> items;
      if(!global_outputs.isEmpty()){
	QTreeWidgetItem *go = this->getTopLevelItem("global outputs");
	this->fillTree(go,global_outputs);
	items.append(go);
      }
      QMap<QString,QMap<QString,QStringList> >::const_iterator ps;
      for(ps=local_outputs.begin();ps!=local_outputs.end();++ps){
	QTreeWidgetItem *o = this->getTopLevelItem("SubMesh '"+ps.key()+"'");
	this->fillTree(o,ps.value());
	items.append(o);	
      }
      this->tree->setHeaderLabel(tr("Contents of file '%1'").arg(f));
      this->tree->insertTopLevelItems(0, items);
    } // end of ImportLicosResultsDialog::loadFile

    QTreeWidgetItem*
    ImportLicosResultsDialog::getTopLevelItem(const QString& n)
    {
      QTreeWidgetItem *ii = new QTreeWidgetItem(this->tree,QStringList() << n);
      // QLinearGradient g(QPointF(0,0), QPointF(0,50));
      // QColor c1(Qt::blue);
      // c1.setAlpha(125);
      // g.setColorAt(0,c1);
      // g.setColorAt(1, Qt::white);
      // ii->setBackground(0,QBrush(g));
      return ii;
    }
    
    void
    ImportLicosResultsDialog::fillTree(QTreeWidgetItem* t,
				       const QMap<QString,QStringList>& items) const
    {
      QMap<QString,QStringList>::const_iterator p;
      for(p=items.begin();p!=items.end();++p){
	const QString& type      = p.key();
	const QStringList& files = p.value();
	QTreeWidgetItem *tw = new QTreeWidgetItem(t,QStringList() << type);
	QStringList::const_iterator p2;
	int i = 0;
	for(p2=files.begin();p2!=files.end();++p2,++i){
	  QTreeWidgetItem *fw;
	  if(type=="curve"){
	    fw = new LicosCurveTreeWidgetItem(*p2,tw);
	  } else {
	    fw = new QTreeWidgetItem(tw);
	  }
	  fw->setData(0,Qt::DisplayRole,*p2);
	  tw->addChild(fw);
	}
	t->addChild(tw);
      }
    } // end of ImportLicosResultsDialog::fillTree
    
    ImportLicosResultsDialog::operator bool () const
    {
      return this->ok;
    }

    bool
    ImportLicosResultsDialog::getOutputFiles(QMap<QString,QStringList>& outputs,
					 const QDomElement& elt)
    {
      QDomNode n2 = elt.firstChild();
      while(!n2.isNull()){
	if(!n2.isElement()){
	  this->fails(tr("Expected only element"));
	  return false;
	}
	QDomElement file = n2.toElement();
	QDomNamedNodeMap fattr = file.attributes();
	if((fattr.count()!=2)||
	   (!fattr.contains ("name"))||
	   (!fattr.contains ("type"))){
	  this->fails(tr("'file' tag must have two "
			 "attributes or its attributes "
			 "are different than 'name' or 'type'"));
	  return false;
	}
	outputs[file.attribute("type")] << file.attribute("name"); 
	n2 = n2.nextSibling();
      }
      return true;
    }

    void
    ImportLicosResultsDialog::fails(const QString& msg)
    {
      QMessageBox::information(this,tr("Licos Results"),msg);
      this->ok = false;
    } // end of ImportLicosResultsDialog::fails()

  } // end of namespace plot
  
} // end of namespace tfel

