/* 
 * File:   UserEditionModel.cpp
 * Author: tsa
 * 
 * Created on 3 septembre 2012, 15:01
 */

#include "UserEditionModel.h"

const Wt::WFormModel::Field UserEditionModel::FirstName = "first-name";
const Wt::WFormModel::Field UserEditionModel::LastName = "last-name";
const Wt::WFormModel::Field UserEditionModel::Email = "email";
const Wt::WFormModel::Field UserEditionModel::ChoosePasswordField = "password";
const Wt::WFormModel::Field UserEditionModel::RepeatPasswordField = "repeat-password";
const Wt::WFormModel::Field UserEditionModel::Role = "role";
const Wt::WFormModel::Field UserEditionModel::State = "state";
const Wt::WFormModel::Field UserEditionModel::MediaEMail = "media-email";
const Wt::WFormModel::Field UserEditionModel::MediaSMS = "media-sms";
const Wt::WFormModel::Field UserEditionModel::MediaMobileApp = "media-mobileapp";

UserEditionModel::UserEditionModel(Echoes::Dbo::User *user)
{
    this->user = user;
    setView(this->user);
    reset();
}

UserEditionModel::~UserEditionModel()
{
}

void UserEditionModel::setView(Echoes::Dbo::User *user)
{
    if (user == NULL)
    {
        currentView = Creation;
    }
    else if (!user->eMail.empty())
    {
        currentView = Modification;
    }
    else
    {
        Wt::log("error") << "[UserEditionModel] couldn't set view";
    }
}


void UserEditionModel::reset()
{
//    addField(FirstName, Wt::WString::tr("Alert.user.edition.first-name"));
//    addField(LastName, Wt::WString::tr("Alert.user.edition.last-name"));
//    addField(Email, Wt::WString::tr("Alert.user.edition.email"));
//    addField(ChoosePasswordField, Wt::WString::tr("Alert.user.edition.password"));
//    addField(RepeatPasswordField, Wt::WString::tr("Alert.user.edition.repeat-password"));
//    addField(Role, Wt::WString::tr("Alert.user.edition.role"));
//    addField(State, Wt::WString::tr("Alert.user.edition.state"));
    addField(MediaEMail, Wt::WString::tr("Alert.user.edition.media-email"));
    addField(MediaSMS, Wt::WString::tr("Alert.user.edition.media-sms"));
    addField(MediaMobileApp, Wt::WString::tr("Alert.user.edition.media-mobileapp"));
    
}

bool UserEditionModel::isVisible(Field field) const
{
    if (field == MediaEMail || field == MediaSMS || field == MediaMobileApp)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool UserEditionModel::validateField(Field field)
{
    if (!isVisible(field))
        return true;

    bool valid = true;
    Wt::WString error;

    if (field == MediaEMail || field == MediaSMS || field == MediaMobileApp)
    {
        error = validateString(valueText(field));

        if (error.empty())
        {
            valid = true;
        }
        else
        {
            valid = false;
        }
    }
    else
        valid = false;

    if (valid)
        setValid(field);
    else
        setValidation(field, Wt::WValidator::Result(Wt::WValidator::Invalid, error));

    return validation(field).state() == Wt::WValidator::Valid;
}


Wt::WString UserEditionModel::validateString(Wt::WString stringToValidate) const
{
        if (static_cast<int> (stringToValidate.toUTF8().length()) < 2)
            return Wt::WString::tr("Alert.user.edition.string-tooshort").arg(2);
        else
            return Wt::WString::Empty;
}

void UserEditionModel::setValid(Field field)
{
    setValidation(field,Wt::WValidator::Result(Wt::WValidator::Valid,Wt::WString::tr("Alert.user.edition.valid")));
}

void UserEditionModel::setSession(Echoes::Dbo::Session *session)
{
    this->session = session;
}
