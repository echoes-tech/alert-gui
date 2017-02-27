/**
 * @file LostPasswordWidgetAlert.cpp
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
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
