/* 
 * File:   ApiManagement.h
 * Author: gkr
 *
 * Created on 18 novembre 2013, 13:37
 */

#ifndef APIMANAGEMENT_H
#define	APIMANAGEMENT_H
/*
#include <Wt/Http/Message>
#include <Wt/Http/Client>
#include "tools/Session.h"
*/
    #include <Wt/WAnimation>
#include <Wt/WPanel>
#include <Wt/WText>

#include <Wt/WIOService>

#include <Wt/Json/Value>
#include <Wt/Http/Message>
#include <Wt/WRandom>

#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

#include "GlobalIncludeFile.h"

class ApiManagement
{
public:
    ApiManagement(Echoes::Dbo::Session *session, std::string apiUrl);
//    virtual ~ApiManagement();
    
    std::string     getApiUrl();

    template <class classType>
    void    callApi(void (classType::*pFunc)(boost::system::error_code err, const Wt::Http::Message& response),
    std::string infoUrl, Echoes::Dbo::Session *session, std::string apiUrl);

private:

    Echoes::Dbo::Session               *session_;
    std::string           apiUrl_;
};

#endif	/* APIMANAGEMENT_H */

