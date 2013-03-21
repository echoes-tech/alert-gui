/* 
 * File:   RegistrationWidgetAlert.cpp
 * Author: tsa
 * 
 * Created on 17 janvier 2013, 19:41
 */


#include "Auth/RegistrationWidgetAlert.h"

RegistrationWidgetAlert::RegistrationWidgetAlert(Wt::Auth::AuthWidget *authWidget) 
: Wt::WTemplateFormView(tr("Wt.Auth.template.registration.echoes.alert")),
    authWidget_(authWidget),
    model_(0),
    created_(false),
    confirmPasswordLogin_(0)
{
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("auth",false);
    organizationGroup = new Wt::WButtonGroup(this);
}


RegistrationWidgetAlert::~RegistrationWidgetAlert() {
    delete organizationGroup;
}

void RegistrationWidgetAlert::setModel(RegistrationModelAlert *model)
{
    if (!model_ && model)
      model->login().changed().connect(this, &RegistrationWidgetAlert::close);

    delete model_;
    model_ = model;
}

void RegistrationWidgetAlert::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_) {
      update();
      created_ = true;
    }

    Wt::WTemplate::render(flags);
}

void RegistrationWidgetAlert::registerUserDetails(Wt::Auth::User& user)
{
    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->firstName = model()->valueText(reinterpret_cast<RegistrationModelAlert*>(model())->FirstNameField);
    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->eMail = model()->valueText(model()->LoginNameField);
    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->lastName = model()->valueText(reinterpret_cast<RegistrationModelAlert*>(model())->LastNameField);

    Organization *org = new Organization();


    Wt::Dbo::ptr<OrganizationType> type;
    bool bCompagny = boost::any_cast<bool>(model()->value(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationTypeCompanyField));
    if (bCompagny)
    {
        type = ((UserDatabase*)user.database())->session_.find<OrganizationType>().where("\"OTY_ID\" = ?").bind(OrganizationType::Company);
        org->name = model()->valueText(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationNameField);
    }
    bool bIndividual = boost::any_cast<bool>(model()->value(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationTypeIndividualField));
    if (bIndividual)
    {
        type = ((UserDatabase*)user.database())->session_.find<OrganizationType>().where("\"OTY_ID\" = ?").bind(OrganizationType::Individual);
        org->name = model()->valueText(reinterpret_cast<RegistrationModelAlert*>(model())->LastNameField);
    }
    bool bAssociation = boost::any_cast<bool>(model()->value(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationTypeAssociationField));
    if (bAssociation)
    {
        type = ((UserDatabase*)user.database())->session_.find<OrganizationType>().where("\"OTY_ID\" = ?").bind(OrganizationType::Association);
        org->name = model()->valueText(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationNameField);
    }

    //triche
    type = ((UserDatabase*)user.database())->session_.find<OrganizationType>().where("\"OTY_ID\" = ?").bind(OrganizationType::Individual);
    org->name = model()->valueText(reinterpret_cast<RegistrationModelAlert*>(model())->LastNameField);

    org->organizationType = type;
    org->token = reinterpret_cast<RegistrationModelAlert*>(model())->generateToken();



    Wt::Dbo::ptr<Organization> ptrOrg = ((UserDatabase*)user.database())->session_.add<Organization>(org);

    //TODO : hardcoded, should be changed when the pack selection will be available
    Wt::Dbo::ptr<Pack> ptrPack = ((UserDatabase*)user.database())->session_.find<Pack>().where("\"PCK_ID\" = ?").bind(1);
    ptrOrg.modify()->pack = ptrPack;

    OptionValue *optionValue = new OptionValue();

    OptionValueId *opvId = new OptionValueId(ptrOrg,((UserDatabase*)user.database())->session_.find<Option>().where("\"OPT_ID\" = ?").bind(Enums::sms));
    optionValue->pk.option = opvId->option;
    optionValue->pk.organization = opvId->organization;
    //FIXME : should be the default value found in the table POP
    optionValue->value = "5";

    Wt::Dbo::ptr<OptionValue> ptrOptionValue = ((UserDatabase*)user.database())->session_.add<OptionValue>(optionValue);


//    Wt::Dbo::collection<Wt::Dbo::ptr<Organization> > colPtrOrg;
//    colPtrOrg.insert(ptrOrg);

    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->organizations.insert(ptrOrg);
    dynamic_cast<UserDatabase*>(user.database())->find(user).get()->user().modify()->currentOrganization = ptrOrg;
}



