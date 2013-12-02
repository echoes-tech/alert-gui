#include <Wt/WServer>
#include <Wt/WResource>
#include <Wt/Http/Request>
#include <Wt/Http/Response>
#include <Wt/WEnvironment>
#include <Wt/WApplication>
#include <Wt/WBootstrapTheme>

#include "GlobalIncludeFile.h"
#include "tools/Session.h"
#include <boost/thread.hpp>

// This globale is version of web site.
std::string      EchoesHome::version_g = "1.0.0 RC2";

//---------------------------------------------------------------------------

Wt::WApplication *createEchoesHomeApplication(const Wt::WEnvironment& env) 
{
    // On instancie la classe EchoesHome qui permet d'afficher le site.
    Wt::WApplication *app = new Wt::WApplication(env);
    
    app->setTitle("ECHOES Alert");
    
    app->messageResourceBundle().use("global",false);
    
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
    
    new EchoesHome(app->root());

    return app;
    
}

/**
Point d'entrée du programme.
@param argc : TBC
@param argv : TBC
*/
int main(int argc, char **argv)
{
    try
    {
        // On passe le premier paramètre d'entrée au serveur
        Wt::WServer server(argv[0]);
        // On définit la configuration du serveur en lui passant les paramètres d'entrée et son fichier de configuration
        server.setServerConfiguration(argc, argv);
        // On fixe le point d'entrée du programme (type de point d'entée, méthode à appeler, uri, chemin favicon)
        
        server.addEntryPoint(Wt::Application, createEchoesHomeApplication,"", "/favicon.ico");
        
        Echoes::Dbo::Session::configureAuth();
        
        // démarrage du serveur en arrière plan
        if (server.start())
        {
            // méthode qui bloque le thread courant en attendant le signal d'exctinction
            Wt::WServer::waitForShutdown();
            server.stop();
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
}
