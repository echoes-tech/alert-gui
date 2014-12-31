/* 
 * File:   HttpRequest.cpp
 * Author: vpl
 * 
 * Created on 17 décembre 2014, 17:29
 */

#include "HttpRequest.h"
#include <Wt/Utils>
#include <Wt/WLogger>
#include <Wt/Json/Parser>
#include <Wt/WMessageBox>
#include <Wt/WApplication>

HttpRequest::HttpRequest(std::string email, std::string token, std::string xmlPageName)
{
    HttpRequest::setLogin(email);
    HttpRequest::setToken(token);
    HttpRequest::setXmlPageName(xmlPageName);
}

HttpRequest::HttpRequest()
{
}

HttpRequest::~HttpRequest()
{
}

void HttpRequest::setLogin(std::string email)
{
    m_login = Wt::Utils::urlEncode(email);
}

std::string HttpRequest::getLogin()
{
    return (m_login);
}

void HttpRequest::setToken(std::string login)
{
    m_token = login;
}

std::string HttpRequest::getToken()
{
    return (m_token);
}

void HttpRequest::setXmlPageName(std::string xmlPageName)
{
    m_xmlPageName = xmlPageName;
}

std::string HttpRequest::getXmlPageName()
{
    return m_xmlPageName;
}

void HttpRequest::setApiUrl(std::string apiUrl)
{
    m_apiUrl = apiUrl;
}

std::string HttpRequest::getApiUrl()
{
    return m_apiUrl;
}

std::string HttpRequest::getApiAddress(std::string resource, std::vector<std::string> listParameter)
{    
    std::string apiAddress = getApiUrl() + "/" + resource
            + "?login=" + m_login
            + "&token=" + m_token;
        
    for(size_t i(0); i<listParameter.size(); i++)
    {
        apiAddress += "&" + listParameter[i];
    }
    return (apiAddress);
}

void HttpRequest::sendGet(std::string resource, std::vector<std::string> listParameter, boost::function<void (Wt::Json::Value)> functor)
{
    std::string apiAddress = getApiAddress(resource, listParameter);
    
    Wt::log("debug") << "[GET] address to call : " << apiAddress;
    Wt::Http::Client *client = new Wt::Http::Client();
    client->done().connect(boost::bind(&HttpRequest::handleResponseGet, this, _1, _2, client, functor));
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

void HttpRequest::handleResponseGet(boost::system::error_code err,
    const Wt::Http::Message& response, Wt::Http::Client *client, boost::function<void (Wt::Json::Value)> functor)
{
    delete client;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                functor(result);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning")
                        << "[" + Wt::WString::tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                        + " Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(Wt::WString::tr("Alert." + m_xmlPageName + ".parsing-error-title"),
                                      Wt::WString::tr("Alert." + m_xmlPageName + ".parsing-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning")
                        << "[" + Wt::WString::tr("Alert." + m_xmlPageName + ".add-form." + m_xmlPageName)
                        + " Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(Wt::WString::tr("Alert." + m_xmlPageName + ".typexception-error-title"),
                                      Wt::WString::tr("Alert." + m_xmlPageName + ".typexception-error"), Wt::Ok);
            }
        }
        else if (response.status() == 404)
        {
            functor(Wt::Json::Value::Null);
        }
    }
    else
    {
        Wt::log("warning") << "[" + Wt::WString::tr("Alert." + m_xmlPageName + ".add-form."
                                       + m_xmlPageName) + " Widget] Http::Client get error: "
                << response.body();
        Wt::WMessageBox::show(Wt::WString::tr("Alert." + m_xmlPageName + ".database-error-title"),
                              Wt::WString::tr("Alert." + m_xmlPageName + ".database-error"), Wt::Ok);
    }
}