Wt::WFormWidget *RegistrationWidgetAlert::createFormWidget(Wt::WFormModel::Field field)
{
    Wt::WFormWidget *result = 0;

    if (strcmp(field,RegistrationModelAlert::OrganizationTypeCompanyField) == 0)
    {
        Wt::WRadioButton *button = new Wt::WRadioButton("");
        organizationGroup->addButton(button, 1);
        result = button;
        result->changed().connect(boost::bind(&RegistrationWidgetAlert::checkOrganization, this));
    }
    else if (strcmp(field,RegistrationModelAlert::OrganizationTypeIndividualField) == 0)
    {
        Wt::WRadioButton *button = new Wt::WRadioButton("");
        organizationGroup->addButton(button, 2);
        result = button;
        result->changed().connect(boost::bind(&RegistrationWidgetAlert::checkOrganization, this));
    }
    else if (strcmp(field,RegistrationModelAlert::OrganizationTypeAssociationField) == 0)
    {
        Wt::WRadioButton *button = new Wt::WRadioButton("");
        organizationGroup->addButton(button, 3);
        result = button;
        result->changed().connect(boost::bind(&RegistrationWidgetAlert::checkOrganization, this));
    }
    else if (strcmp(field,RegistrationModelAlert::OrganizationNameField) == 0)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&RegistrationWidgetAlert::checkOrganization, this));
    }
    else if (strcmp(field,Wt::Auth::RegistrationModel::LoginNameField) == 0)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&RegistrationWidgetAlert::checkLoginName, this));
    }
    else if (strcmp(field,Wt::Auth::RegistrationModel::EmailField) == 0)
    {
        result = new Wt::WLineEdit();
    }
    else if (strcmp(field,RegistrationModelAlert::TestField) == 0)
    {
        result = new Wt::WLineEdit();
    }
    else if (strcmp(field,RegistrationModelAlert::FirstNameField) == 0)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&RegistrationWidgetAlert::checkFirstName, this));
    }
    else if (strcmp(field,RegistrationModelAlert::LastNameField) == 0)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&RegistrationWidgetAlert::checkLastName, this));
    }
    else if (strcmp(field,Wt::Auth::RegistrationModel::ChoosePasswordField) == 0)
    {
        Wt::WLineEdit *p = new Wt::WLineEdit();
        p->setEchoMode(Wt::WLineEdit::Password);
//        p->keyWentUp().connect
//                (boost::bind(&RegistrationWidgetAlert::checkPassword3, this));
        p->changed().connect
                (boost::bind(&RegistrationWidgetAlert::checkPassword, this));
        result = p;
    }
    else if (strcmp(field,Wt::Auth::RegistrationModel::RepeatPasswordField) == 0)
    {
        Wt::WLineEdit *p = new Wt::WLineEdit();
        p->setEchoMode(Wt::WLineEdit::Password);
        p->changed().connect
                (boost::bind(&RegistrationWidgetAlert::checkPassword2, this));
        result = p;
    }

    return result;
}

void RegistrationWidgetAlert::checkFirstName()
{
    updateModelField(model(), RegistrationModelAlert::FirstNameField);
    model()->validateField(RegistrationModelAlert::FirstNameField);
    model()->setValidated(RegistrationModelAlert::FirstNameField, false);
    update();
}

void RegistrationWidgetAlert::checkLastName()
{
    updateModelField(model(), RegistrationModelAlert::LastNameField);
    model()->validateField(RegistrationModelAlert::LastNameField);
    model()->setValidated(RegistrationModelAlert::LastNameField, false);
    update();
}

