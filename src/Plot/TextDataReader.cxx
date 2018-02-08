/*!
 * \file   TextDataReader.cxx
 * \brief
 * \author Helfer Thomas
 * \date   13 Nov 2007
 */

#include <cmath>
#include <sstream>
#include <stdexcept>
#include <QtCore/QFile>
#include <QtCore/QRegExp>
#include <QtCore/QTextStream>
#include "TFEL/Raise.hxx"
#include "TFEL/Plot/TextDataReader.hxx"

namespace tfel {

  namespace plot {

    TextDataReader::TextDataReader()
        : separator("") {}  // end of TextDataReader::TextDataReader

    TextDataReader::TextDataReader(const QString& s)
        : separator(s) {}  // end of TextDataReader::TextDataReader

    void TextDataReader::extractData(const QString& f) {
      QFile file(f);
      tfel::raise_if(!file.open(QIODevice::ReadOnly),
                     "TextDataReader::TextDataReader: "
                     "can't open '" +
                         f.toStdString() + "'.");
      QTextStream s(&file);
      this->extractData(s);
    }

    void TextDataReader::extractData(QTextStream& in) {
      auto splitLine = [this](const QString line) {
        if (this->separator.isEmpty()) {
          return line.split(QRegExp("\\s+"), QString::SkipEmptyParts);
        }
        return line.split(QRegExp("\\s+" + this->separator + "\\s+"),
                          QString::SkipEmptyParts);
      };
      this->lines.clear();
      this->legends.clear();
      this->preamble.clear();
      bool firstComments = true;
      unsigned short nbr = 1;
      auto treatLine = [this, splitLine, &nbr](const QString& line) {
        if (line.isEmpty()) {
          return;
        }
        if (line[0] == '#') {
          return;
        }
        const auto tokens = splitLine(line);
        if (tokens.empty()) {
          return;
        }
        Line l;
        l.nbr = nbr;
        for (const auto& t : tokens) {
          bool b = true;
          const auto v = t.toDouble(&b);
          if (b) {
            l.values.push_back(v);
          } else {
            l.values.push_back(nan("invalid value"));
          }
        }
        this->lines.push_back(l);
      }; // end of treatLine
      auto line = in.readLine();
      if (!line.isEmpty()) {
        if (line[0] == '#') {
          line.remove(0, 1u);
          this->legends = splitLine(line);
          this->preamble.push_back(line);
          while ((firstComments) && (!in.atEnd())) {
            line = in.readLine();
            const auto tokens = splitLine(line);
            if (tokens.empty()) {
              break;
            }
            if (tokens[0][0] != '#') {
              treatLine(line);
              break;
            }
            line.remove(0, 1u);
            this->preamble.push_back(line);
          }
        } else {
          const auto tokens = splitLine(line);
          if (!tokens.empty()) {
            bool b = true;
            tokens[0].toDouble(&b);
            if (b) {
              treatLine(line);
            } else {
              this->legends = splitLine(line);
            }
          }
        }
      }
      while (!in.atEnd()) {
        ++nbr;
        treatLine(in.readLine());
      }
    }  // end of TextDataReader::TextDataReader

    const QStringList& TextDataReader::getLegends() const {
      return this->legends;
    }  // end of TextDataReader::getLegends

    const QStringList& TextDataReader::getPreamble() const {
      return this->preamble;
    }  // end of TextDataReader::getPreamble

    QString TextDataReader::getLegend(const unsigned short c) const {
      tfel::raise_if(c == 0,
                     "TextDataReader::getLegend: "
                     "invalid column index.");
      if (c >= this->legends.size() + 1) {
        return "";
      }
      return this->legends[c - 1];
    }  // end of TextDataReader::getLegend

    bool TextDataReader::hasColumn(const QString& n) const {
      return this->legends.contains(n);
    }

    unsigned short TextDataReader::findColumn(const QString& n) const {
      tfel::raise_if(!this->hasColumn(n),
                     "TextDataReader::findColumn: "
                     "no column named '" +
                         n.toStdString() + "' found'.");
      return static_cast<unsigned short>(this->legends.indexOf(n) + 1);
    }  // end of TextDataReader::findColumn

    QVector<double> TextDataReader::getColumn(
        const unsigned short i) const {
      QVector<double> tab;
      this->getColumn(tab, i);
      return tab;
    }  // end of TextDataReader::getColumn

    void TextDataReader::getColumn(QVector<double>& tab,
                                   const unsigned short i) const {
      tab.reserve(this->lines.size());
      int j;
      // current line
      QVector<Line>::const_iterator line;
      // sanity check
      tfel::raise_if(
          i == 0u,
          "TextDataReader::getColumn: "
          "column '0' requested (column numbers begins at '1').");
      // treatment
      for (line = this->lines.begin(), j = 0; line != this->lines.end();
           ++line, ++j) {
        if(line->values.size() < i){
          tfel::raise(
              "TextDataReader::getColumn: "
              "line '" +
              std::to_string(line->nbr) +
              "' "
              "does not have '" +
              std::to_string(i) + "' columns.");
          tab.push_back(line->values[i - 1]);
        }
      }
    }  // end of TextDataReader::getColumn

    QVector<TextDataReader::Line>::const_iterator
    TextDataReader::begin() const {
      return this->lines.begin();
    }  // end of TextDataReader::begin

    QVector<TextDataReader::Line>::const_iterator TextDataReader::end()
        const {
      return this->lines.end();
    }  // end of TextDataReader::end

    unsigned short TextDataReader::getNumberOfColumns() const {
      if (this->lines.isEmpty()) {
        return 0u;
      }
      return static_cast<unsigned short>(this->lines[0].values.size());
    }  // end of TextDataReader::getNumberOfColumns

    const QString& TextDataReader::getSeparator() const {
      return this->separator;
    }  // end of TextDataReader::getSeparator

    TextDataReader::~TextDataReader() = default;

  }  // end of namespace plot

}  // end of namespace tfel
