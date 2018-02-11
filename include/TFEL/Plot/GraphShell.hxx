/*!
 * \file  GraphShell.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GRAPHSHELL_H_
#define LIB_TFEL_PLOT_GRAPHSHELL_H_

#include <QtCore/QObject>
#include <QtCore/QStringList>
#include <QtWidgets/QTextEdit>
#include <QtGui/QKeyEvent>
#include <QtGui/QTextCursor>

#include <TFEL/Math/Parser/ExternalFunctionManager.hxx>
#include <TFEL/Plot/GnuplotInterpreter.hxx>
#include <TFEL/Plot/Config.hxx>

namespace tfel {

  namespace plot {

    /*!
     * This class provides a simple shell
     * to interpreter gnuplot shell
     */
    class TFELGNUPLOTINTERPRETER_VISIBILITY_EXPORT GraphShell
        : public QTextEdit {
      Q_OBJECT

     public:
      GraphShell(Graph &, QWidget *const = nullptr);

      virtual GnuplotInterpreter::ParsingResult treatNewCommand(
          const QString &);

      ~GraphShell() override;

     public slots:

      void displayOutputMsg(const QString &);

      void displayErrorMsg(const QString &);

      void saveBuffer();

      void setPrompt(const QString &);

      void importGnuplotFile(const QString &);

     signals:

      void quitCommandTreated();

      void graphicalPlot();

     protected slots:

      void forwardQuitCommand();

      void forwardGraphicalPlot();

     protected:
      static QString getModifier(const QKeyEvent &);

      void keyPressEvent(QKeyEvent *) override;

      void mousePressEvent(QMouseEvent *) override;

      void mouseMoveEvent(QMouseEvent *) override;

      void mouseReleaseEvent(QMouseEvent *) override;

      bool canInsertFromMimeData(const QMimeData *) const override;

      void insertFromMimeData(const QMimeData *) override;

      const QString &getPrompt() const;

      void displayPrompt();

      void setCurrentLine(const QString &);

      void searchHistoryBackWard();

      void searchHistoryUpWard();

      QColor getUserInputTextColor();

      void addToKillRing(const QString &);

      GnuplotInterpreter gi;

      QString prompt;

      QVector<QString> history;
      QVector<QString>::size_type pHistory;

      QTextCursor bscursor;
      QStringList ring;
      int pring;  //<! position if the ring
      bool s;
      bool ctrlx;  //<! true if previous key pressed was Ctrl-X
      bool ctrlc;  //<! true if previous key pressed was Ctrl-Y
      bool yank;   //<! true if previous command was a yank

    };  // end of GraphShell

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GRAPHSHELL_H */
