/* 
 * File:   AssetManagementModel.cpp
 * Author: tsa
 * 
 * Created on 3 septembre 2012, 15:01
 */

#include "AssetManagementModel.h"

const Wt::WFormModel::Field AssetManagementModel::AssetName = "asset-name";

AssetManagementModel::AssetManagementModel()
{ 
    reset();
}

AssetManagementModel::~AssetManagementModel()
{
    
}


void AssetManagementModel::reset()
{
    WFormModel::reset();
    addField(AssetName, Wt::WString::tr("Alert.asset.asset-name-info"));
}

bool AssetManagementModel::isVisible(Field field) const
{
    return true;
}

bool AssetManagementModel::validateField(Field field)
{
    if (!isVisible(field))
        return true;

    bool valid = true;
    Wt::WString error;
    
    if (field == AssetManagementModel::AssetName)
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



Wt::WString AssetManagementModel::validateString(Wt::WString stringToValidate) const
{
    // Todo : préciser le message d'erreur dans le xml
    if (static_cast<int> (stringToValidate.toUTF8().length()) < 2)
        return Wt::WString::tr("Alert.alert.string-tooshort").arg(2);
    else
        return Wt::WString::Empty;
}

Wt::WString AssetManagementModel::label(Field field) const
{
    return Wt::WString::tr(std::string("Alert.asset.") + field + std::string("-label"));
}

void AssetManagementModel::setValid(Field field)
{
    setValidation(field,Wt::WValidator::Result(Wt::WValidator::Valid,Wt::WString::tr("Alert.asset.valid")));
}