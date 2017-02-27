/**
 * @file ApiManagement.cpp
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


    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Impossible to call the API";
    }
}

std::string     ApiManagement::getApiUrl()
{
    return apiUrl_;
}
