/*!
 * \file  ImportTextDataDialogBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 juin 2012
 */

#ifndef LIB_TFEL_PLOT_IMPORTTEXTDATADIALOGBASE_H_
#define LIB_TFEL_PLOT_IMPORTTEXTDATADIALOGBASE_H_

#include <vector>
#include <memory>
#include <QtCore/QString>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QComboBox>
#include <TFEL/Plot/Config.hxx>
#include "TFEL/Plot/TextDataReader.hxx"
#include "TFEL/Plot/CurveConfigurationDialogBase.hxx"

namespace tfel {

  namespace plot {

    struct Graph;

    struct TFELPLOT_VISIBILITY_EXPORT ImportTextDataDialogBase
        : CurveConfigurationDialogBase {
      ImportTextDataDialogBase(Graph &, QWidget *const = nullptr);

      int exec() override;

      bool exec(const QString &);

      operator bool() const;

     protected slots:

      void abscissaChanged(const QString &);

      void ordinateChanged(const QString &);

      void currentAbscissaIndexChanged(int);

      void currentOrdinateIndexChanged(int);

      void import();

      void importAndClose();

      void keyChanged(const QString &);

     protected:
      static bool isUnsignedShort(const QString &);

      static unsigned short convertToUnsignedShort(const QString &);

      //! \return the curve keys (if available)
      virtual QStringList getCurveKeys() = 0;

      virtual QString getFileDescription() const = 0;

      virtual QStringList getFileExtensions() const = 0;

      void createGUI();

      void loadFile();

      void fails(const QString &);

      std::shared_ptr<TextDataReader> data;

      Graph &g;
      QStringList legends;
      QString file;
      QLineEdit *a;
      QLineEdit *o;
      QComboBox *la;
      QComboBox *lo;
      bool userDefinedKey;
      bool afirst;
      bool ofirst;

      bool ok;

     private:
      Q_OBJECT

    };  // end of ImportTextDataDialogBase

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_IMPORTTEXTDATADIALOGBASE_H */