void RegistrationWidgetAlert::checkOrganization()
{
    updateModelField(model(), RegistrationModelAlert::OrganizationTypeCompanyField);
    updateModelField(model(), RegistrationModelAlert::OrganizationTypeIndividualField);
    updateModelField(model(), RegistrationModelAlert::OrganizationTypeAssociationField);
    updateModelField(model(), RegistrationModelAlert::OrganizationNameField);  
    
    //template<typename T> T any_cast(any & operand);
    
    bool bCompagny = boost::any_cast<bool>(model()->value(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationTypeCompanyField));
    if (bCompagny)
    {
        reinterpret_cast<RegistrationModelAlert*>(model())->setRegistrationType(RegistrationModelAlert::Company);
        model()->setReadOnly(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationNameField,false);
    }
    bool bIndividual = boost::any_cast<bool>(model()->value(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationTypeIndividualField));
    if (bIndividual)
    {
        reinterpret_cast<RegistrationModelAlert*>(model())->setRegistrationType(RegistrationModelAlert::Individual);
        model()->setReadOnly(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationNameField,true);
        
        const std::string emptyString="";
        model()->setValue(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationNameField,boost::any(emptyString));
    }
    bool bAssociation = boost::any_cast<bool>(model()->value(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationTypeAssociationField));
    if (bAssociation)
    {
        reinterpret_cast<RegistrationModelAlert*>(model())->setRegistrationType(RegistrationModelAlert::Association);
        reinterpret_cast<RegistrationModelAlert*>(model())->setReadOnly(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationNameField,false);
    }
    model()->validateField(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationNameField);
    model()->setValidated(reinterpret_cast<RegistrationModelAlert*>(model())->OrganizationNameField, false);
    update();
}

void RegistrationWidgetAlert::checkLoginName()
{
    updateModelField(model(), Wt::Auth::RegistrationModel::LoginNameField);
    model()->validateField(Wt::Auth::RegistrationModel::LoginNameField);
    model()->setValidated(Wt::Auth::RegistrationModel::LoginNameField, false);
    update();
}

void RegistrationWidgetAlert::checkPassword()
{
    Wt::WTemplateFormView::updateModelField(model(), Wt::Auth::RegistrationModel::LoginNameField);
    Wt::WTemplateFormView::updateModelField(model(), Wt::Auth::RegistrationModel::ChoosePasswordField);
//    updateModelField(model(), Wt::Auth::RegistrationModel::EmailField);
    model()->validateField(Wt::Auth::RegistrationModel::ChoosePasswordField);
    model()->setValidated(Wt::Auth::RegistrationModel::ChoosePasswordField, false);
    update();
}

void RegistrationWidgetAlert::checkPassword2()
{
    updateModelField(model(), Wt::Auth::RegistrationModel::ChoosePasswordField);
    updateModelField(model(), Wt::Auth::RegistrationModel::RepeatPasswordField);
    model()->validateField(Wt::Auth::RegistrationModel::RepeatPasswordField);
    model()->setValidated(Wt::Auth::RegistrationModel::RepeatPasswordField, false);
    update();
}

