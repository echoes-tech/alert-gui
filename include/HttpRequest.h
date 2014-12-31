/* 
 * File:   HttpRequest.h
 * Author: vpl
 *
 * Created on 17 décembre 2014, 17:29
 */

#ifndef HTTPREQUEST_H
#define	HTTPREQUEST_H

#include <string>
#include <vector>

#include <boost/system/system_error.hpp>
#include <boost/function.hpp>

#include <Wt/WString>
#include <Wt/Json/Value>
#include <Wt/Http/Client>

class HttpRequest {
public:
    HttpRequest(std::string email, std::string token, std::string xmlPageName);
    HttpRequest();
    virtual ~HttpRequest();
    void sendGet(std::string resource, std::vector<std::string> listParameter, boost::function<void (Wt::Json::Value)> functor);
    void setApiUrl(std::string apiUrl);
    void setLogin(std::string login);
    void setToken(std::string token);
    void setXmlPageName(std::string xmlPageName);
    std::string getApiUrl();
    std::string getLogin();
    std::string getToken();
    std::string getXmlPageName();
    std::string getApiAddress(std::string resource, std::vector<std::string> listParameter);
    
private:
    std::string     m_apiUrl;
    std::string     m_xmlPageName;
    std::string     m_login;
    std::string     m_token;
    void handleResponseGet(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client, boost::function<void (Wt::Json::Value)> functor);
};

#endif	/* HTTPREQUEST_H */

