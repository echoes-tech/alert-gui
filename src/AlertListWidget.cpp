/* 
 * File:   AlertListWidget.cpp
 * Author: tsa
 * 
 * Created on 25 octobre 2012, 17:43
 */

#include "AlertListWidget.h"

AlertListWidget::AlertListWidget(Session *session)
: Wt::WContainerWidget()
{
    this->session = session;

    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("alert-list", false);
    app->messageResourceBundle().use("alert", false);
    createUI();

}

AlertListWidget::AlertListWidget(const AlertListWidget& orig)
{
}

AlertListWidget::~AlertListWidget()
{
}

void AlertListWidget::createUI()
{
    this->clear();
    
    Wt::WText *helpText = new Wt::WText();
    helpText->setTextFormat(Wt::XHTMLUnsafeText);
    helpText->setText("<br /><div class ='alert alert-info'>" + tr("Alert.alert.alert-form-info") + "</div>");
    
    
    
    Wt::WTable *alertsTable = new Wt::WTable(this);
    alertsTable->addStyleClass("table");
    alertsTable->addStyleClass("table-bordered");
    alertsTable->addStyleClass("table-striped");

    Wt::WText *noAlertText = new Wt::WText(Wt::WString::tr("Alert.alert-list.no-alert"));

    int row = 0;
    int col = 0;

    alertsTable->setHeaderCount(1, Wt::Horizontal);

    alertsTable->elementAt(row, col)->setColumnSpan(8);
    alertsTable->elementAt(row, col)->setContentAlignment(Wt::AlignTop | Wt::AlignCenter);
    alertsTable->elementAt(row, col)->setPadding(5);

    new Wt::WText(tr("Alert.alert-list.alert-form"), alertsTable->elementAt(row, 0));

    row = 1;
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-name"), alertsTable->elementAt(row, col));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.criteria-name"), alertsTable->elementAt(row, ++col));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-value"), alertsTable->elementAt(row, ++col));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-unit"), alertsTable->elementAt(row, ++col));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-key-value"), alertsTable->elementAt(row, ++col));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-media"), alertsTable->elementAt(row, ++col));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-snooze"), alertsTable->elementAt(row, ++col));
    new Wt::WText(Wt::WString::tr("Alert.alert-list.alert-actions"), alertsTable->elementAt(row, ++col));


    try
    {
        Wt::Dbo::Transaction transaction(*this->session);
        //TODO : don't understand why the two lines below are needed, clean this
        Wt::Dbo::ptr<User> tempUser = session->find<User > ().where("\"USR_ID\" = ?").bind(session->user().id());
        if (tempUser)
        {
            Wt::Dbo::ptr<Organization> tempOrga = tempUser->currentOrganization;
            std::string queryString = "SELECT ale, acr, ava, mev, ams, inu FROM \"T_ALERT_ALE\" ale,"
                    " \"T_ALERT_VALUE_AVA\" ava,"
                    " \"T_ALERT_CRITERIA_ACR\" acr,"
                    " \"T_MEDIA_VALUE_MEV\" mev,"
                    " \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" ams, "
                    " \"T_INFORMATION_UNIT_UNT\" inu "
                    " WHERE \"ALE_ID\" IN "
                    "("
                    "SELECT \"AMS_ALE_ALE_ID\" FROM \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" WHERE \"AMS_MEV_MEV_ID\" IN "
                    "("
                    "SELECT \"MEV_ID\" FROM \"T_MEDIA_VALUE_MEV\" WHERE \"MEV_USR_USR_ID\" IN "
                    "("
                    "SELECT \"T_USER_USR_USR_ID\" FROM \"TJ_USR_ORG\" WHERE \"T_ORGANIZATION_ORG_ORG_ID\" = "
                    + boost::lexical_cast<std::string > (this->session->user().get()->currentOrganization.id())
                    + ""
                    ")"
                    " )"
                    " AND \"AMS_ALE_ALE_ID\" IS NOT NULL"
                    ") "
                    " AND ale.\"ALE_AVA_AVA_ID\" = ava.\"AVA_ID\" "
                    " AND ale.\"ALE_DELETE\" IS NULL "
                    " AND ava.\"AVA_ACR_ACR_ID\" = acr.\"ACR_ID\" "
                    " AND ams.\"AMS_ALE_ALE_ID\" = ale.\"ALE_ID\" "
                    " AND mev.\"MEV_ID\" = ams.\"AMS_MEV_MEV_ID\" "
                    " AND inu.\"INU_ID\" = ava.\"INU_ID_INU_ID\" ";

            Wt::Dbo::Query
                    <
                    boost::tuple
                    <
                    Wt::Dbo::ptr<Alert>,
                    Wt::Dbo::ptr<AlertCriteria>,
                    Wt::Dbo::ptr<AlertValue>,
                    Wt::Dbo::ptr<MediaValue>,
                    Wt::Dbo::ptr<AlertMediaSpecialization>,
                    Wt::Dbo::ptr<InformationUnit>
                    >
                    , Wt::Dbo::DynamicBinding
                    > resQuery = this->session->query
                    <
                    boost::tuple
                    <
                    Wt::Dbo::ptr<Alert>,
                    Wt::Dbo::ptr<AlertCriteria>,
                    Wt::Dbo::ptr<AlertValue>,
                    Wt::Dbo::ptr<MediaValue>,
                    Wt::Dbo::ptr<AlertMediaSpecialization>,
                    Wt::Dbo::ptr<InformationUnit>
                    >, Wt::Dbo::DynamicBinding
                    > (queryString);


            Wt::Dbo::collection<boost::tuple<
                    Wt::Dbo::ptr<Alert>,
                    Wt::Dbo::ptr<AlertCriteria>,
                    Wt::Dbo::ptr<AlertValue>,
                    Wt::Dbo::ptr<MediaValue>,
                    Wt::Dbo::ptr<AlertMediaSpecialization>,
                    Wt::Dbo::ptr<InformationUnit> > > listTuples = resQuery.resultList();
            if (listTuples.size() > 0)
            {
                noAlertText->hide();
                for (Wt::Dbo::collection<boost::tuple<
                        Wt::Dbo::ptr<Alert>,
                        Wt::Dbo::ptr<AlertCriteria>,
                        Wt::Dbo::ptr<AlertValue>,
                        Wt::Dbo::ptr<MediaValue>,
                        Wt::Dbo::ptr<AlertMediaSpecialization>,
                        Wt::Dbo::ptr<InformationUnit> > >::const_iterator i = listTuples.begin(); i != listTuples.end(); ++i)
                {
                    row++;
                    //                    alertsTable->elementAt(row, 1)->setPadding(5, Wt::Left | Wt::Right);
                    //                    alertsTable->elementAt(row, 2)->setPadding(5, Wt::Left | Wt::Right);
                    //                alertsTable->elementAt(row, 3)->setPadding(10, Wt::Right);
                    //                alertsTable->elementAt(row, 4)->setPadding(10);

                    int colNum = 0;

                    new Wt::WText(i->get < 0 > ().get()->name, alertsTable->elementAt(row, colNum));


                    Wt::WString tempOperator = "Alert.alert.operator." + i->get < 1 > ().get()->name;
                    new Wt::WText(tr(tempOperator.toUTF8().c_str()), alertsTable->elementAt(row, ++colNum));
                    alertsTable->elementAt(row, colNum)->setContentAlignment(Wt::AlignCenter);

                    new Wt::WText(i->get < 2 > ().get()->value, alertsTable->elementAt(row, ++colNum));
                    alertsTable->elementAt(row, colNum)->setContentAlignment(Wt::AlignCenter);

                    Wt::WString tempUnitName = "Alert.alert.unit." + i->get < 5 > ().get()->name;
                    new Wt::WText(tr(tempUnitName.toUTF8().c_str()), alertsTable->elementAt(row, ++colNum));
                    alertsTable->elementAt(row, colNum)->setContentAlignment(Wt::AlignCenter);

                    new Wt::WText(i->get < 2 > ().get()->keyValue.get(), alertsTable->elementAt(row, ++colNum));
                    alertsTable->elementAt(row, colNum)->setContentAlignment(Wt::AlignCenter);

                    new Wt::WText(i->get < 3 > ().get()->value, alertsTable->elementAt(row, ++colNum));
                    alertsTable->elementAt(row, colNum)->setContentAlignment(Wt::AlignCenter);

                    new Wt::WText(boost::lexical_cast<std::string > (i->get < 4 > ().get()->snoozeDuration), alertsTable->elementAt(row, ++colNum));
                    alertsTable->elementAt(row, colNum)->setContentAlignment(Wt::AlignCenter);

                    Wt::WPushButton *delButton = new Wt::WPushButton(alertsTable->elementAt(row, ++colNum));
                    delButton->setAttributeValue("class","btn btn-danger");
                    delButton->setTextFormat(Wt::XHTMLUnsafeText);
                    delButton->setText("<i class='icon-remove icon-white'></i> " + tr("Alert.alert-list.delete-alert"));
                    delButton->clicked().connect(boost::bind(&AlertListWidget::deleteAlert, this, (i->get < 0 > ().id())));

                }
            }
            else
            {
                noAlertText->show();
                alertsTable->hide();
            }
        }
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error").arg(e.what()).arg("1"), Wt::Ok);
        Wt::log("error") << "[AlertListWidget] [createUI] " << e.what();
    }

    Wt::WVBoxLayout *mainVerticalLayout = new Wt::WVBoxLayout();
    //    Wt::WHBoxLayout *topHorizontalLayout = new Wt::WHBoxLayout();
    Wt::WHBoxLayout *bottomHorizontalLayout = new Wt::WHBoxLayout();


    //    topHorizontalLayout->addWidget(mainForm);
    
    bottomHorizontalLayout->addWidget(noAlertText);
    bottomHorizontalLayout->addWidget(alertsTable);

    // empty container to reduce table width which is linked to the container
    Wt::WContainerWidget *emptyContainer = new Wt::WContainerWidget();
    bottomHorizontalLayout->addWidget(emptyContainer);

    //    mainVerticalLayout->addLayout(topHorizontalLayout);
    mainVerticalLayout->addWidget(helpText);
    mainVerticalLayout->addLayout(bottomHorizontalLayout);

    this->setLayout(mainVerticalLayout);
}

