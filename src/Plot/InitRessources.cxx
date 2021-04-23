/*!
 * \file  InitRessources.cxx
 * \brief
 * \author Thomas Helfer
 * \brief  27/01/2018
 */

#include <QtCore/QtGlobal>
#include "TFEL/Plot/InitRessources.hxx"

static void TFELPlotInitRessources() { Q_INIT_RESOURCE(TFELPlotResources); }

namespace tfel {

  namespace plot {

    void initRessources() {
      ::TFELPlotInitRessources();
    }  // end of initRessources

  }  // end of namespace plot

}  // end of namespace tfel
