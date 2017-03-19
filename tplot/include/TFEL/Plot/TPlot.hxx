/*! 
 * \file  TPlot.hxx
 * \brief
 * \author Helfer Thomas
 * \brief 27 mai 2012
 */

#ifndef _LIB_TFEL_PLOT_TPLOT_H_
#define _LIB_TFEL_PLOT_TPLOT_H_ 

#include<QtNetwork/QLocalServer>
#include<QtNetwork/QLocalSocket>
#ifdef TFEL_QT4
#include<QtGui/QMainWindow>
#endif /* TFEL_QT4 */
#ifdef TFEL_QT5
#include<QtWidgets/QMainWindow>
#endif /* TFEL_QT5 */

#include"TFEL/Config/TFELConfig.hxx"
#include"TFEL/Utilities/TextData.hxx"
#include"TFEL/Utilities/ArgumentParserBase.hxx"
#include"TFEL/Utilities/GenTypeBase.hxx"
#include"TFEL/Plot/TextDataReader.hxx"
#include"TFEL/Plot/Curve.hxx"
#include"TFEL/Plot/Graph.hxx"
#include"TFEL/Plot/GraphShell.hxx"

namespace tfel
{

  namespace plot
  {

    class TPlot
      : public QMainWindow,
	public tfel::utilities::ArgumentParserBase<TPlot>
    {

      Q_OBJECT

    public slots:

      virtual void about();

      virtual void print();

      virtual void importGnuplotScript();

      virtual void importTextData();

      virtual void importLicosCurve();

      virtual void importLicosResults();

#ifdef TPLOT_ENABLE_CLI
      virtual void readCLIInput();
#endif /* TPLOT_ENABLE_CLI */
      
    public:

      /*!
       * \param const bool, stand alone mode
       */
      TPlot(const int,
	    const char * const * const);

      bool graphicalOutput() const;

      void treatPendingInputs();

      ~TPlot();

    protected slots:

      virtual void createCurvesMenu();

      virtual void createThemesMenu();

      virtual void showShell();

      virtual void showGraphConfigurationDialog();

      virtual void selectFontDialog();

      virtual void exportToTable();
      
      virtual void exportToPDF();
      
      virtual void exportToSVG();
      
#ifdef TFEL_QT4
      virtual void exportToPostScript();
#endif /* TFEL_QT4 */
      
      virtual void exportToPNG();

      virtual void newWindow();

      virtual void copyToClipboardAsBitmap();

      virtual void copyToClipboardAsSVG();

      virtual void copyToClipboardAsTable();

      virtual void paste();

      virtual void insertImageFromFile();

    protected:

      static bool
      isUnsignedShort(const QString&);

      static unsigned short
      convertToUnsignedShort(const QString&);

      virtual void
      keyPressEvent(QKeyEvent *) override;

      virtual void
      dragEnterEvent(QDragEnterEvent *) override;
      
      virtual void
      dropEvent(QDropEvent *) override;

      void
      initialize();

      struct TFEL_VISIBILITY_LOCAL CurveOptions
      {
	CurveOptions();
	
	QString title;
	bool hasTitle;
	bool hasThemeLineStyle;
	unsigned short themestyle;
	bool hasStyle;
	Curve::Style style;
	bool hasColor;
	QColor color;
	Graph::GraphAxis axis;
      };

      struct TFEL_VISIBILITY_LOCAL Data
	: public CurveOptions
      {
	QString fileName;
	QString xvalues;
	QString yvalues;
      }; // end of struct Data

      struct TFEL_VISIBILITY_LOCAL DataInputBase
      {
	QString fileName;
      };
      
      struct TFEL_VISIBILITY_LOCAL TextDataInput
	: public DataInputBase
      {};
      
      struct TFEL_VISIBILITY_LOCAL LicosCurve
	: public DataInputBase
      {};

      struct TFEL_VISIBILITY_LOCAL MTestCurve
	: public DataInputBase
      {};

      struct TFEL_VISIBILITY_LOCAL AlcyoneCurve
	: public DataInputBase
      {};
      
      struct TFEL_VISIBILITY_LOCAL LicosResults
	: public DataInputBase
      {};

      struct TFEL_VISIBILITY_LOCAL AlcyoneResults
	: public DataInputBase
      {};
      
      struct TFEL_VISIBILITY_LOCAL Function
	: public CurveOptions
      {
	QString f;
      }; // end of struct Function

      struct TFEL_VISIBILITY_LOCAL GnuplotScript
      {
	QString fileName;
      };

      typedef tfel::meta::GenerateTypeList<Data,TextDataInput,
					   LicosCurve,LicosResults,
					   MTestCurve,AlcyoneCurve,
					   Function,GnuplotScript>::type InputTypes;

      struct TFEL_VISIBILITY_LOCAL Input
	: public tfel::utilities::GenTypeBase<InputTypes>
      {
	CurveOptions&
	getCurveOptions();
      }; // end of getCurveOptions

      static const std::vector<std::string>
      tokenize(const std::string&,const char);
	
      void registerArgumentCallBacks();

      void treatUnknownArgument();
#ifdef TPLOT_ENABLE_CLI
      //! treat the `--cli` option
      void treatCLI();
      //! treat the `--input-socket` option
      void treatInputSocket();
      //! treat the `--output-socket` option
      void treatOutputSocket();
#endif /* TPLOT_ENABLE_CLI */

      void treatGnuplotInstruction();

      void treatTheme();

      void treatNoBorder();

      void treatXY();

      void treatX2Y();

      void treatXY2();

      void treatX2Y2();

      void treatOutput();

      void treatColor();

      void treatUsing();

      void treatWithGrid();

      void treatXMin();

      void treatXMax();

      void treatYMin();

      void treatYMax();

      void treatX2Min();

      void treatX2Max();

      void treatY2Min();

      void treatY2Max();

      void treatLineStyle();

      void treatKeyHorizontalPosition();

      void treatKeyVerticalPosition();

      void treatTitle();

      void treatUpperTitle();

      void treatDownTitle();

      void treatLeftTitle();

      void treatRightTitle();

      void treatUpperLabel();

      void treatDownLabel();

      void treatLeftLabel();

      void treatRightLabel();

      void treatCurveStyle(const Curve::Style);

      void treatWithSolidLine();

      void treatWithDotLine();

      void treatWithDashLine();

      void treatWithDashDotLine();

      void treatWithDashDotDotLine();

      void treatWithLinePlus();

      void treatWithDiamonds();

      void treatWithCrosses();

      void treatWithSquares();

      void treatWithTriangles();

      void treatWithDots();

      void treatFontSize();

      void treatFontFamily();

      void setRanges();

      void getStringFromArgs(QString&,
			     const std::string&,
			     const std::string&);

      QString getStringOption(const bool = false);

      double getDoubleOption();

      virtual std::string
      getVersionDescription() const override;
  
      virtual std::string
      getUsageDescription() const override;

      void
      treatDataInput(const Data&);

      void
      treatFunctionInput(const Function&);

      friend class
      tfel::utilities::ArgumentParserBase<TPlot>;

      void createActions();

      void createMainMenu();

      Graph      *g;
      GraphShell *shell;

      QAction *gca;
      QAction *pa;
      QAction *ea;
      QAction *na;
      QAction *fa;
      QAction *etxta;
      QAction *epdfa;
      QAction *esvga;
#ifdef TFEL_QT4
      QAction *epsa;
#endif /* TFEL_QT4 */
      QAction *epnga;

      QAction *xloga;
      QAction *xuloga;
      QAction *x2loga;
      QAction *x2uloga;
      QAction *yloga;
      QAction *yuloga;
      QAction *y2loga;
      QAction *y2uloga;

      QAction *aa;
      QAction *aa2;
      QAction *igsa;
      QAction *itda;
      QAction *ilca;
      QAction *ilra;

      QAction *cba;
      QAction *csvga;
      QAction *ctablea;
      QAction *pastea;

      QAction *iia;

      QAction *showGridAction;
      QAction *showShellAction;

      QMenu *fm;
      QMenu *edm;
      QMenu *em;
      QMenu *gm;
      QMenu *cm;
      QMenu *tm;
      QMenu *dm;
      QMenu *hm;
      QMenu *sm;

#ifdef TPLOT_ENABLE_CLI
      /* data members related to the command-line mode */
      //! the server, used to read data from the command line
      QLocalServer *s   = nullptr;
      //! the input socket
      QLocalSocket *in  = nullptr;
      //! the output socket
      QLocalSocket *out = nullptr;
#endif /* TPLOT_ENABLE_CLI */
      //! list of text data
      std::map<QString,std::shared_ptr<TextDataReader> > dataSources;
      std::vector<Input> inputs;
      QString output;
      QString name;
      QString upperTitle;
      QString downTitle;
      QString rightTitle;
      QString leftTitle;
      QString upperLabel;
      QString downLabel;
      QString rightLabel;
      QString leftLabel;
      QString keyVerticalPosition;
      QString keyHorizontalPosition;
      double xmin;
      double xmax;
      double ymin;
      double ymax;
      double x2min;
      double x2max;
      double y2min;
      double y2max;
      bool hasGrid       = false;
      bool hasXMinValue  = false;
      bool hasYMinValue  = false;
      bool hasXMaxValue  = false;
      bool hasYMaxValue  = false;
      bool hasX2MinValue = false;
      bool hasY2MinValue = false;
      bool hasX2MaxValue = false;
      bool hasY2MaxValue = false;
      bool goutput       = false;
    }; // end of struct TPlots

  } // end of namespace plot

} // end of namespace tfel

#endif /* _LIB_TFEL_PLOT_TPLOT_H */

