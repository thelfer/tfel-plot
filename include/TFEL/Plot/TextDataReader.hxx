/*!
 * \file   TextData.hxx
 * \brief
 * \author Helfer Thomas
 * \date   13 Nov 2007
 */

#ifndef LIB_TFEL_PLOT_TEXTDATAREADER_H_
#define LIB_TFEL_PLOT_TEXTDATAREADER_H_

#include <vector>
#include <QtCore/QString>
#include <QtCore/QTextStream>
#include <QtCore/QStringList>

#include "TFEL/Plot/Config.hxx"

namespace tfel {

  namespace plot {

    /*!
     * \brief class in charge of reading data in a text file
     */
    struct TFELPLOT_VISIBILITY_EXPORT TextDataReader {
      /*!
       * \brief helper class in charge of storing of line of data line
       * starting with '#' ared considered comments and not stored.
       * If the first line begins with a '#', the following words are
       * intepreted as the title of each column
       */
      struct Line {
        //! line number
        unsigned short nbr;
        //! line tokens
        std::vector<double> values;
      };  // end of struct Line
      //! \brief constructor
      TextDataReader();
      /*!
       * \brief constructor
       * \param[in] s: separator
       */
      TextDataReader(const QString&);
      /*!
       * \brief extract data from the file
       * \param[in] f: file name
       */
      void extractData(const QString&);
      /*!
       * \brief extract data from the device
       * \param[in] f: input device
       */
      void extractData(QTextStream&);
      /*!
       * \param[in] i: column number
       * \return the specified column, converting all values to double
       * \throw std::runtime_error if a line is shorter than the
       * specified column number or if a value can't be converted to
       * a double
       */
      std::vector<double> getColumn(const unsigned short) const;
      /*!
       * \brief extract the specified column, converting all values to
       * double
       * \param[out] tab : column values
       * \param[in]  i   : column number
       * \throw std::runtime_error if a line is shorter than the
       * specified column number or if a value can't be converted to
       * a double
       */
      void getColumn(std::vector<double>&, const unsigned short) const;
      /*!
       * \return if a column has the specified title
       * \param[in] n: column title
       */
      bool hasColumn(const QString&) const;
      /*!
       * \return the column having the specified title
       * \param[in] n: column title
       * \throw std::runtime_error if no column with the specified
       * title is found
       */
      unsigned short findColumn(const QString&) const;
      /*!
       * \return the legend associated to the curves
       */
      const QStringList& getLegends() const;
      /*!
       * \return the title of the specified column
       * \param[in] c : column number
       * \throw std::runtime_error if no title is found
       */
      QString getLegend(const unsigned short c) const;
      /*!
       * \return an iterator to the first line
       */
      std::vector<Line>::const_iterator begin() const;
      /*!
       * \return an iterator past the last line
       */
      std::vector<Line>::const_iterator end() const;
      /*!
       * get the first commented lines
       */
      const QStringList& getPreamble() const;
      //! \return the number of columns
      unsigned short getNumberOfColumns() const;
      //! \return the separator
      const QString& getSeparator() const;
      //! destructor
      ~TextDataReader();

     private:
      //! \brief move constructor (disabled)
      TextDataReader(TextDataReader&&) = delete;
      //! \brief copy constructor (disabled)
      TextDataReader(const TextDataReader&) = delete;
      //! \brief move assignement (disabled)
      TextDataReader& operator=(TextDataReader&&) = delete;
      //! \brief copy assignement (disabled)
      TextDataReader& operator=(const TextDataReader&) = delete;
      //! list of all tokens of the file, sorted by line
      std::vector<Line> lines;
      //! list of column titles
      QStringList legends;
      //! first commented lines
      QStringList preamble;
      //! separator
      const QString separator;
    };  // end of struct TextDataReader

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_TEXTDATAREADER_H */
