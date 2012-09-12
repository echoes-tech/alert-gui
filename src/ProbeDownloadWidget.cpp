/* 
 * File:   ProbeDownloadWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "ProbeDownloadWidget.h"

//#include "Wt/Auth/AbstractUserDatabase"
#include "AuthWidget.h"
//#include "Login.h"

#include "Wt/WAnchor"
#include "Wt/WApplication"
#include "Wt/WContainerWidget"
#include "Wt/WDialog"
#include "Wt/WImage"
#include "Wt/WLineEdit"
#include "Wt/WPushButton"
#include "Wt/WText"

#include <Wt/WComboBox>
#include <Wt/WSelectionBox>
#include <Wt/WStringListModel>
#include <Wt/Dbo/Query>
#include <Wt/WSpinBox>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/Mail/Client>
#include <Wt/Mail/Message>


#include "tools/Session.h"
//#include "Login.h"
#include "user/User.h"


#include <memory>
#include <Wt/WTableView>
#include <Wt/Utils>
#include <Wt/WLabel>


ProbeDownloadWidget::ProbeDownloadWidget()
: Wt::WTemplateFormView(Wt::WString::tr("Alert.Probe.Download.template"))
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("probe",false);
}

void ProbeDownloadWidget::setModel(ProbeDownloadModel *model)
{
    model_ = model;
}

void ProbeDownloadWidget::setSession(Session *session)
{
    this->session = session;
    this->userId = session->user().id();
}

void ProbeDownloadWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
        update();
        created_ = true;
    }

    WTemplate::render(flags);
}

Wt::WFormWidget *ProbeDownloadWidget::createFormWidget(ProbeDownloadModel::Field field)
{
    Wt::WFormWidget *result = 0;

    if (field == ProbeDownloadModel::Os)
    {
        comboOs = new Wt::WComboBox();
        Wt::WStringListModel *slmOs = new Wt::WStringListModel();   
        
        slmOs->insertString(0,"Debian");
        slmOs->insertString(1,"Ubuntu");
        slmOs->insertString(2,"Redhat");
        slmOs->insertString(3,tr("other"));
        comboOs->setModel(slmOs);
        
        result = comboOs;
    }
    else if (field == ProbeDownloadModel::Arch)
    {
        comboArch = new Wt::WComboBox();
        Wt::WStringListModel *slmArch = new Wt::WStringListModel();   
        
        slmArch->insertString(0,"32 bits");
        slmArch->insertString(1,"64 bits");
        comboArch->setModel(slmArch);
        
        result = comboArch;
    }
    else if (field == ProbeDownloadModel::LabelProbe)
    {
        Wt::WLineEdit *line = new Wt::WLineEdit();
        line->setText(tr("text-label-probe"));
        result = line;
    }

    return result;
}

void ProbeDownloadWidget::update()
{

    updateView(model_);

    if (!created_)
    {
        
        if (model_->displaySendMail)
        {
            Wt::WPushButton *sendMailButton = new Wt::WPushButton(tr("Alert.probe.download.send-mail"));
            bindWidget("send-mail-button", sendMailButton);
            sendMailButton->clicked().connect(this, &ProbeDownloadWidget::sendMail);
        }
        else
        {
            Wt::WLabel *label = new Wt::WLabel();
            bindWidget("label-probe-text", label);
        }
    }
}



void ProbeDownloadWidget::sendMail()
{
    Probe *probe = new Probe();
    Wt::Dbo::ptr<Probe> probePtr;
    {
        Wt::Dbo::Transaction transaction(*session);
        probe = new Probe();
        probe->name = "probe_name";
        probe->organization = session->user().get()->currentOrganization;
        probePtr = session->add<Probe>(probe);
    }
    Wt::Mail::Message mailMessage;
    Wt::WString mail;
    const Wt::WString constMailRecipient = "contact@echoes-tech.com";
    const Wt::WString constMailRecipientName = "Contact";
    mail =  Wt::Utils::htmlEncode(Wt::WString("Nouvelle demande de creation de sonde.").toUTF8(), Wt::Utils::EncodeNewLines)
            + "</br> " + session->user().get()->firstName + " " + session->user().get()->lastName
            + " : " + session->user().get()->eMail
            + "</br>OS : " + comboOs->currentText()
            + "</br>Arch : " + comboArch->currentText()
            + "</br>Id Probe : " + boost::lexical_cast<std::string>(probePtr.id());
    mailMessage.addRecipient(Wt::Mail::To, Wt::Mail::Mailbox(constMailRecipient.toUTF8(), constMailRecipientName.toUTF8()));
    mailMessage.setSubject("Demande de sonde");
    mailMessage.addHtmlBody(mail);
    session->auth().sendMail(mailMessage);
    this->refresh();
}





bool ProbeDownloadWidget::validate()
{
    return model_->validate();
}


void ProbeDownloadWidget::close()
{
    delete this;
}

