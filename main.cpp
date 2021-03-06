/**
 * @file main.cpp
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

#include <signal.h>

#include <Wt/WServer>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/WEnvironment>
#include <Wt/WApplication>
#include <Wt/WBootstrapTheme>
#include <Wt/WLoadingIndicator>

#include "GlobalIncludeFile.h"
#include "tools/Session.h"
#include <boost/thread.hpp>
#include "Conf.h"

// This globale is version of web site.
std::string      EchoesHome::version_g = "1.4.0";

//---------------------------------------------------------------------------

Wt::WApplication *createEchoesHomeApplication(const Wt::WEnvironment& env) 
{
    // On instancie la classe EchoesHome qui permet d'afficher le site.
    Wt::WApplication *app = new Wt::WApplication(env);
    
    app->setTitle("Echoes Alert");
    
    
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "global",false);
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "auth",false);
    
    app->loadingIndicator()->setMessage(Wt::WString::tr("Alert.loading"));
    
    app->enableUpdates(true);
    
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/bootstrap.css");
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/bootstrap-responsive.css");
    
    

    //    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/colorpicker.css");
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/jquery.gritter.css");
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/unicorn.grey.css");
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/jquery-ui.css");
//    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/fullcalendar.css");
//    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/uniform.css");
//    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/select2.css");
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/unicorn.login.css");
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/unicorn.main.css");
    
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/css/echoes-alert-custom.css");
       
    app->useStyleSheet(Wt::WApplication::resourcesUrl() + "themes/bootstrap/wt.css");
    
    
    
    
    Wt::WBootstrapTheme *theme = new Wt::WBootstrapTheme();

    theme->activeClass();
    app->setTheme(theme);
    
    app->requireJQuery(Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/jquery.js");
    app->require(Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/jquery.peity.js");
    app->require(Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/jquery.gritter.js");
//    app->require(Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/jquery.ui.custom.js");
    app->require(Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/jquery-ui.custom.js");
    
    app->require(Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/bootstrap.js");
    
    
    app->require(Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/unicorn.interface.js");
    app->require(Wt::WApplication::resourcesUrl() + "themes/bootstrap/js/unicorn.js");
    
//    app->require("http://twitter.github.io/bootstrap/assets/js/application.js");
    
    Wt::WString::setDefaultEncoding(Wt::CharEncoding::UTF8);
    
    new EchoesHome(app->root());

    return app;
    
}

/**
 * Main function
 * @param argc Number of arguments passed to the program
 * @param argv Array of arguments passed to the program
 * @return Exit status
 */
int main(int argc, char **argv)
{
    int res = EXIT_FAILURE;

    try
    {
        // use argv[0] as the application name to match a suitable entry
        // in the Wt configuration file
        Wt::WServer server(argv[0]);

        // On définit la configuration du serveur en lui passant les paramètres d'entrée et son fichier de configuration
        server.setServerConfiguration(argc, argv);

        if (conf.readProperties(server))
        {
            // On fixe le point d'entrée du programme (type de point d'entée, méthode à appeler, uri, chemin favicon)
            server.addEntryPoint(Wt::Application, createEchoesHomeApplication, "", "/favicon.ico");

            Echoes::Dbo::Session::configureAuth();

            // démarrage du serveur en arrière plan
            if (server.start())
            {
                // méthode qui bloque le thread courant en attendant le signal d'exctinction
                int sig = Wt::WServer::waitForShutdown();

                Wt::log("info") << "[Main] Shutdown (signal = " << sig << ")";

                server.stop();

                if (sig == SIGHUP)
                {
                    Wt::WServer::restart(argc, argv, environ);
                }

                res = EXIT_SUCCESS;
            }
        }
        else
        {
            Wt::log("fatal") << "[Main] Every properties are not correctly set in " << WT_CONFIG_XML;
        }
    }
    catch (Wt::WServer::Exception& e)
    {
        std::cerr << e.what() << std::endl;
    }
    catch (std::exception &e)
    {
        std::cerr << "exception: " << e.what() << std::endl;
    }

    return res;
}
