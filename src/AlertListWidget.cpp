/* 
 * File:   AlertListWidget.cpp
 * Author: tsa
 * 
 * Created on 25 octobre 2012, 17:43
 */

#include "AlertListWidget.h"

AlertListWidget::AlertListWidget(Session *session)
: Wt::WGroupBox()
{
    this->session = session;
    
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("alert-list",false);      
    createUI();

}

AlertListWidget::AlertListWidget(const AlertListWidget& orig) {
}

AlertListWidget::~AlertListWidget() {
}

void AlertListWidget::createUI()
{
    this->clear();
    this->setTitle(tr("Alert.alert-list.title"));

    
    Wt::WTable *alertsTable = new Wt::WTable(this);
    alertsTable->setStyleClass("alert-table");
    
    int row = 0;
    
    alertsTable->setHeaderCount(2,Wt::Horizontal);
    
    alertsTable->elementAt(row, 1)->setColumnSpan(6);
    alertsTable->elementAt(row, 1)->setContentAlignment(Wt::AlignTop | Wt::AlignCenter);
    alertsTable->elementAt(row, 1)->setPadding(5);
    
    Wt::WText *title = new Wt::WText(tr("Alert.alert-list.alert-form"),alertsTable->elementAt(row, 1));
    title->decorationStyle().font().setSize(Wt::WFont::XLarge);
    
    row = 1;
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-name"), alertsTable->elementAt(row, 1));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.criteria-name"), alertsTable->elementAt(row, 2));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-value"), alertsTable->elementAt(row, 3));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-media"), alertsTable->elementAt(row, 4));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-snooze"), alertsTable->elementAt(row, 5));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-actions"), alertsTable->elementAt(row, 6));
  
    
    try
    {
        Wt::log("info") << "Debug : before transaction boost tuple";
        Wt::Dbo::Transaction transaction(*this->session);
        //TODO : don't understand why the two lines below are needed, clean this
        Wt::Dbo::ptr<User> tempUser = session->find<User>().where("\"USR_ID\" = ?").bind(session->user().id());
        if (tempUser)
        {
            Wt::Dbo::ptr<Organization> tempOrga = tempUser->currentOrganization;
            std::string queryString = "SELECT ale, acr, ava, mev, ams FROM \"T_ALERT_ALE\" ale, \"T_ALERT_VALUE_AVA\" ava, \"T_ALERT_CRITERIA_ACR\" acr, \"T_MEDIA_VALUE_MEV\" mev, \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" ams WHERE \"ALE_ID\" IN "
            "("
                "SELECT \"AMS_ALE_ALE_ID\" FROM \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" WHERE \"AMS_MEV_MEV_ID\" IN "
                "("
                    "SELECT \"MEV_ID\" FROM \"T_MEDIA_VALUE_MEV\" WHERE \"MEV_USR_USR_ID\" IN "
                    "("
                        "SELECT \"T_USER_USR_USR_ID\" FROM \"TJ_USR_ORG\" WHERE \"T_ORGANIZATION_ORG_ORG_ID\" = " + boost::lexical_cast<std::string>(this->session->user().get()->currentOrganization.id()) + ""
                    ")"
               " )"
               " AND \"AMS_ALE_ALE_ID\" IS NOT NULL"
            ") "
            " AND ale.\"ALE_AVA_AVA_ID\" = ava.\"AVA_ID\" "
            " AND ale.\"ALE_DELETE\" IS NULL "
            " AND ava.\"AVA_ACR_ACR_ID\" = acr.\"ACR_ID\" "
            " AND ams.\"AMS_ALE_ALE_ID\" = ale.\"ALE_ID\" "
            " AND mev.\"MEV_ID\" = ams.\"AMS_MEV_MEV_ID\" ";
 
            Wt::log("info") << "Debug : " << queryString ;
            Wt::Dbo::Query
                <
                    boost::tuple
                    <
                        Wt::Dbo::ptr<Alert>,
                        Wt::Dbo::ptr<AlertCriteria>,
                        Wt::Dbo::ptr<AlertValue>,
                        Wt::Dbo::ptr<MediaValue>,
                        Wt::Dbo::ptr<AlertMediaSpecialization>
                    > 
                    ,Wt::Dbo::DynamicBinding
                > resQuery = this->session->query
                <
                    boost::tuple
                    <
                        Wt::Dbo::ptr<Alert>, 
                        Wt::Dbo::ptr<AlertCriteria>, 
                        Wt::Dbo::ptr<AlertValue>,
                        Wt::Dbo::ptr<MediaValue>,
                        Wt::Dbo::ptr<AlertMediaSpecialization>
                    >,Wt::Dbo::DynamicBinding
                >(queryString);
     

            Wt::Dbo::collection<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<AlertCriteria>,Wt::Dbo::ptr<AlertValue>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertMediaSpecialization> > > listTuples = resQuery.resultList();
            if (listTuples.size() > 0)
            {
            for (Wt::Dbo::collection<boost::tuple<Wt::Dbo::ptr<Alert>,Wt::Dbo::ptr<AlertCriteria>,Wt::Dbo::ptr<AlertValue>,Wt::Dbo::ptr<MediaValue>,Wt::Dbo::ptr<AlertMediaSpecialization> > >::const_iterator i = listTuples.begin(); i != listTuples.end(); ++i) 
                {
                    row++;
                    alertsTable->elementAt(row, 1)->setPadding(5, Wt::Left | Wt::Right);
                    alertsTable->elementAt(row, 2)->setPadding(5, Wt::Left | Wt::Right);
    //                alertsTable->elementAt(row, 3)->setPadding(10, Wt::Right);
    //                alertsTable->elementAt(row, 4)->setPadding(10);

                    new Wt::WText(i->get<0>().get()->name, alertsTable->elementAt(row, 1));


                    Wt::WString tempOperator = "Alert.alert.operator." + i->get<1>().get()->name;
                    new Wt::WText(tr(tempOperator.toUTF8().c_str()), alertsTable->elementAt(row, 2));
                    alertsTable->elementAt(row, 2)->setContentAlignment(Wt::AlignCenter);

                    new Wt::WText(i->get<2>().get()->value, alertsTable->elementAt(row, 3));
                    alertsTable->elementAt(row, 3)->setContentAlignment(Wt::AlignCenter);

                    new Wt::WText(i->get<3>().get()->value, alertsTable->elementAt(row, 4));
                    alertsTable->elementAt(row, 4)->setContentAlignment(Wt::AlignCenter);

                    new Wt::WText(boost::lexical_cast<std::string>(i->get<4>().get()->snoozeDuration), alertsTable->elementAt(row, 5));
                    alertsTable->elementAt(row, 5)->setContentAlignment(Wt::AlignCenter);

                    Wt::WPushButton *delButton = new Wt::WPushButton(tr("Alert.alert-list.delete-alert"), alertsTable->elementAt(row, 6));
                    delButton->clicked().connect(boost::bind(&AlertListWidget::deleteAlert,this,(i->get<0>().id())));


                }
            }
            else
            {
                alertsTable->hide();
            }
        }
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error").arg(e.what()).arg("1"),Wt::Ok);
        Wt::log("error") << "[AlertListWidget] [createUI] " << e.what();
    }
}

void AlertListWidget::deleteAlert(long long id)
{
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::ptr<Alert>  ptrAlert = session->find<Alert>().where("\"ALE_ID\" = ?").bind(id);
        //FIXME: doesn't work, bug posted in Wt forge #1479
        //ptrAsset.modify()->alerts.clear();
        std::string executeString = "DELETE FROM \"TJ_AST_ALE\" " 
                                    " WHERE \"TJ_AST_ALE\".\"T_ALERT_ALE_ALE_ID\" = " + boost::lexical_cast<std::string>(id);
        session->execute(executeString);
        
        std::string executeString2 = "DELETE FROM \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" " 
                                    " WHERE \"AMS_ALE_ALE_ID\" = " + boost::lexical_cast<std::string>(id);
        session->execute(executeString2);   
        
        
        ptrAlert.modify()->deleteTag = Wt::WDateTime::currentDateTime();
        transaction.commit();
        UserActionManagement::registerUserAction(Enums::del,Constants::T_ALERT_ALE,ptrAlert.id());
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertListWidget] [deleteAlert] " << e.what();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
    }
    createUI();
}

void AlertListWidget::refresh()
{
    createUI();
}
