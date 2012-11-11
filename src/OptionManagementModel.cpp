/* 
 * File:   OptionManagementModel.cpp
 * Author: tsa
 * 
 * Created on 11 novembre 2012, 13:15
 */

#include "OptionManagementModel.h"

const Wt::WFormModel::Field OptionManagementModel::smsQuota = "sms-quota";
const Wt::WFormModel::Field OptionManagementModel::smsAsk = "sms-ask";

OptionManagementModel::OptionManagementModel() 
{
    reset();
}

OptionManagementModel::~OptionManagementModel() 
{
    
}

void OptionManagementModel::reset()
{
    WFormModel::reset();
    addField(smsQuota, Wt::WString::tr("Alert.option.sms-quota-info"));
}

bool OptionManagementModel::isVisible(Field field) const
{
    return true;
}

bool OptionManagementModel::validateField(Field field)
{
    if (!isVisible(field))
        return true;

    bool valid = true;
    Wt::WString error;
    
    if (field == OptionManagementModel::smsQuota)
    {
        // Check whether the asset doesn't already exists
        error = validateString(valueText(field));
        valid = error.empty();
    }
    else
    {
        valid = false;
    }

    if (valid)
        setValid(field);
    else
        setValidation(field, Wt::WValidator::Result(Wt::WValidator::Invalid, error));

    return validation(field).state() == Wt::WValidator::Valid;
}

Wt::WString OptionManagementModel::validateString(Wt::WString stringToValidate) const
{
        if (static_cast<int> (stringToValidate.toUTF8().length()) < 2)
            return Wt::WString::tr("Alert.option.string-tooshort").arg(2);
        else
            return Wt::WString::Empty;
}

Wt::WString OptionManagementModel::label(Field field) const
{
    return Wt::WString::tr(std::string("Alert.option.") + field + std::string("-label"));
}

void OptionManagementModel::setValid(Field field)
{
    setValidation(field,Wt::WValidator::Result(Wt::WValidator::Valid,Wt::WString::tr("Alert.option.valid")));
}