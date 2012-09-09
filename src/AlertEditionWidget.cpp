/* 
 * File:   AlertEditionWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "AlertEditionWidget.h"

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
#include <Wt/Dbo/Query>


#include "tools/Session.h"
//#include "Login.h"
#include "user/User.h"
#include "UserEditionModel.h"


#include <memory>


AlertEditionWidget::AlertEditionWidget()
: Wt::WTemplateFormView(Wt::WString::tr("Alert.Alert.Edition.template"))
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("alert",false);
}

void AlertEditionWidget::setModel(AlertEditionModel *model)
{
    delete model_;
    model_ = model;
}

void AlertEditionWidget::setSession(Session *session)
{
    delete this->session;
    this->session = session;
}

void AlertEditionWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
        update();
        created_ = true;
    }

    WTemplate::render(flags);
}

Wt::WFormWidget *AlertEditionWidget::createFormWidget(UserEditionModel::Field field)
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
        result->changed().connect(boost::bind(&AlertEditionWidget::checkMediaEmail, this));
        
    }
    else if (field == UserEditionModel::MediaSMS)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&AlertEditionWidget::checkMediaSms, this));
    }
    else if (field == UserEditionModel::MediaMobileApp)
    {
        result = new Wt::WLineEdit();
    }

    return result;
}

void AlertEditionWidget::checkMediaEmail()
{
    updateModelField(model_, UserEditionModel::MediaEMail);
    model_->validateField(UserEditionModel::MediaEMail);
    model_->setValidated(UserEditionModel::MediaEMail, false);
    update();
}

void AlertEditionWidget::checkMediaSms()
{
    updateModelField(model_, UserEditionModel::MediaSMS);
    model_->validateField(UserEditionModel::MediaSMS);
    model_->setValidated(UserEditionModel::MediaSMS, false);
    update();
}

void AlertEditionWidget::update()
{

    updateView(model_);

    if (!created_)
    {

        serverSelectionBox = new Wt::WSelectionBox();
        applicationSelectionBox = new Wt::WSelectionBox();
        informationSelectionBox = new Wt::WSelectionBox();
        
        Wt::WStringListModel *slmServer = new Wt::WStringListModel;
        Wt::Dbo::collection<Wt::Dbo::ptr<Asset> > assets;
        Wt::Dbo::Query<Wt::Dbo::ptr<Asset> > query;
        std::string queryString;
        {
            Wt::Dbo::Transaction transaction(*session);
            queryString =  "SELECT ast FROM \"T_ASSET_AST\" ast where \"AST_PRB_PRB_ID\" IN"
            "("
                "SELECT \"PRB_ID\" FROM \"T_PROBE_PRB\" where \"PRB_ORG_ORG_ID\" IN "
                "("
                    "select \"T_ORGANIZATION_ORG_ORG_ID\" from \"TJ_USR_ORG\" where \"T_USER_USR_USR_ID\" = 0"
                ")"
            ");";
            
            query = session->query<Wt::Dbo::ptr<Asset> >(queryString);//.bind(session->user().id());
            assets = query.resultList();
            
            int idx = 0;
            for (Wt::Dbo::collection<Wt::Dbo::ptr<Asset> >::const_iterator i = assets.begin(); i != assets.end(); ++i)
            {
                Wt::log("info") << (*i).get()->name;
                slmServer->insertString(idx,(*i)->name);
                idx++;
            }
        }
        serverSelectionBox->setModel(slmServer);
        bindWidget("server-sbox", serverSelectionBox);
        
        // applications
        Wt::WStringListModel *slmApplication = new Wt::WStringListModel;
        {
            Wt::Dbo::Transaction transaction(*session);
            query = session->query<Wt::Dbo::ptr<Asset> >(queryString);//.bind(session->user().id());
            assets = query.resultList();
        
        
            Wt::log("info") << "avant it";
            Wt::log("info") << "it size : " << assets.size();
            int idx = 0;
            for (Wt::Dbo::collection<Wt::Dbo::ptr<Asset> >::const_iterator i = assets.begin(); i != assets.end(); ++i)
            {
                Wt::Dbo::collection<Wt::Dbo::ptr<Plugin> > plugins = (*i).get()->plugins;
                for (Wt::Dbo::collection<Wt::Dbo::ptr<Plugin> >::const_iterator j = plugins.begin(); j != plugins.end(); ++j)
                {
                    Wt::log("info") << "plugin : " << (*j).get()->name;
                    slmApplication->insertString(idx,(*j).get()->name);
                    idx++;
                }
            }
        }
        Wt::log("info") << "apres it";
        applicationSelectionBox->setModel(slmApplication);
        bindWidget("application-sbox", applicationSelectionBox);
        
        
        
        created_ = true;
    }
}

Wt::WStringListModel *AlertEditionWidget::getMediasForCurrentUser(int mediaType)
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

void AlertEditionWidget::addMedia(UserEditionModel::Field field, int medId, Wt::WSelectionBox *sBox)
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

void AlertEditionWidget::deleteMedia(int medId, Wt::WSelectionBox *sBox)
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

void AlertEditionWidget::addEmail()
{
//    addMedia(model_->MediaEMail,1,mediaEmailSelectionBox);
}

void AlertEditionWidget::deleteEmail()
{
//    deleteMedia(1,mediaEmailSelectionBox);  
}

void AlertEditionWidget::addSms()
{
//    addMedia(model_->MediaSMS,0,mediaSmsSelectionBox);
}

void AlertEditionWidget::deleteSms()
{
//    deleteMedia(0,mediaSmsSelectionBox);  
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

bool AlertEditionWidget::validate()
{
    return model_->validate();
}

void AlertEditionWidget::doRegister()
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

void AlertEditionWidget::registerUserDetails(User& user)
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

void AlertEditionWidget::close()
{
    delete this;
}

//void AlertEditionWidget::confirmIsYou()
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

//void AlertEditionWidget::confirmedIsYou()
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
