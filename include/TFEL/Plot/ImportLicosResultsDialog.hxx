/*! 
 * \file  ImportLicosResultsDialog.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_LICOSRESULTSTREEVIEW_H_
#define _LIB_TFEL_PLOT_LICOSRESULTSTREEVIEW_H_ 

#ifdef TFEL_QT4
#include<QtGui/QDialog>
#include<QtGui/QTreeView>
#include<QtGui/QTreeWidgetItem>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QDialog>
#include<QtWidgets/QTreeView>
#include<QtWidgets/QTreeWidgetItem>
#endif /* TFEL_QT5 */
#include<QtXml/QDomElement>

#include"TFEL/Plot/Config.hxx"

namespace tfel
{

  namespace plot
  {

    class Graph;

    class TFELPLOT_VISIBILITY_EXPORT LicosResultsTreeWidget
      : public QTreeWidget
    {

      Q_OBJECT
      
    public:

      LicosResultsTreeWidget(Graph&,
			     QWidget * parent = nullptr);

    protected slots:

      virtual void
      itemSelected(const QModelIndex &);

    protected:
	
      Graph& graph;

    };

    class TFELPLOT_VISIBILITY_EXPORT ImportLicosResultsDialog 
      : public QDialog
    {
      
      Q_OBJECT
      
    public:

      ImportLicosResultsDialog(Graph&,
			       QWidget * const = nullptr);

      ImportLicosResultsDialog(Graph&,
			       const QString&,
			       QWidget * const = nullptr);
   
      operator bool() const;

    protected:

      virtual void
      loadFile(const QString&);
      
      virtual bool
      getOutputFiles(QMap<QString,QStringList>&,
		     const QDomElement&);

      virtual QTreeWidgetItem*
      getTopLevelItem(const QString&);

      virtual void
      fillTree(QTreeWidgetItem *,
	       const QMap<QString,QStringList>&) const;

      virtual void
      fails(const QString&);

      LicosResultsTreeWidget *tree;

      bool ok;

    }; // end of ImportLicosResultsDialog

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_LICOSRESULTSTREEVIEW_H */

