/* 
 * File:   AlertEditionModel.cpp
 * Author: tsa
 * 
 * Created on 3 septembre 2012, 15:01
 */

#include "AlertEditionModel.h"


const Wt::WFormModel::Field AlertEditionModel::ThresholdOperator = "threshold-operator";
const Wt::WFormModel::Field AlertEditionModel::ThresholdValue = "threshold-value";

AlertEditionModel::AlertEditionModel(User *user)
{
    this->user = user;
    setView(this->user);
    reset();
}

AlertEditionModel::~AlertEditionModel()
{
}

void AlertEditionModel::setView(User *user)
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
        Wt::log("error") << "[AlertEditionModel] couldn't set view";
    }
}


void AlertEditionModel::reset()
{
    addField(ThresholdOperator, Wt::WString::tr("Alert.user.edition.threshold-operator"));
    addField(ThresholdValue, Wt::WString::tr("Alert.user.edition.threshold-value"));
    
}

bool AlertEditionModel::isVisible(Field field) const
{
    return true;
}


bool AlertEditionModel::validateField(Field field)
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


Wt::WString AlertEditionModel::validateString(Wt::WString stringToValidate) const
{
        if (static_cast<int> (stringToValidate.toUTF8().length()) < 2)
            return Wt::WString::tr("Alert.user.edition.string-tooshort").arg(2);
        else
            return Wt::WString::Empty;
}

void AlertEditionModel::setValid(Field field)
{
    setValidation(field,Wt::WValidator::Result(Wt::WValidator::Valid,Wt::WString::tr("Alert.user.edition.valid")));
}
