/* 
 * File:   RegistrationWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "RegistrationWidget.h"

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


#include "tools/Session.h"
//#include "Login.h"
#include "user/User.h"

#include <memory>

namespace skeletons
{
extern const char *Auth_xml1;
}

namespace Wt
{

//LOGGER("Auth::RegistrationWidget");	     

namespace Auth
{

RegistrationWidget::RegistrationWidget(AuthWidget *authWidget)
: WTemplateFormView(tr("Wt.Auth.template.registration")),
authWidget_(authWidget),
model_(0),
created_(false),
confirmPasswordLogin_(0)
{
    WApplication *app = WApplication::instance();
    app->messageResourceBundle().use("auth",false);
//    organizationContainer = new Wt::WGroupBox(tr("Wt.Auth.template.registration.organization-type.group"));
    organizationGroup = new Wt::WButtonGroup(this);
}

void RegistrationWidget::setModel(RegistrationModel *model)
{
    if (!model_ && model)
        model->login().changed().connect(this, &RegistrationWidget::close);

    delete model_;
    model_ = model;
}

void RegistrationWidget::render(WFlags<RenderFlag> flags)
{
    if (!created_)
    {
        update();
        created_ = true;
    }

    WTemplate::render(flags);
}

WFormWidget *RegistrationWidget::createFormWidget(WFormModel::Field field)
{
    WFormWidget *result = 0;

    if (field == RegistrationModel::OrganizationTypeCompanyField)
    {
        Wt::WRadioButton *button = new Wt::WRadioButton("");
        organizationGroup->addButton(button, 1);
        result = button;
        result->changed().connect(boost::bind(&RegistrationWidget::checkOrganization, this));
    }
    else if (field == RegistrationModel::OrganizationTypeIndividualField)
    {
        Wt::WRadioButton *button = new Wt::WRadioButton("");
        organizationGroup->addButton(button, 2);
        result = button;
        result->changed().connect(boost::bind(&RegistrationWidget::checkOrganization, this));
    }
    else if (field == RegistrationModel::OrganizationTypeAssociationField)
    {
        Wt::WRadioButton *button = new Wt::WRadioButton("");
        organizationGroup->addButton(button, 3);
        result = button;
        result->changed().connect(boost::bind(&RegistrationWidget::checkOrganization, this));
    }
    else if (field == RegistrationModel::OrganizationNameField)
    {
        result = new WLineEdit();
        result->changed().connect(boost::bind(&RegistrationWidget::checkOrganization, this));
    }
    else if (field == RegistrationModel::LoginNameField)
    {
        result = new WLineEdit();
        result->changed().connect(boost::bind(&RegistrationWidget::checkLoginName, this));
    }
    else if (field == RegistrationModel::EmailField)
    {
        result = new WLineEdit();
    }
    else if (field == RegistrationModel::TestField)
    {
        result = new WLineEdit();
    }
    else if (field == RegistrationModel::ChoosePasswordField)
    {
        WLineEdit *p = new WLineEdit();
        p->setEchoMode(WLineEdit::Password);
        p->keyWentUp().connect
                (boost::bind(&RegistrationWidget::checkPassword, this));
        p->changed().connect
                (boost::bind(&RegistrationWidget::checkPassword, this));
        result = p;
    }
    else if (field == RegistrationModel::RepeatPasswordField)
    {
        WLineEdit *p = new WLineEdit();
        p->setEchoMode(WLineEdit::Password);
        p->changed().connect
                (boost::bind(&RegistrationWidget::checkPassword2, this));
        result = p;
    }

    return result;
}

//void RegistrationWidget::companyClick()
//{
//    model_->setValue(model_->OrganizationTypeCompanyField,boost::any(true));
//    model_->setValue(model_->OrganizationTypeIndividualField,boost::any(false));
//    model_->setValue(model_->OrganizationTypeAssociationField,boost::any(false));
//    model_->setRegistrationType(RegistrationModel::Company);
//    model_->fields_[model_->OrganizationNameField].readOnly = false;
//    update();
//}
//
//void RegistrationWidget::individualClick()
//{
//    model_->setValue(model_->OrganizationTypeCompanyField,boost::any(false));
//    model_->setValue(model_->OrganizationTypeIndividualField,boost::any(true));
//    model_->setValue(model_->OrganizationTypeAssociationField,boost::any(false));
//    model_->setRegistrationType(RegistrationModel::Individual);
//    model_->fields_[model_->OrganizationNameField].readOnly = true;
//    update();
//}
//
//void RegistrationWidget::associationClick()
//{
//    model_->setValue(model_->OrganizationTypeCompanyField,boost::any(false));
//    model_->setValue(model_->OrganizationTypeIndividualField,boost::any(false));
//    model_->setValue(model_->OrganizationTypeAssociationField,boost::any(true));
//    model_->setRegistrationType(RegistrationModel::Association);
//    model_->fields_[model_->OrganizationNameField].readOnly = false;
//    update();
//
//}

void RegistrationWidget::update()
{
    if (!model_->passwordAuth() && !model_->oAuth().empty())
        bindString("password-description",
                   tr("Wt.Auth.password-or-oauth-registration"));
    else
        bindEmpty("password-description");

    updateView(model_);

    if (!created_)
    {
        WLineEdit *password = resolve<WLineEdit *>
                (RegistrationModel::ChoosePasswordField);
        WLineEdit *password2 = resolve<WLineEdit *>
                (RegistrationModel::RepeatPasswordField);
        WText *password2Info = resolve<WText *>
                (RegistrationModel::RepeatPasswordField + std::string("-info"));
        
        // FIXME : usefull ?
//        resolve<WLineEdit *>(RegistrationModel::TestField);
        
        if (password && password2 && password2Info)
            model_->validatePasswordsMatchJS(password, password2, password2Info);
    }

    WAnchor *isYou = resolve<WAnchor *>("confirm-is-you");
    if (!isYou)
    {
        isYou = new WAnchor(std::string("#"), tr("Wt.Auth.confirm-is-you"));
        isYou->hide();
        bindWidget("confirm-is-you", isYou);
    }

    if (model_->isConfirmUserButtonVisible())
    {
        if (!isYou->clicked().isConnected())
            isYou->clicked().connect(this, &RegistrationWidget::confirmIsYou);
        isYou->show();
    }
    else
        isYou->hide();

    if (model_->isFederatedLoginVisible())
    {
        if (!conditionValue("if:oauth"))
        {
            setCondition("if:oauth", true);
            if (model_->passwordAuth())
                bindString("oauth-description", tr("Wt.Auth.or-oauth-registration"));
            else
                bindString("oauth-description", tr("Wt.Auth.oauth-registration"));

            WContainerWidget *icons = new WContainerWidget();
            icons->addStyleClass("Wt-field");

            for (unsigned i = 0; i < model_->oAuth().size(); ++i)
            {
                const OAuthService *service = model_->oAuth()[i];

                WImage *w = new WImage("css/oauth-" + service->name() + ".png", icons);
                w->setToolTip(service->description());
                w->setStyleClass("Wt-auth-icon");
                w->setVerticalAlignment(AlignMiddle);
                OAuthProcess * const process
                        = service->createProcess(service->authenticationScope());
                w->clicked().connect(process, &OAuthProcess::startAuthenticate);

                process->authenticated().connect
                        (boost::bind(&RegistrationWidget::oAuthDone, this, process, _1));

                WObject::addChild(process);
            }

            bindWidget("icons", icons);
        }
    }
    else
    {
        setCondition("if:oauth", false);
        bindEmpty("icons");
    }

    if (!created_)
    {
        WPushButton *okButton = new WPushButton(tr("Wt.Auth.register"));
        WPushButton *cancelButton = new WPushButton(tr("Wt.WMessageBox.Cancel"));

        bindWidget("ok-button", okButton);
        bindWidget("cancel-button", cancelButton);

        okButton->clicked().connect(this, &RegistrationWidget::doRegister);
        cancelButton->clicked().connect(this, &RegistrationWidget::close);

        created_ = true;
    }
}

void RegistrationWidget::oAuthDone(OAuthProcess *oauth,
                                   const Identity& identity)
{
    if (identity.isValid())
    {
        log("SECURE") << oauth->service().name() << ": identified: as "
                << identity.id() << ", " << identity.name() << ", "
                << identity.email();

        if (!model_->registerIdentified(identity))
            update();
    }
    else
    {
        if (authWidget_)
            authWidget_->displayError(oauth->error());
        log("SECURE") << oauth->service().name() << ": error: " << oauth->error();
    }
}

void RegistrationWidget::checkLoginName()
{
    updateModelField(model_, RegistrationModel::LoginNameField);
    model_->validateField(RegistrationModel::LoginNameField);
    model_->setValidated(RegistrationModel::LoginNameField, false);
    update();
}

void RegistrationWidget::checkOrganization()
{
    updateModelField(model_, RegistrationModel::OrganizationTypeCompanyField);
    updateModelField(model_, RegistrationModel::OrganizationTypeIndividualField);
    updateModelField(model_, RegistrationModel::OrganizationTypeAssociationField);
    updateModelField(model_, RegistrationModel::OrganizationNameField);  
    
    bool bCompagny = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeCompanyField].value);
    if (bCompagny)
    {
        model_->setRegistrationType(RegistrationModel::Company);
        model_->fields_[model_->OrganizationNameField].readOnly = false;
    }
    bool bIndividual = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeIndividualField].value);
    if (bIndividual)
    {
        model_->setRegistrationType(RegistrationModel::Individual);
        model_->fields_[model_->OrganizationNameField].readOnly = true;
        
        const std::string emptyString="";
        model_->fields_[model_->OrganizationNameField].value = boost::any(emptyString);
    }
    bool bAssociation = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeAssociationField].value);
    if (bAssociation)
    {
        model_->setRegistrationType(RegistrationModel::Association);
        model_->fields_[model_->OrganizationNameField].readOnly = false;
    }
    model_->validateField(RegistrationModel::OrganizationNameField);
    model_->setValidated(RegistrationModel::OrganizationNameField, false);
    update();
}

void RegistrationWidget::checkPassword()
{
    updateModelField(model_, RegistrationModel::LoginNameField);
    updateModelField(model_, RegistrationModel::ChoosePasswordField);
    updateModelField(model_, RegistrationModel::EmailField);
    model_->validateField(RegistrationModel::ChoosePasswordField);
    model_->setValidated(RegistrationModel::ChoosePasswordField, false);
    update();
}

void RegistrationWidget::checkPassword2()
{
    updateModelField(model_, RegistrationModel::ChoosePasswordField);
    updateModelField(model_, RegistrationModel::RepeatPasswordField);
    model_->validateField(RegistrationModel::RepeatPasswordField);
    model_->setValidated(RegistrationModel::RepeatPasswordField, false);
    update();
}

bool RegistrationWidget::validate()
{
    return model_->validate();
}

void RegistrationWidget::doRegister()
{
    std::auto_ptr<AbstractUserDatabase::Transaction>
            t(model_->users().startTransaction());

    updateModel(model_);

    if (validate())
    {
        User user = model_->doRegister();
        if (user.isValid())
        {
            registerUserDetails(user);
            model_->login().login(user);
        }
        else
            update();
    }
    else
        update();

    if (t.get())
        t->commit();
}

void RegistrationWidget::registerUserDetails(User& user)
{
    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->firstName = model_->valueText(model_->LoginNameField);
    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->eMail = model_->valueText(model_->EmailField);
    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->lastName = model_->valueText(model_->TestField);
            
}

void RegistrationWidget::close()
{
    delete this;
}

void RegistrationWidget::confirmIsYou()
{
    updateModel(model_);

    switch (model_->confirmIsExistingUser())
    {
    case RegistrationModel::ConfirmWithPassword:
    {
        delete confirmPasswordLogin_;
        confirmPasswordLogin_ = new Login();
        confirmPasswordLogin_->login(model_->existingUser(), WeakLogin);
        confirmPasswordLogin_
                ->changed().connect(this, &RegistrationWidget::confirmedIsYou);

        WDialog *dialog =
                authWidget_->createPasswordPromptDialog(*confirmPasswordLogin_);
        dialog->show();
    }

        break;
    case RegistrationModel::ConfirmWithEmail:
        // FIXME send a confirmation email to merge the new identity
        // with the existing one. We need to include the provisional
        // id in the token -- no problem there, integrity is verified by a
        // hash in the database

        log("INFO") << "confirming a new identity to existing user not yet implemented";

        break;
    default:
        log("ERROR") << "that's gone haywire.";
    }
}

void RegistrationWidget::confirmedIsYou()
{
    if (confirmPasswordLogin_->state() == StrongLogin)
    {
        model_->existingUserConfirmed();
    }
    else
    {
        delete confirmPasswordLogin_;
        confirmPasswordLogin_ = 0;
    }
}

}
}
