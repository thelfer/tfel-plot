/*!
 * \file   DataCurve.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 jan 2008
 */

#ifndef _LIB_TFEL_DATACURVE_HXX_
#define _LIB_TFEL_DATACURVE_HXX_ 

#include<memory>
#include<QtCore/QVector>
#include<QtCore/QFileSystemWatcher>
#include"TFEL/Plot/Config.hxx"
#include"TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include"TFEL/Plot/CurveBase.hxx"

namespace tfel{

  namespace plot{

    struct TextDataReader;

    class TFELPLOT_VISIBILITY_EXPORT DataCurve
      : public CurveBase
    {

      Q_OBJECT

    public slots:

      void setNumberOfSamples(const unsigned short,const bool) override;

    public:
      
      typedef tfel::math::parser::ExternalFunctionManager    ExternalFunctionManager;
      typedef tfel::math::parser::ExternalFunctionManagerPtr ExternalFunctionManagerPtr;

      DataCurve(const QString&,
		const unsigned short,
		const unsigned short);

      DataCurve(const QString&,
		const QString&,
		const unsigned short,
		ExternalFunctionManagerPtr = ExternalFunctionManagerPtr());

      DataCurve(const QString&,
		const unsigned short,
		const QString&,
		ExternalFunctionManagerPtr = ExternalFunctionManagerPtr());

      DataCurve(const QString&,
		const QString&,
		const QString&,
		ExternalFunctionManagerPtr = ExternalFunctionManagerPtr());

      DataCurve(const QVector<qreal>&,
		const QVector<qreal>&);

      virtual bool  hasRange(void) const override;
      virtual qreal minRange(void) const override;
      virtual qreal maxRange(void) const override;

      virtual void
      getValues(QVector<Point>&,const qreal,
		const qreal,const unsigned short) override;

      virtual bool
      hasSpecifiedNumberOfSamples(void) const override;

      virtual unsigned short
      getNumberOfSamples(void) const override;

      const QVector<qreal>&
      getAbscissa(void) const;

      const QVector<qreal>&
      getValues(void) const;

      virtual ~DataCurve();

    public slots:
      
      void updatedDataFile(const QString&);

    protected slots:
      
      void  executeDelayedDataLoading(void);

    protected:

      static double
      readDouble(const std::string&,
		 const unsigned short);

      static bool
      isUnsignedInteger(const std::string&);

      static unsigned short
      convertToUnsignedShort(const std::string&);

      void loadDataFromFile(const bool);

      void getValues(QVector<double>&,
		     const TextDataReader&,
		     const QString&);
      
      QFileSystemWatcher *watcher;

      ExternalFunctionManagerPtr fm;

      const QString file;
      const QString cx;
      const QString cy;
      const unsigned short ucx;
      const unsigned short ucy;
      QVector<qreal> xvalues;
      QVector<qreal> yvalues;

    }; // end of DataCurve
    
  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_DATACURVE_HXX */

