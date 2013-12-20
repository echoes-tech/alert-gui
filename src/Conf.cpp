/* 
 * GUI Configuration
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

#include "Conf.h"

using namespace std;

Conf conf;

Conf::Conf() :
m_sessConnectParams(""),
m_apiHost(""),
m_apiPort(443),
m_apiHttps(true)
{
}

Conf::~Conf()
{
}

bool Conf::readProperties(Wt::WServer& server)
{
    Wt::log("debug") << "[Conf] Read properties from " << WT_CONFIG_XML;

    bool res = false;

    if (readDbProperties(server) && readApiProperties(server))
    {
        res = true;
    }

    return res;
}

bool Conf::readDbProperties(Wt::WServer& server)
{
    bool res = false;

    struct Db
    {
        string host;
        string port;
        string name;
        string user;
        string password;
    } db;

  if
      (
       server.readConfigurationProperty("db-host", db.host)
       && server.readConfigurationProperty("db-port", db.port)
       && server.readConfigurationProperty("db-name", db.name)
       && server.readConfigurationProperty("db-user", db.user)
       && server.readConfigurationProperty("db-password", db.password)
      )
    {
        try
        {
            setSessConnectParams
            (
                db.host,
                boost::lexical_cast<unsigned>(db.port),
                db.name,
                db.user,
                db.password
            );

            res = true;
        }
        catch (boost::bad_lexical_cast &)
        {
             Wt::log("error") << "[Conf] Property named 'db-port' in " << WT_CONFIG_XML << " should be an unsigned integer";
        }
    }
    else
    {
        Wt::log("error") << "[Conf] Incomplete DB properties in " << WT_CONFIG_XML;
    }

    return res;
}

bool Conf::readApiProperties(Wt::WServer& server)
{
    bool res = false;

    struct Api
    {
        string host;
        string port;
        string https;
    } api;

    if
    (
        server.readConfigurationProperty("api-host", api.host)
        && server.readConfigurationProperty("api-port", api.port)
        && server.readConfigurationProperty("api-https", api.https)
    )
    {
        setApiHost(api.host);
        try
        {
            setApiPort(boost::lexical_cast<unsigned>(api.port));
        }
        catch (boost::bad_lexical_cast &)
        {
             Wt::log("error") << "[Conf] Property named 'api-port' in " << WT_CONFIG_XML << " should be an unsigned integer. By default, it is set to '443'";
        }
        if (api.https.compare("true") == 0)
        {
            setApiHttps(true);
        }
        else if (api.https.compare("false") == 0)
        {
            setApiHttps(false);
        }
        else
        {
             Wt::log("error") << "[Conf] Property named 'api-https' in " << WT_CONFIG_XML << " should be an boolean (true or false). By default, it is set to 'true'";
        }

        res = true;
    }
    else
    {
        Wt::log("warning") << "[Conf] Incomplete API properties in " << WT_CONFIG_XML;
    }

    return res;
}

void Conf::setSessConnectParams
(
    string dbHost,
    unsigned dbPort,
    string dbName,
    string dbUser,
    string dbPassword
)
{
    m_sessConnectParams = "hostaddr=" + dbHost +
                         " port=" + boost::lexical_cast<string>(dbPort) +
                         " dbname=" + dbName +
                         " user=" + dbUser +
                         " password=" + dbPassword;
    return;
}

string Conf::getSessConnectParams() const
{
    return m_sessConnectParams;
}

void Conf::setApiHost(std::string apipHost)
{
    m_apiHost = apipHost;
    return;
}

std::string Conf::getApiHost() const
{
    return m_apiHost;
}

void Conf::setApiPort(unsigned apiPort)
{
    m_apiPort = apiPort;
    return;
}

unsigned Conf::getApiPort() const
{
    return m_apiPort;
}

void Conf::setApiHttps(bool apiHttps)
{
    m_apiHttps = apiHttps;
    return;
}

bool Conf::isApiHttps() const
{
    return m_apiHttps;
}

