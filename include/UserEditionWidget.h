/* 
 * Gui UserEditionWidget.h
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 15/11/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef USEREDITIONWIDGET_H
#define	USEREDITIONWIDGET_H

#include <Wt/Json/Value>
#include <Wt/Http/Message>
#include <Wt/WRandom>

#include "GlobalIncludeFile.h"
#include "CreatePageWidget.h"

typedef std::map<long long, std::string>        MapLongString;

class UserEditionWidget :
public CreatePageWidget
{
public:
    UserEditionWidget(Echoes::Dbo::Session *session, std::string apiUrl, int type);

    void                        update();
    std::vector<std::string>    getTitlesTableWidget();
    std::vector<std::string>    getTitlesTableText();
    std::vector<long long>      getIdsTable();
    vector_type                 getResourceRowTable(long long id);
    Wt::WValidator              *editValidator(int who);
    void                        closePopup();
    void                        recoverListMedia();
    
    void                        getMedia(boost::system::error_code err, const Wt::Http::Message& response);

    void                        addResource(std::vector<Wt::WInteractWidget*> argument);
    Wt::WDialog                 *deleteResource(long long id);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);

    void                        close();
    
    
    void                        deleteMedia(boost::system::error_code err, const Wt::Http::Message& response);
    void                        postMedia(boost::system::error_code err, const Wt::Http::Message& response);
    void                        putMedia(boost::system::error_code err, const Wt::Http::Message& response);

private:
 
    void                        setSession(Echoes::Dbo::Session *session);
    void                        setApiUrl(std::string apiUrl);
    std::string                 getApiUrl();

    bool                  created_;
    bool                  newClass_;
    Echoes::Dbo::Session               *session_;
    std::string           apiUrl_;
    Wt::Json::Value       result_;
    int                   type_;  
    MapLongString    mediasTokens;
};



#endif	/* USEREDITIONWIDGET_H */

