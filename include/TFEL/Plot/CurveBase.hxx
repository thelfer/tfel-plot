/*! 
 * \file  CurveBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 06 juin 2012
 */

#ifndef _LIB_TFEL_PLOT_CURVEBASE_H_
#define _LIB_TFEL_PLOT_CURVEBASE_H_ 

#include"TFEL/Plot/Curve.hxx"

namespace tfel{

  namespace plot{

    class CurveBase
      : public Curve
    {

      Q_OBJECT

    public slots:

      virtual void setFilled(const bool,const bool) override;
      virtual void setKey(const QString&,const bool) override;
      virtual void setColor(const QColor&,const bool) override;
      virtual void setStyle(const Style&,const bool) override;
      virtual void setWidth(const unsigned short,const bool) override;
      virtual void unsetColor(const bool) override;

    public:

      virtual bool
      shouldFollowThemeLineStyle() const override;
      
      virtual unsigned short
      getThemeLineStyle() const override;

      virtual void
      setThemeLineStyle(const unsigned short) override;

      virtual void sendRemoveSignal(void) override;
      virtual bool hasSpecifiedColor(void)   const override;
      virtual bool isFilled(void) const override;
      virtual bool       hasKey(void) const override;
      virtual QString    getKey(void) const override;
      virtual QColor     getColor(void)  const override;
      virtual Curve::Style   getStyle(void)  const override;
      virtual unsigned short getWidth() const override;
      
      virtual ~CurveBase();

    protected:

      CurveBase();      

      QString  legend;
      QColor color;

      Curve::Style style = Curve::SOLIDLINE;
      unsigned short width = 1;
      unsigned short themeLineStyle = 0;
      bool useThemeLineStyle = true;
      bool hasLegend = true;
      bool hasColor = false;
      bool filled = false;

    }; // end of struct Curve

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_CURVEBASE_H */

