/*
 * Copyright (C) 2011 Emweb bvba, Kessel-Lo, Belgium.
 *
 * See the LICENSE file for terms of use.
 */

#include "Auth/LostPasswordWidgetAlert.h"

LostPasswordWidgetAlert::LostPasswordWidgetAlert(Wt::Auth::AbstractUserDatabase& users,
        const Wt::Auth::AuthService& auth,
        Wt::WContainerWidget *parent)
: WTemplate(Wt::WString::tr("Wt.Auth.template.lost-password"), parent),
users_(users),
baseAuth_(auth) {
    addFunction("tr", &Functions::tr);
    addFunction("block", &Functions::block);

    Wt::WLineEdit *email = new Wt::WLineEdit();
    email->setFocus();
    email->addStyleClass("form-control");
    email->setAttributeValue("placeholder",tr("Wt.Auth.email"));
    Wt::WRegExpValidator *validator = new Wt::WRegExpValidator("[a-zA-Z0-9._%+-]+@[a-zA-Z0-9.-]+\\.[a-zA-Z]{2,4}");
    email->setValidator(validator);

    this->okButton = new Wt::WPushButton(Wt::WString::tr("Wt.Auth.send"));
    this->cancelButton = new Wt::WPushButton(Wt::WString::tr("Wt.WMessageBox.Cancel"));

    okButton->clicked().connect(this, &LostPasswordWidgetAlert::send);
    cancelButton->clicked().connect(this, &LostPasswordWidgetAlert::cancel);

    bindWidget("email", email);
//    bindWidget("send-button", this->okButton);
//    
//    bindWidget("cancel-button", this->cancelButton);
}

void LostPasswordWidgetAlert::send() 
{
    Wt::WFormWidget *email = resolve<Wt::WFormWidget *>("email");

    Wt::Auth::User user = users_.findWithEmail(email->valueText().toUTF8());
    
    std::string msg = "";
    
    
    if (user.isValid()) {
        baseAuth_.lostPassword(email->valueText().toUTF8(), users_);
        msg = "Wt.Auth.mail-sent";
    }
    else
    {
        msg = "Wt.Auth.mail-invalid";
    }
    

    hide();

    Wt::WMessageBox * const box = new Wt::WMessageBox(Wt::WString::tr("Wt.Auth.lost-password"),
            Wt::WString::tr(msg),
            Wt::Information, Wt::Ok);

    box->buttonClicked().connect
            (boost::bind(&LostPasswordWidgetAlert::deleteBox, this, box));

    box->show();
}

void LostPasswordWidgetAlert::deleteBox(Wt::WMessageBox *box) {
    delete box;
    delete this;
}

void LostPasswordWidgetAlert::cancel() {
    delete this;
}

Wt::WPushButton *LostPasswordWidgetAlert::getOkButton()
{
    return this->okButton;
}

Wt::WPushButton *LostPasswordWidgetAlert::getCancelButton()
{
    return this->cancelButton;
}
