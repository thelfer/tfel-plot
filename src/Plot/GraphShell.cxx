/*!
 * \file  GraphShell.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 15 juin 2012
 */

#include <QtCore/QtDebug>
#include <QtCore/QMimeData>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QShortcut>
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>
#include <QtGui/QKeySequence>
#include <QtGui/QTextBlock>
#include <QtGui/QClipboard>
#include <QtGui/QTextDocumentWriter>

#include "TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include "TFEL/Plot/Graph.hxx"
#include "TFEL/Plot/GraphShell.hxx"

namespace tfel {

  namespace plot {

    QString GraphShell::getModifier(const QKeyEvent& e) {
      QString m;
      if (e.modifiers() & Qt::ControlModifier) {
        m = "Ctrl";
      }
      if (e.modifiers() & Qt::AltModifier) {
        if (m.isEmpty()) {
          m = "Alt";
        } else {
          m += "-Alt";
        }
      }
      if (e.modifiers() & Qt::ShiftModifier) {
        if (m.isEmpty()) {
          m = "Shift";
        } else {
          m += "-Shift";
        }
      }
      if (e.modifiers() & Qt::MetaModifier) {
        if (m.isEmpty()) {
          m = "Meta";
        } else {
          m += "-Meta";
        }
      }
      return m;
    }

    GraphShell::GraphShell(Graph& g, QWidget* const p)
        : QTextEdit(p),
          gi(g),
          prompt(""),
          pHistory(0),
          s(false),
          ctrlx(false),
          ctrlc(false),
          yank(false) {
      QObject::connect(&gi, &GnuplotInterpreter::errorMsg, this,
                       &GraphShell::displayErrorMsg);
      QObject::connect(&gi, &GnuplotInterpreter::outputMsg, this,
                       &GraphShell::displayOutputMsg);
      QObject::connect(&gi, &GnuplotInterpreter::quitCommandTreated, this,
                       &GraphShell::forwardQuitCommand);
      QObject::connect(&gi, &GnuplotInterpreter::graphicalPlot, this,
                       &GraphShell::forwardGraphicalPlot);
      this->setUndoRedoEnabled(false);
      this->displayPrompt();
    }

    void GraphShell::forwardQuitCommand() {
      emit quitCommandTreated();
    }  // end of GraphShell::forwardQuitCommand

    void GraphShell::forwardGraphicalPlot() {
      emit graphicalPlot();
    }  // end of GraphShell::forwardGraphicalPlot

    GnuplotInterpreter::ParsingResult GraphShell::treatNewCommand(
        const QString& l) {
      const auto r = this->gi.parseString(l);
      this->history.append(l);
      this->pHistory = this->history.size();
      return r;
    }  // end of GraphShell::treatNewCommand

    void GraphShell::importGnuplotFile(const QString& f) {
      QFile file(f);
      if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        QMessageBox::critical(this, tr("GraphShell::importGnuplotFile"),
                              tr("opening file '%1' failed").arg(f));
        return;
      }
      QTextStream in(&file);
      QString line;
      while (!in.atEnd()) {
        line += in.readLine().trimmed();
        // one shall look for '\' at the end of line and handle line
        // concatenation...
        if (line.isEmpty()) {
          continue;
        }
        if (line.startsWith('#')) {
          this->append(line);
          line.clear();
          continue;
        }
        if (!line.endsWith('\\')) {
          this->setCurrentLine(line);
          this->treatNewCommand(line);
          this->append("");
          this->displayPrompt();
          line.clear();
        } else {
          line.chop(1);
          line.append(' ');
        }
      }
      if (!line.isEmpty()) {
        this->setCurrentLine(line);
      }
    }  // end of GraphShell::importGnuplotFile

    void GraphShell::searchHistoryBackWard() {
      if (this->pHistory != 0) {
        --(this->pHistory);
        this->setCurrentLine(this->history[this->pHistory]);
      }
    }  // end of GraphShell:searchHistoryBackWard

