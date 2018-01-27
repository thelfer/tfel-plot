/*!
 * \file   Curves.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jan 2008
 */

#ifndef LIB_TFEL_PLOT_CURVES_HXX_
#define LIB_TFEL_PLOT_CURVES_HXX_ 

#include<cmath>
#include<stdexcept>
#include<limits>

#include<QtCore/QObject>
#include<QtCore/QVector>
#include<QtCore/QString>
#include<QtGui/QColor>
#include<QtWidgets/QComboBox>

#include"TFEL/Plot/Config.hxx"
#include"TFEL/Plot/Point.hxx"

namespace tfel{

  namespace plot{

    /*!
     * \brief base class describing a curve.
     * \see the `DataCurve` and the `EvaluatedCurve` classes for
     * details.
     */
    class TFELPLOT_VISIBILITY_EXPORT Curve
      : public QObject
    {

      Q_OBJECT

    public:

      /*!
       * \brief list of all available styles for drawing a curve
       */
      enum Style{
	SOLIDLINE      =  0u,
	DOTLINE        =  1u,
	DASHLINE       =  2u,
	DASHDOTLINE    =  3u,
	DASHDOTDOTLINE =  4u,
	LINEPLUS       =  5u, 
	DOT            =  6u,
	SQUARE         =  7u,
	TRIANGLE       =  8u,
	CROSS          =  9u,
	DIAMOND        = 10u,
	PLUS           = 11u
      }; // end of Style

      static QComboBox*
      getStyleComboBox();
			
      static int
      curveStyleToInt(const Curve::Style);

      static Curve::Style
      intToCurveStyle(const int);

      virtual bool
      shouldFollowThemeLineStyle() const = 0;
      
      virtual unsigned short
      getThemeLineStyle() const = 0;

      virtual void
      setThemeLineStyle(const unsigned short) = 0;

      virtual void sendRemoveSignal() = 0;
      virtual bool hasRange() const = 0;
      virtual qreal minRange() const = 0;
      virtual qreal maxRange() const = 0;
      virtual void
      getValues(QVector<Point>&,const qreal,
		const qreal,const unsigned short) = 0;
      virtual bool hasSpecifiedColor()   const = 0;
      virtual bool hasSpecifiedNumberOfSamples()   const = 0;
      virtual bool isFilled() const  = 0;
      virtual bool       hasKey() const = 0;
      virtual QString    getKey() const = 0;
      virtual QColor     getColor()  const = 0;
      virtual Curve::Style   getStyle()  const = 0;
      virtual unsigned short getNumberOfSamples() const = 0;
      virtual unsigned short getWidth() const = 0;
      //! destructor
      virtual ~Curve();

    public slots:

      virtual void setFilled(const bool,const bool = true) = 0;
      virtual void setKey(const QString&,const bool = true) = 0;
      virtual void setColor(const QColor&,const bool = true) = 0;
      virtual void setStyle(const Style&,const bool = true) = 0;
      virtual void setNumberOfSamples(const unsigned short,const bool = true) = 0;
      virtual void setWidth(const unsigned short,const bool = true) = 0;
      virtual void unsetColor(const bool = true) = 0;

    signals:

      void updated(Curve *);

      void remove(Curve *);

      void colorChanged(const QColor&);

      void keyChanged(const QString&);

      void styleChanged(int);

      void widthChanged(int);

    }; // end of struct Curve

  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_CURVES_HXX */

