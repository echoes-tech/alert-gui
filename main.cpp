#include <Wt/WServer>

#include <EchoesHome.h>
//---------------------------------------------------------------------------

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
        server.setServerConfiguration(argc, argv, WTHTTP_CONFIGURATION);
        // On fixe le point d'entrée du programme (type de point d'entée, méthode à appeler, uri, chemin favicon)
        server.addEntryPoint(Wt::Application, createEchoesHomeApplication,"", "/favicon.ico");
        
        Session::configureAuth();
        
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
