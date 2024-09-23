/*!
 * \file  ImportLicosCurveDialoag.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 19 juin 2012
 */

#include "TFEL/Plot/ImportLicosCurveDialog.hxx"

namespace tfel {

  namespace plot {

    ImportLicosCurveDialog::ImportLicosCurveDialog(Graph& graph,
                                                   QWidget* const p)
        : ImportTextDataDialogBase(graph, p) {
    }  // end of ImportLicosCurveDialog

    QStringList ImportLicosCurveDialog::getCurveKeys() {
      QStringList k;
      const auto pr = this->data->getPreamble();
      auto p = pr.begin();
      const auto pe = pr.end();
      if (p == pe) {
        return k;
      }
      if ((*p != " first column: time") && (*p != " first column : time")) {
        return k;
      }
      k.push_back("time");
      ++p;
      while (p != pe) {
        const auto& tokens = p->split(QRegularExpression("\\s+"), Qt::SkipEmptyParts);
        if (tokens.size() < 3) {
          return k;
        }
        auto offset = 2u;
        if (tokens[1] != "column:") {
          if (tokens.size() < 4) {
            return k;
          }
          if ((tokens[1] == "column") && (tokens[2] == ":")) {
            offset = 3u;
          } else {
            return k;
          }
        }
        auto pt = tokens.begin() + offset;
        const auto pte = tokens.end();
        QString r;
        while (pt != pte) {
          r += *pt + ' ';
          ++pt;
        }
        k.push_back(r.trimmed());
        ++p;
      }
      return k;
    }  // end of ImportLicosCurveDialog::getCurveKeys

    QString ImportLicosCurveDialog::getFileDescription() const {
      return "Licos Curve";
    }  // end of ImportLicosCurveDialog::getFileDescription

    QStringList ImportLicosCurveDialog::getFileExtensions() const {
      return QStringList() << "txt"
                           << "dat"
                           << "data"
                           << "res";
    }  // end of ImportLicosCurveDialog::getFileExtensions

    ImportLicosCurveDialog::~ImportLicosCurveDialog() = default;

  }  // end of namespace plot

}  // end of namespace tfel
