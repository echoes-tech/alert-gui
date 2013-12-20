/* 
 * Header of GUI Configuration
 * @author ECHOES Technologies (FPO)
 * @date 18/12/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2013 BY ECHOES TECHNOLGIES SAS
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

