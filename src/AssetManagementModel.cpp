/* 
 * Asset Management Model
 * @author ECHOES Technologies (TSA)
 * @date 03/09/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "AssetManagementModel.h"

const Wt::WFormModel::Field AssetManagementModel::AssetName = "asset-name";

AssetManagementModel::AssetManagementModel()
{
    reset();
}

AssetManagementModel::~AssetManagementModel() {}

void AssetManagementModel::reset()
{
    addField(AssetName, Wt::WString::tr("Alert.user.edition.asset-name"));
}

bool AssetManagementModel::isVisible(Field field) const
{
    if (field == AssetName)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool AssetManagementModel::validateField(Field field)
{
    if (!isVisible(field))
        return true;

    bool valid = true;
    Wt::WString error;

    if (field == AssetName)
    {
        // Check whether the asset doesn't already exists
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


Wt::WString AssetManagementModel::validateString(Wt::WString stringToValidate) const
{
    // Todo : préciser le message d'erreur dans le xml
        if (static_cast<int> (stringToValidate.toUTF8().length()) < 2)
            return Wt::WString::tr("Alert.user.edition.string-tooshort").arg(2);
        else
            return Wt::WString::Empty;
}

void AssetManagementModel::setValid(Field field)
{
    setValidation(field,Wt::WValidator::Result(Wt::WValidator::Valid,Wt::WString::tr("Alert.user.edition.valid")));
}

void AssetManagementModel::setSession(Session *session)
{
    this->session_ = session;
}

