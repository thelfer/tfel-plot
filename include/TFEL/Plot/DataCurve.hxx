/*!
 * \file   DataCurve.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 jan 2008
 */

#ifndef LIB_TFEL_PLOT_DATACURVE_HXX_
#define LIB_TFEL_PLOT_DATACURVE_HXX_ 

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
		const QString&,
		const unsigned short,
		const unsigned short);

      DataCurve(const QString&,
		const QString&,
		const QString&,
		const unsigned short,
		ExternalFunctionManagerPtr = ExternalFunctionManagerPtr());

      DataCurve(const QString&,
		const QString&,
		const unsigned short,
		const QString&,
		ExternalFunctionManagerPtr = ExternalFunctionManagerPtr());

      DataCurve(const QString&,
		const QString&,
		const QString&,
		const QString&,
		ExternalFunctionManagerPtr = ExternalFunctionManagerPtr());

      DataCurve(const QVector<qreal>&,
		const QVector<qreal>&);

      bool  hasRange() const override;
      qreal minRange() const override;
      qreal maxRange() const override;

      void getValues(QVector<Point>&,const qreal,
		     const qreal,const unsigned short) override;

      bool hasSpecifiedNumberOfSamples() const override;

      unsigned short getNumberOfSamples() const override;

      const QVector<qreal>&
      getAbscissa() const;

      const QVector<qreal>&
      getValues() const;

      ~DataCurve() override;

    public slots:
      
      void updatedDataFile(const QString&);

    protected slots:
      
      void  executeDelayedDataLoading();

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
      const QString separator;
      const QString cx;
      const QString cy;
      const unsigned short ucx;
      const unsigned short ucy;
      QVector<qreal> xvalues;
      QVector<qreal> yvalues;

    }; // end of DataCurve
    
  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_DATACURVE_HXX */

