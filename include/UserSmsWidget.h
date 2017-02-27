/**
 * @file UserSmsWidget.h
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

#ifndef USERSMSWIDGET_H
#define	USERSMSWIDGET_H

#include <Wt/Json/Value>
#include <Wt/Http/Message>

#include "GlobalIncludeFile.h"
#include "CreatePageWidget.h"

class CreatePageWidget;

class UserSmsWidget :
public CreatePageWidget
{
public:
    UserSmsWidget(Session *session, std::string apiUrl);
    
    void                        update();
    void                        popupAddTables(Wt::WTabWidget *tabW);
    std::vector<std::string>   getTitlesTableWidget();
    std::vector<std::string>    getTitlesTableText();
    std::vector<long long>      getIdsTable();
    vector_type                 getResourceRowTable(long long id);
    Wt::WValidator              *editValidator(int who);
    void                        closePopup();
    void                        recoverListAsset();
    
    void                        getSms(boost::system::error_code err, const Wt::Http::Message& response);

    void                        addResource(std::vector<Wt::WInteractWidget*> argument);
    Wt::WDialog                 *deleteResource(long long id);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);

    void                        close();
    
    void                        setSession(Session *session);
    void                        setApiUrl(std::string apiUrl);
    std::string                 getApiUrl();

private:

  bool                  created_;
  bool                  newClass_;
  Session               *session_;
  std::string           apiUrl_;
  Wt::Json::Value       result_;
};

#endif	/* USERSMSWIDGET_H */
