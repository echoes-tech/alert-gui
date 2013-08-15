/* 
 * File:   OptionManagementWidget.cpp
 * Author: tsa
 * 
 * Created on 11 novembre 2012, 12:09
 */

#include "OptionManagementWidget.h"

OptionManagementWidget::OptionManagementWidget(OptionManagementModel *model, Session *session) 
: Wt::WContainerWidget()
{
    model_ = model;
    created_ = false;
    this->session = session;
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("options",false);      
    createUI();
}

OptionManagementWidget::~OptionManagementWidget() 
{
}

void OptionManagementWidget::setApiUrl(std::string apiUrl)
{
    this->apiUrl = apiUrl;
}

std::string OptionManagementWidget::getApiUrl() const
{
    return apiUrl;
}

void OptionManagementWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
//        update();
        created_ = true;
    }

    Wt::WContainerWidget::render(flags);
}

void OptionManagementWidget::createUI()
{
    // Top template
    mainForm = new Wt::WTemplateFormView(Wt::WString::tr("Alert.Option.Management.template"));    
    
    // widgets of the template
    mainForm->bindWidget("sms-quota", createFormWidget(OptionManagementModel::smsQuota));
    mainForm->bindWidget("sms-ask", createFormWidget(OptionManagementModel::smsAsk));
    mainForm->bindWidget("user-role-button", createFormWidget(OptionManagementModel::userRoleButton));
    mainForm->bindWidget("user-role-combo", createFormWidget(OptionManagementModel::userRoleCombo));

    mainForm->updateModel(model_);
    mainForm->refresh();

    Wt::WVBoxLayout *mainVerticalLayout = new Wt::WVBoxLayout();
    Wt::WHBoxLayout *topHorizontalLayout = new Wt::WHBoxLayout();
    topHorizontalLayout->addWidget(mainForm);
    
    mainVerticalLayout->addLayout(topHorizontalLayout);
    this->setLayout(mainVerticalLayout);
    
    try
    {
        Wt::log("info") << "Debug : before transaction";
        Wt::Dbo::Transaction transaction(*this->session);
        Wt::log("info") << "Debug : user found";
        int smsQuotaValue = 0;
         
        Wt::Dbo::ptr<PackOption> ptrPackOption = session->find<PackOption>()
                .where("\"POP_PCK_PCK_ID\" = ?").bind(this->session->user()->currentOrganization.get()->pack.id())
                .where("\"POP_OPT_OPT_ID\" = ?").bind(Enums::quotaSms)
                .limit(1);
        if (ptrPackOption.get())
        {
            Wt::Dbo::ptr<OptionValue> ptrOptionValue = session->find<OptionValue>().where("\"OPT_ID_OPT_ID\" = ?").bind(ptrPackOption.get()->pk.option.id())
                                                            .where("\"ORG_ID_ORG_ID\" = ?").bind(this->session->user()->currentOrganization.id())
                                                            .limit(1);
            if (ptrOptionValue.get())
            {
                smsQuotaValue = boost::lexical_cast<int>(ptrOptionValue.get()->value);
            }
        }
            
        transaction.commit();
        model_->setValue(model_->smsQuota,boost::any(smsQuotaValue));
        model_->setReadOnly(model_->smsQuota,true);
        mainForm->updateView(model_);
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::WMessageBox::show(tr("Alert.global.database-error-title"),tr("Alert.global.database-error").arg(e.what()).arg("1"),Wt::Ok);
        Wt::log("error") << "[AssetManagementWidget] " << e.what();
    }
}

bool OptionManagementWidget::validate()
{
    return model_->validate();
}

