/*! 
 * \file  CurveBase.cxx
 * \brief
 * \author Helfer Thomas
 * \brief 06 juin 2012
 */

#include"TFEL/Plot/CurveBase.hxx"

namespace tfel{

  namespace plot{
        
    CurveBase::CurveBase() = default;

    bool
    CurveBase::shouldFollowThemeLineStyle() const
    {
      return this->useThemeLineStyle;
    }
      
    unsigned short
    CurveBase::getThemeLineStyle() const
    {
      return this->themeLineStyle;
    } // end of Curve::getThemeLineStyle

    void
    CurveBase::setThemeLineStyle(const unsigned short t)
    {
      this->themeLineStyle = t;
    } // end of Curve::getThemeLineStyle

    unsigned short
    CurveBase::getWidth() const
    {
      return this->width;
    }
    
    void CurveBase::setWidth(const unsigned short w,const bool b)
    {
      this->width = w;
      emit widthChanged(w);
      if(b){
	emit updated(this);
      }
    }

    void CurveBase::sendRemoveSignal()
    {
      emit remove(this);
    }

    bool CurveBase::isFilled() const
    {
      return this->filled;
    } // end of CurveBase::isFilled

    void CurveBase::setFilled(const bool f,
			      const bool b)
    {
      this->filled = f;
      if(b){
	emit updated(this);
      }
    } // end of CurveBase::setFilled

    void
    CurveBase::setKey(const QString& l,
		      const bool b)
    {
      this->hasLegend = !l.isEmpty();
      this->legend = l;
      emit keyChanged(l);
      if(b){
	emit updated(this);
      }
    } // end of CurveBase::setKey

    bool CurveBase::hasKey() const
    {
      return this->hasLegend;
    } // end of CurveBase::hasKey

    QString CurveBase::getKey() const
    {
      return this->legend;
    } // end of CurveBase::getKey()

    bool CurveBase::hasSpecifiedColor() const
    {
      return this->hasColor;
    } // end of CurveBase::hasSpecifiedColor

    void CurveBase::setColor(const QColor& c,
			     const bool b)
    {
      this->hasColor = true;
      this->color = c;
      emit colorChanged(c);
      if(b){
	emit updated(this);
      }
    } // end of CurveBase::setColor

    void CurveBase::unsetColor(const bool b)
    {
      this->hasColor = false;
      this->color = QColor();
      emit colorChanged(this->color);
      if(b){
	emit updated(this);
      }
    } // end of CurveBase::unsetColor

    void CurveBase::setStyle(const Style& s,
			     const bool b)
    {
      this->useThemeLineStyle = false;
      this->style = s;
      emit styleChanged(this->style);
      if(b){
	emit updated(this);
      }
    } // end of CurveBase::setStyle

    QColor CurveBase::getColor() const
    {
      if(!this->hasColor){
	throw(std::runtime_error("CurveBase::getColor: "
				 "no color has been specified."));
      }
      return this->color;
    } // end of CurveBase::getColor

    Curve::Style CurveBase::getStyle() const
    {
      return this->style;
    } // end of CurveBase::getStyle

    CurveBase::~CurveBase() = default;

  } // end of namespace plot

} // end of namespace tfel

