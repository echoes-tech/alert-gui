/* 
 * File:   UserEditionWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "UserEditionWidget.h"

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


#include "tools/Session.h"
//#include "Login.h"
#include "user/User.h"
#include "UserEditionModel.h"


#include <memory>


UserEditionWidget::UserEditionWidget()
: Wt::WTemplateFormView(Wt::WString::tr("Alert.User.Edition.template"))
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("user",false);
}

void UserEditionWidget::setModel(UserEditionModel *model)
{
    model_ = model;
}

void UserEditionWidget::setSession(Session *session)
{
    this->session = session;
}

void UserEditionWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
        update();
        created_ = true;
    }

    WTemplate::render(flags);
}

Wt::WFormWidget *UserEditionWidget::createFormWidget(UserEditionModel::Field field)
{
    Wt::WFormWidget *result = 0;

    if (field == UserEditionModel::FirstName)
    {
        result = new Wt::WLineEdit();
//        result->changed().connect(boost::bind(&RegistrationWidget::checkOrganization, this));
    }
    else if (field == UserEditionModel::LastName)
    {
        result = new Wt::WLineEdit();
    }
    else if (field == UserEditionModel::Email)
    {
        result = new Wt::WLineEdit();
    }
    else if (field == UserEditionModel::ChoosePasswordField)
    {
        Wt::WLineEdit *p = new Wt::WLineEdit();
        p->setEchoMode(Wt::WLineEdit::Password);
//        p->keyWentUp().connect
//                (boost::bind(&RegistrationWidget::checkPassword, this));
//        p->changed().connect
//                (boost::bind(&RegistrationWidget::checkPassword, this));
        result = p;
    }
    else if (field == UserEditionModel::RepeatPasswordField)
    {
        Wt::WLineEdit *p = new Wt::WLineEdit();
        p->setEchoMode(Wt::WLineEdit::Password);
//        p->changed().connect
//                (boost::bind(&RegistrationWidget::checkPassword2, this));
        result = p;
    }
    else if (field == UserEditionModel::Role)
    {
        Wt::WComboBox *combo = new Wt::WComboBox();
        result = combo;
    }
    else if (field == UserEditionModel::State)
    {
        result = new Wt::WComboBox();
    }
    else if (field == UserEditionModel::MediaEMail)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&UserEditionWidget::checkMediaEmail, this));
        
    }
    else if (field == UserEditionModel::MediaSMS)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&UserEditionWidget::checkMediaSms, this));
    }
    else if (field == UserEditionModel::MediaMobileApp)
    {
        result = new Wt::WLineEdit();
    }

    return result;
}

void UserEditionWidget::checkMediaEmail()
{
    updateModelField(model_, UserEditionModel::MediaEMail);
    model_->validateField(UserEditionModel::MediaEMail);
    model_->setValidated(UserEditionModel::MediaEMail, false);
    update();
}

void UserEditionWidget::checkMediaSms()
{
    updateModelField(model_, UserEditionModel::MediaSMS);
    model_->validateField(UserEditionModel::MediaSMS);
    model_->setValidated(UserEditionModel::MediaSMS, false);
    update();
}

void UserEditionWidget::update()
{
//    if (!model_->passwordAuth() && !model_->oAuth().empty())
//        bindString("password-description",
//                   tr("Wt.Auth.password-or-oauth-registration"));
//    else
//        bindEmpty("password-description");

    updateView(model_);

//    if (!created_)
//    {
//        WLineEdit *password = resolve<WLineEdit *>
//                (RegistrationModel::ChoosePasswordField);
//        WLineEdit *password2 = resolve<WLineEdit *>
//                (RegistrationModel::RepeatPasswordField);
//        WText *password2Info = resolve<WText *>
//                (RegistrationModel::RepeatPasswordField + std::string("-info"));
//        
//        // FIXME : usefull ?
////        resolve<WLineEdit *>(RegistrationModel::TestField);
//        
//        if (password && password2 && password2Info)
//            model_->validatePasswordsMatchJS(password, password2, password2Info);
//    }

//    WAnchor *isYou = resolve<WAnchor *>("confirm-is-you");
//    if (!isYou)
//    {
//        isYou = new WAnchor(std::string("#"), tr("Wt.Auth.confirm-is-you"));
//        isYou->hide();
//        bindWidget("confirm-is-you", isYou);
//    }

//    if (model_->isConfirmUserButtonVisible())
//    {
//        if (!isYou->clicked().isConnected())
//            isYou->clicked().connect(this, &RegistrationWidget::confirmIsYou);
//        isYou->show();
//    }
//    else
//        isYou->hide();

//    if (model_->isFederatedLoginVisible())
//    {
//        if (!conditionValue("if:oauth"))
//        {
//            setCondition("if:oauth", true);
//            if (model_->passwordAuth())
//                bindString("oauth-description", tr("Wt.Auth.or-oauth-registration"));
//            else
//                bindString("oauth-description", tr("Wt.Auth.oauth-registration"));
//
//            WContainerWidget *icons = new WContainerWidget();
//            icons->addStyleClass("Wt-field");
//
//            for (unsigned i = 0; i < model_->oAuth().size(); ++i)
//            {
//                const OAuthService *service = model_->oAuth()[i];
//
//                WImage *w = new WImage("css/oauth-" + service->name() + ".png", icons);
//                w->setToolTip(service->description());
//                w->setStyleClass("Wt-auth-icon");
//                w->setVerticalAlignment(AlignMiddle);
//                OAuthProcess * const process
//                        = service->createProcess(service->authenticationScope());
//                w->clicked().connect(process, &OAuthProcess::startAuthenticate);
//
//                process->authenticated().connect
//                        (boost::bind(&RegistrationWidget::oAuthDone, this, process, _1));
//
//                WObject::addChild(process);
//            }
//
//            bindWidget("icons", icons);
//        }
//    }
//    else
//    {
//        setCondition("if:oauth", false);
//        bindEmpty("icons");
//    }
//
    if (!created_)
    {
        Wt::WPushButton *addButtonEmail = new Wt::WPushButton(tr("Alert.user.edition.add-button"));
        Wt::WPushButton *deleteButtonEmail = new Wt::WPushButton(tr("Alert.user.edition.delete-button"));

        bindWidget("add-button-email", addButtonEmail);
        bindWidget("delete-button-email", deleteButtonEmail);

        addButtonEmail->clicked().connect(this, &UserEditionWidget::addEmail);
        deleteButtonEmail->clicked().connect(this, &UserEditionWidget::deleteEmail);
        
        Wt::WPushButton *addButtonSms = new Wt::WPushButton(tr("Alert.user.edition.add-button"));
        Wt::WPushButton *deleteButtonSms = new Wt::WPushButton(tr("Alert.user.edition.delete-button"));

        bindWidget("add-button-sms", addButtonSms);
        bindWidget("delete-button-sms", deleteButtonSms);

        addButtonSms->clicked().connect(this, &UserEditionWidget::addSms);
        deleteButtonSms->clicked().connect(this, &UserEditionWidget::deleteSms);

        mediaEmailSelectionBox = new Wt::WSelectionBox();
        mediaSmsSelectionBox = new Wt::WSelectionBox();
        mediaEmailSelectionBox->setModel(getMediasForCurrentUser(1));
        mediaSmsSelectionBox->setModel(getMediasForCurrentUser(0));
        
        bindWidget("email-sbox", mediaEmailSelectionBox);
        bindWidget("sms-sbox", mediaSmsSelectionBox);
        
        Wt::WPushButton *saveButton = new Wt::WPushButton(tr("Alert.user.edition.save-button"));
        Wt::WPushButton *cancelButton = new Wt::WPushButton(tr("Alert.user.edition.cancel-button"));

        bindWidget("save-button", saveButton);
        bindWidget("cancel-button", cancelButton);
        
        created_ = true;
    }
}

Wt::WStringListModel *UserEditionWidget::getMediasForCurrentUser(int mediaType)
{
    Wt::WStringListModel *res = new Wt::WStringListModel();
        
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> > medias = session->find<MediaValue>().where("\"MEV_USR_USR_ID\" = ?").bind(model_->user->self().id())
                                                                                            .where("\"MEV_MED_MED_ID\" = ?").bind(mediaType);
        int idx = 0;
        for (Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> >::const_iterator i = medias.begin(); i != medias.end(); ++i)
        {
            res->insertString(idx,(*i)->value);
            idx++;
        }
    }
    return res;
}

void UserEditionWidget::addMedia(UserEditionModel::Field field, int medId, Wt::WSelectionBox *sBox)
{
    if (model_->validateField(field))
    {
        Wt::WString emailToAdd = model_->valueText(field);
        {
            Wt::Dbo::Transaction transaction(*session);
            Wt::Dbo::ptr<User> ptrUser = model_->user->self();
            Wt::Dbo::ptr<Media> media = session->find<Media>().where("\"MED_ID\" = ?").bind(medId);
            
            MediaValue *mev = new MediaValue();
            mev->user= ptrUser;
            mev->media = media;
            mev->notifEndOfAlert = false;
            mev->snoozeDuration = 0;
            mev->value = emailToAdd;
            session->add<MediaValue>(mev);
        }
    }
    else
    {
        //todo
    }
    sBox->setModel(getMediasForCurrentUser(medId));
    sBox->refresh();
    
    const std::string emptyString="";
    model_->setValue(field,boost::any(emptyString));
    update();
}

void UserEditionWidget::deleteMedia(int medId, Wt::WSelectionBox *sBox)
{
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::ptr<MediaValue> ptdMevToDelete = session->find<MediaValue>().where("\"MEV_VALUE\" = ?").bind(sBox->valueText())
                                   .where("\"MEV_USR_USR_ID\" = ?").bind(model_->user->self().id());
        ptdMevToDelete.remove();
    }
    sBox->setModel(getMediasForCurrentUser(medId));
    sBox->refresh();
    update(); 
}

void UserEditionWidget::addEmail()
{
    addMedia(model_->MediaEMail,1,mediaEmailSelectionBox);
}

void UserEditionWidget::deleteEmail()
{
    deleteMedia(1,mediaEmailSelectionBox);  
}

void UserEditionWidget::addSms()
{
    addMedia(model_->MediaSMS,0,mediaSmsSelectionBox);
}

void UserEditionWidget::deleteSms()
{
    deleteMedia(0,mediaSmsSelectionBox);  
}

//void RegistrationWidget::checkLoginName()
//{
//    updateModelField(model_, RegistrationModel::LoginNameField);
//    model_->validateField(RegistrationModel::LoginNameField);
//    model_->setValidated(RegistrationModel::LoginNameField, false);
//    update();
//}

//void RegistrationWidget::checkFirstName()
//{
//    updateModelField(model_, RegistrationModel::FirstNameField);
//    model_->validateField(RegistrationModel::FirstNameField);
//    model_->setValidated(RegistrationModel::FirstNameField, false);
//    update();
//}
//
//void RegistrationWidget::checkLastName()
//{
//    updateModelField(model_, RegistrationModel::LastNameField);
//    model_->validateField(RegistrationModel::LastNameField);
//    model_->setValidated(RegistrationModel::LastNameField, false);
//    update();
//}
//
//void RegistrationWidget::checkOrganization()
//{
//    updateModelField(model_, RegistrationModel::OrganizationTypeCompanyField);
//    updateModelField(model_, RegistrationModel::OrganizationTypeIndividualField);
//    updateModelField(model_, RegistrationModel::OrganizationTypeAssociationField);
//    updateModelField(model_, RegistrationModel::OrganizationNameField);  
//    
//    bool bCompagny = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeCompanyField].value);
//    if (bCompagny)
//    {
//        model_->setRegistrationType(RegistrationModel::Company);
//        model_->fields_[model_->OrganizationNameField].readOnly = false;
//    }
//    bool bIndividual = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeIndividualField].value);
//    if (bIndividual)
//    {
//        model_->setRegistrationType(RegistrationModel::Individual);
//        model_->fields_[model_->OrganizationNameField].readOnly = true;
//        
//        const std::string emptyString="";
//        model_->fields_[model_->OrganizationNameField].value = boost::any(emptyString);
//    }
//    bool bAssociation = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeAssociationField].value);
//    if (bAssociation)
//    {
//        model_->setRegistrationType(RegistrationModel::Association);
//        model_->fields_[model_->OrganizationNameField].readOnly = false;
//    }
//    model_->validateField(RegistrationModel::OrganizationNameField);
//    model_->setValidated(RegistrationModel::OrganizationNameField, false);
//    update();
//}
//
//void RegistrationWidget::checkPassword()
//{
//    updateModelField(model_, RegistrationModel::LoginNameField);
//    updateModelField(model_, RegistrationModel::ChoosePasswordField);
//    updateModelField(model_, RegistrationModel::EmailField);
//    model_->validateField(RegistrationModel::ChoosePasswordField);
//    model_->setValidated(RegistrationModel::ChoosePasswordField, false);
//    update();
//}
//
//void RegistrationWidget::checkPassword2()
//{
//    updateModelField(model_, RegistrationModel::ChoosePasswordField);
//    updateModelField(model_, RegistrationModel::RepeatPasswordField);
//    model_->validateField(RegistrationModel::RepeatPasswordField);
//    model_->setValidated(RegistrationModel::RepeatPasswordField, false);
//    update();
//}

bool UserEditionWidget::validate()
{
    return model_->validate();
}

void UserEditionWidget::doRegister()
{
//    std::auto_ptr<AbstractUserDatabase::Transaction>
//            t(model_->users().startTransaction());
//
//    updateModel(model_);
//
//    if (validate())
//    {
//        User user = model_->doRegister();
//        if (user.isValid())
//        {
//            registerUserDetails(user);
//            model_->login().login(user);
//        }
//        else
//            update();
//    }
//    else
//        update();
//
//    if (t.get())
//        t->commit();
}

void UserEditionWidget::registerUserDetails(User& user)
{
//    Session *session = static_cast<Session*>(dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().get()->session());
//    
//    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->firstName = model_->valueText(model_->FirstNameField);
//    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->eMail = model_->valueText(model_->LoginNameField);
//    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->lastName = model_->valueText(model_->LastNameField);
//    
//    Organization *org = new Organization();
//    
//    
//    Wt::Dbo::ptr<OrganizationType> type;
//    bool bCompagny = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeCompanyField].value);
//    if (bCompagny)
//    {
//        type = session->find<OrganizationType>().where("\"OTY_ID\" = ?").bind(OrganizationType::Company);
//        org->name = model_->valueText(model_->OrganizationNameField);
//    }
//    bool bIndividual = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeIndividualField].value);
//    if (bIndividual)
//    {
//        type = session->find<OrganizationType>().where("\"OTY_ID\" = ?").bind(OrganizationType::Individual);
//        org->name = model_->valueText(model_->LastNameField);
//    }
//    bool bAssociation = boost::any_cast<bool>(model_->fields_[model_->OrganizationTypeAssociationField].value);
//    if (bAssociation)
//    {
//        type = session->find<OrganizationType>().where("\"OTY_ID\" = ?").bind(OrganizationType::Association);
//        org->name = model_->valueText(model_->OrganizationNameField);
//    }
//    
//    org->organizationType = type;
//    
//    Wt::Dbo::ptr<Organization> ptrOrg = session->add<Organization>(org);
////    Wt::Dbo::collection<Wt::Dbo::ptr<Organization> > colPtrOrg;
////    colPtrOrg.insert(ptrOrg);
//    
//    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->organizations.insert(ptrOrg);
}

void UserEditionWidget::close()
{
    delete this;
}

//void UserEditionWidget::confirmIsYou()
//{
//    updateModel(model_);
//
//    switch (model_->confirmIsExistingUser())
//    {
//    case RegistrationModel::ConfirmWithPassword:
//    {
//        delete confirmPasswordLogin_;
//        confirmPasswordLogin_ = new Login();
//        confirmPasswordLogin_->login(model_->existingUser(), WeakLogin);
//        confirmPasswordLogin_
//                ->changed().connect(this, &RegistrationWidget::confirmedIsYou);
//
//        WDialog *dialog =
//                authWidget_->createPasswordPromptDialog(*confirmPasswordLogin_);
//        dialog->show();
//    }
//
//        break;
//    case RegistrationModel::ConfirmWithEmail:
//        // FIXME send a confirmation email to merge the new identity
//        // with the existing one. We need to include the provisional
//        // id in the token -- no problem there, integrity is verified by a
//        // hash in the database
//
//        log("INFO") << "confirming a new identity to existing user not yet implemented";
//
//        break;
//    default:
//        log("ERROR") << "that's gone haywire.";
//    }
//}

//void UserEditionWidget::confirmedIsYou()
//{
//    if (confirmPasswordLogin_->state() == StrongLogin)
//    {
//        model_->existingUserConfirmed();
//    }
//    else
//    {
//        delete confirmPasswordLogin_;
//        confirmPasswordLogin_ = 0;
//    }
//}
