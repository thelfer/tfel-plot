/*!
 * \file   Curves.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   04 jan 2008
 */

#ifndef _LIB_TFEL_CURVES_HXX_
#define _LIB_TFEL_CURVES_HXX_ 

#include<cmath>
#include<stdexcept>
#include<limits>

#include<QtCore/QObject>
#include<QtCore/QVector>
#include<QtCore/QString>
#ifdef TFEL_QT4
#include<QtGui/QComboBox>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QComboBox>
#endif /* TFEL_QT5 */
#include<QtGui/QColor>

#include"TFEL/Plot/Point.hxx"

namespace tfel{

  namespace plot{

    class Curve
      : public QObject
    {

      Q_OBJECT

    public:

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

      virtual void sendRemoveSignal(void) = 0;
      virtual bool hasRange(void) const = 0;
      virtual qreal minRange(void) const = 0;
      virtual qreal maxRange(void) const = 0;
      virtual void
      getValues(QVector<Point>&,const qreal,
		const qreal,const unsigned short) = 0;
      virtual bool hasSpecifiedColor(void)   const = 0;
      virtual bool hasSpecifiedNumberOfSamples(void)   const = 0;
      virtual bool isFilled(void) const  = 0;
      virtual bool       hasKey(void) const = 0;
      virtual QString    getKey(void) const = 0;
      virtual QColor     getColor(void)  const = 0;
      virtual Curve::Style   getStyle(void)  const = 0;
      virtual unsigned short getNumberOfSamples(void) const = 0;
      virtual unsigned short getWidth() const = 0;
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

#endif /* _LIB_TFEL_CURVES_HXX */

