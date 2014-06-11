/* 
 * File:   UserSmsWidget.h
 * Author: gkr
 *
 * Created on 6 novembre 2013, 14:55
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

