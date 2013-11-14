/* 
 * File:   UserEditionWidget.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:50
 */

#ifndef USEREDITIONWIDGET_H
#define	USEREDITIONWIDGET_H

#include <Wt/Json/Value>
#include <Wt/Http/Message>
#include <Wt/WRandom>

#include "GlobalIncludeFile.h"
#include "CreatePageWidget.h"

class UserEditionWidget :
public CreatePageWidget
{
public:
    UserEditionWidget(Session *session, std::string apiUrl, int type);

    void                        update();
    void                        popupAddTables(Wt::WTabWidget *tabW);
    std::vector<std::string>    getTitlesTableWidget();
    std::vector<std::string>    getTitlesTableText();
    std::vector<long long>      getIdsTable();
    vector_type                 getResourceRowTable(long long id);
    Wt::WValidator              *editValidator(int who);
    void                        closePopup();
    void                        recoverListAsset();
    
    void                        getMedia(boost::system::error_code err, const Wt::Http::Message& response);

    void                        addResource(std::vector<Wt::WInteractWidget*> argument);
    Wt::WDialog                 *deleteResource(long long id);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);

    void                        close();
    
    
    void                        deleteMedia(boost::system::error_code err, const Wt::Http::Message& response);
    void                        postMedia(boost::system::error_code err, const Wt::Http::Message& response);
    void                        putMedia(boost::system::error_code err, const Wt::Http::Message& response);

private:
 
    void                        setSession(Session *session);
    void                        setApiUrl(std::string apiUrl);
    std::string                 getApiUrl();

    bool                  created_;
    bool                  newClass_;
    Session               *session_;
    std::string           apiUrl_;
    Wt::Json::Value       result_;
    int                   type_;  
};



#endif	/* USEREDITIONWIDGET_H */

