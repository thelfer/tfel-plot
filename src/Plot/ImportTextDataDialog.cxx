/*!
 * \file  ImportTextDataDialog.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 02 juin 2012
 */

#include "TFEL/Plot/ImportTextDataDialog.hxx"

namespace tfel {

  namespace plot {

    ImportTextDataDialog::ImportTextDataDialog(Graph& graph, QWidget* const p)
        : ImportTextDataDialogBase(graph, p) {}  // end of TextDataTreeWidget

    QStringList ImportTextDataDialog::getCurveKeys() {
      QStringList keys;
      for (const auto& l : this->data->getLegends()) {
        keys.append(l);
      }
      return keys;
    }

    QString ImportTextDataDialog::getFileDescription() const {
      return "Text Data";
    }

    QStringList ImportTextDataDialog::getFileExtensions() const {
      return QStringList() << "txt"
                           << "dat"
                           << "data"
                           << "res";
    }

  }  // end of namespace plot

}  // end of namespace tfel
