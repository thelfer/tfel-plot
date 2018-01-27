/*! 
 * \file  ImportLicosResultsDialog.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juin 2012
 */

#ifndef LIB_TFEL_PLOT_LICOSRESULTSTREEVIEW_H_
#define LIB_TFEL_PLOT_LICOSRESULTSTREEVIEW_H_ 

#include<QtWidgets/QDialog>
#include<QtWidgets/QTreeView>
#include<QtWidgets/QTreeWidgetItem>
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
			     QWidget * = nullptr);

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

#endif /* LIB_TFEL_PLOT_LICOSRESULTSTREEVIEW_H */

