/*! 
 * \file  GraphConfigurationDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juin 2012
 */

#include<QtCore/QtDebug>
#ifdef TFEL_QT4
#include<QtGui/QLabel>
#include<QtGui/QTabWidget>
#include<QtGui/QGridLayout>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QLabel>
#include<QtWidgets/QTabWidget>
#include<QtWidgets/QGridLayout>
#endif /* TFEL_QT5 */

#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/GraphConfigurationDialog.hxx"

namespace tfel
{

  namespace plot
  {
    
    GraphConfigurationDialog::GraphConfigurationDialog(Graph& g)
      : QDialog(&g),
	graph(g),
	xmin(new QLineEdit),
	xmax(new QLineEdit),
	ymin(new QLineEdit),
	ymax(new QLineEdit),
	x2min(new QLineEdit),
	x2max(new QLineEdit),
	y2min(new QLineEdit),
	y2max(new QLineEdit),
	xle(new QLineEdit),
	x2le(new QLineEdit),
	yle(new QLineEdit),
	y2le(new QLineEdit),
	utle(new QLineEdit),
	dtle(new QLineEdit),
	ltle(new QLineEdit),
	rtle(new QLineEdit)
    {
      QVBoxLayout *l    = new QVBoxLayout;
      QTabWidget* tabs  = new QTabWidget;
      tabs->addTab(this->createRangesPage(),tr("Graph ranges"));
      tabs->addTab(this->createLabelsPage(),tr("Graph labels"));
      tabs->addTab(this->createTitlesPage(),tr("Graph titles"));
      l->addWidget(tabs);
      this->setLayout(l);
    } // end of  GraphConfigurationDialog::GraphConfigurationDialog

    QWidget * GraphConfigurationDialog::createLabelsPage()
    {
      QWidget *labels   = new QWidget;
      QGridLayout *grid = new QGridLayout;
      grid->addWidget(new QLabel(tr("x-axis label")),0,0);
      grid->addWidget(this->xle,0,1);
      QObject::connect(&(this->graph),SIGNAL(xLabelChanged(const QString&)),
		       this,SLOT(graphXLabelChanged(const QString&)));
      QObject::connect(this->xle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(xLabelChanged(const QString&)));
      grid->addWidget(new QLabel(tr("y-axis label")),1,0);
      grid->addWidget(this->yle,1,1);
      QObject::connect(&(this->graph),SIGNAL(yLabelChanged(const QString&)),
		       this,SLOT(graphYLabelChanged(const QString&)));
      QObject::connect(this->yle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(yLabelChanged(const QString&)));
      grid->addWidget(new QLabel(tr("x2-axis label")),2,0);
      grid->addWidget(this->x2le,2,1);
      QObject::connect(&(this->graph),SIGNAL(x2LabelChanged(const QString&)),
		       this,SLOT(graphX2LabelChanged(const QString&)));
      QObject::connect(this->x2le,SIGNAL(textChanged(const QString&)),
		       this,SLOT(x2LabelChanged(const QString&)));
      grid->addWidget(new QLabel(tr("y2-axis label")),3,0);
      grid->addWidget(this->y2le,3,1);
      QObject::connect(&(this->graph),SIGNAL(y2LabelChanged(const QString&)),
		       this,SLOT(graphY2LabelChanged(const QString&)));
      QObject::connect(this->y2le,SIGNAL(textChanged(const QString&)),
		       this,SLOT(y2LabelChanged(const QString&)));
      labels->setLayout(grid);
      return labels;
    }