    void GraphShell::searchHistoryUpWard() {
      if (!this->history.isEmpty()) {
        if (this->pHistory + 1 < this->history.size()) {
          ++(this->pHistory);
          this->setCurrentLine(this->history[this->pHistory]);
        }
      }
    }  // end of GraphShell:searchHistoryUpWard

    void GraphShell::setCurrentLine(const QString& l) {
      this->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
      this->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
      this->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
      this->textCursor().removeSelectedText();
      this->displayPrompt();
      this->insertHtml(l);
    }  // end of GraphShell:searchHistoryUpWard

    bool GraphShell::canInsertFromMimeData(const QMimeData* m) const {
      return m->hasText();
    }

    void GraphShell::insertFromMimeData(const QMimeData* m) {
      if (m->hasText()) {
        const auto slist = m->text().split("\n", QString::SkipEmptyParts);
        QString line;
        for (const auto& l : slist) {
          line += l;
          if (!line.endsWith('\\')) {
            this->setCurrentLine(line);
            this->treatNewCommand(line);
            this->append("");
            this->displayPrompt();
            line.clear();
          } else {
            line.chop(1);
            line.append(' ');
          }
        }
        if (!line.isEmpty()) {
          this->setCurrentLine(line);
        }
      }
    }

    void GraphShell::mousePressEvent(QMouseEvent* e) {
      if (e->button() == Qt::LeftButton) {
        this->bscursor = this->cursorForPosition(e->pos());
        s = true;
        e->accept();
        return;
      } else {
        QTextEdit::mousePressEvent(e);
      }
    }  // end of GraphShell::mousePressEvent