void RegistrationWidgetAlert::update() {
    if (!model()->passwordAuth() && !model()->oAuth().empty())
        Wt::WTemplateFormView::bindString("password-description",
            Wt::WString::tr("Wt.Auth.password-or-oauth-registration"));
    else
        Wt::WTemplateFormView::bindEmpty("password-description");

    updateView(model());

    if (!created_) {
        Wt::WLineEdit *password = resolve<Wt::WLineEdit *>
                (RegistrationModelAlert::ChoosePasswordField);
        Wt::WLineEdit *password2 = resolve<Wt::WLineEdit *>
                (RegistrationModelAlert::RepeatPasswordField);
        Wt::WText *password2Info = resolve<Wt::WText *>
                (RegistrationModelAlert::RepeatPasswordField + std::string("-info"));

        if (password && password2 && password2Info)
            model()->validatePasswordsMatchJS(password, password2, password2Info);
    }

    Wt::WAnchor *isYou = resolve<Wt::WAnchor *>("confirm-is-you");
    if (!isYou) {
        isYou = new Wt::WAnchor(std::string("#"), tr("Wt.Auth.confirm-is-you"));
        isYou->hide();
        bindWidget("confirm-is-you", isYou);
    }

    if (model()->isConfirmUserButtonVisible()) {
        if (!isYou->clicked().isConnected())
            isYou->clicked().connect(this, &RegistrationWidgetAlert::confirmIsYou);
        isYou->show();
    } else
        isYou->hide();

    if (model()->isFederatedLoginVisible()) {
        if (!conditionValue("if:oauth")) {
            setCondition("if:oauth", true);
            if (model()->passwordAuth())
                bindString("oauth-description", tr("Wt.Auth.or-oauth-registration"));
            else
                bindString("oauth-description", tr("Wt.Auth.oauth-registration"));

            Wt::WContainerWidget *icons = new Wt::WContainerWidget();
            icons->addStyleClass("Wt-field");

            for (unsigned i = 0; i < model()->oAuth().size(); ++i) {
                const Wt::Auth::OAuthService *service = model()->oAuth()[i];

                Wt::WImage *w = new Wt::WImage("css/oauth-" + service->name() + ".png", icons);
                w->setToolTip(service->description());
                w->setStyleClass("Wt-auth-icon");
                w->setVerticalAlignment(Wt::AlignMiddle);
                Wt::Auth::OAuthProcess * const process
                        = service->createProcess(service->authenticationScope());
                w->clicked().connect(process, &Wt::Auth::OAuthProcess::startAuthenticate);

                process->authenticated().connect
                        (boost::bind(&RegistrationWidgetAlert::oAuthDone, this, process, _1));

                Wt::WObject::addChild(process);
            }

            bindWidget("icons", icons);
        }
    } else {
        setCondition("if:oauth", false);
        bindEmpty("icons");
    }

    if (!created_) {
        Wt::WPushButton *okButton = new Wt::WPushButton(tr("Wt.Auth.register"));
        Wt::WPushButton *cancelButton = new Wt::WPushButton(tr("Wt.WMessageBox.Cancel"));

        bindWidget("ok-button", okButton);
        bindWidget("cancel-button", cancelButton);

        okButton->clicked().connect(this, &RegistrationWidgetAlert::doRegister);
        cancelButton->clicked().connect(this, &RegistrationWidgetAlert::close);

        created_ = true;
    }
}

void RegistrationWidgetAlert::oAuthDone(Wt::Auth::OAuthProcess *oauth, const Wt::Auth::Identity& identity) {
    if (identity.isValid()) {

        if (!model()->registerIdentified(identity))
            update();
    } else {
        if (authWidget_)
            authWidget_->displayError(oauth->error());
    }
}

bool RegistrationWidgetAlert::validate() {
    return model()->validate();
}

void RegistrationWidgetAlert::doRegister() {
    std::auto_ptr<Wt::Auth::AbstractUserDatabase::Transaction>
            t(model()->users().startTransaction());
    
    updateModel(model());

    if (validate()) {
        Wt::Auth::User user = model()->doRegister();
        if (user.isValid()) {
            registerUserDetails(user);
            model()->login().login(user);
        } else
            update();
    } else
        update();

    if (t.get())
        t->commit();
}

void RegistrationWidgetAlert::close() {
    delete this;
}

void RegistrationWidgetAlert::confirmIsYou() {
    updateModel(model());

    switch (model()->confirmIsExistingUser()) {
        case RegistrationModelAlert::ConfirmWithPassword:
        {
            delete confirmPasswordLogin_;
            confirmPasswordLogin_ = new Wt::Auth::Login();
            confirmPasswordLogin_->login(model()->existingUser(), Wt::Auth::WeakLogin);
            confirmPasswordLogin_
                    ->changed().connect(this, &RegistrationWidgetAlert::confirmedIsYou);

            Wt::WDialog *dialog =
                    authWidget_->createPasswordPromptDialog(*confirmPasswordLogin_);
            dialog->show();
        }

            break;
        case RegistrationModelAlert::ConfirmWithEmail:
            // FIXME send a confirmation email to merge the new identity
            // with the existing one. We need to include the provisional
            // id in the token -- no problem there, integrity is verified by a
            // hash in the database

            //LOG_INFO("confirming a new identity to existing user not yet implemented");

            break;
        default:
            break;
            //LOG_ERROR("that's gone haywire.");
    }
}

void RegistrationWidgetAlert::confirmedIsYou() {
    if (confirmPasswordLogin_->state() == Wt::Auth::StrongLogin) {
        model_->existingUserConfirmed();
    } else {
        delete confirmPasswordLogin_;
        confirmPasswordLogin_ = 0;
    }
}
