/*! 
 * \file  GraphConfigurationDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 11 juin 2012
 */

#include<QtCore/QtDebug>
#include<QtWidgets/QLabel>
#include<QtWidgets/QTabWidget>
#include<QtWidgets/QGridLayout>

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
      auto *l    = new QVBoxLayout;
      auto* tabs = new QTabWidget;
      tabs->addTab(this->createRangesPage(),tr("Graph ranges"));
      tabs->addTab(this->createLabelsPage(),tr("Graph labels"));
      tabs->addTab(this->createTitlesPage(),tr("Graph titles"));
      l->addWidget(tabs);
      this->setLayout(l);
    } // end of  GraphConfigurationDialog::GraphConfigurationDialog

    QWidget * GraphConfigurationDialog::createLabelsPage()
    {
      auto *labels = new QWidget;
      auto *grid   = new QGridLayout;
      grid->addWidget(new QLabel(tr("x-axis label")),0,0);
      grid->addWidget(this->xle,0,1);
      QObject::connect(&(this->graph),&Graph::xLabelChanged,
		       this,&GraphConfigurationDialog::graphXLabelChanged);
      QObject::connect(this->xle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::xLabelChanged);
      grid->addWidget(new QLabel(tr("y-axis label")),1,0);
      grid->addWidget(this->yle,1,1);
      QObject::connect(&(this->graph),&Graph::yLabelChanged,
		       this,&GraphConfigurationDialog::graphYLabelChanged);
      QObject::connect(this->yle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::yLabelChanged);
      grid->addWidget(new QLabel(tr("x2-axis label")),2,0);
      grid->addWidget(this->x2le,2,1);
      QObject::connect(&(this->graph),&Graph::x2LabelChanged,
		       this,&GraphConfigurationDialog::graphX2LabelChanged);
      QObject::connect(this->x2le,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::x2LabelChanged);
      grid->addWidget(new QLabel(tr("y2-axis label")),3,0);
      grid->addWidget(this->y2le,3,1);
      QObject::connect(&(this->graph),&Graph::y2LabelChanged,
		       this,&GraphConfigurationDialog::graphY2LabelChanged);
      QObject::connect(this->y2le,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::y2LabelChanged);
      labels->setLayout(grid);
      return labels;
    }

    QWidget* GraphConfigurationDialog::createRangesPage()
    {
      auto *ranges = new QWidget;
      auto *grid   = new QGridLayout;
      // x axis
      grid->addWidget(new QLabel(tr("x-axis minimum")),0,0);
      grid->addWidget(this->xmin,0,1);
      grid->addWidget(new QLabel(tr("x-axis maximum")),1,0);
      grid->addWidget(this->xmax,1,1);
      QObject::connect(&(this->graph),&Graph::xRangeChanged,
		       this,&GraphConfigurationDialog::graphXRangeChanged);
      QObject::connect(this->xmin,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::xMinRangeChanged);
      QObject::connect(this->xmax,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::xMaxRangeChanged);
      // y axis
      grid->addWidget(new QLabel(tr("y-axis minimum")),2,0);
      grid->addWidget(this->ymin,2,1);
      grid->addWidget(new QLabel(tr("y-axis maximum")),3,0);
      grid->addWidget(this->ymax,3,1);
      QObject::connect(&(this->graph),&Graph::yRangeChanged,
		       this,&GraphConfigurationDialog::graphYRangeChanged);
      QObject::connect(this->ymin,&QLineEdit::textChanged,
      		       this,&GraphConfigurationDialog::yMinRangeChanged);
      QObject::connect(this->ymax,&QLineEdit::textChanged,
      		       this,&GraphConfigurationDialog::yMaxRangeChanged);
      // x2 axis
      grid->addWidget(new QLabel(tr("x2-axis minimum")),4,0);
      grid->addWidget(this->x2min,4,1);
      grid->addWidget(new QLabel(tr("x2-axis maximum")),5,0);
      grid->addWidget(this->x2max,5,1);
      QObject::connect(&(this->graph),&Graph::x2RangeChanged,
		       this,&GraphConfigurationDialog::graphX2RangeChanged);
      QObject::connect(this->x2min,&QLineEdit::textChanged,
      		       this,&GraphConfigurationDialog::x2MinRangeChanged);
      QObject::connect(this->x2max,&QLineEdit::textChanged,
      		       this,&GraphConfigurationDialog::x2MaxRangeChanged);
      // y2 axis
      grid->addWidget(new QLabel(tr("y2-axis minimum")),6,0);
      grid->addWidget(this->y2min,6,1);
      grid->addWidget(new QLabel(tr("y2-axis maximum")),7,0);
      grid->addWidget(this->y2max,7,1);
      QObject::connect(&(this->graph),&Graph::y2RangeChanged,
		       this,&GraphConfigurationDialog::graphY2RangeChanged);
      QObject::connect(this->y2min,&QLineEdit::textChanged,
      		       this,&GraphConfigurationDialog::y2MinRangeChanged);
      QObject::connect(this->y2max,&QLineEdit::textChanged,
      		       this,&GraphConfigurationDialog::y2MaxRangeChanged);
      ranges->setLayout(grid);
      return ranges;
    }

    void GraphConfigurationDialog::xMinRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),&Graph::xRangeChanged,
			  this,&GraphConfigurationDialog::graphXRangeChanged);
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::xaxis);
      vmin = v.toDouble(&b);
      if(b){
	this->graph.setXRange(vmin,vmax);
      }
      QObject::connect(&(this->graph),&Graph::xRangeChanged,
		       this,&GraphConfigurationDialog::graphXRangeChanged);
    } // end of GraphConfigurationDialog::xMinRangeChanged

    void GraphConfigurationDialog::xMaxRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),&Graph::xRangeChanged,
			  this,&GraphConfigurationDialog::graphXRangeChanged);
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::xaxis);
      vmax = v.toDouble(&b);
      if(b){
	this->graph.setXRange(vmin,vmax);
      }
      QObject::connect(&(this->graph),&Graph::xRangeChanged,
		       this,&GraphConfigurationDialog::graphXRangeChanged);
    }

    void
    GraphConfigurationDialog::graphXRangeChanged(const qreal vmin, const qreal vmax)
    {
      QObject::disconnect(this->xmin,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::xMinRangeChanged);
      QObject::disconnect(this->xmax,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::xMaxRangeChanged);
      this->xmin->setText(QString::number(vmin));
      this->xmax->setText(QString::number(vmax));
      QObject::connect(this->xmin,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::xMinRangeChanged);
      QObject::connect(this->xmax,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::xMaxRangeChanged);
    }

    void GraphConfigurationDialog::yMinRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),&Graph::yRangeChanged,
			  this,&GraphConfigurationDialog::graphYRangeChanged);
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::yaxis);
      vmin = v.toDouble(&b);
      if(b){
	this->graph.setYRange(vmin,vmax);
      }
      QObject::connect(&(this->graph),&Graph::yRangeChanged,
		       this,&GraphConfigurationDialog::graphYRangeChanged);
    } // end of GraphConfigurationDialog::yMinRangeChanged

    void GraphConfigurationDialog::yMaxRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),&Graph::yRangeChanged,
			  this,&GraphConfigurationDialog::graphYRangeChanged);
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::yaxis);
      vmax = v.toDouble(&b);
      if(b){
	this->graph.setYRange(vmin,vmax);
      }
      QObject::connect(&(this->graph),&Graph::yRangeChanged,
		       this,&GraphConfigurationDialog::graphYRangeChanged);
    }

    void GraphConfigurationDialog::graphYRangeChanged(const qreal vmin, const qreal vmax)
    {
      QObject::disconnect(this->ymin,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::yMinRangeChanged);
      QObject::disconnect(this->ymax,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::yMaxRangeChanged);
      this->ymin->setText(QString::number(vmin));
      this->ymax->setText(QString::number(vmax));
      QObject::connect(this->ymin,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::yMinRangeChanged);
      QObject::connect(this->ymax,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::yMaxRangeChanged);
    }

    void GraphConfigurationDialog::x2MinRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),&Graph::x2RangeChanged,
			  this,&GraphConfigurationDialog::graphX2RangeChanged);
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::x2axis);
      vmin = v.toDouble(&b);
      if(b){
	this->graph.setX2Range(vmin,vmax);
      }
      QObject::connect(&(this->graph),&Graph::x2RangeChanged,
		       this,&GraphConfigurationDialog::graphX2RangeChanged);
    } // end of GraphConfigurationDialog::x2MinRangeChanged

    void GraphConfigurationDialog::x2MaxRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),&Graph::x2RangeChanged,
			  this,&GraphConfigurationDialog::graphX2RangeChanged);
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::x2axis);
      vmax = v.toDouble(&b);
      if(b){
	this->graph.setX2Range(vmin,vmax);
      }
      QObject::connect(&(this->graph),&Graph::x2RangeChanged,
		       this,&GraphConfigurationDialog::graphX2RangeChanged);
    }

    void GraphConfigurationDialog::graphX2RangeChanged(const qreal vmin, const qreal vmax)
    {
      QObject::disconnect(this->x2min,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::x2MinRangeChanged);
      QObject::disconnect(this->x2max,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::x2MaxRangeChanged);
      this->x2min->setText(QString::number(vmin));
      this->x2max->setText(QString::number(vmax));
      QObject::connect(this->x2min,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::x2MinRangeChanged);
      QObject::connect(this->x2max,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::x2MaxRangeChanged);
    }

    void GraphConfigurationDialog::y2MinRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),&Graph::y2RangeChanged,
			  this,&GraphConfigurationDialog::graphY2RangeChanged);
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::y2axis);
      vmin = v.toDouble(&b);
      if(b){
	this->graph.setY2Range(vmin,vmax);
      }
      QObject::connect(&(this->graph),&Graph::y2RangeChanged,
		       this,&GraphConfigurationDialog::graphY2RangeChanged);
    } // end of GraphConfigurationDialog::y2MinRangeChanged

    void GraphConfigurationDialog::y2MaxRangeChanged(const QString& v)
    {
      QObject::disconnect(&(this->graph),&Graph::y2RangeChanged,
			  this,&GraphConfigurationDialog::graphY2RangeChanged);
      bool b;
      qreal vmin;
      qreal vmax;
      this->graph.getRange(vmin,vmax,Graph::y2axis);
      vmax = v.toDouble(&b);
      if(b){
	this->graph.setY2Range(vmin,vmax);
      }
      QObject::connect(&(this->graph),&Graph::y2RangeChanged,
		       this,&GraphConfigurationDialog::graphY2RangeChanged);
    }

    void GraphConfigurationDialog::graphY2RangeChanged(const qreal vmin, const qreal vmax)
    {
      QObject::disconnect(this->y2min,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::y2MinRangeChanged);
      QObject::disconnect(this->y2max,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::y2MaxRangeChanged);
      this->y2min->setText(QString::number(vmin));
      this->y2max->setText(QString::number(vmax));
      QObject::connect(this->y2min,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::y2MinRangeChanged);
      QObject::connect(this->y2max,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::y2MaxRangeChanged);
    }

    QWidget* GraphConfigurationDialog::createTitlesPage()
    {
      auto *titles = new QWidget;
      auto *grid   = new QGridLayout;
      grid->addWidget(new QLabel(tr("Upper title")),0,0);
      grid->addWidget(this->utle,0,1);
      QObject::connect(&(this->graph),&Graph::upperTitleChanged,
		       this,&GraphConfigurationDialog::graphUpperTitleChanged);
      QObject::connect(this->utle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::upperTitleChanged);
      grid->addWidget(new QLabel(tr("Down title")),1,0);
      grid->addWidget(this->dtle,1,1);
      QObject::connect(&(this->graph),&Graph::downTitleChanged,
		       this,&GraphConfigurationDialog::graphDownTitleChanged);
      QObject::connect(this->dtle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::downTitleChanged);
      grid->addWidget(new QLabel(tr("Left title")),2,0);
      grid->addWidget(this->ltle,2,1);
      QObject::connect(&(this->graph),&Graph::leftTitleChanged,
		       this,&GraphConfigurationDialog::graphLeftTitleChanged);
      QObject::connect(this->ltle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::leftTitleChanged);
      grid->addWidget(new QLabel(tr("Right title")),3,0);
      grid->addWidget(this->rtle,3,1);
      QObject::connect(&(this->graph),&Graph::rightTitleChanged,
		       this,&GraphConfigurationDialog::graphRightTitleChanged);
      QObject::connect(this->rtle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::rightTitleChanged);
      titles->setLayout(grid);
      return titles;
    }

    void GraphConfigurationDialog::graphXLabelChanged(const QString& l)
    {
      QObject::disconnect(this->xle,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::xLabelChanged);
      this->xle->setText(l);
      QObject::connect(this->xle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::xLabelChanged);
    }

    void GraphConfigurationDialog::xLabelChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),&Graph::xLabelChanged,
			  this,&GraphConfigurationDialog::graphXLabelChanged);
      this->graph.setXLabel(l,true);
      QObject::connect(&(this->graph),&Graph::xLabelChanged,
		       this,&GraphConfigurationDialog::graphXLabelChanged);
    } // end of GraphConfigurationDialog::xLabelChanged

    void GraphConfigurationDialog::graphX2LabelChanged(const QString& l)
    {
      QObject::disconnect(this->x2le,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::x2LabelChanged);
      this->x2le->setText(l);
      QObject::connect(this->x2le,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::x2LabelChanged);
    }

    void GraphConfigurationDialog::x2LabelChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),&Graph::x2LabelChanged,
			  this,&GraphConfigurationDialog::graphX2LabelChanged);
      this->graph.setX2Label(l,true);
      QObject::connect(&(this->graph),&Graph::x2LabelChanged,
		       this,&GraphConfigurationDialog::graphX2LabelChanged);
    } // end of GraphConfigurationDialog::x2LabelChanged

    void GraphConfigurationDialog::graphYLabelChanged(const QString& l)
    {
      QObject::disconnect(this->yle,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::yLabelChanged);
      this->yle->setText(l);
      QObject::connect(this->yle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::yLabelChanged);
    }

    void GraphConfigurationDialog::yLabelChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),&Graph::yLabelChanged,
			  this,&GraphConfigurationDialog::graphYLabelChanged);
      this->graph.setYLabel(l,true);
      QObject::connect(&(this->graph),&Graph::yLabelChanged,
		       this,&GraphConfigurationDialog::graphYLabelChanged);
    } // end of GraphConfigurationDialog::yLabelChanged

    void GraphConfigurationDialog::graphY2LabelChanged(const QString& l)
    {
      QObject::disconnect(this->y2le,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::y2LabelChanged);
      this->y2le->setText(l);
      QObject::connect(this->y2le,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::y2LabelChanged);
    }

    void GraphConfigurationDialog::y2LabelChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),&Graph::y2LabelChanged,
			  this,&GraphConfigurationDialog::graphY2LabelChanged);
      this->graph.setY2Label(l,true);
      QObject::connect(&(this->graph),&Graph::y2LabelChanged,
		       this,&GraphConfigurationDialog::graphY2LabelChanged);
    } // end of GraphConfigurationDialog::y2LabelChanged

    void GraphConfigurationDialog::upperTitleChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),&Graph::upperTitleChanged,
			  this,&GraphConfigurationDialog::graphUpperTitleChanged);
      this->graph.setUpperTitle(l,true);
      QObject::connect(&(this->graph),&Graph::upperTitleChanged,
		       this,&GraphConfigurationDialog::graphUpperTitleChanged);
    } // end of GraphConfigurationDialog::upperTitleChanged

    void GraphConfigurationDialog::graphUpperTitleChanged(const QString& l)
    {
      QObject::disconnect(this->utle,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::upperTitleChanged);
      this->utle->setText(l);
      QObject::connect(this->utle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::upperTitleChanged);
    }      

    void GraphConfigurationDialog::downTitleChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),&Graph::downTitleChanged,
			  this,&GraphConfigurationDialog::graphDownTitleChanged);
      this->graph.setDownTitle(l,true);
      QObject::connect(&(this->graph),&Graph::downTitleChanged,
		       this,&GraphConfigurationDialog::graphDownTitleChanged);
    } // end of GraphConfigurationDialog::downTitleChanged

    void GraphConfigurationDialog::graphDownTitleChanged(const QString& l)
    {
      QObject::disconnect(this->dtle,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::downTitleChanged);
      this->dtle->setText(l);
      QObject::connect(this->dtle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::downTitleChanged);
    }      

    void GraphConfigurationDialog::leftTitleChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),&Graph::leftTitleChanged,
			  this,&GraphConfigurationDialog::graphLeftTitleChanged);
      this->graph.setLeftTitle(l,true);
      QObject::connect(&(this->graph),&Graph::leftTitleChanged,
		       this,&GraphConfigurationDialog::graphLeftTitleChanged);
    } // end of GraphConfigurationDialog::leftTitleChanged

    void GraphConfigurationDialog::graphLeftTitleChanged(const QString& l)
    {
      QObject::disconnect(this->ltle,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::leftTitleChanged);
      this->ltle->setText(l);
      QObject::connect(this->ltle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::leftTitleChanged);
    }      

    void GraphConfigurationDialog::rightTitleChanged(const QString& l)
    {
      QObject::disconnect(&(this->graph),&Graph::rightTitleChanged,
			  this,&GraphConfigurationDialog::graphRightTitleChanged);
      this->graph.setRightTitle(l,true);
      QObject::connect(&(this->graph),&Graph::rightTitleChanged,
		       this,&GraphConfigurationDialog::graphRightTitleChanged);
    } // end of GraphConfigurationDialog::rightTitleChanged

    void GraphConfigurationDialog::graphRightTitleChanged(const QString& l)
    {
      QObject::disconnect(this->rtle,&QLineEdit::textChanged,
			  this,&GraphConfigurationDialog::rightTitleChanged);
      this->rtle->setText(l);
      QObject::connect(this->rtle,&QLineEdit::textChanged,
		       this,&GraphConfigurationDialog::rightTitleChanged);
    }      


  } // end of namespace plot
  
} // end of namespace tfel
