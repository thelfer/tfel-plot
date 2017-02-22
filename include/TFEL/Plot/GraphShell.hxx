/*! 
 * \file  GraphShell.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_GNUPLOTSHELL_H_
#define _LIB_TFEL_PLOT_GNUPLOTSHELL_H_ 

#include<QtCore/QObject>
#include<QtCore/QStringList>

#ifdef TFEL_QT4
#include<QtGui/QTextEdit>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QTextEdit>
#endif /* TFEL_QT5 */
#include<QtGui/QKeyEvent>
#include<QtGui/QTextCursor>

#include<TFEL/Math/Parser/ExternalFunctionManager.hxx>
#include<TFEL/Plot/GnuplotInterpreter.hxx>
#include<TFEL/Plot/Config.hxx>

namespace tfel{

  namespace plot{
    
    /*!
     * This class provides a simple shell
     * to interpreter gnuplot shell
     */
    class TFELPLOT_VISIBILITY_EXPORT GraphShell
      : public QTextEdit
    {

      Q_OBJECT

    public:

      GraphShell(Graph&,
		 QWidget *const = nullptr);

      virtual void
      treatNewCommand(const QString&);

      ~GraphShell();

    public slots:

      void displayOutputMsg(const QString&);

      void displayErrorMsg(const QString&);

      void saveBuffer();

      void setPrompt(const QString&);

      void importGnuplotFile(const QString&);

    protected:

      static QString
      getModifier(const QKeyEvent&);

      virtual void
      keyPressEvent(QKeyEvent *) override;

      virtual void
      mousePressEvent(QMouseEvent *) override;

      virtual void
      mouseMoveEvent(QMouseEvent *) override;

      virtual void
      mouseReleaseEvent(QMouseEvent *) override;

      virtual bool
      canInsertFromMimeData (const QMimeData *) const override;

      virtual void
      insertFromMimeData(const QMimeData *) override;

      const QString&
      getPrompt() const;

      void
      displayPrompt();

      void
      setCurrentLine(const QString&);

      void
      searchHistoryBackWard();

      void
      searchHistoryUpWard();

      QColor
      getUserInputTextColor();

      void
      addToKillRing(const QString&);

      GnuplotInterpreter gi;

      QString prompt;

      QVector<QString> history;
      QVector<QString>::size_type pHistory;

      QTextCursor bscursor;
      QStringList ring;
      int  pring;  //<! position if the ring
      bool s;
      bool ctrlx; //<! true if previous key pressed was Ctrl-X
      bool ctrlc; //<! true if previous key pressed was Ctrl-Y
      bool yank;  //<! true if previous command was a yank
      
    }; // end of GraphShell
    
  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_GNUPLOTSHELL_H */
