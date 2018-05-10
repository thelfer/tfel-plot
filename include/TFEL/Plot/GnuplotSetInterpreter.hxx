/*!
 * \file  GnuplotSetInterpreter.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 13 juin 2012
 */

#ifndef LIB_TFEL_PLOT_GNUPLOTSETINTERPRETER_H_
#define LIB_TFEL_PLOT_GNUPLOTSETINTERPRETER_H_

#include "TFEL/Plot/GnuplotInterpreter.hxx"

namespace tfel {

  namespace plot {

    /*!
     * \brief an helper structure use to treat instructions beginning
     * with the `set` keyword.
     */
    struct GnuplotInterpreter::SetInterpreter : GnuplotInterpreterBase {
      /*!
       * \brief constructor
       * \param[in] i: interpreter
       * \param[in] g: graph
       */
      SetInterpreter(GnuplotInterpreter&, Graph&);
      /*!
       * \brief eval the rest of the instruction
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void eval(const_iterator&, const const_iterator);

     protected:
      void registerCallBacks();
      /*!
       * \brief treat the `set border` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatBorder(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set noborder` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatNoBorder(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set xlabel` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatXLabel(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set x2label` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatX2Label(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set ylabel` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatYLabel(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set y2label` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatY2Label(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set xtics` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatXTics(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set x2tics` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatX2Tics(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set ytics` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatYTics(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set y2tics` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatY2Tics(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set xrange` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatXRange(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set x2range` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatX2Range(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set yrange` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatYRange(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set y2range` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatY2Range(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set output` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatOutput(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set terminal` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatTerminal(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set grid` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatGrid(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set arrow` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatArrow(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set key` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatKey(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set title` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatTitle(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set theme` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatTheme(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set dummy` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatDummy(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set logscale` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatLogScale(const_iterator&, const const_iterator);
      /*!
       * \brief treat the `set curve` case
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      void treatCurve(const_iterator&, const const_iterator);
      /*!
       * \brief an helper function used to read tics
       * \return the tics reads
       * \param[in,out] p:  current position
       * \param[in]     pe: end of current instruction
       */
      std::map<qreal, QString> treatTics(const_iterator&,
                                         const const_iterator);

      typedef void (SetInterpreter::*MemFuncPtr)(const_iterator&,
                                                 const const_iterator);

      std::map<std::string, MemFuncPtr> setCallBacks;
      GnuplotInterpreter& interpreter;
    };

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_GNUPLOTSETINTERPRETER_H */
