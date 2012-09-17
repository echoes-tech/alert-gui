/* 
 * File:   UserEditionWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */
#include <Wt/WMessageBox>

#include "Enums.h"
#include "UserEditionWidget.h"


UserEditionWidget::UserEditionWidget()
: Wt::WTemplateFormView(Wt::WString::tr("Alert.User.Edition.template"))
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("user", false);
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

    updateView(model_);


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
        mediaEmailSelectionBox->setModel(getMediasForCurrentUser(Enums::email));
        mediaSmsSelectionBox->setModel(getMediasForCurrentUser(Enums::sms));
        
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
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::ptr<MediaValue> ptdMevToDelete = session->find<MediaValue>().where("\"MEV_VALUE\" = ?").bind(sBox->valueText())
                                    .where("\"MEV_MED_MED_ID\" = ?").bind(medId)
                                    .where("\"MEV_USR_USR_ID\" = ?").bind(model_->user->self().id())
                                    .limit(1);
        ptdMevToDelete.remove();
        sBox->setModel(getMediasForCurrentUser(medId));
        sBox->refresh();
        update(); 
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[UserEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.user.dependant-alert-exists-title"),tr("Alert.user.dependant-alert-exists"),Wt::Ok);
    }
    
}

void UserEditionWidget::addEmail()
{
    addMedia(model_->MediaEMail,Enums::email,mediaEmailSelectionBox);
}

void UserEditionWidget::deleteEmail()
{
    deleteMedia(1,mediaEmailSelectionBox);  
}

void UserEditionWidget::addSms()
{
    addMedia(model_->MediaSMS,Enums::sms,mediaSmsSelectionBox);
}

void UserEditionWidget::deleteSms()
{
    deleteMedia(0,mediaSmsSelectionBox);  
}


bool UserEditionWidget::validate()
{
    return model_->validate();
}


void UserEditionWidget::close()
{
    delete this;
}


