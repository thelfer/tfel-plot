/*!
 * \file   CLIHandler.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 mars 2017
 */

#include<readline/readline.h>
#include<readline/history.h>
#include<QtCore/QDebug>
#include"TFEL/Plot/CLIHandler.hxx"

namespace tfel
{

  namespace plot
  {

    CLIHandler::CLIHandler(const QString& fout,
			   QObject* p)
      : QObject(p)
    {
      this->sout = new QLocalServer(this);
      this->sout->listen(fout);
      this->sout->waitForNewConnection(-1);
      this->out = this->sout->nextPendingConnection();
      this->connect(this->out,SIGNAL(disconnected()),
		    this,SLOT(socketDisconnected()));
    } // end of CLIHandler::CLIHandler

    void CLIHandler::socketDisconnected(void){
      emit finished();
    }
    
    void CLIHandler::process(){
      while(true){
	auto l = readline("tplot> ");
	if(l==nullptr){
	  return;
	}
	auto l2 = std::string(l)+'\n';
	this->out->write(l2.c_str());
	this->out->waitForBytesWritten();
	add_history(l);
	std::free(l);
      }
    } // end of CLIHandler::process()
    
    CLIHandler::~CLIHandler() = default;
    
  } // end of namespace plot
  
} // end of namespace tfel

