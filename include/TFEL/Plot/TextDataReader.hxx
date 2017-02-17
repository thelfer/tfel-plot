/*!
 * \file   TextData.hxx
 * \brief    
 * \author Helfer Thomas
 * \date   13 Nov 2007
 */

#ifndef _LIB_TFEL_UTILITIES_TEXTDATAREADER_H_
#define _LIB_TFEL_UTILITIES_TEXTDATAREADER_H_ 

#include<QtCore/QVector>
#include<QtCore/QString>
#include<QtCore/QTextStream>
#include<QtCore/QStringList>

#include"TFEL/Plot/Config.hxx"

namespace tfel
{

  namespace plot
  {

    /*!
     * \brief class in charge of reading data in a text file
     */
    struct TFELUTILITIES_VISIBILITY_EXPORT TextDataReader
    {
      /*!
       * \brief helper class in charge of storing of line of data line
       * starting with '#' ared considered comments and not stored.
       * If the first line begins with a '#', the following words are
       * intepreted as the title of each column
       */
      struct Line
      {
	//! line number
	unsigned short nbr;
	//! line tokens
	QVector<double> values;
      }; // end of struct Line
      /*!
       * \brief constructor
       */
      TextDataReader();
      /*!
       * \brief constructor
       * \param[in] file : file name
       */
      TextDataReader(const QString&);
      /*!
       * extract data from the device
       * \param[in] f : input device
       */
      void
      extractData(QTextStream&);
      /*!
       * \param[in] i : column number
       * \return the specified column, converting all values to double
       * \throw std::runtime_error if a line is shorter than the
       * specified column number or if a value can't be converted to
       * a double
       */
      QVector<double>
      getColumn(const unsigned short) const;
      /*!
       * \brief extract the specified column, converting all values to
       * double
       * \param[out] tab : column values
       * \param[in]  i   : column number
       * \throw std::runtime_error if a line is shorter than the
       * specified column number or if a value can't be converted to
       * a double
       */
      void
      getColumn(QVector<double>&,
		const unsigned short) const;
      /*!
       * \return the column having the specified title
       * \param[in] name : column title
       * \throw std::runtime_error if no column with the specified
       * title is found
       */
      unsigned short
      findColumn(const QString&) const;
      /*!
       * \return the legend associated to the curves
       */
      const QStringList&
      getLegends(void) const;
      /*!
       * \return the title of the specified column
       * \param[in] c : column number
       * \throw std::runtime_error if no title is found
       */
      QString
      getLegend(const unsigned short c) const;
      /*!
       * \return an iterator to the first line
       */
      QVector<Line>::const_iterator
      begin() const;
      /*!
       * \return an iterator past the last line
       */
      QVector<Line>::const_iterator
      end() const;
      /*!
       * get the first commented lines
       */
      const QStringList&
      getPreamble(void) const;
      /*!
       * \return the number of columns
       */
      unsigned short
      getNumberOfColumns(void) const;
    private:
      /*!
       * \brief copy constructor (disabled)
       */
      TextDataReader(const TextDataReader&);
      /*!
       * \brief assignement operator (disabled)
       */
      TFEL_VISIBILITY_LOCAL 
      TextDataReader&
      operator = (const TextDataReader&);
      //! list of all tokens of the file, sorted by line
      QVector<Line> lines;
      //! list of column titles
      QStringList legends;
      //! first commented lines
      QStringList preamble;
    }; // end of struct TextDataReader

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_UTILITIES_TEXTDATAREADER_H */
