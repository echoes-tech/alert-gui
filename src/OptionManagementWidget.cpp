/* 
 * File:   OptionManagementWidget.cpp
 * Author: tsa
 * 
 * Created on 11 novembre 2012, 12:09
 */

#include "OptionManagementWidget.h"

using namespace std;

OptionManagementWidget::OptionManagementWidget(OptionManagementModel *model, Echoes::Dbo::Session *session, string apiUrl) 
: Wt::WContainerWidget()
{
    setApiUrl(apiUrl);
    model_ = model;
    created_ = false;
    this->session = session;
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "options",false);
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "auth",false);
    createUI();
}

OptionManagementWidget::~OptionManagementWidget() 
{
}

void OptionManagementWidget::setApiUrl(string apiUrl)
{
    this->apiUrl_ = apiUrl;
}

string OptionManagementWidget::getApiUrl() const
{
    return apiUrl_;
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
    Wt::WTemplateFormView *changePasswordWidget = (Wt::WTemplateFormView*)((EchoesHome*)((Wt::WApplication::instance()->root()->widget(0))))->displayPasswordChangeWidget();
    changePasswordWidget->resolveWidget("cancel-button")->hide();
    changePasswordWidget->destroyed().connect(boost::bind(&OptionManagementWidget::replaceChangePasswordWidget, this));
    // Top template
    mainForm = new Wt::WTemplateFormView(Wt::WString::tr("Alert.Option.Management.template"));    
    
    // widgets of the template
    mainForm->bindWidget("sms-quota", createFormWidget(OptionManagementModel::smsQuota));
    mainForm->bindWidget("sms-ask", createFormWidget(OptionManagementModel::smsAsk));
    mainForm->bindWidget("user-role-button", createFormWidget(OptionManagementModel::userRoleButton));
    mainForm->bindWidget("user-role-combo", createFormWidget(OptionManagementModel::userRoleCombo));
    mainForm->bindWidget("update-password", changePasswordWidget);

    mainForm->updateModel(model_);
    mainForm->refresh();

    Wt::WVBoxLayout *mainVerticalLayout = new Wt::WVBoxLayout();
    Wt::WHBoxLayout *topHorizontalLayout = new Wt::WHBoxLayout();
    topHorizontalLayout->addWidget(mainForm);
    
    mainVerticalLayout->addLayout(topHorizontalLayout);
    this->setLayout(mainVerticalLayout);
    
    // Je n'ai pas pu essayer pas encore en place
    string apiAddress = this->getApiUrl() + "/options/"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8()) 
            + "&token=" + Wt::Utils::urlEncode(session->user()->token.toUTF8())
            + "&type=" + boost::lexical_cast<string>(Echoes::Dbo::EOptionType::QUOTA_SMS);
    Wt::Http::Client *client1 = new Wt::Http::Client(this);
    client1->done().connect(boost::bind(&OptionManagementWidget::getQuota, this, _1, _2));

    Wt::log("debug") << "OptionManagementWidget : [GET] address to call : " << apiAddress;
    if (client1->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void OptionManagementWidget::replaceChangePasswordWidget()
{
    mainForm->bindWidget("update-password", new Wt::WText(tr("Alert.option.password-changed")));
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
        button->setText("<i class='icon-shopping-cart icon-white'></i> " + tr("Alert.option.ask-sms-button"));
        button->clicked().connect(boost::bind(&OptionManagementWidget::askSms, this));
        result = button;
    }
    else if (field == OptionManagementModel::userRoleButton)
    {
        Wt::WPushButton *button = new Wt::WPushButton();
        button->setAttributeValue("class","btn btn-info");
        button->setTextFormat(Wt::XHTMLUnsafeText);
        button->setText("<i class='icon-th-large icon-white'></i> " + tr("Alert.option.user-role-button"));
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
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&OptionManagementWidget::getRoles, this, _1, _2));

    string apiAddress = this->getApiUrl() + "/roles"
            + "?login=" + Wt::Utils::urlEncode(session->user()->eMail.toUTF8())
            + "&token=" + session->user()->token.toUTF8();

//    + this->getCredentials();

    Wt::log("debug") << "[GET] address to call : " << apiAddress;
    
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void OptionManagementWidget::getQuota(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WString smsQuotaValue = "0";
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() >= 200 && response.status() < 300)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::Array result1;
                Wt::Json::parse(response.body(), result);
                result1 = result;
                
                Wt::Json::Object tmp;
                Wt::Json::Array::const_iterator idx1;
                if (result.isNull() == false)
                {
                    for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                    {
                        tmp = (*idx1);
                        smsQuotaValue = tmp.get("value");
                    }
                }
                else
                {
                    Wt::log("warning") << "[Alerts Widget] Unexpected ";
                }
                
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alerts Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alerts Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Alerts Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"), tr("Alert.option.database-error"),Wt::Ok);
    }
    model_->setValue(model_->smsQuota,boost::any(smsQuotaValue));
    model_->setReadOnly(model_->smsQuota, true);
    mainForm->updateView(model_);
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

