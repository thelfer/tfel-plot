/*!
 * \file   CLIHandler.hxx
 * \brief    
 * \author THOMAS HELFER
 * \date   09 mars 2017
 */

#ifndef LIB_TFEL_PLOT_CLIHANDLER_HXX
#define LIB_TFEL_PLOT_CLIHANDLER_HXX

#include<QtCore/QObject>
#include<QtCore/QString>
#include<QtNetwork/QLocalSocket>
#include<QtNetwork/QLocalServer>

namespace tfel
{
  namespace plot
  {

    /*!
     * \brief class handling reading the command line using the
     * readling library.
     */
    class CLIHandler
      : public QObject
    {
      Q_OBJECT
    public:
      /*!
       * \brief constructor
       * \param[in] fout: output socket name
       * \param[in] p:    parent object
       */
      CLIHandler(const QString&,
		 QObject* = nullptr);
      /*!
       * \param[in] fout: input socket name
       */
      virtual void connect(const QString&);
      //! destructor
      ~CLIHandler() override;
    public slots:
      //! \brief execute an infinite loop.
      virtual void process();
      //! \brief slot called whan a the server is disconnected.
      virtual void socketDisconnected();
    signals:
      //! signal called when reading from the command line is finished
      void finished();
      /*!
       * \brief send and error message
       */
      void error(QString);
    private:
      QLocalServer *sout;
      QLocalSocket *in;
      QLocalSocket *out;
    }; // end of class CLIHandler
    
  } // end of namespace plot
  
} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_CLIHANDLER_HXX */
