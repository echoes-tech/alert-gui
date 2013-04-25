/* 
 * File:   MonitoringWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "MonitoringWidget.h"

MonitoringWidget::MonitoringWidget(Session *session)
: Wt::WContainerWidget()
{
    created_ = false;
    this->session = session;
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("monitoring",false);      
    createUI();
}

void MonitoringWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
//        update();
        created_ = true;
    }

    Wt::WContainerWidget::render(flags);
}

void MonitoringWidget::createUI()
{
//    this->mainStack->addWidget(this);
    
//    Wt::Dbo::QueryModel<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertTracking> > > *qm = new Wt::Dbo::QueryModel<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertTracking> > >();
    
    
//    Wt::WTableView *tview = new Wt::WTableView(this);
//    try
//    {
//        Wt::Dbo::Transaction transaction(*(this->session));
//        //TODO : don't understand why the two lines below are needed, clean this
//        Wt::Dbo::ptr<User> tempUser = this->session->find<User>().where("\"USR_ID\" = ?").bind(this->session->user().id());
//        Wt::Dbo::ptr<Organization> tempOrga = tempUser->currentOrganization;
//        std::string queryString = "SELECT ale, mev, atr FROM \"T_ALERT_TRACKING_ATR\" atr, \"T_ALERT_ALE\" ale , \"T_MEDIA_VALUE_MEV\" mev "
//            " WHERE atr.\"ATR_ALE_ALE_ID\" = ale.\"ALE_ID\" "
//            " AND ale.\"ALE_DELETE\" IS NULL "
//            " AND atr.\"ATR_MEV_MEV_ID\" = mev.\"MEV_ID\" "
//            " AND mev.\"MEV_USR_USR_ID\" IN"
//            "("
//                "SELECT \"T_USER_USR_USR_ID\" FROM \"TJ_USR_ORG\" WHERE \"T_ORGANIZATION_ORG_ORG_ID\" = " + boost::lexical_cast<std::string>(this->session->user().get()->currentOrganization.id()) + ""
//            ")";
//        Wt::Dbo::Query<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertTracking> >,Wt::Dbo::DynamicBinding> q = this->session->query<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertTracking> >,Wt::Dbo::DynamicBinding>(queryString);
//        qm->setQuery(q, false);
//        qm->addColumn("ATR_SEND_DATE", "Date", Wt::ItemIsSelectable);
//        qm->addColumn("ALE_NAME", "Name", Wt::ItemIsSelectable);
//        qm->addColumn("MEV_VALUE", "Value", Wt::ItemIsSelectable);
//        
//        tview->setModel(qm);
//    }  
//    catch (Wt::Dbo::Exception e)
//    {
//        Wt::log("error") << e.what();
//    }
    
    
//    new ScatterPlot(this);
}




void MonitoringWidget::close()
{
    delete this;
}




ScatterPlot::ScatterPlot(Wt::WContainerWidget *parent):
  Wt::WContainerWidget(parent)
{
  new Wt::WText(Wt::WString::tr("scatter plot 2"), this);

  Wt::WStandardItemModel *model = new Wt::WStandardItemModel(40, 2, this);
  model->setHeaderData(0, Wt::WString("X"));
  model->setHeaderData(1, Wt::WString("Y = sin(X)"));

  for (unsigned i = 0; i < 40; ++i) {
    double x = (static_cast<double>(i) - 20) / 4;

    model->setData(i, 0, x);
    model->setData(i, 1, x*x);
  }
 
  /*
   * Create the scatter plot.
   */
  Wt::Chart::WCartesianChart *chart = new Wt::Chart::WCartesianChart(this);
  chart->setModel(model);        // set the model
  chart->setXSeriesColumn(0);    // set the column that holds the X data
  chart->setLegendEnabled(true); // enable the legend

  chart->setType(Wt::Chart::ScatterPlot);   // set type to ScatterPlot

  // Typically, for mathematical functions, you want the axes to cross
  // at the 0 mark:
  chart->axis(Wt::Chart::XAxis).setLocation(Wt::Chart::ZeroValue);
  chart->axis(Wt::Chart::YAxis).setLocation(Wt::Chart::ZeroValue);

  // Provide space for the X and Y axis and title. 
  chart->setPlotAreaPadding(80, Wt::Left);
  chart->setPlotAreaPadding(40, Wt::Top | Wt::Bottom);

  // Add the curves
  Wt::Chart::WDataSeries s(1, Wt::Chart::CurveSeries);
  s.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
  chart->addSeries(s);

  chart->resize(800, 300); // WPaintedWidget must be given explicit size

  chart->setMargin(10, Wt::Top | Wt::Bottom);            // add margin vertically
  chart->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right); // center horizontally

//  Wt::Chart::ChartConfig *config = new Wt::Chart::ChartConfig(chart, this);
//  config->setValueFill(ZeroValueFill);
}