Wt::WFormWidget *OptionManagementWidget::createFormWidget(Wt::WFormModel::Field field)
{
    Wt::WFormWidget *result = 0;

    if (field == OptionManagementModel::smsQuota)
    {
        Wt::WLineEdit *line = new Wt::WLineEdit();
        line->setReadOnly(true);
        line->setSelectable(false);
        line->setEnabled(false);
        result = line;
    }
    else if (field == OptionManagementModel::smsAsk)
    {
        Wt::WPushButton *button = new Wt::WPushButton();
        button->setAttributeValue("class","btn btn-info");
        button->setTextFormat(Wt::XHTMLUnsafeText);
        button->setText("<i class='glyphicon glyphicon-shopping-cart icon-white'></i> " + tr("Alert.option.ask-sms-button"));
        button->clicked().connect(boost::bind(&OptionManagementWidget::askSms, this));
        result = button;
    }
    else if (field == OptionManagementModel::userRoleButton)
    {
        Wt::WPushButton *button = new Wt::WPushButton();
        button->setAttributeValue("class","btn btn-info");
        button->setTextFormat(Wt::XHTMLUnsafeText);
        button->setText("<i class='glyphicon glyphicon-th-large icon-white'></i> " + tr("Alert.option.user-role-button"));
        button->clicked().connect(boost::bind(&OptionManagementWidget::changeRole, this));
        result = button;
    }
    else if (field == OptionManagementModel::userRoleCombo)
    {
        Wt::WComboBox *combo = new Wt::WComboBox();
        combo->addItem("Role");
        result = combo;
    }

    return result;
}

void OptionManagementWidget::askSms()
{
    Wt::Mail::Message mailMessage;
    Wt::WString mail;
    const Wt::WString constMailRecipient = "contact@echoes-tech.com";
    const Wt::WString constMailRecipientName = "Contact";
    mail =  Wt::Utils::htmlEncode(Wt::WString::tr("Alert.option.ask-sms-quota-body").toUTF8(), Wt::Utils::EncodeNewLines)
            + "</br> " + session->user().get()->firstName + " " + session->user().get()->lastName
            + " : " + session->user().get()->eMail;
    mailMessage.addRecipient(Wt::Mail::To, Wt::Mail::Mailbox(constMailRecipient.toUTF8(), constMailRecipientName.toUTF8()));
    mailMessage.setSubject(Wt::WString::tr("Alert.option.ask-sms-quota-subject"));
    mailMessage.addHtmlBody(mail);
    session->auth().sendMail(mailMessage);
    this->refresh();
    Wt::WMessageBox::show(tr("Alert.option.ask-sms-title"),tr("Alert.option.ask-sms"),Wt::Ok);
}

void OptionManagementWidget::changeRole()
{
    
}

void OptionManagementWidget::fillRoleSelector()
{
    std::string urlToCall = this->getApiUrl() + "/roles";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&OptionManagementWidget::getRoles, this, _1, _2));

    std::string apiAddress = urlToCall;
//    + this->getCredentials();

    std::cout << "address to call : " << apiAddress << std::endl;
    
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    } 
}

void OptionManagementWidget::getRoles(boost::system::error_code err, const Wt::Http::Message& response)
{
//    Wt::WApplication::instance()->resumeRendering();
//    int idx = 0;
//    role->clear();
//    
//    if (response.status() >= 200 && response.status() < 400)
//    {
//        Wt::Json::Value result ;
//        Wt::Json::Array& result1 = Wt::Json::Array::Empty;
//        try
//        {                  
//            Wt::Json::parse(response.body(), result);
//            result1 = result;
//              //descriptif
//            for (Wt::Json::Array::const_iterator idx1 = result1.begin() ; idx1 < result1.end(); idx1++)
//            {
//                Wt::Json::Object tmp = (*idx1);
//                pluginsComboBox->addItem(tmp.get("name"));
//                mapIdPluginsComboBox[idx] = tmp.get("id");
//                idx++;
//            }
//        }
//        catch (Wt::Json::ParseError const& e)
//        {
//            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON:" << response.body();
//        }
//
//        catch (Wt::Json::TypeException const& e)
//        {
//            Wt::log("warning") << "[handleHttpResponse] Problems parsing JSON.:" << response.body();
//        }          
//    }
//    else
//    {
//         Wt::log("warning") << "fct handleHttpResponse" << response.body();
//    }
}

