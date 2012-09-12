/* 
 * File:   ProbeDownloadModel.cpp
 * Author: tsa
 * 
 * Created on 3 septembre 2012, 15:01
 */

#include "ProbeDownloadModel.h"


const Wt::WFormModel::Field ProbeDownloadModel::Os = "os";
const Wt::WFormModel::Field ProbeDownloadModel::Arch = "arch";
const Wt::WFormModel::Field ProbeDownloadModel::LabelProbe = "label-probe";

ProbeDownloadModel::ProbeDownloadModel(User *user)
{
    this->user = user;
    setView(this->user);
    reset();
}

ProbeDownloadModel::~ProbeDownloadModel()
{
}

void ProbeDownloadModel::setView(User *user)
{
    if (user == NULL)
    {
        currentView = Creation;
    }
    else if (!user->eMail.empty())
    {
        this->session = static_cast<Session*>(user->session());
        currentView = Modification;
    }
    else
    {
        Wt::log("error") << "[ProbeDownloadModel] couldn't set view";
    }
}


void ProbeDownloadModel::reset()
{
    displaySendMail = false;
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::ptr<Probe> ptrProbe = session->find<Probe>().where("\"PRB_ORG_ORG_ID\" = ?").bind(session->user().get()->currentOrganization.id()).limit(1);
        if (ptrProbe.id() == -1)
        {
            addField(Os, Wt::WString::tr("Alert.probe.download.os"));
            addField(Arch, Wt::WString::tr("Alert.probe.download.arch"));
            displaySendMail = true;
        }
        else
        {
            addField(LabelProbe, Wt::WString::tr("Alert.probe.download.label-probe"));
        }
    }
    
}

bool ProbeDownloadModel::isVisible(Field field) const
{
    return true;
}


bool ProbeDownloadModel::validateField(Field field)
{
    if (!isVisible(field))
        return true;

    bool valid = true;
    Wt::WString error;

    valid = true;

    if (valid)
        setValid(field);
    else
        setValidation(field, Wt::WValidator::Result(Wt::WValidator::Invalid, error));

    return validation(field).state() == Wt::WValidator::Valid;
}


Wt::WString ProbeDownloadModel::validateString(Wt::WString stringToValidate) const
{
        if (static_cast<int> (stringToValidate.toUTF8().length()) < 2)
            return Wt::WString::tr("Alert.user.edition.string-tooshort").arg(2);
        else
            return Wt::WString::Empty;
}

void ProbeDownloadModel::setValid(Field field)
{
    setValidation(field,Wt::WValidator::Result(Wt::WValidator::Valid,Wt::WString::tr("Alert.user.edition.valid")));
}
