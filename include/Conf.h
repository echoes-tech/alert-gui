/**
 * @file Conf.h
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

#ifndef CONF_H
#define	CONF_H

#include <Wt/WServer>
#include <Wt/WLogger>

class Conf
{
    public:
        Conf();
        virtual ~Conf();

        /**
         * Reads ECHOES configuration properties.
         * @param WServer
         * @return true if all values are defined or false
         */
        bool readProperties(Wt::WServer& server);
        bool readDbProperties(Wt::WServer& server);
        bool readApiProperties(Wt::WServer& server);

        std::string getSessConnectParams() const;
        std::string getApiHost() const;
        unsigned getApiPort() const;        
        bool isApiHttps() const;

    private:
        std::string m_sessConnectParams;
        std::string m_apiHost;
        unsigned m_apiPort;
        bool m_apiHttps;

        void setSessConnectParams
        (
                std::string dbHost,
                unsigned dbPort,
                std::string dbName,
                std::string dbUser,
                std::string dbPassword
        );
        void setApiHost(std::string apiHost);
        void setApiPort(unsigned apiPort);        
        void setApiHttps(bool apiHttps);
};

extern Conf conf;

#endif	/* CONF_H */