    QWidget* GraphConfigurationDialog::createRangesPage()
    {
      QWidget *ranges   = new QWidget;
      QGridLayout *grid = new QGridLayout;
      // x axis
      grid->addWidget(new QLabel(tr("x-axis minimum")),0,0);
      grid->addWidget(this->xmin,0,1);
      grid->addWidget(new QLabel(tr("x-axis maximum")),1,0);
      grid->addWidget(this->xmax,1,1);
      QObject::connect(&(this->graph),SIGNAL(xRangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphXRangeChanged(const qreal,
						    const qreal)));
      QObject::connect(this->xmin,SIGNAL(textChanged(const QString&)),
		       this,SLOT(xMinRangeChanged(const QString&)));
      QObject::connect(this->xmax,SIGNAL(textChanged(const QString&)),
		       this,SLOT(xMaxRangeChanged(const QString&)));
      // y axis
      grid->addWidget(new QLabel(tr("y-axis minimum")),2,0);
      grid->addWidget(this->ymin,2,1);
      grid->addWidget(new QLabel(tr("y-axis maximum")),3,0);
      grid->addWidget(this->ymax,3,1);
      QObject::connect(&(this->graph),SIGNAL(yRangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphYRangeChanged(const qreal,
						    const qreal)));
      QObject::connect(this->ymin,SIGNAL(textChanged(const QString&)),
      		       this,SLOT(yMinRangeChanged(const QString&)));
      QObject::connect(this->ymax,SIGNAL(textChanged(const QString&)),
      		       this,SLOT(yMaxRangeChanged(const QString&)));
      // x2 axis
      grid->addWidget(new QLabel(tr("x2-axis minimum")),4,0);
      grid->addWidget(this->x2min,4,1);
      grid->addWidget(new QLabel(tr("x2-axis maximum")),5,0);
      grid->addWidget(this->x2max,5,1);
      QObject::connect(&(this->graph),SIGNAL(x2RangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphX2RangeChanged(const qreal,
						    const qreal)));
      QObject::connect(this->x2min,SIGNAL(textChanged(const QString&)),
      		       this,SLOT(x2MinRangeChanged(const QString&)));
      QObject::connect(this->x2max,SIGNAL(textChanged(const QString&)),
      		       this,SLOT(x2MaxRangeChanged(const QString&)));
      // y2 axis
      grid->addWidget(new QLabel(tr("y2-axis minimum")),6,0);
      grid->addWidget(this->y2min,6,1);
      grid->addWidget(new QLabel(tr("y2-axis maximum")),7,0);
      grid->addWidget(this->y2max,7,1);
      QObject::connect(&(this->graph),SIGNAL(y2RangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphY2RangeChanged(const qreal,
						    const qreal)));
      QObject::connect(this->y2min,SIGNAL(textChanged(const QString&)),
      		       this,SLOT(y2MinRangeChanged(const QString&)));
      QObject::connect(this->y2max,SIGNAL(textChanged(const QString&)),
      		       this,SLOT(y2MaxRangeChanged(const QString&)));
      ranges->setLayout(grid);
      return ranges;
    }

