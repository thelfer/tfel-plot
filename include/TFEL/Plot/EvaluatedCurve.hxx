/*!
 * \file   EvaluatedCurve.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   14 fév 2008
 */

#ifndef LIB_TFEL_PLOT_EVALUATEDCURVE_HXX_
#define LIB_TFEL_PLOT_EVALUATEDCURVE_HXX_ 

#include"TFEL/Plot/Config.hxx"
#include"TFEL/Math/Evaluator.hxx"
#include"TFEL/Plot/CurveBase.hxx"

namespace tfel
{

  namespace plot
  {
    
    class TFELPLOT_VISIBILITY_EXPORT EvaluatedCurve
      : public CurveBase
    {

      Q_OBJECT

    public slots:

      virtual void
      setNumberOfSamples(const unsigned short,
			 const bool) override;

    public:

      EvaluatedCurve(std::shared_ptr<tfel::math::Evaluator>&,
		     const QString& = "x");
      virtual bool  hasRange() const override;
      virtual qreal minRange() const override;
      virtual qreal maxRange() const override;
      virtual void
      getValues(QVector<Point>&,const qreal,
		const qreal,const unsigned short) override;
      virtual bool
      hasSpecifiedNumberOfSamples() const override;
      virtual unsigned short
      getNumberOfSamples() const override;
      virtual ~EvaluatedCurve();
    private:
      std::shared_ptr<tfel::math::Evaluator> ev;
      unsigned short samples;
      bool hasSamples;
      bool isConstant;
    }; // end of struct EvaluatedCurve

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_EVALUATEDCURVE_HXX */

