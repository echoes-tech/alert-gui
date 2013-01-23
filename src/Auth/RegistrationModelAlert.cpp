/* 
 * File:   RegistrationModelAlert.cpp
 * Author: tsa
 * 
 * Created on 17 janvier 2013, 17:36
 */

#include "Auth/RegistrationModelAlert.h"
#include <Wt/WFormModel>

const Wt::WFormModel::Field RegistrationModelAlert::TestField = "test";

const Wt::WFormModel::Field RegistrationModelAlert::FirstNameField = "first-name";
const Wt::WFormModel::Field RegistrationModelAlert::LastNameField = "last-name";
const Wt::WFormModel::Field RegistrationModelAlert::OrganizationTypeCompanyField = "organization-type-company";
const Wt::WFormModel::Field RegistrationModelAlert::OrganizationTypeIndividualField = "organization-type-individual";
const Wt::WFormModel::Field RegistrationModelAlert::OrganizationTypeAssociationField = "organization-type-association";
const Wt::WFormModel::Field RegistrationModelAlert::OrganizationNameField = "organization-name";

RegistrationModelAlert::RegistrationModelAlert(const Wt::Auth::AuthService& baseAuth,
                                     Wt::Auth::AbstractUserDatabase& users,
                                     Wt::Auth::Login& login,
                                     Wt::WObject *parent)
: Wt::Auth::RegistrationModel(baseAuth, users, login, parent)
{
    reset();
}

RegistrationModelAlert::~RegistrationModelAlert() {
}

void RegistrationModelAlert::reset()
{
    Wt::Auth::RegistrationModel::reset();

    this->addField(OrganizationTypeCompanyField, Wt::WString::tr("Wt.Auth.organization-type-company-info"));
    this->addField(OrganizationTypeIndividualField, Wt::WString::tr("Wt.Auth.organization-type-individual-info"));
    this->addField(OrganizationTypeAssociationField, Wt::WString::tr("Wt.Auth.organization-type-association-info"));
    
    this->addField(OrganizationNameField, Wt::WString::tr("Wt.Auth.organization-name-info"));
    
    this->addField(FirstNameField, Wt::WString::tr("Wt.Auth.first-name-info"));
    
    this->addField(LastNameField, Wt::WString::tr("Wt.Auth.last-name-info"));
 
    this->addField(TestField, Wt::WString::tr("test"));

}

std::string RegistrationModelAlert::generateToken()
{
    std::string res = "";
    std::string chars(
        "abcdefghijklmnopqrstuvwxyz"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "1234567890"
//        "!@#$%^&*()"
//        "`~-_=+[{]{\\|;:'\",<.>/? "
    );
    boost::random::random_device rng;
    boost::random::uniform_int_distribution<> index_dist(0, chars.size() - 1);
    for(int i = 0; i < 25; ++i) 
    {
        res += chars[index_dist(rng)];
    }
    return res;
}  

void RegistrationModelAlert::setRegistrationType(RegistrationModelAlert::RegistrationType rType)
{
    this->rType = rType;
}

