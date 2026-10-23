/*!
 * \file   CLIHandler.cxx
 * \brief
 * \author THOMAS HELFER
 * \date   09 mars 2017
 */

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <QtCore/QtDebug>
#include <readline/readline.h>
#include <readline/history.h>
#include "TFEL/Plot/CLIHandler.hxx"

namespace tfel {

  namespace plot {

    CLIHandler::CLIHandler(const QString& fout, QObject* p) : QObject(p) {
      this->sout = new QLocalServer(this);
      this->sout->listen(fout);
      if (!this->sout->waitForNewConnection(-1)) {
        throw(
            std::runtime_error("CLIHandler::CLIHandler: "
                               "can't connect to the GUI through socket "
                               "'" +
                               fout.toStdString() + "'"));
      }
      this->out = this->sout->nextPendingConnection();
      if (this->out == nullptr) {
        throw(
            std::runtime_error("CLIHandler::CLIHandler: "
                               "no pending connection on socket "
                               "'" +
                               fout.toStdString() + "'"));
      }
      QObject::connect(this->out, &QLocalSocket::disconnected, this,
                       &CLIHandler::socketDisconnected);
    }  // end of CLIHandler::CLIHandler

    void CLIHandler::connect(const QString& fin) {
      // the GUI process is started before it listens to its output
      // socket: the connection attempt can be refused. Retry until
      // the socket becomes available (the GUI is considered unreachable
      // after a few seconds).
      this->in = new QLocalSocket(this);
      bool connected = false;
      for (int count = 0; (count != 50) && (!connected); ++count) {
        this->in->connectToServer(fin, QIODevice::ReadOnly);
        connected = this->in->waitForConnected(100);
        if (!connected) {
          this->in->abort();
        }
      }
      if (!connected) {
        throw(
            std::runtime_error("CLIHandler::connect: "
                               "can't connect to socket "
                               "'" +
                               fin.toStdString() + "'"));
      }
    }  // end of CLIHandler::connect

    void CLIHandler::socketDisconnected() { emit finished(); }

    void CLIHandler::process() {
      while (true) {
        auto read = [this]() {
          // the status, output and error lines may be sent by the GUI
          // in a single packet: only wait for new data when no complete
          // line is buffered, as waitForReadyRead() waits for new data
          // and would otherwise time out while the requested line is
          // already available
          while (!this->in->canReadLine()) {
            if (!this->in->waitForReadyRead()) {
              break;
            }
          }
          auto r = QString::fromLatin1(this->in->readLine());
          r.chop(1);
          return r;
        };
        // the buffer returned by readline is allocated with malloc:
        // own it in a unique_ptr so that it is released on every
        // exit path of the loop
        using LineDeleter = void (*)(void*);
        std::unique_ptr<char, LineDeleter> l{readline("tplot> "), std::free};
        if (l == nullptr) {
          emit finished();
          return;
        }
        const auto l2 = std::string(l.get()) + '\n';
        this->out->write(l2.c_str());
        this->out->waitForBytesWritten();
        const auto s = read();
        qDebug() << s;
        const auto r = read();
        qDebug() << r;
        const auto e = read();
        qDebug() << e;
        if (s == "2") {
          this->out->write("ok");
          this->out->waitForBytesWritten();
          emit finished();
          return;
        }
        if (!r.isEmpty()) {
          std::cout << r.toStdString() << std::endl;
        }
        if (!e.isEmpty()) {
          std::cout << e.toStdString() << std::endl;
        }
        add_history(l.get());
      }
    }  // end of CLIHandler::process()

    CLIHandler::~CLIHandler() = default;

  }  // end of namespace plot

}  // end of namespace tfel
