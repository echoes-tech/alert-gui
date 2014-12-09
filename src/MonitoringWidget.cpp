/* 
 * File:   MonitoringWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "MonitoringWidget.h"



using namespace std;

class NumericItem : public Wt::WStandardItem {
public:
virtual NumericItem *clone() const {
    return new NumericItem();
}

virtual void setData(const boost::any &data, int role = Wt::UserRole) 
{
    boost::any dt;

    if (role == Wt::EditRole) {
    double d = Wt::asNumber(data);

    if (d != d)
        dt = data;
    else
        dt = boost::any(d);
    }

    Wt::WStandardItem::setData(dt, role);
}

};


MonitoringWidget::MonitoringWidget(Echoes::Dbo::Session *session)
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
    
    Wt::WTable *alertsSentTable = new Wt::WTable(this);
    alertsSentTable->addStyleClass("table");
    alertsSentTable->addStyleClass("table-bordered");
    alertsSentTable->addStyleClass("table-striped");
    alertsSentTable->addStyleClass("table-striped widget-box");

    int row = 0;
    int col = 0;

    alertsSentTable->setHeaderCount(2, Wt::Horizontal);

    alertsSentTable->elementAt(row, col)->setColumnSpan(4);
    alertsSentTable->elementAt(row, col)->setContentAlignment(Wt::AlignTop | Wt::AlignCenter);
    alertsSentTable->elementAt(row, col)->setPadding(5);
    
    new Wt::WText(tr("Alert.summary.alerts-sent"), alertsSentTable->elementAt(row, 0));

    row = 1;
    new Wt::WText(Wt::WString::tr("Alert.summary.alert-date"), alertsSentTable->elementAt(row, col));
    new Wt::WText(Wt::WString::tr("Alert.summary.alert-name"), alertsSentTable->elementAt(row, ++col));
//    new Wt::WText(Wt::WString::tr("Alert.summary.alert-code"), alertsSentTable->elementAt(row, ++col));
    new Wt::WText(Wt::WString::tr("Alert.summary.alert-media"), alertsSentTable->elementAt(row, ++col));
//    new Wt::WText(Wt::WString::tr("Alert.summary.alert-message"), alertsSentTable->elementAt(row, ++col));
    new Wt::WText(Wt::WString::tr("Alert.summary.alert-type"), alertsSentTable->elementAt(row, ++col));
//    new Wt::WText(Wt::WString::tr("Alert.summary.alert-status"), alertsSentTable->elementAt(row, ++col));

    
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        string queryString = "SELECT ale, med, atr, ams "
                "FROM \"T_ALERT_TRACKING_ATR\" atr, \"T_ALERT_ALE\" ale , \"T_MEDIA_MED\" med, \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" ams" 
            " WHERE atr.\"ATR_ALE_ALE_ID\" = ale.\"ALE_ID\" "
            " AND ale.\"ALE_DELETE\" IS NULL "
            " AND atr.\"ATR_SEND_DATE\" IS NOT NULL"
            " AND atr.\"ATR_MED_MED_ID\" = med.\"MED_ID\" "
            " AND med.\"MED_USR_USR_ID\" IN"
            "("
                "SELECT \"USR_ID\" FROM \"T_USER_USR\" WHERE \"USR_ORG_ORG_ID\" = " + boost::lexical_cast<string>(this->session->user()->organization.id()) + ""
            ")"
            " AND ams.\"AMS_ALE_ALE_ID\" = ale.\"ALE_ID\"" 
            " AND ams.\"AMS_MED_MED_ID\" = med.\"MED_ID\"" 
            " ORDER BY atr.\"ATR_SEND_DATE\" DESC"
            " LIMIT 20"
                ;
        Wt::Dbo::Query<boost::tuple<Wt::Dbo::ptr<Echoes::Dbo::Alert>,
                Wt::Dbo::ptr<Echoes::Dbo::Media>,
                Wt::Dbo::ptr<Echoes::Dbo::AlertTracking>,
                Wt::Dbo::ptr<Echoes::Dbo::AlertMediaSpecialization>>,Wt::Dbo::DynamicBinding> q = 
                this->session->query<boost::tuple<Wt::Dbo::ptr<Echoes::Dbo::Alert>,
                                                    Wt::Dbo::ptr<Echoes::Dbo::Media>,
                                                    Wt::Dbo::ptr<Echoes::Dbo::AlertTracking>,
                                                    Wt::Dbo::ptr<Echoes::Dbo::AlertMediaSpecialization>>,
                                                    Wt::Dbo::DynamicBinding>(queryString);
        
        
        Wt::Dbo::collection<boost::tuple<Wt::Dbo::ptr<Echoes::Dbo::Alert>,
                Wt::Dbo::ptr<Echoes::Dbo::Media>,
                Wt::Dbo::ptr<Echoes::Dbo::AlertTracking>,
                Wt::Dbo::ptr<Echoes::Dbo::AlertMediaSpecialization>>> listTuples = q.resultList();
        
        if (listTuples.size() > 0)
        {
            for (Wt::Dbo::collection<boost::tuple<Wt::Dbo::ptr<Echoes::Dbo::Alert>,
                Wt::Dbo::ptr<Echoes::Dbo::Media>,
                Wt::Dbo::ptr<Echoes::Dbo::AlertTracking>,
                Wt::Dbo::ptr<Echoes::Dbo::AlertMediaSpecialization>>>::const_iterator i = listTuples.begin(); i != listTuples.end(); ++i)
            {
                row++;

                int colNum = 0;
                
                
                new Wt::WText(i->get<2>()->sendDate.toString(), alertsSentTable->elementAt(row, colNum));
                alertsSentTable->elementAt(row, colNum)->setContentAlignment(Wt::AlignCenter);
                
                new Wt::WText(i->get<0>()->name, alertsSentTable->elementAt(row, ++colNum));
                
//                if (Wt::WString::tr(i->get<0>()->name.toUTF8()).toUTF8().find_first_of("?") == 0)
//                {
//                    new Wt::WText(("N/A"), alertsSentTable->elementAt(row, ++colNum));
//                }
//                else
//                {
//                    new Wt::WText(Wt::WString::tr(i->get<0>()->name.toUTF8()), alertsSentTable->elementAt(row, ++colNum));
//                }
                

                new Wt::WText(i->get<1>()->value, alertsSentTable->elementAt(row, ++colNum));
                alertsSentTable->elementAt(row, colNum)->setContentAlignment(Wt::AlignCenter);
                
//                new Wt::WText(i->get<3>()->message, alertsSentTable->elementAt(row, ++colNum));
                
                new Wt::WText(i->get<1>()->mediaType->name, alertsSentTable->elementAt(row, ++colNum));
                
//                if (i->get<2>()->alertTrackingEvents.size() > 0)
//                {
//                    new Wt::WText(i->get<2>()->alertTrackingEvents.front()->value, alertsSentTable->elementAt(row, ++colNum));
//                }
//                else
//                {
//                    new Wt::WText("N/A", alertsSentTable->elementAt(row, ++colNum));
//                }
            }
        }
    }  
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    
        
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
