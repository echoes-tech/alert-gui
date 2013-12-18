/* 
 * File:   ApiManagement.cpp
 * Author: gkr
 * 
 * Created on 18 novembre 2013, 13:37
 */

#include "ApiManagement.h"

ApiManagement::ApiManagement(Echoes::Dbo::Session *session, std::string apiUrl)
{
    apiUrl_ = apiUrl;
    session_ = session;
}
/*
ApiManagement::~ApiManagement()
{

}
*/
template <class classType>
void    ApiManagement::callApi(void (classType::*pFunc)(boost::system::error_code err, const Wt::Http::Message& response), std::string infoUrl)
{
    Wt::Http::Client *client = new Wt::Http::Client();
    client->done().connect(boost::bind(pFunc, this, _1, _2));

    std::string apiAddress = apiUrl_ + infoUrl
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();

    std::cout << "[GET] address to call : " << apiAddress << std::endl;

    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
        std::cout << "Error Client Http" << std::endl;
}

std::string     ApiManagement::getApiUrl()
{
    return apiUrl_;
}