    void GraphShell::mouseMoveEvent(QMouseEvent* e) {
      if (s) {
        QTextCursor n = this->cursorForPosition(e->pos());
        int np = n.position() - this->bscursor.position();
        if (np > 0) {
          QTextCursor c = this->bscursor;
          c.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
                         0);
          c.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                         np);
          QClipboard* clipboard = QApplication::clipboard();
          clipboard->setText(n.selectedText(), QClipboard::Selection);
        } else {
          n.movePosition(QTextCursor::NextCharacter, QTextCursor::MoveAnchor,
                         0);
          n.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor,
                         -np);
          QClipboard* clipboard = QApplication::clipboard();
          clipboard->setText(n.selectedText(), QClipboard::Selection);
        }
        e->accept();
      } else {
        QTextEdit::mouseMoveEvent(e);
      }
    }

    void GraphShell::mouseReleaseEvent(QMouseEvent* e) {
      if (e->button() == Qt::LeftButton) {
        s = false;
      } else {
        QTextEdit::mouseReleaseEvent(e);
      }
    }  // end of GraphShell::mouseReleaseEvent

    void GraphShell::keyPressEvent(QKeyEvent* e) {
      int ps = this->getPrompt().length();
      QTextCursor c = this->textCursor();
      int posInLine = c.position() - c.block().position();
      if (posInLine == ps) {
        this->setTextColor(this->getUserInputTextColor());
      }
      if ((this->yank) &&
          ((e->modifiers() == Qt::ControlModifier) &&
           (e->key() != Qt::Key_Y)) &&
          ((e->modifiers() == Qt::AltModifier) && (e->key() == Qt::Key_Y))) {
        yank = false;
      }
      if (((e->modifiers() == Qt::ControlModifier) &&
           (e->key() == Qt::Key_X)) &&
          (!ctrlx)) {
        ctrlx = true;
        return;
      }
      if (this->ctrlx) {
        this->ctrlx = false;
        if ((e->modifiers() == Qt::ControlModifier) &&
            (e->key() == Qt::Key_S)) {
          this->saveBuffer();
        } else {
          if (e->modifiers() == Qt::NoModifier) {
            qDebug() << "unknown shortcut : Ctr-X " + e->text();
          } else {
            QString m = GraphShell::getModifier(*e);
            qDebug() << "unknown shortcut : Ctr-X " + m + "-" + e->text();
          }
        }
        return;
      }
      if (this->ctrlc) {
        this->ctrlc = false;
        if (e->modifiers() == Qt::NoModifier) {
          qDebug() << "unknown shortcut : Ctr-C " + e->text();
        } else {
          QString m = GraphShell::getModifier(*e);
          qDebug() << "unknown shortcut : Ctr-C " + m + "-" + e->text();
        }
        return;
      }
      if (e->key() == Qt::Key_Return) {
        this->moveCursor(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
        c = this->textCursor();
        QString l = c.block().text();
        l = l.mid(ps);
        if (!l.isEmpty()) {
          this->treatNewCommand(l);
        }
        this->append("");
        this->displayPrompt();
      } else if ((e->modifiers() == Qt::ControlModifier) &&
                 (e->key() == Qt::Key_Z)) {
        return;
      } else if ((e->modifiers() == Qt::ControlModifier) &&
                 (e->key() == Qt::Key_V)) {
        return;
      } else if ((e->modifiers() == Qt::ControlModifier) &&
                 (e->key() == Qt::Key_K)) {
        c = this->textCursor();
        c.movePosition(QTextCursor::EndOfBlock, QTextCursor::KeepAnchor);
        this->addToKillRing(c.selectedText());
        c.removeSelectedText();
      } else if ((e->modifiers() == Qt::ControlModifier) &&
                 (e->key() == Qt::Key_Y)) {
        if (!this->ring.isEmpty()) {
          this->yank = true;
          this->pring = this->ring.size() - 1;
          this->insertPlainText(this->ring[this->pring]);
        }
      } else if ((e->modifiers() == Qt::ControlModifier) &&
                 (e->key() == Qt::Key_E)) {
        this->moveCursor(QTextCursor::EndOfBlock, QTextCursor::MoveAnchor);
        this->setTextColor(this->getUserInputTextColor());
      } else if ((e->key() == Qt::Key_Home) ||
                 ((e->modifiers() == Qt::ControlModifier) &&
                  (e->key() == Qt::Key_A))) {
        this->moveCursor(QTextCursor::StartOfBlock, QTextCursor::MoveAnchor);
        c = this->textCursor();
        c.setPosition(c.position() + ps, QTextCursor::MoveAnchor);
        this->setTextCursor(c);
        this->setTextColor(this->getUserInputTextColor());
      } else if ((e->key() == Qt::Key_Left) ||
                 ((e->modifiers() == Qt::ControlModifier) &&
                  (e->key() == Qt::Key_B)) ||
                 (e->key() == Qt::Key_Backspace)) {
        if (!(posInLine <= ps)) {
          if ((e->key() == Qt::Key_Backspace) || (e->key() == Qt::Key_Left)) {
            QTextEdit::keyPressEvent(e);
          } else {
            this->moveCursor(QTextCursor::PreviousCharacter,
                             QTextCursor::MoveAnchor);
          }
        } else {
          return;
        }
      } else if ((e->modifiers() == Qt::ControlModifier) &&
                 (e->key() == Qt::Key_F)) {
        this->moveCursor(QTextCursor::NextCharacter, QTextCursor::MoveAnchor);
      } else if ((e->key() == Qt::Key_Up) ||
                 ((e->modifiers() == Qt::ControlModifier) &&
                  (e->key() == Qt::Key_P)) ||
                 (e->key() == Qt::Key_PageUp)) {
        this->searchHistoryBackWard();
      } else if ((e->key() == Qt::Key_Down) ||
                 ((e->modifiers() == Qt::ControlModifier) &&
                  (e->key() == Qt::Key_N)) ||
                 (e->key() == Qt::Key_PageDown)) {
        this->searchHistoryUpWard();
      } else if ((e->modifiers() == Qt::AltModifier) &&
                 (e->key() == Qt::Key_Y)) {
        if ((this->ring.isEmpty()) || (!this->yank)) {
          return;
        }
        int cps = this->ring[this->pring].size();
        --(this->pring);
        if (this->pring == -1) {
          this->pring = this->ring.size();
          --(this->pring);
        }
        c = this->textCursor();
        c.movePosition(QTextCursor::PreviousCharacter, QTextCursor::KeepAnchor,
                       cps);
        c.removeSelectedText();
        this->setTextColor(this->getUserInputTextColor());
        c.insertText(this->ring[this->pring]);
        this->setTextCursor(c);
      } else if ((e->modifiers() == Qt::AltModifier) &&
                 (e->key() == Qt::Key_W)) {
        this->addToKillRing(this->textCursor().selectedText());
      } else if ((e->modifiers() == Qt::AltModifier) &&
                 (e->key() == Qt::Key_B)) {
        if (!(posInLine <= ps)) {
          this->moveCursor(QTextCursor::PreviousWord, QTextCursor::MoveAnchor);
        } else {
          return;
        }
      } else if ((e->modifiers() == Qt::AltModifier) &&
                 (e->key() == Qt::Key_D)) {
        c = this->textCursor();
        c.movePosition(QTextCursor::NoMove, QTextCursor::MoveAnchor);
        c.movePosition(QTextCursor::NextWord, QTextCursor::KeepAnchor);
        this->addToKillRing(c.selectedText());
        c.removeSelectedText();
      } else if ((e->modifiers() == Qt::AltModifier) &&
                 (e->key() == Qt::Key_F)) {
        this->moveCursor(QTextCursor::NextWord, QTextCursor::MoveAnchor);
      } else {
        QTextEdit::keyPressEvent(e);
      }
    }

    void GraphShell::displayPrompt() {
      this->setTextColor(Qt::blue);
      this->textCursor().insertText(this->getPrompt());
      this->setTextColor(this->getUserInputTextColor());
    }

    QColor GraphShell::getUserInputTextColor() { return Qt::black; }

    const QString& GraphShell::getPrompt() const { return this->prompt; }

    void GraphShell::displayOutputMsg(const QString& m) {
      this->setTextColor(Qt::green);
      this->append(m);
      this->setTextColor(this->getUserInputTextColor());
    }

    void GraphShell::displayErrorMsg(const QString& m) {
      this->setTextColor(Qt::red);
      this->append(m);
      this->setTextColor(this->getUserInputTextColor());
    }

    void GraphShell::saveBuffer() {
      const QString& f =
          QFileDialog::getSaveFileName(this, tr("Save File"), "", "(*.odf)");
      if (!f.isEmpty()) {
        QTextDocumentWriter writer;
        writer.setFileName(f);
        writer.setFormat("odf");
        qDebug() << this->document()->characterCount();
        if (!writer.write(this->document())) {
          QMessageBox::critical(this, tr("GraphShell::saveBuffer"),
                                tr("saving to file '%1' failed").arg(f));
        }
      }
    }  // end of GraphShell::saveBuffer

    void GraphShell::setPrompt(const QString& p) {
      this->moveCursor(QTextCursor::End, QTextCursor::MoveAnchor);
      this->moveCursor(QTextCursor::StartOfLine, QTextCursor::MoveAnchor);
      this->moveCursor(QTextCursor::End, QTextCursor::KeepAnchor);
      this->textCursor().removeSelectedText();
      this->prompt = p;
      this->displayPrompt();
    }

    void GraphShell::addToKillRing(const QString& t) {
      if (t.isEmpty()) {
        return;
      }
      if (this->ring.size() == 100) {
        this->ring.pop_front();
      }
      this->ring << t;
    }  // end of GraphShell::addToKillRing

    GraphShell::~GraphShell() = default;

  }  // end of namespace plot

}  // end of namespace tfel
