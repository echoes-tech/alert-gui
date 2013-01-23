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
//    addFunction("id", WT_TEMPLATE_FUNCTION(id));
    addFunction("tr", WT_TEMPLATE_FUNCTION(tr));

    Wt::WLineEdit *email = new Wt::WLineEdit();
    email->setFocus();

    Wt::WPushButton *okButton = new Wt::WPushButton(Wt::WString::tr("Wt.Auth.send"));
    Wt::WPushButton *cancelButton = new Wt::WPushButton(Wt::WString::tr("Wt.WMessageBox.Cancel"));

    okButton->clicked().connect(this, &LostPasswordWidgetAlert::send);
    cancelButton->clicked().connect(this, &LostPasswordWidgetAlert::cancel);

    bindWidget("email", email);
    bindWidget("send-button", okButton);
    bindWidget("cancel-button", cancelButton);
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
            Wt::NoIcon, Wt::Ok);

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
