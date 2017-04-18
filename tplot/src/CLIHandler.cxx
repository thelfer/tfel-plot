/*!
 * \file   CLIHandler.cxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 mars 2017
 */

#include<iostream>
#include<cstring>
#include<readline/readline.h>
#include<readline/history.h>
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
      QObject::connect(this->out,SIGNAL(disconnected()),
		       this,SLOT(socketDisconnected()));
    } // end of CLIHandler::CLIHandler

    void CLIHandler::connect(const QString& fin)
    {
      this->in = new QLocalSocket(this);
      this->in->connectToServer(fin,QIODevice::ReadOnly);
      if(this->in->state()!=QLocalSocket::ConnectedState){
	throw(std::runtime_error("CLIHandler::connect: "
				 "can't connect to socket "
				 "'"+fin.toStdString()+"'"));
      }
    }
    
    void CLIHandler::socketDisconnected(void){
      emit finished();
    }
    
    void CLIHandler::process(){
      while(true){
	auto l = readline("tplot> ");
	if(l==nullptr){
	  emit finished();
	  return;
	}
	auto l2 = std::string(l)+'\n';
	this->out->write(l2.c_str());
	this->out->waitForBytesWritten();
	this->in->waitForReadyRead();
	const auto s = QString::fromLatin1(this->in->readLine());
	const auto r = QString::fromLatin1(this->in->readLine());
	const auto e = QString::fromLatin1(this->in->readLine());
	if(s=="2"){
	  this->out->write("ok");
	  this->out->waitForBytesWritten();
	  emit finished();
	  return;
	}
	if(!r.isEmpty()){
	  std::cout << r.toStdString() << std::endl;
	}
	add_history(l);
	std::free(l);
      }
    } // end of CLIHandler::process()
    
    CLIHandler::~CLIHandler() = default;
    
  } // end of namespace plot
  
} // end of namespace tfel

