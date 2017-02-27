/**
 * @file OptionManagementWidget.h
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

#ifndef OPTIONMANAGEMENTWIDGET_H
#define	OPTIONMANAGEMENTWIDGET_H

#include "GlobalIncludeFile.h"
#include <Wt/WApplication>
#include <Wt/WContainerWidget>

class OptionManagementWidget : public Wt::WContainerWidget {
public:
    OptionManagementWidget(OptionManagementModel *model, Echoes::Dbo::Session *session, std::string apiUrl);
    virtual ~OptionManagementWidget();
protected:

  virtual bool validate();


  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);
  void createUI();
  void replaceChangePasswordWidget();

  virtual Wt::WFormWidget *createFormWidget(Wt::WFormModel::Field field);
  
  void setApiUrl(std::string apiUrl);
  std::string getApiUrl() const;

private:
  OptionManagementModel *model_;
  Echoes::Dbo::Session * session;
  std::string apiUrl_;
  Wt::WComboBox roleComboBox;

  Wt::WTemplateFormView *mainForm;
  
  bool created_;
  
  void askSms();
  void changeRole();
  void fillRoleSelector();
  void getQuota(boost::system::error_code err, const Wt::Http::Message& response);
  void getRoles(boost::system::error_code err, const Wt::Http::Message& response);
  
  
};

#endif	/* OPTIONMANAGEMENTWIDGET_H */
