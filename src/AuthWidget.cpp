///* 
// * File:   AuthWidget.cpp
// * Author: tsa
// * 
// * Created on 14 août 2012, 10:51
// */
//
//#include "AuthWidget.h"
//
//
//
//
////namespace skeletons
////{
////extern const char * Auth_xml1;
////}
//
//namespace Wt
//{
//
//
//
//namespace Auth
//{
//
//AuthWidget::AuthWidget(const AuthService& baseAuth,
//                       AbstractUserDatabase& users, Login& login,
//                       WContainerWidget *parent)
//: WTemplateFormView(WString::Empty, parent),
//model_(new AuthModel(baseAuth, users, this)),
//login_(login)
//{
//    init();
//}
//
//AuthWidget::AuthWidget(Login& login, WContainerWidget *parent)
//: WTemplateFormView(WString::Empty, parent),
//model_(0),
//login_(login)
//{
//    init();
//}
//
//void AuthWidget::init()
//{
//    WApplication *app = WApplication::instance();
//    app->messageResourceBundle().use("auth", false);
//    registrationModel_ = 0;
//    registrationEnabled_ = false;
//    created_ = false;
//    dialog_ = 0;
//    messageBox_ = 0;
//
////    addFunction("id", WT_TEMPLATE_FUNCTION(id));
////    addFunction("tr", WT_TEMPLATE_FUNCTION(tr));
//
//    
////    app->builtinLocalizedStrings().useBuiltin(skeletons::Auth_xml1);
////     app->theme()->apply(this, this, AuthWidgets);
//}
//
//void AuthWidget::setModel(AuthModel *model)
//{
//    delete model_;
//    model_ = model;
//}
//
//void AuthWidget::setRegistrationEnabled(bool enabled)
//{
//    registrationEnabled_ = enabled;
//}
//
//void AuthWidget::setInternalBasePath(const std::string& basePath)
//{
//    basePath_ = Wt::Utils::append(Wt::Utils::prepend(basePath, '/'), '/');
//}
//
//void AuthWidget::onPathChange(const std::string& path)
//{
//    handleRegistrationPath(path);
//}
//
//bool AuthWidget::handleRegistrationPath(const std::string& path)
//{
//    if (!basePath_.empty())
//    {
//        WApplication *app = WApplication::instance();
//        if (app->internalPathMatches(basePath_))
//        {
//            std::string ap = app->internalSubPath(basePath_);
//
//            if (ap == "register")
//            {
//                registerNewUser();
//                return true;
//            }
//        }
//    }
//
//    return false;
//}
//
//void AuthWidget::registerNewUser()
//{
//    registerNewUser(Identity::Invalid);
//}
//
//void AuthWidget::registerNewUser(const Identity& oauth)
//{
//    UserActionManagement::registerUserAction(Enums::EAction::display,"register",0);
//    showDialog(tr("Wt.Auth.registration"), createRegistrationView(oauth));
//}
//
//WDialog *AuthWidget::showDialog(const WString& title, WWidget *contents)
//{
//    delete dialog_;
//    dialog_ = 0;
//
//    if (contents)
//    {
//        dialog_ = new WDialog(title);
//        dialog_->contents()->addWidget(contents);
//        dialog_->contents()->childrenChanged()
//                .connect(this, &AuthWidget::closeDialog);
//
//        if (!WApplication::instance()->environment().ajax())
//        {
//            /*
//             * try to center it better, we need to set the half width and
//             * height as negative margins.
//             */
//            dialog_->setMargin("-21em", Left); // .Wt-form width
//            dialog_->setMargin("-200px", Top); // ???
//        }
//
//        dialog_->show();
//    }
//
//    return dialog_;
//}
//
//void AuthWidget::closeDialog()
//{
//    if (messageBox_)
//    {
//        delete messageBox_;
//        messageBox_ = 0;
//    }
//    else
//    {
//        delete dialog_;
//        dialog_ = 0;
//    }
//}
//
//RegistrationModelAlert *AuthWidget::createRegistrationModel()
//{
//    RegistrationModelAlert *result = new RegistrationModelAlert(*model_->baseAuth(),
//                                                      model_->users(),
//                                                      login_, this);
//
//    if (model_->passwordAuth())
//        result->addPasswordAuth(model_->passwordAuth());
//
//    result->addOAuth(model_->oAuth());
//
//    return result;
//}
//
//WWidget *AuthWidget::createRegistrationView(const Identity& id)
//{
//    if (!registrationModel_)
//        registrationModel_ = createRegistrationModel();
//    else
//        registrationModel_->reset();
//
//    if (id.isValid())
//        registrationModel_->registerIdentified(id);
//    
//    RegistrationWidgetAlert *w = new RegistrationWidgetAlert(this);
//    w->setModel(registrationModel_);
//
//    return w;
//}
//
//void AuthWidget::handleLostPassword()
//{
//    showDialog(tr("Wt.Auth.lostpassword"), createLostPasswordView());
//}
//
//WWidget *AuthWidget::createLostPasswordView()
//{
//    return new LostPasswordWidgetAlert(model_->users(), *model_->baseAuth());
//}
//
//void AuthWidget::letUpdatePassword(const User& user, bool promptPassword)
//{
//    showDialog(tr("Wt.Auth.updatepassword"),
//               createUpdatePasswordView(user, promptPassword));
//}
//
//WWidget *AuthWidget::createUpdatePasswordView(const User& user,
//                                              bool promptPassword)
//{
//    return new UpdatePasswordWidget(user, createRegistrationModel(),
//                                    promptPassword ? model_ : 0);
//}
//
//WDialog *AuthWidget::createPasswordPromptDialog(Login& login)
//{
//    return new PasswordPromptDialog(login, model_);
//}
//
//void AuthWidget::logout()
//{
//    login_.logout();
//}
//
//void AuthWidget::displayError(const WString& m)
//{
//    delete messageBox_;
//
//    WMessageBox *box = new WMessageBox(tr("Wt.Auth.error"), m, NoIcon, Ok);
//    box->buttonClicked().connect(this, &AuthWidget::closeDialog);
//    box->show();
//
//    messageBox_ = box;
//}
//
//void AuthWidget::displayInfo(const WString& m)
//{
//    delete messageBox_;
//
//    WMessageBox *box = new WMessageBox(tr("Wt.Auth.notice"), m, NoIcon, Ok);
//    box->buttonClicked().connect(this, &AuthWidget::closeDialog);
//    box->show();
//
//    messageBox_ = box;
//}
//
//void AuthWidget::render(WFlags<RenderFlag> flags)
//{
//    if (!created_)
//    {
//        create();
//        created_ = true;
//    }
//
//    WTemplate::render(flags);
//}
//
//void AuthWidget::create()
//{
//    if (created_)
//        return;
//
//    created_ = true;
//
//    login_.changed().connect(this, &AuthWidget::onLoginChange);
//    onLoginChange();
//}
//
//void AuthWidget::onLoginChange()
//{
//    clear();
//
//    if (login_.loggedIn())
//    {
//        createLoggedInView();
//    }
//    else
//    {
//        if (model_->baseAuth()->authTokensEnabled())
//        {
//            WApplication::instance()->removeCookie
//                    (model_->baseAuth()->authTokenCookieName());
//        }
//
//        model_->reset();
//        createLoginView();
//    }
//}
//
//void AuthWidget::createLoginView()
//{
//    setTemplateText(tr("Wt.Auth.template.login"));
//
//    createPasswordLoginView();
//    createOAuthLoginView();
//}
//
//void AuthWidget::createPasswordLoginView()
//{
//    updatePasswordLoginView();
//}
//
//WFormWidget *AuthWidget::createFormWidget(WFormModel::Field field)
//{
//    WFormWidget *result = 0;
//
//    if (field == AuthModel::LoginNameField)
//    {
//        result = new WLineEdit();
//        result->setFocus();
//    }
//    else if (field == AuthModel::PasswordField)
//    {
//        WLineEdit *p = new WLineEdit();
//        p->enterPressed().connect(this, &AuthWidget::attemptPasswordLogin);
//        p->setEchoMode(WLineEdit::Password);
//        result = p;
//    }
//    else if (field == AuthModel::RememberMeField)
//    {
//        result = new WCheckBox();
//    }
//
//    return result;
//}
//
//void AuthWidget::updatePasswordLoginView()
//{
//    if (model_->passwordAuth())
//    {
//        setCondition("if:passwords", true);
//
//        updateView(model_);
//
//        WInteractWidget *login = resolve<WInteractWidget *>("login");
//
//        if (!login)
//        {
//            login = new WPushButton(tr("Wt.Auth.login"));
//            login->addStyleClass("btn");
//            login->addStyleClass("btn-inverse");
//            login->clicked().connect(this, &AuthWidget::attemptPasswordLogin);
//            bindWidget("login", login);
//
//            model_->configureThrottling(login);
//
//            if (model_->baseAuth()->emailVerificationEnabled())
//            {
//                WAnchor *anchor = new WAnchor();
//                anchor->setText(tr("Wt.Auth.lost-password"));
//                anchor->addStyleClass("flip-link");
//                anchor->setId("to-recover");
//                anchor->clicked().connect(this, &AuthWidget::handleLostPassword);
//                bindWidget("lost-password", anchor);
//            }
//            else
//                bindEmpty("lost-password");
//
//            if (registrationEnabled_)
//            {
//                WInteractWidget *w;
//                if (!basePath_.empty())
//                {
//                    w = new WAnchor
//                            (WLink(WLink::InternalPath, basePath_ + "register"),
//                             tr("Wt.Auth.register"));
//                    w->addStyleClass("flip-link");
//                }
//                else
//                {
//                    w = new WAnchor();
//                    ((WAnchor*)w)->setText(tr("Wt.Auth.register"));
//                    w->clicked().connect(this, &AuthWidget::registerNewUser);
//                    w->addStyleClass("flip-link");
//                }
//
//                bindWidget("register", w);
//            }
//            else
//                bindEmpty("register");
//
//            if (model_->baseAuth()->emailVerificationEnabled()
//                    && registrationEnabled_)
//                bindString("sep", " | ");
//            else
//                bindEmpty("sep");
//        }
//
//        model_->updateThrottling(login);
//    }
//    else
//    {
//        bindEmpty("lost-password");
//        bindEmpty("sep");
//        bindEmpty("register");
//        bindEmpty("login");
//    }
//}
//
//void AuthWidget::createOAuthLoginView()
//{
//    if (!model_->oAuth().empty())
//    {
//        setCondition("if:oauth", true);
//
//        WContainerWidget *icons = new WContainerWidget();
//        icons->setInline(isInline());
//
//        for (unsigned i = 0; i < model_->oAuth().size(); ++i)
//        {
//            const OAuthService *auth = model_->oAuth()[i];
//
//            WImage *w = new WImage("css/oauth-" + auth->name() + ".png", icons);
//            w->setToolTip(auth->description());
//            w->setStyleClass("Wt-auth-icon");
//            w->setVerticalAlignment(AlignMiddle);
//
//            OAuthProcess * const process
//                    = auth->createProcess(auth->authenticationScope());
//#ifndef WT_TARGET_JAVA
//            w->clicked().connect(process, &OAuthProcess::startAuthenticate);
//#else
//            process->connectStartAuthenticate(w->clicked());
//#endif
//
//            process->authenticated().connect
//                    (boost::bind(&AuthWidget::oAuthDone, this, process, _1));
//
//            WObject::addChild(process);
//        }
//
//        bindWidget("icons", icons);
//    }
//}
//
//void AuthWidget::oAuthDone(OAuthProcess *oauth, const Identity& identity)
//{
//    /*
//     * FIXME: perhaps consider moving this to the model with signals or
//     * by passing the Login object ?
//     */
//    if (identity.isValid())
//    {
//        log("secure") << oauth->service().name() << ": identified: as "
//                << identity.id() << ", "
//                << identity.name() << ", " << identity.email();
//
//        std::auto_ptr<AbstractUserDatabase::Transaction>
//                t(model_->users().startTransaction());
//
//        User user = model_->baseAuth()->identifyUser(identity, model_->users());
//        if (user.isValid())
//            login_.login(user);
//        else
//            registerNewUser(identity);
//
//        if (t.get())
//            t->commit();
//    }
//    else
//    {
//        log("secure") << oauth->service().name() << ": error: " << oauth->error();
//        displayError(oauth->error());
//    }
//}
//
//void AuthWidget::attemptPasswordLogin()
//{
//    updateModel(model_);
//
//    if (model_->validate())
//        model_->login(login_);
//    else
//        updatePasswordLoginView();
//}
//
//void AuthWidget::createLoggedInView()
//{
//    setTemplateText(tr("Wt.Auth.template.logged-in"));
//
////    bindString("user-name", login_.user().identity(Identity::LoginName));
//
//    WPushButton *logout = new WPushButton(tr("Wt.Auth.logout"));
//    logout->clicked().connect(this, &AuthWidget::logout);
//
////    bindWidget("logout", logout);
//    
//    
//    WContainerWidget *menuContainer = new WContainerWidget();
//    menuContainer->setId("user-nav");
//    menuContainer->setAttributeValue("class","navbar navbar-inverse");
//    WMenu *menu = new WMenu(menuContainer);
//    
//    
//    menu->setAttributeValue("class","nav btn-group");
//    
////    WMenuItem *alertMenuItem = new WMenuItem("Alertes");
//////    logoutMenuItem->clicked().connect(this, &AuthWidget::logout);
////    alertMenuItem->setAttributeValue("class","btn btn-inverse");
////    
////    WText *alertCount = new WText("5");
////    alertCount->setAttributeValue("class","label label-important");
////    WAnchor *anchorTemp = (WAnchor*)alertMenuItem->widget(0);
////    anchorTemp->addWidget(alertCount);
//    
//    
//    WMenuItem *logoutMenuItem = new WMenuItem(tr("Wt.Auth.logout"));
////    newMenuItem->setAttributeValue("name",boost::lexical_cast<std::string>(enumPT.index()));
//    logoutMenuItem->clicked().connect(this, &AuthWidget::logout);
//    logoutMenuItem->setAttributeValue("class","btn btn-inverse");
//    
////    menu->addItem(alertMenuItem);
//    
//    WMenuItem *userNameMenuItem = new WMenuItem(login_.user().email());
//    userNameMenuItem->setAttributeValue("class","label label-inverse active");
//    userNameMenuItem->setAttributeValue("style","top:4px");
//    
//    menu->addItem(userNameMenuItem);
//    
//    menu->addItem(logoutMenuItem);
//    
//    
//    
//    
//    bindWidget("menu",menuContainer);
//}
//
//void AuthWidget::processEnvironment()
//{
//    const WEnvironment& env = WApplication::instance()->environment();
//
//    if (registrationEnabled_)
//        if (handleRegistrationPath(env.internalPath()))
//            return;
//
//    std::string emailToken
//            = model_->baseAuth()->parseEmailToken(env.internalPath());
//
//    if (!emailToken.empty())
//    {
//        EmailTokenResult result = model_->processEmailToken(emailToken);
//        switch (result.result())
//        {
//        case EmailTokenResult::Invalid:
//            displayError(tr("Wt.Auth.error-invalid-token"));
//            break;
//        case EmailTokenResult::Expired:
//            displayError(tr("Wt.Auth.error-token-expired"));
//            break;
//        case EmailTokenResult::UpdatePassword:
//            letUpdatePassword(result.user(), false);
//            break;
//        case EmailTokenResult::EmailConfirmed:
//            displayInfo(tr("Wt.Auth.info-email-confirmed"));
//            login_.login(result.user());
//        }
//
////        WApplication::instance()->setInternalPath("/");
//
//        return;
//    }
//
//    User user = model_->processAuthToken();
//    if (user.isValid())
//        login_.login(user, WeakLogin);
//}
//
//}
//}
//