    void
    GraphConfigurationDialog::xMinRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),SIGNAL(xRangeChanged(const qreal,
							      const qreal)),
			  this,SLOT(graphXRangeChanged(const qreal,
						       const qreal)));
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::xaxis);
      vmin = v.toDouble(&b);
      if(b){
	this->graph.setXRange(vmin,vmax);
      }
      QObject::connect(&(this->graph),SIGNAL(xRangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphXRangeChanged(const qreal,
						    const qreal)));
    } // end of GraphConfigurationDialog::xMinRangeChanged

    void
    GraphConfigurationDialog::xMaxRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),SIGNAL(xRangeChanged(const qreal,
							      const qreal)),
			  this,SLOT(graphXRangeChanged(const qreal,
						       const qreal)));
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::xaxis);
      vmax = v.toDouble(&b);
      if(b){
	this->graph.setXRange(vmin,vmax);
      }
      QObject::connect(&(this->graph),SIGNAL(xRangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphXRangeChanged(const qreal,
						    const qreal)));
    }

    void
    GraphConfigurationDialog::graphXRangeChanged(const qreal vmin, const qreal vmax)
    {
      QObject::disconnect(this->xmin,SIGNAL(textChanged(const QString&)),
			  this,SLOT(xMinRangeChanged(const QString&)));
      QObject::disconnect(this->xmax,SIGNAL(textChanged(const QString&)),
			  this,SLOT(xMaxRangeChanged(const QString&)));
      this->xmin->setText(QString::number(vmin));
      this->xmax->setText(QString::number(vmax));
      QObject::connect(this->xmin,SIGNAL(textChanged(const QString&)),
		       this,SLOT(xMinRangeChanged(const QString&)));
      QObject::connect(this->xmax,SIGNAL(textChanged(const QString&)),
		       this,SLOT(xMaxRangeChanged(const QString&)));
    }

    void
    GraphConfigurationDialog::yMinRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),SIGNAL(yRangeChanged(const qreal,
							      const qreal)),
			  this,SLOT(graphYRangeChanged(const qreal,
						       const qreal)));
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::yaxis);
      vmin = v.toDouble(&b);
      if(b){
	this->graph.setYRange(vmin,vmax);
      }
      QObject::connect(&(this->graph),SIGNAL(yRangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphYRangeChanged(const qreal,
						    const qreal)));
    } // end of GraphConfigurationDialog::yMinRangeChanged

    void
    GraphConfigurationDialog::yMaxRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),SIGNAL(yRangeChanged(const qreal,
							      const qreal)),
			  this,SLOT(graphYRangeChanged(const qreal,
						       const qreal)));
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::yaxis);
      vmax = v.toDouble(&b);
      if(b){
	this->graph.setYRange(vmin,vmax);
      }
      QObject::connect(&(this->graph),SIGNAL(yRangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphYRangeChanged(const qreal,
						    const qreal)));
    }

    void
    GraphConfigurationDialog::graphYRangeChanged(const qreal vmin, const qreal vmax)
    {
      QObject::disconnect(this->ymin,SIGNAL(textChanged(const QString&)),
			  this,SLOT(yMinRangeChanged(const QString&)));
      QObject::disconnect(this->ymax,SIGNAL(textChanged(const QString&)),
			  this,SLOT(yMaxRangeChanged(const QString&)));
      this->ymin->setText(QString::number(vmin));
      this->ymax->setText(QString::number(vmax));
      QObject::connect(this->ymin,SIGNAL(textChanged(const QString&)),
		       this,SLOT(yMinRangeChanged(const QString&)));
      QObject::connect(this->ymax,SIGNAL(textChanged(const QString&)),
		       this,SLOT(yMaxRangeChanged(const QString&)));
    }

    void
    GraphConfigurationDialog::x2MinRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),SIGNAL(x2RangeChanged(const qreal,
							      const qreal)),
			  this,SLOT(graphX2RangeChanged(const qreal,
						       const qreal)));
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::x2axis);
      vmin = v.toDouble(&b);
      if(b){
	this->graph.setX2Range(vmin,vmax);
      }
      QObject::connect(&(this->graph),SIGNAL(x2RangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphX2RangeChanged(const qreal,
						    const qreal)));
    } // end of GraphConfigurationDialog::x2MinRangeChanged

    void
    GraphConfigurationDialog::x2MaxRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),SIGNAL(x2RangeChanged(const qreal,
							      const qreal)),
			  this,SLOT(graphX2RangeChanged(const qreal,
						       const qreal)));
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::x2axis);
      vmax = v.toDouble(&b);
      if(b){
	this->graph.setX2Range(vmin,vmax);
      }
      QObject::connect(&(this->graph),SIGNAL(x2RangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphX2RangeChanged(const qreal,
						    const qreal)));
    }

    void
    GraphConfigurationDialog::graphX2RangeChanged(const qreal vmin, const qreal vmax)
    {
      QObject::disconnect(this->x2min,SIGNAL(textChanged(const QString&)),
			  this,SLOT(x2MinRangeChanged(const QString&)));
      QObject::disconnect(this->x2max,SIGNAL(textChanged(const QString&)),
			  this,SLOT(x2MaxRangeChanged(const QString&)));
      this->x2min->setText(QString::number(vmin));
      this->x2max->setText(QString::number(vmax));
      QObject::connect(this->x2min,SIGNAL(textChanged(const QString&)),
		       this,SLOT(x2MinRangeChanged(const QString&)));
      QObject::connect(this->x2max,SIGNAL(textChanged(const QString&)),
		       this,SLOT(x2MaxRangeChanged(const QString&)));
    }

    void
    GraphConfigurationDialog::y2MinRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),SIGNAL(y2RangeChanged(const qreal,
							      const qreal)),
			  this,SLOT(graphY2RangeChanged(const qreal,
						       const qreal)));
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::y2axis);
      vmin = v.toDouble(&b);
      if(b){
	this->graph.setY2Range(vmin,vmax);
      }
      QObject::connect(&(this->graph),SIGNAL(y2RangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphY2RangeChanged(const qreal,
						    const qreal)));
    } // end of GraphConfigurationDialog::y2MinRangeChanged

    void
    GraphConfigurationDialog::y2MaxRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),SIGNAL(y2RangeChanged(const qreal,
							      const qreal)),
			  this,SLOT(graphY2RangeChanged(const qreal,
						       const qreal)));
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::y2axis);
      vmax = v.toDouble(&b);
      if(b){
	this->graph.setY2Range(vmin,vmax);
      }
      QObject::connect(&(this->graph),SIGNAL(y2RangeChanged(const qreal,
							   const qreal)),
		       this,SLOT(graphY2RangeChanged(const qreal,
						    const qreal)));
    }

    void
    GraphConfigurationDialog::graphY2RangeChanged(const qreal vmin, const qreal vmax)
    {
      QObject::disconnect(this->y2min,SIGNAL(textChanged(const QString&)),
			  this,SLOT(y2MinRangeChanged(const QString&)));
      QObject::disconnect(this->y2max,SIGNAL(textChanged(const QString&)),
			  this,SLOT(y2MaxRangeChanged(const QString&)));
      this->y2min->setText(QString::number(vmin));
      this->y2max->setText(QString::number(vmax));
      QObject::connect(this->y2min,SIGNAL(textChanged(const QString&)),
		       this,SLOT(y2MinRangeChanged(const QString&)));
      QObject::connect(this->y2max,SIGNAL(textChanged(const QString&)),
		       this,SLOT(y2MaxRangeChanged(const QString&)));
    }

    QWidget* GraphConfigurationDialog::createTitlesPage()
    {
      QWidget *titles   = new QWidget;
      QGridLayout *grid = new QGridLayout;
      grid->addWidget(new QLabel(tr("Upper title")),0,0);
      grid->addWidget(this->utle,0,1);
      QObject::connect(&(this->graph),SIGNAL(upperTitleChanged(const QString&)),
		       this,SLOT(graphUpperTitleChanged(const QString&)));
      QObject::connect(this->utle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(upperTitleChanged(const QString&)));
      grid->addWidget(new QLabel(tr("Down title")),1,0);
      grid->addWidget(this->dtle,1,1);
      QObject::connect(&(this->graph),SIGNAL(downTitleChanged(const QString&)),
		       this,SLOT(graphDownTitleChanged(const QString&)));
      QObject::connect(this->dtle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(downTitleChanged(const QString&)));
      grid->addWidget(new QLabel(tr("Left title")),2,0);
      grid->addWidget(this->ltle,2,1);
      QObject::connect(&(this->graph),SIGNAL(leftTitleChanged(const QString&)),
		       this,SLOT(graphLeftTitleChanged(const QString&)));
      QObject::connect(this->ltle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(leftTitleChanged(const QString&)));
      grid->addWidget(new QLabel(tr("Right title")),3,0);
      grid->addWidget(this->rtle,3,1);
      QObject::connect(&(this->graph),SIGNAL(rightTitleChanged(const QString&)),
		       this,SLOT(graphRightTitleChanged(const QString&)));
      QObject::connect(this->rtle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(rightTitleChanged(const QString&)));
      titles->setLayout(grid);
      return titles;
    }

    void
    GraphConfigurationDialog::graphXLabelChanged(const QString& l)
    {
      QObject::disconnect(this->xle,SIGNAL(textChanged(const QString&)),
			  this,SLOT(xLabelChanged(const QString&)));
      this->xle->setText(l);
      QObject::connect(this->xle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(xLabelChanged(const QString&)));
    }

    void
    GraphConfigurationDialog::xLabelChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),SIGNAL(xLabelChanged(const QString&)),
			  this,SLOT(graphXLabelChanged(const QString&)));
      this->graph.setXLabel(l,true);
      QObject::connect(&(this->graph),SIGNAL(xLabelChanged(const QString&)),
		       this,SLOT(graphXLabelChanged(const QString&)));
    } // end of GraphConfigurationDialog::xLabelChanged

    void
    GraphConfigurationDialog::graphX2LabelChanged(const QString& l)
    {
      QObject::disconnect(this->x2le,SIGNAL(textChanged(const QString&)),
			  this,SLOT(x2LabelChanged(const QString&)));
      this->x2le->setText(l);
      QObject::connect(this->x2le,SIGNAL(textChanged(const QString&)),
		       this,SLOT(x2LabelChanged(const QString&)));
    }

    void
    GraphConfigurationDialog::x2LabelChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),SIGNAL(x2LabelChanged(const QString&)),
			  this,SLOT(graphX2LabelChanged(const QString&)));
      this->graph.setX2Label(l,true);
      QObject::connect(&(this->graph),SIGNAL(x2LabelChanged(const QString&)),
		       this,SLOT(graphX2LabelChanged(const QString&)));
    } // end of GraphConfigurationDialog::x2LabelChanged

    void
    GraphConfigurationDialog::graphYLabelChanged(const QString& l)
    {
      QObject::disconnect(this->yle,SIGNAL(textChanged(const QString&)),
			  this,SLOT(yLabelChanged(const QString&)));
      this->yle->setText(l);
      QObject::connect(this->yle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(yLabelChanged(const QString&)));
    }

    void
    GraphConfigurationDialog::yLabelChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),SIGNAL(yLabelChanged(const QString&)),
			  this,SLOT(graphYLabelChanged(const QString&)));
      this->graph.setYLabel(l,true);
      QObject::connect(&(this->graph),SIGNAL(yLabelChanged(const QString&)),
		       this,SLOT(graphYLabelChanged(const QString&)));
    } // end of GraphConfigurationDialog::yLabelChanged

    void
    GraphConfigurationDialog::graphY2LabelChanged(const QString& l)
    {
      QObject::disconnect(this->y2le,SIGNAL(textChanged(const QString&)),
			  this,SLOT(y2LabelChanged(const QString&)));
      this->y2le->setText(l);
      QObject::connect(this->y2le,SIGNAL(textChanged(const QString&)),
		       this,SLOT(y2LabelChanged(const QString&)));
    }

    void
    GraphConfigurationDialog::y2LabelChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),SIGNAL(y2LabelChanged(const QString&)),
			  this,SLOT(graphY2LabelChanged(const QString&)));
      this->graph.setY2Label(l,true);
      QObject::connect(&(this->graph),SIGNAL(y2LabelChanged(const QString&)),
		       this,SLOT(graphY2LabelChanged(const QString&)));
    } // end of GraphConfigurationDialog::y2LabelChanged

    void
    GraphConfigurationDialog::upperTitleChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),SIGNAL(upperTitleChanged(const QString&)),
			  this,SLOT(graphUpperTitleChanged(const QString&)));
      this->graph.setUpperTitle(l,true);
      QObject::connect(&(this->graph),SIGNAL(upperTitleChanged(const QString&)),
		       this,SLOT(graphUpperTitleChanged(const QString&)));
    } // end of GraphConfigurationDialog::upperTitleChanged

    void
    GraphConfigurationDialog::graphUpperTitleChanged(const QString& l)
    {
      QObject::disconnect(this->utle,SIGNAL(textChanged(const QString&)),
			  this,SLOT(upperTitleChanged(const QString&)));
      this->utle->setText(l);
      QObject::connect(this->utle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(upperTitleChanged(const QString&)));
    }      

    void
    GraphConfigurationDialog::downTitleChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),SIGNAL(downTitleChanged(const QString&)),
			  this,SLOT(graphDownTitleChanged(const QString&)));
      this->graph.setDownTitle(l,true);
      QObject::connect(&(this->graph),SIGNAL(downTitleChanged(const QString&)),
		       this,SLOT(graphDownTitleChanged(const QString&)));
    } // end of GraphConfigurationDialog::downTitleChanged

    void
    GraphConfigurationDialog::graphDownTitleChanged(const QString& l)
    {
      QObject::disconnect(this->dtle,SIGNAL(textChanged(const QString&)),
			  this,SLOT(downTitleChanged(const QString&)));
      this->dtle->setText(l);
      QObject::connect(this->dtle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(downTitleChanged(const QString&)));
    }      

    void
    GraphConfigurationDialog::leftTitleChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),SIGNAL(leftTitleChanged(const QString&)),
			  this,SLOT(graphLeftTitleChanged(const QString&)));
      this->graph.setLeftTitle(l,true);
      QObject::connect(&(this->graph),SIGNAL(leftTitleChanged(const QString&)),
		       this,SLOT(graphLeftTitleChanged(const QString&)));
    } // end of GraphConfigurationDialog::leftTitleChanged

    void
    GraphConfigurationDialog::graphLeftTitleChanged(const QString& l)
    {
      QObject::disconnect(this->ltle,SIGNAL(textChanged(const QString&)),
			  this,SLOT(leftTitleChanged(const QString&)));
      this->ltle->setText(l);
      QObject::connect(this->ltle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(leftTitleChanged(const QString&)));
    }      

    void
    GraphConfigurationDialog::rightTitleChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),SIGNAL(rightTitleChanged(const QString&)),
			  this,SLOT(graphRightTitleChanged(const QString&)));
      this->graph.setRightTitle(l,true);
      QObject::connect(&(this->graph),SIGNAL(rightTitleChanged(const QString&)),
		       this,SLOT(graphRightTitleChanged(const QString&)));
    } // end of GraphConfigurationDialog::rightTitleChanged

    void
    GraphConfigurationDialog::graphRightTitleChanged(const QString& l)
    {
      QObject::disconnect(this->rtle,SIGNAL(textChanged(const QString&)),
			  this,SLOT(rightTitleChanged(const QString&)));
      this->rtle->setText(l);
      QObject::connect(this->rtle,SIGNAL(textChanged(const QString&)),
		       this,SLOT(rightTitleChanged(const QString&)));
    }      


  } // end of namespace plot
  
} // end of namespace tfel
