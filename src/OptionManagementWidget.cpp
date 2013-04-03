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
        Wt::Dbo::ptr<User> tempUser = session->find<User>().where("\"USR_ID\" = ?").bind(session->user().id());
        Wt::log("info") << "Debug : user found";
        int smsQuotaValue = 0;
        if (tempUser)
        {
            Wt::Dbo::ptr<Organization> tempOrga = tempUser->currentOrganization;
            Wt::Dbo::ptr<PackOption> ptrPackOption = session->find<PackOption>()
                    .where("\"POP_PCK_PCK_ID\" = ?").bind(tempOrga.get()->pack.id())
                    .where("\"POP_OPT_OPT_ID\" = ?").bind(Enums::quotaSms)
                    .limit(1);
            if (ptrPackOption.get())
            {
                Wt::Dbo::ptr<OptionValue> ptrOptionValue = session->find<OptionValue>().where("\"OPT_ID_OPT_ID\" = ?").bind(ptrPackOption.get()->pk.option.id())
                                                                .where("\"ORG_ID_ORG_ID\" = ?").bind(tempOrga.id())
                                                                .limit(1);
                if (ptrOptionValue.get())
                {
                    smsQuotaValue = boost::lexical_cast<int>(ptrOptionValue.get()->value);
                }
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
    
    if (field == OptionManagementModel::smsAsk)
    {
        Wt::WPushButton *button = new Wt::WPushButton(tr("Alert.option.ask-sms-button"));
        button->clicked().connect(boost::bind(&OptionManagementWidget::askSms, this));
        result = button;
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