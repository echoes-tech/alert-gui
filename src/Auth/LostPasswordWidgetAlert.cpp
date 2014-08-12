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
    addFunction("id", &Functions::id);
    addFunction("tr", &Functions::tr);
    addFunction("block", &Functions::block);

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

  baseAuth_.lostPassword(email->valueText().toUTF8(), users_);

  cancel();

  Wt::WMessageBox *const box = new Wt::WMessageBox(tr("Wt.Auth.lost-password"),
				     tr("Wt.Auth.mail-sent"),
				     Wt::NoIcon, Wt::Ok);
#ifndef WT_TARGET_JAVA
  box->buttonClicked().connect
    (boost::bind(&LostPasswordWidgetAlert::deleteBox, box));
#else
  box->buttonClicked().connect
    (boost::bind(&LostPasswordWidgetAlert::deleteBox, this, box));
#endif
  box->show();
}

void LostPasswordWidgetAlert::deleteBox(Wt::WMessageBox *box) {
    delete box;
}

void LostPasswordWidgetAlert::cancel() {
    delete this;
}
