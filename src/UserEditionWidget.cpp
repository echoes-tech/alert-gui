/* 
 * File:   UserEditionWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "UserEditionWidget.h"

UserEditionWidget::UserEditionWidget()
: Wt::WTemplateFormView(Wt::WString::tr("Alert.User.Edition.template"))
{
    created_ = false;
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

//    if (field == UserEditionModel::FirstName)
//    {
//        result = new Wt::WLineEdit();
////        result->changed().connect(boost::bind(&RegistrationWidget::checkOrganization, this));
//    }
//    else if (field == UserEditionModel::LastName)
//    {
//        result = new Wt::WLineEdit();
//    }
//    else if (field == UserEditionModel::Email)
//    {
//        result = new Wt::WLineEdit();
//    }
//    else if (field == UserEditionModel::ChoosePasswordField)
//    {
//        Wt::WLineEdit *p = new Wt::WLineEdit();
//        p->setEchoMode(Wt::WLineEdit::Password);
////        p->keyWentUp().connect
////                (boost::bind(&RegistrationWidget::checkPassword, this));
////        p->changed().connect
////                (boost::bind(&RegistrationWidget::checkPassword, this));
//        result = p;
//    }
//    else if (field == UserEditionModel::RepeatPasswordField)
//    {
//        Wt::WLineEdit *p = new Wt::WLineEdit();
//        p->setEchoMode(Wt::WLineEdit::Password);
////        p->changed().connect
////                (boost::bind(&RegistrationWidget::checkPassword2, this));
//        result = p;
//    }
//    else if (field == UserEditionModel::Role)
//    {
//        Wt::WComboBox *combo = new Wt::WComboBox();
//        result = combo;
//    }
//    else if (field == UserEditionModel::State)
//    {
//        result = new Wt::WComboBox();
//    }
    if (field == UserEditionModel::MediaEMail)
    {
        emailEdit = new Wt::WLineEdit();
        result = emailEdit;
        result->changed().connect(boost::bind(&UserEditionWidget::checkMediaEmail, this));
        
    }
    else if (field == UserEditionModel::MediaSMS)
    {
        smsEdit = new Wt::WLineEdit();
        result = smsEdit;
        result->changed().connect(boost::bind(&UserEditionWidget::checkMediaSms, this));
    }
    else if (field == UserEditionModel::MediaMobileApp)
    {
        mobileappEdit = new Wt::WLineEdit();
        result = mobileappEdit;
        result->changed().connect(boost::bind(&UserEditionWidget::checkMediaMobileApp, this));
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

void UserEditionWidget::checkMediaMobileApp()
{
    updateModelField(model_, UserEditionModel::MediaMobileApp);
    model_->validateField(UserEditionModel::MediaMobileApp);
    model_->setValidated(UserEditionModel::MediaMobileApp, false);
    update();
}

void UserEditionWidget::update()
{

    updateView(model_);

    if (!created_)
    {

        emailsTable = new Wt::WTable();
        smsTable = new Wt::WTable();
        mobileappTable = new Wt::WTable();
        
        createMediaTable(Enums::email);
        createMediaTable(Enums::sms);
        createMediaTable(Enums::mobileapp);
        
        bindWidget("email-table", emailsTable);
        bindWidget("sms-table", smsTable);
        bindWidget("mobileapp-table", mobileappTable);

        
        created_ = true;
    }
    
    

}

void UserEditionWidget::createMediaTable(int medEnumId)
{
    Wt::WTable *mediasTable;
    std::string desc;
    switch (medEnumId)
    {
        case Enums::email:
        {
            mediasTable = emailsTable;
            desc = "email";
            break;
        }
        case Enums::sms:
        {
            mediasTable = smsTable;
            desc = "sms";
            break;
        }
        case Enums::mobileapp:
        {
            mediasTable = mobileappTable;
            desc = "mobileapp";
            break;
        }
    }
    
    mediasTable->addStyleClass("table");
    mediasTable->addStyleClass("table-bordered");
    mediasTable->addStyleClass("table-striped");

    int row = 0;


    mediasTable->setHeaderCount(2,Wt::Horizontal);
    mediasTable->elementAt(row, 0)->setColumnSpan(2);
    mediasTable->columnAt(1)->setStyleClass("asset-action-width");

    Wt::WText *tableMediaTitle = new Wt::WText("<div class='widget-title widget-title-ea-table'><span class='icon'><i class='icon-envelope'></i></span><h5>"+ tr("Alert.media.add-"+desc+"-table") + "</h5></div>",mediasTable->elementAt(row, 0));
    mediasTable->elementAt(row, 0)->setPadding(*(new Wt::WLength("0px")));
    tableMediaTitle->setTextFormat(Wt::XHTMLUnsafeText);
    ++row;
    new Wt::WText(tr("Alert.media."+desc+"-name"),mediasTable->elementAt(row, 0));
    new Wt::WText(tr("Alert.global.action"),mediasTable->elementAt(row, 1));




    std::map<long long,Wt::WString> mediaMap = getMediasForCurrentUser(medEnumId);
    for (std::map<long long,Wt::WString>::const_iterator i = mediaMap.begin() ; i != mediaMap.end(); ++i)
    {
        ++row;
        new Wt::WText(i->second,mediasTable->elementAt(row, 0));
        Wt::WPushButton *deleteButton = new Wt::WPushButton(mediasTable->elementAt(row, 1));
        deleteButton->setTextFormat(Wt::XHTMLUnsafeText);
        deleteButton->setText("<i class='icon-remove icon-white'></i> " + tr("Alert.user.edition.delete-button"));
        deleteButton->addStyleClass("btn");
        deleteButton->addStyleClass("btn-danger");
        deleteButton->clicked().connect(boost::bind(&UserEditionWidget::deleteMedia,this,medEnumId,i->first,mediasTable->elementAt(row, 1)));
    }

    ++row;
    Wt::WPushButton *addButton = new Wt::WPushButton(mediasTable->elementAt(row, 1));
    addButton->setTextFormat(Wt::XHTMLUnsafeText);
    addButton->setText("<i class='icon-plus icon-white'></i> " + tr("Alert.user.edition.add-button"));
    addButton->addStyleClass("btn");
    addButton->addStyleClass("btn-info");
    

    switch (medEnumId)
    {
        case Enums::email:
        {
            mediasTable->elementAt(row, 0)->addWidget(emailEdit);
            addButton->clicked().connect(this, &UserEditionWidget::addEmail);
            break;
        }
        case Enums::sms:
        {
            mediasTable->elementAt(row, 0)->addWidget(smsEdit);
            addButton->clicked().connect(this, &UserEditionWidget::addSms);
            break;
        }
        case Enums::mobileapp:
        {
            mediasTable->elementAt(row, 0)->addWidget(mobileappEdit);
            addButton->clicked().connect(this, &UserEditionWidget::addMobileApp);
            break;
        }
    }
    
}


std::map<long long, Wt::WString> UserEditionWidget::getMediasForCurrentUser(int mediaType)
{
    std::map<long long, Wt::WString> res;
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> > medias = session->find<MediaValue>().where("\"MEV_USR_USR_ID\" = ?").bind(session->user().id())
                                                                                            .where("\"MEV_MED_MED_ID\" = ?").bind(mediaType)
                                                                                            .where("\"MEV_DELETE\" IS NULL");
        int idx = 0;
        for (Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> >::const_iterator i = medias.begin(); i != medias.end(); ++i)
        {
            Wt::WString test = i->get()->value;
            res[i->id()] = test;
            idx++;
        }
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"),tr("Alert.asset.database-error").arg(e.what()).arg("1"),Wt::Ok);
        Wt::log("error") << "[UserEditionWidget] " << e.what();
    }
    return res;
}

void UserEditionWidget::addMedia(Wt::WFormModel::Field field, int medEnumId)
{
    if (model_->validateField(field))
    {
        Wt::WString mediaToAdd = model_->valueText(field);
        try
        {
            Wt::Dbo::Transaction transaction(*session);
            Wt::Dbo::ptr<User> ptrUser = session->user();
            Wt::Dbo::ptr<Media> media = session->find<Media>().where("\"MED_ID\" = ?").bind(medEnumId);
            
            MediaValue *mev = new MediaValue();
            mev->user = ptrUser;
            mev->media = media;
            mev->value = mediaToAdd;
            mev->isDefault = false;
            mev->isConfirmed = false;
            mev->token = Wt::WRandom::generateId(25);
            Wt::Dbo::ptr<MediaValue> ptrMev = session->add<MediaValue>(mev);
            ptrMev.flush();
            model_->setValidated(field,false);
            transaction.commit();
            
            Wt::WPushButton *deleteButton;
            int tableRow;
            switch(medEnumId)
            {
                case Enums::email:
                {
                    tableRow = emailsTable->rowCount() - 1;
                    emailsTable->insertRow(tableRow);
                    new Wt::WText(mediaToAdd,emailsTable->elementAt(tableRow, 0));
                    deleteButton = new Wt::WPushButton(tr("Alert.user.edition.delete-button"),emailsTable->elementAt(tableRow, 1));
                    break;
                }
                case Enums::sms:
                {
                    tableRow = smsTable->rowCount() - 1;
                    smsTable->insertRow(tableRow);
                    new Wt::WText(mediaToAdd,smsTable->elementAt(tableRow, 0));
                    deleteButton = new Wt::WPushButton(tr("Alert.user.edition.delete-button"),smsTable->elementAt(tableRow, 1));
                    break;
                }
                case Enums::mobileapp:
                {
                    tableRow = mobileappTable->rowCount() - 1;
                    mobileappTable->insertRow(tableRow);
                    new Wt::WText(mediaToAdd,mobileappTable->elementAt(tableRow, 0));
                    deleteButton = new Wt::WPushButton(tr("Alert.user.edition.delete-button"),mobileappTable->elementAt(tableRow, 1));
                    break;
                }
            }
            
            deleteButton->setTextFormat(Wt::XHTMLUnsafeText);
            deleteButton->setText("<i class='icon-remove icon-white'></i> " + tr("Alert.user.edition.delete-button"));
            deleteButton->addStyleClass("btn");
            deleteButton->addStyleClass("btn-danger");

            deleteButton->clicked().connect(boost::bind(&UserEditionWidget::deleteMedia,this,medEnumId,ptrMev.id(),smsTable->elementAt(tableRow, 1)));
            
            
            UserActionManagement::registerUserAction(Enums::add,Constants::T_MEDIA_VALUE_MEV,ptrMev.id());
        }
        catch (Wt::Dbo::Exception e)
        {
            Wt::log("error") << "[UserEditionWidget] " << e.what();
            Wt::WMessageBox::show(tr("Alert.user.problem-adding-media-title"),tr("Alert.user.problem-adding-media"),Wt::Ok);
        }
    }
    else
    {
        //todo
    }
//    sBox->setModel(getMediasForCurrentUser(medId));
//    sBox->refresh();
    
    const std::string emptyString="";
    model_->setValue(field,boost::any(emptyString));
    update();
}

void UserEditionWidget::deleteMedia(int medEnumId, long long medId, Wt::WTableCell * cell)
{
    try
    {
        Wt::Dbo::Transaction transaction2(*session);
        
        std::string qryString = "DELETE FROM \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" "
                                " WHERE \"AMS_ALE_ALE_ID\" IS NULL"
                                " AND \"AMS_MEV_MEV_ID\" IN "
                                " (SELECT \"MEV_ID\" FROM \"T_MEDIA_VALUE_MEV\" WHERE \"MEV_USR_USR_ID\" = " + boost::lexical_cast<std::string>(session->user().id())  + ")";

        session->execute(qryString);

        
        transaction2.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[UserEditionWidget] [deleteMedia]" << e.what();
//        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
//        return;
    }
    
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::ptr<MediaValue> mediaValue = session->find<MediaValue>().where("\"MEV_ID\" = ?").bind(medId)
                                                                           .where("\"MEV_MED_MED_ID\" = ?").bind(medEnumId)
                                                                           .where("\"MEV_USR_USR_ID\" = ?").bind(session->user().id());
        
        mediaValue.modify()->deleteTag = Wt::WDateTime::currentDateTime();

        transaction.commit();
        UserActionManagement::registerUserAction(Enums::del,Constants::T_MEDIA_VALUE_MEV,medId);
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[UserEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.user.dependant-alert-exists-title"),tr("Alert.user.dependant-alert-exists"),Wt::Ok);
        return;
    }
    
    switch(medEnumId)
    {
        case Enums::email:
        {
            
            emailsTable->deleteRow(cell->row());
            break;
        }
        case Enums::sms:
        {
            smsTable->deleteRow(cell->row());
            break;
        }
        case Enums::mobileapp:
        {
            mobileappTable->deleteRow(cell->row());
            break;
        }
    }
    
//    render(Wt::RenderFull); 
}

void UserEditionWidget::addMobileApp()
{
    addMedia(model_->MediaMobileApp,Enums::mobileapp);
}

void UserEditionWidget::addEmail()
{
    addMedia(model_->MediaEMail,Enums::email);
}

void UserEditionWidget::addSms()
{
    addMedia(model_->MediaSMS,Enums::sms);
}

bool UserEditionWidget::validate()
{
    return model_->validate();
}


void UserEditionWidget::close()
{
    delete this;
}
