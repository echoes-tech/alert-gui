/* 
 * File:   AlertEditionModel.cpp
 * Author: tsa
 * 
 * Created on 3 septembre 2012, 15:01
 */

#include "AlertEditionModel.h"

const Wt::WFormModel::Field AlertEditionModel::ThresholdOperator = "threshold-operator";
const Wt::WFormModel::Field AlertEditionModel::ThresholdValueKey = "threshold-value-key";
const Wt::WFormModel::Field AlertEditionModel::ThresholdValue = "threshold-value";
//const Wt::WFormModel::Field AlertEditionModel::ValueExample = "threshold-value-exemple";
const Wt::WFormModel::Field AlertEditionModel::Unit = "information-unit";

AlertEditionModel::AlertEditionModel(User *user) : WFormModel()
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
        currentView = Modification;
    }
    else
    {
        Wt::log("error") << "[AlertEditionModel] couldn't set view";
    }
}


void AlertEditionModel::reset()
{
    Wt::WFormModel::reset();
    addField(ThresholdOperator, Wt::WString::tr("Alert.alert.threshold-operator-info"));
    addField(ThresholdValueKey, Wt::WString::tr("Alert.alert.threshold-value-key-info"));
    addField(ThresholdValue, Wt::WString::tr("Alert.alert.threshold-value-info"));
//    addField(ValueExample, Wt::WString::tr("Alert.alert.threshold-value-example-info"));
    addField(Unit, Wt::WString::tr("Alert.alert.information-unit-info"));
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
    
    if (field == AlertEditionModel::ThresholdValue)
    {
        error = validateThresholdValue(valueText(field));
        valid = error.empty();
    }
    else if (field == AlertEditionModel::ThresholdValueKey)
    {
        error = validateString(valueText(field));
        valid = error.empty();
    }
    else if (field == AlertEditionModel::ThresholdOperator)
    {
        error = validateThresholdOperator(valueText(field));
        valid = error.empty();
    }
    else if (field == AlertEditionModel::Unit)
    {
        error = validateUnit(valueText(field));
        valid = error.empty();
    }
    else
    {
        valid = true;
    }

    if (valid)
        setValid(field);
    else
        setValidation(field, Wt::WValidator::Result(Wt::WValidator::Invalid, error));

    return validation(field).state() == Wt::WValidator::Valid;
}

Wt::WString AlertEditionModel::validateThresholdValue(Wt::WString stringToValidate) const
{
    Wt::WString res = Wt::WString::Empty;
    if (stringToValidate.toUTF8().length() > 0)
    {    
        try 
        {
            boost::lexical_cast<double>(stringToValidate); // double could be anything with >> operator.
        }
        catch(boost::bad_lexical_cast &) 
        { 
            res = Wt::WString::tr("Alert.alert.NaN");
        }
    }
    else 
    {
        res = Wt::WString::tr("Alert.alert.string-tooshort");
    }
    return res;
  
}

Wt::WString AlertEditionModel::validateThresholdOperator(Wt::WString stringToValidate) const
{
    Wt::WString res = Wt::WString::Empty;
    if (stringToValidate.toUTF8().length() < 1)
    {    
        res = Wt::WString::tr("Alert.alert.operator-empty");
    }
    return res;
  
}

Wt::WString AlertEditionModel::validateUnit(Wt::WString stringToValidate) const
{
    Wt::WString res = Wt::WString::Empty;
    if (stringToValidate.toUTF8().length() < 1)
    {    
        res = Wt::WString::tr("Alert.alert.information-unit-empty");
    }
    return res;
  
}

Wt::WString AlertEditionModel::validateSnooze(Wt::WString stringToValidate) const
{
    Wt::WString res = Wt::WString::Empty;
    if (stringToValidate.toUTF8().length() > 0)
    {
        try 
        {
            double x = boost::lexical_cast<double>(stringToValidate); // double could be anything with >> operator.
            if (x < 60)
            {
                res = Wt::WString::tr("Alert.alert.snooze-too-small");
            }
        }
        catch(boost::bad_lexical_cast &) 
        { 
            res = Wt::WString::tr("Alert.alert.NaN");
        }    
    }
    else 
    {
        res = Wt::WString::tr("Alert.alert.string-tooshort");
    }
    return res;
  
}



Wt::WString AlertEditionModel::validateString(Wt::WString stringToValidate) const
{
        if (static_cast<int> (stringToValidate.toUTF8().length()) < 2)
            return Wt::WString::tr("Alert.alert.string-tooshort").arg(2);
        else
            return Wt::WString::Empty;
}

Wt::WString AlertEditionModel::label(Field field) const
{
    return Wt::WString::tr(std::string("Alert.alert.") + field + std::string("-label"));
}

void AlertEditionModel::setValid(Field field)
{
    setValidation(field,Wt::WValidator::Result(Wt::WValidator::Valid,Wt::WString::tr("Alert.user.edition.valid")));
}

void AlertEditionModel::modifyField(Field field, const Wt::WString& info)
{
    for (unsigned int i = 0; i < fields().size(); i++)
    {
        if (strcmp(fields().at(i), field) == 0)
        {
            addField(field,info);
            break;
        }
    }
}

void AlertEditionModel::setSession(Session *session)
{
    this->session = session;
}