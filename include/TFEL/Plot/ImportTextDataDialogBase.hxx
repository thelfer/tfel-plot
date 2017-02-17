/*! 
 * \file  ImportTextDataDialogBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_IMPORTTEXTDATADIALOGBASE_H_
#define _LIB_TFEL_PLOT_IMPORTTEXTDATADIALOGBASE_H_ 

#include<memory>

#include<QtCore/QString>
#include<QtCore/QVector>
#ifdef TFEL_QT4
#include<QtGui/QDialog>
#include<QtGui/QLineEdit>
#include<QtGui/QComboBox>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QDialog>
#include<QtWidgets/QLineEdit>
#include<QtWidgets/QComboBox>
#endif /* TFEL_QT5 */

#include<TFEL/Plot/Config.hxx>
#include"TFEL/Plot/TextDataReader.hxx"
#include"TFEL/Plot/CurveConfigurationDialogBase.hxx"

namespace tfel
{

  namespace plot
  {

    struct Graph;

    class TFELPLOT_VISIBILITY_EXPORT ImportTextDataDialogBase 
      : public CurveConfigurationDialogBase
    {
      
      Q_OBJECT
      
    public:

      ImportTextDataDialogBase(Graph&,
			       QWidget * const = nullptr);
   
#ifdef TFEL_QT4
      virtual bool exec();
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
      virtual int exec() override;
#endif /* TFEL_QT5 */

      bool exec(const QString&);
   
      operator bool () const;

    protected slots:

      void abscissaChanged(const QString&);

      void ordinateChanged(const QString&);

      void currentAbscissaIndexChanged(int);

      void currentOrdinateIndexChanged(int);

      void import();

      void importAndClose();
      
      void keyChanged(const QString&);

    protected:

      static bool
      isUnsignedShort(const QString&);

      static unsigned short
      convertToUnsignedShort(const QString&);

      /*!
       * \return the curve keys (if available) 
       */
      virtual QVector<QString>
      getCurveKeys(void) = 0;

      virtual QString
      getFileDescription() const = 0;
    
      virtual QStringList
      getFileExtensions() const = 0;

      void
      createGUI(void);

      void
      loadFile(void);

      void
      fails(const QString&);

      std::shared_ptr<TextDataReader> data;

      Graph& g;
      QStringList legends;
      QString     file;
      QLineEdit *a;
      QLineEdit *o;
      QComboBox *la;
      QComboBox *lo;
      bool userDefinedKey;
      bool afirst;
      bool ofirst;

      bool ok;

    }; // end of ImportTextDataDialogBase

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_IMPORTTEXTDATADIALOGBASE_H */