void AlertListWidget::deleteAlert(long long id)
{
    /**
     ** Created Window for confirm delete this alert. 
     **/
    Wt::WMessageBox *result = new Wt::WMessageBox("Attention",
            tr("Alert.alert-list.delete-alert-message"), Wt::Warning,
            Wt::Yes | Wt::No);
    // Edit Buttons
    Wt::WPushButton *yes_ = result->button(Wt::Yes);
    Wt::WPushButton *no_ = result->button(Wt::No);
    // Init Buttons
    yes_->clicked().connect(result, &Wt::WMessageBox::accept);
    no_->clicked().connect(result, &Wt::WMessageBox::reject);
    
    if (result->exec() == Wt::WMessageBox::Accepted)
        try
        {
            Wt::Dbo::Transaction transaction(*session);
            std::string queryStr = "SELECT ale FROM \"T_ALERT_ALE\" ale WHERE \"ALE_ID\" = ? FOR UPDATE";
            Wt::Dbo::ptr<Alert> ptrAlert = session->query<Wt::Dbo::ptr<Alert > >(queryStr).bind(id).limit(1);
        
            ptrAlert.modify()->deleteTag = Wt::WDateTime::currentDateTime();
            transaction.commit();
            UserActionManagement::registerUserAction(Enums::del, Constants::T_ALERT_ALE, ptrAlert.id());
        }
        catch (Wt::Dbo::Exception e)
        {
            Wt::log("error") << "[AlertListWidget] [deleteAlert] " << e.what();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
        }
    // For clear memory.
   delete result;
   createUI();
}

void AlertListWidget::refresh()
{
    createUI();
}