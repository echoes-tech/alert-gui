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

    // homePage_ = initHome();
    homePage_ = initTab();

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

WWidget *EchoesHome::firstTab()
{
    return example("home.examples.hello", "firstTab");
}

/**
Initialisation de l'appli, test des tabs
@return WTabWidget
*/
WWidget *EchoesHome::initTab()
{
    WContainerWidget *result = new WContainerWidget();
    tabList = new WTabWidget(AlignTop | AlignJustify, result);

    WAnimation animation(WAnimation::SlideInFromRight, WAnimation::EaseIn);
    tabList->contentsStack()->setTransitionAnimation(animation, true);

    WContainerWidget *test = new WContainerWidget();

    WText *w = new WText(tr("home.examples.hello"), test);
    w->setInternalPathEncoding(true);
    test->addWidget(linkSourceBrowser("hello"));


    WContainerWidget *test2 = new WContainerWidget();

    WText *w2 = new WText(tr("home.examples.chart"), test2);
    w2->setInternalPathEncoding(true);
    test->addWidget(linkSourceBrowser("charts"));

    tabList->addTab(test,tr("hello-world"))->setPathComponent("");

    tabList->addTab(test2,tr("charts"));

    //tabList->addTab(wrapView(&EchoesHome::firstTab()),tr("hello-world"))->setPathComponent("");

    //tabList->setInternalPathEnabled("/examples");

    return result;
}

WWidget *EchoesHome::linkSourceBrowser(const std::string& example)
{
    /*
     * Instead of using a WAnchor, which will not progress properly because
     * it is wrapped with wrapView() (-- should we not fix that?), we use
     * a WText which contains an anchor, and enable internal path encoding.
     */
    std::string path = "#/" + SRC_INTERNAL_PATH + "/" + example;
    WText *a = new WText(tr("source-browser-link").arg(path));
    a->setInternalPathEncoding(true);
    return a;
}

/**
Méthode qui créé un conteneur qui sera utilisé pour afficher le contenu d'un tab
@return WWidget
*/
WWidget *EchoesHome::example(const char *textKey, const std::string& sourceDir)
{
    WContainerWidget *result = new WContainerWidget();
    WText *w = new WText(tr(textKey), result);
    w->setInternalPathEncoding(true);
    result->addWidget(linkSourceBrowser(sourceDir));
    return result;
}

WWidget *EchoesHome::wrapView(WWidget *(EchoesHome::*createWidget)())
{
    return makeStaticModel(boost::bind(createWidget, this));
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
