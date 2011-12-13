#include "EchoesHome.h"

#include <Wt/WEnvironment>
#include <Wt/WStackedWidget>
#include <Wt/WMenuItem>
#include <Wt/WTabWidget>
#include <Wt/WText>
#include <Wt/WTemplate>
#include <Wt/WViewWidget>
#include <Wt/WWidget>


static const std::string SRC_INTERNAL_PATH = "src";

/**
Class EchoesHome : étend la classe WApplication.
@param env : variables d'environnement
*/
EchoesHome::EchoesHome(const WEnvironment& env)
    : WApplication(env)
{

    // Définition des ressources (template)
    messageResourceBundle().use(appRoot() + "echoes-home", false);

    // Initialisation de la page principale du site
    root()->clear();

    // homePage_ = initHome();
    homePage_ = initTabs();

    // ajout du widget principal à la racine du site
    root()->addWidget(homePage_);
}

/**
Initialisation de l'application.
@return WTemplate
*/
WWidget *EchoesHome::initHome()
{
    // on utilise un WTemplate qui permet de se baser sur un fichier XML pour générer les pages web. Fichier XML défini plus haut avec messageResourceBundle.
    WTemplate *result = new WTemplate(tr("template"), root());
    return result;
}

/**
Initialisation de l'appli, test des tabs.
@return WTabWidget
*/
WWidget *EchoesHome::initTabs()
{
    // on créée un contenair qui va être utilisé par le WTabWidget
    WContainerWidget *result = new WContainerWidget();
    tabList = new WTabWidget(AlignTop | AlignJustify, result);

    // on créée une animation pour animer les transitions entre les tabs
    WAnimation animation(WAnimation::SlideInFromRight, WAnimation::EaseIn);
    tabList->contentsStack()->setTransitionAnimation(animation, true);

    // on créée les différents tabs
    tabList->addTab(createTab("home.examples.hello","hello"),tr("hello-world"))->setPathComponent("");
    tabList->addTab(createTab("home.examples.chart","charts"),tr("charts"));

    return result;
}

/**
Création d'un onglet.
@param textKey la clé qui permet de retrouver le texte de l'onglet dans le template
@param sourceDir
@return le container qui contient le texte
*/
WWidget *EchoesHome::createTab(const char *textKey, const char *sourceDir)
{
    WContainerWidget *result = new WContainerWidget();

    // texte contenu dans l'onglet
    WText *w = new WText(tr(textKey), result);
    w->setInternalPathEncoding(true);
    return result;

}

/**
Méthode utilisée pour transformer une chaîne classique en objet WString. A utiliser en entrée des méthodes WT qui demandent une WString.
@param key chaîne passée en entrée
@return objet WString
*/
WString EchoesHome::tr(const char *key)
{
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
