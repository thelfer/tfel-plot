/*!
 * \file   DataCurve.hxx
 * \brief  
 * 
 * \author Helfer Thomas
 * \date   19 jan 2008
 */

#ifndef LIB_TFEL_PLOT_DATACURVE_HXX_
#define LIB_TFEL_PLOT_DATACURVE_HXX_ 

#include<vector>
#include<memory>
#include<QtCore/QFileSystemWatcher>
#include"TFEL/Plot/Config.hxx"
#include"TFEL/Math/Parser/ExternalFunctionManager.hxx"
#include"TFEL/Plot/CurveBase.hxx"

namespace tfel{

  namespace plot{

    struct TextDataReader;

    struct TFELPLOT_VISIBILITY_EXPORT DataCurve : CurveBase {
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

      DataCurve(const std::vector<qreal>&,
		const std::vector<qreal>&);

      bool  hasRange() const override;
      qreal minRange() const override;
      qreal maxRange() const override;

      void getValues(std::vector<Point>&,const qreal,
		     const qreal,const unsigned short) override;

      bool hasSpecifiedNumberOfSamples() const override;

      unsigned short getNumberOfSamples() const override;

      const std::vector<qreal>&
      getAbscissa() const;

      const std::vector<qreal>&
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

      void getValues(std::vector<double>&,
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
      std::vector<qreal> xvalues;
      std::vector<qreal> yvalues;

     private:
      Q_OBJECT

    }; // end of DataCurve
    
  } // end of namespace plot

} // end of namespace tfel

#endif /* LIB_TFEL_PLOT_DATACURVE_HXX */

