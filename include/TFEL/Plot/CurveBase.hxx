/*! 
 * \file  CurveBase.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 06 juin 2012
 */

#ifndef LIB_TFEL_PLOT_CURVEBASE_H_
#define LIB_TFEL_PLOT_CURVEBASE_H_ 

#include"TFEL/Plot/Curve.hxx"

namespace tfel{

  namespace plot{

    class TFELPLOT_VISIBILITY_EXPORT CurveBase
      : public Curve
    {

      Q_OBJECT

    public slots:

      void setFilled(const bool,const bool) override;
      void setKey(const QString&,const bool) override;
      void setColor(const QColor&,const bool) override;
      void setStyle(const Style&,const bool) override;
      void setWidth(const unsigned short,const bool) override;
      void unsetColor(const bool) override;

    public:
     bool shouldFollowThemeLineStyle() const override;

     unsigned short getThemeLineStyle() const override;

     void setThemeLineStyle(const unsigned short) override;

     void sendRemoveSignal() override;
     bool hasSpecifiedColor() const override;
     bool isFilled() const override;
     bool hasKey() const override;
     QString getKey() const override;
     QColor getColor() const override;
     Curve::Style getStyle() const override;
     unsigned short getWidth() const override;

     ~CurveBase() override;

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

#endif /* LIB_TFEL_PLOT_CURVEBASE_H */

