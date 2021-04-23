/*!
 * \file   EvaluatedCurve.hxx
 * \brief
 *
 * \author Helfer Thomas
 * \date   14 fév 2008
 */

#ifndef LIB_TFEL_PLOT_EVALUATEDCURVE_HXX_
#define LIB_TFEL_PLOT_EVALUATEDCURVE_HXX_

#include "TFEL/Plot/Config.hxx"
#include "TFEL/Math/Evaluator.hxx"
#include "TFEL/Plot/CurveBase.hxx"

namespace tfel {

  namespace plot {

    struct TFELPLOT_VISIBILITY_EXPORT EvaluatedCurve : CurveBase {
     public slots:

      void setNumberOfSamples(const unsigned short, const bool) override;

     public:
      EvaluatedCurve(std::shared_ptr<tfel::math::Evaluator>&,
                     const QString& = "x");
      bool hasRange() const override;
      qreal minRange() const override;
      qreal maxRange() const override;
      void getValues(std::vector<Point>&,
                     const qreal,
                     const qreal,
                     const unsigned short) override;
      bool hasSpecifiedNumberOfSamples() const override;
      unsigned short getNumberOfSamples() const override;
      //! destructor
      ~EvaluatedCurve() override;

     private:
      std::shared_ptr<tfel::math::Evaluator> ev;
      unsigned short samples;
      bool hasSamples;
      bool isConstant;
      Q_OBJECT
    };  // end of struct EvaluatedCurve

  }  // end of namespace plot

}  // end of namespace tfel

#endif /* LIB_TFEL_PLOT_EVALUATEDCURVE_HXX */
