#include "EchoesHome.h"

#include <Wt/WWidget>
#include <Wt/WTemplate>

/**
Class EchoesHome : étend la classe WApplication
@param env : variables d'environnement
*/
EchoesHome::EchoesHome(const WEnvironment& env)
    : WApplication(env)
{
    // Définition des ressources (template)
    messageResourceBundle().use(appRoot() + "echoes-home", false);

    // Initialisation de la page principale du site
    root()->clear();

    homePage_ = initHome();

    // ajout du widget principal à la racine du site
    root()->addWidget(homePage_);
}

/**
Initialisation de l'application
@return WTemplate
*/
WWidget *EchoesHome::initHome()
{
    // on utilise un WTemplate qui permet de se baser sur un fichier XML pour générer les pages web. Fichier XML défini plus haut avec messageResourceBundle.
    WTemplate *result = new WTemplate(tr("template"), root());
    return result;
}

/**
TBC
*/
WString EchoesHome::tr(const char *key)
{
    // Sert à fabriquer un objet WString à partir d'une chaîne "normale". A utiliser en entrée des fonctions WT qui demandent une WString.
    return WString::tr(key);
}

/**
Point d'entrée
@return EchoesHome
*/
WApplication *createEchoesHomeApplication(const WEnvironment& env)
{
    // On instancie la classe EchoesHome qui permet d'afficher le site.
    return new EchoesHome(env);
}