bool RegistrationModelAlert::validateField(Field field)
{
    if (!isVisible(field))
        return true;

    bool valid = true;
    Wt::WString error;

    if (strcmp(field,LoginNameField) == 0)
    {
        error = validateLoginName(valueText(field));

        if (error.empty())
        {
            checkUserExists(valueText(field));
            bool exists = existingUser_.isValid();
            valid = !exists;

            if (exists && confirmIsExistingUser() == ConfirmationNotPossible)
                error = Wt::WString::tr("Wt.Auth.user-name-exists");
        }
        else
            valid = false;

        if (isReadOnly(field))
            valid = true;
    }
    else if (field == ChoosePasswordField)
    {
        Wt::Auth::AbstractPasswordService::AbstractStrengthValidator *v
                = passwordAuth()->strengthValidator();

        if (v)
        {
            Wt::WValidator::Result r
                    = v->validate(valueText(ChoosePasswordField),
                                  valueText(LoginNameField),
                                  valueText(EmailField).toUTF8());
            valid = r.state() == Wt::WValidator::Valid;
            error = r.message();
        }
        else
            valid = true;
    }
    else if (strcmp(field,RepeatPasswordField) == 0)
    {
        if (validation(ChoosePasswordField).state() == Wt::WValidator::Valid)
        {
            if (valueText(ChoosePasswordField) != valueText(RepeatPasswordField))
                error = Wt::WString::tr("Wt.Auth.passwords-dont-match");
            valid = error.empty();
        }
        else
            return true; // Do not validate the repeat field yet
    }
    else if (strcmp(field,EmailField) == 0)
    {
        std::string email = valueText(EmailField).toUTF8();

        if (!email.empty())
        {
            if (static_cast<int> (email.length()) < 3
                    || email.find('@') == std::string::npos)
                error = Wt::WString::tr("Wt.Auth.email-invalid");

            if (error.empty())
            {
                Wt::Auth::User user = users().findWithEmail(email);
                if (user.isValid())
                    error = Wt::WString::tr("Wt.Auth.email-exists");
            }
        }
        else
        {
            if (emailPolicy_ != EmailOptional)
                error = Wt::WString::tr("Wt.Auth.email-invalid");
        }

        valid = error.empty();
    }
    else if (field == OrganizationNameField)
    {
//        std::string name = valueText(OrganizationNameField).toUTF8();
//        if (registrationType_ != Individual)
//        {
//            if (!name.empty())
//            {
//                if (static_cast<int> (name.length()) < 2)
//                    error = WString::tr("Wt.Auth.company-name-invalid");
//            }
//            else
//            {
//                error = WString::tr("Wt.Auth.company-name-invalid");
//            }
//        }
//        valid = error.empty();
        valid = true;
    }
    else if (strcmp(field,FirstNameField) == 0)
    {
        std::string name = valueText(FirstNameField).toUTF8();
        if (!name.empty())
        {
            if (static_cast<int> (name.length()) < 2)
                error = Wt::WString::tr("Wt.Auth.first-name-invalid");
        }
        else
        {
            error = Wt::WString::tr("Wt.Auth.first-name-invalid");
        }
        valid = error.empty();
    }
    else if (strcmp(field,LastNameField) == 0)
    {
        std::string name = valueText(LastNameField).toUTF8();
        if (!name.empty())
        {
            if (static_cast<int> (name.length()) < 2)
                error = Wt::WString::tr("Wt.Auth.last-name-invalid");
        }
        else
        {
            error = Wt::WString::tr("Wt.Auth.last-name-invalid");
        }
        valid = error.empty();
    }
    else
        return true;

    if (valid)
        setValid(field);
    else
        setValidation(field, Wt::WValidator::Result(Wt::WValidator::Invalid, error));

    return validation(field).state() == Wt::WValidator::Valid;
}

bool RegistrationModelAlert::isVisible(Field field) const
{
    if (strcmp(field,LoginNameField) == 0)
    {
        if (baseAuth()->identityPolicy() == Wt::Auth::OptionalIdentity)
            return passwordAuth() && !idpIdentity_.isValid();
        else
            return true;
    }
    else if ((strcmp(field,ChoosePasswordField) == 0) || (strcmp(field,RepeatPasswordField) == 0))
    {
        return passwordAuth() && !idpIdentity_.isValid();
    }
    else if ((strcmp(field,OrganizationNameField) == 0)
            || (strcmp(field,OrganizationTypeAssociationField) == 0)
            || (strcmp(field,OrganizationTypeIndividualField) == 0)
            || (strcmp(field,OrganizationTypeCompanyField) == 0)
            || (strcmp(field,FirstNameField) == 0)
            || (strcmp(field,LastNameField) == 0))
    {
        return true;
    }
    else if (strcmp(field,TestField) == 0)
    {   
        return false;
    }
    else if (strcmp(field,EmailField) == 0)
    {
        if (baseAuth()->identityPolicy() == Wt::Auth::EmailAddressIdentity)
            return false;
        else
            if (emailPolicy_ == EmailDisabled)
            return false;
        else
            return true;
    }
    else
        return false;
}