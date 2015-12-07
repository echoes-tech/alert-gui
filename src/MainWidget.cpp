/* 
 * Gui MainWidget.cpp
 * 
 * @author ECHOES Technologies (TSA)
 * 
 * @date 14/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "MainWidget.h"
#include "SummaryBoard.h"

using namespace std;

MainWidget::MainWidget(Echoes::Dbo::Session *session, const string &apiUrl)
: Wt::WContainerWidget()
{
    setApiUrl(apiUrl);
    
    
    contentContainer = new Wt::WContainerWidget(this);

    sideBarContainer = new Wt::WContainerWidget();
    footerContainer = new Wt::WContainerWidget();
    
    breadCrumbsContainer = new Wt::WContainerWidget(this);
    
    this->session = session;
    
    m_pageDisplayRights = new vector<Enums::EPageType>();
    m_indexFromPageType = new map<unsigned int, unsigned int>();
    m_valueFromMenuIndex = new map<unsigned int, string>();
    reset();
    
}

void MainWidget::getRightsFromUser()
{
    m_pageDisplayRights->clear();
    unsigned int menuIndex = 0;
    for (auto i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
    {
        switch (i->index())
        {
            case Enums::EPageType::DASHBOARD:
            {
                break;
            }
            default:
            {
                m_pageDisplayRights->push_back(*i);
                (*m_indexFromPageType)[i->index()] = menuIndex;
                (*m_valueFromMenuIndex)[menuIndex++] = i->value();
                break;
            }
        }
        
    }
}

void MainWidget::reset()
{
    delete breadCrumbsContainer;
    // voir si utile, refait dans initMenus()
    sideBarContainer->clear();
    footerContainer->clear();
    contentContainer->clear();
    
    getRightsFromUser();
    
    titleText = new Wt::WText();
    breadCrumbsContainer = new Wt::WContainerWidget(this);
    breadCrumbsAnchor0 = new Wt::WAnchor("");
    breadCrumbsAnchor1 = new Wt::WAnchor("");
    breadCrumbsAnchor2 = new Wt::WAnchor("");
    breadCrumbsAnchor0->setRefInternalPath("/messages");
    created_ = false;
    
    
    try
    {
        Wt::Dbo::Transaction transaction(*(this->session));
        const_cast<Echoes::Dbo::User *>(this->session->user().get());
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << e.what();
    }
    
    // Todo : check, sans doute inutile
    initMenus();
    initFooter();
    contentContainer->setId("content");
    breadCrumbsContainer->setId("breadcrumb");
    
    
}


// Peut-être à supprimer maintenant qu'on ne surcharge plus la méthode
void MainWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
//    if (!created_)
//    {
////        update();
//        created_ = true;
//    }

    Wt::WContainerWidget::render(flags);
}

void MainWidget::initMenus(void)
{
    sideBarContainer->clear();
    sideBarContainer->setId("sidebar");
        
    Wt::WAnchor *phoneMenuAnchor = new Wt::WAnchor("#");
    phoneMenuAnchor->setText("Menu");
    phoneMenuAnchor->setAttributeValue("class","visible-phone");
    
    sideBarContainer->insertWidget(0,phoneMenuAnchor);
    menu = new Wt::WMenu(sideBarContainer);
    menu->setInternalPathEnabled("/");
    
}
    
void    MainWidget::initFooter(void)
{
    footerContainer->clear();
    footerContainer->setId("footer");
    Wt::WText   *footerText = new Wt::WText(tr("Footer.text.date") + " "
                            + Wt::WDateTime::currentDateTime().toString("yyyy") + " "
                            + tr("Footer.text") 
                            + "<br />"
                            + "Version " + EchoesHome::version_g);
    footerContainer->insertWidget(0, footerText);
}

void MainWidget::createUI()
{
    if (!created_)
    {
        reset();
        initMenus();
        createContentDiv();
        createContainerFluid();

        breadCrumbsAnchor0->setTextFormat(Wt::XHTMLUnsafeText);
        breadCrumbsAnchor0->setText("<i class='icon-home'></i>" + tr("Alert.admin.home"));

        breadCrumbsContainer->addWidget(breadCrumbsAnchor0);


        for (auto i = m_pageDisplayRights->begin(); i != m_pageDisplayRights->end(); ++i)
        {
            createMenuItem(*i, menu, getIconName(*i));
            createPage(*i);
        }
        created_ = true;
    }
}

Wt::WContainerWidget * MainWidget::createContentHeader()
{
    Wt::WContainerWidget *res = new Wt::WContainerWidget();
    Wt::WHBoxLayout *layout = new Wt::WHBoxLayout();
    res->setLayout(layout);
    layout->addWidget(titleText);
    res->setId("content-header");
    return res;
}


void MainWidget::createPage(Enums::EPageType enumPT)
{
    switch (enumPT.index())
    {
        case Enums::EPageType::ASSET:
        {
            amw = new AssetManagementWidget(this->session, this->_apiUrl);
            break;
        }
        /*case Enums::EPageType::WELCOME:
        {
            wcw = new SummaryBoard(this->session);
            break;
        }*/
        case Enums::EPageType::DASHBOARD:
        {
            dsw = new DashBoard(this->session);
            break;
        }
        case Enums::EPageType::RECIPIENTS:
        {
            rpw = new RecipientsWidget(this->session, this->_apiUrl);
            break;            
        }
        case Enums::EPageType::INFORMATIONS:
        {
            inw = new InformationsWidget(this->session, this->_apiUrl);
            break;
        }
        case Enums::EPageType::ALERTS:
        {
            alw = new AlertsWidget(this->session, this->_apiUrl);
            break;            
        }     
        case Enums::EPageType::PLUGIN:
        {
            plw = new PluginsWidget(this->session, _apiUrl);
            break;            
        }      
        case Enums::EPageType::ALIASES:
        {
            rcw = new AliasesWidget(session, _apiUrl);
            break;
        }
        case Enums::EPageType::OPTIONS:
        {
            try
            {
                Wt::Dbo::Transaction transaction(*(this->session));
                omm = new OptionManagementModel();
                transaction.commit();
            }
            catch (Wt::Dbo::Exception e)
            {
                Wt::log("error") << e.what();
            }
            omw = new OptionManagementWidget(omm, this->session, _apiUrl);
            break;
        }
        case Enums::EPageType::UNITS:
        {
            unw = new UnitsWidget(this->session, this->_apiUrl);
            break;
        }
        case Enums::EPageType::PROBES:
        {
            prb = new ProbesWidget(this->session, this->_apiUrl);
            break ;
        }
        case Enums::EPageType::MESSAGES:
        {
            msw = new MessagesWidget(this->session, this->_apiUrl);
            break ;
        }
        case Enums::EPageType::HELP:
        {
            hpw = new HelpWidget(this->session, this->_apiUrl);
            break;
        }
        case Enums::EPageType::ROLES:
        {
            row = new RolesWidget(this->session, this->_apiUrl);
            break ;
        }
        default:
            break;
    }

}

// ToDo : template ?
void MainWidget::updateTitle(unsigned int index)
{
    for (Enums::EPageType::const_iterator i = Enums::EPageType::begin(); i != Enums::EPageType::end(); ++i)
    {
        if (i->index() == index)
        {
            this->titleText->setText("<h1>" + tr(boost::lexical_cast<string>("Alert.admin.")+i->value()+boost::lexical_cast<string>("-tab")) + "</h1>");
            break;
        }
    }
}

void MainWidget::updateBreadcrumbs()
{
    this->breadCrumbsContainer->removeWidget(breadCrumbsAnchor2);
    
    string internalPath = Wt::WApplication::instance()->internalPath();
    vector<string> internalPathSplitResult;
    vector<string> internalPathWithoutBlank;
    boost::split(internalPathSplitResult, internalPath, boost::is_any_of("/"), boost::token_compress_on);
    for (vector<string>::const_iterator i = internalPathSplitResult.begin() ; i != internalPathSplitResult.end() ; i++)
    {
        if (i->compare("") != 0)
        {
            internalPathWithoutBlank.push_back(*i);
        }
    }
    
    for (unsigned int i = 0; i < internalPathWithoutBlank.size(); i++)
    {
        if (i >= 3)
        {
            break;
        }
        
        switch (i+1)
        {
            case 1:
            {
                breadCrumbsAnchor1->setText(tr(boost::lexical_cast<string>("Alert.admin.")+internalPathWithoutBlank[i]+boost::lexical_cast<string>("-tab")));
                breadCrumbsAnchor1->setRefInternalPath("/" + internalPathWithoutBlank[i]);
                this->breadCrumbsContainer->addWidget(breadCrumbsAnchor1);
                breadCrumbsAnchor1->setAttributeValue("class", getBreadcrumbsClass(internalPathWithoutBlank.size(),1).c_str());
                 break;
            }
            case 2:
            {
                breadCrumbsAnchor2->setText(tr(boost::lexical_cast<string>("Alert.admin.")+internalPathWithoutBlank[i]+boost::lexical_cast<string>("-tab")));
                breadCrumbsAnchor2->setRefInternalPath("/" + internalPathWithoutBlank[i-1] + "/" + internalPathWithoutBlank[i]);
                this->breadCrumbsContainer->addWidget(breadCrumbsAnchor2);
                breadCrumbsAnchor2->setAttributeValue("class", getBreadcrumbsClass(internalPathWithoutBlank.size(),2).c_str());
                break;
            }
            default:
            {
                break;
            }
        }

    }

}

string MainWidget::getBreadcrumbsClass(int pathSize, int level)
{
    string res;
    if (pathSize == level)
    {
        res = "current";
    }
    else
    {
        res = "";
    }
    return res;
}

void MainWidget::updateContainerFluid(int type)
{
    for (int i = 0 ; i < this->contentFluid->count() ; i++)
    {
        contentFluid->removeWidget(contentFluid->widget(i));
    }
    

    switch (type)
    {
        /*case Enums::EPageType::WELCOME:
        {
            wcw->refresh();
            this->contentFluid->addWidget(wcw);
            break;
        }*/
        case Enums::EPageType::DASHBOARD:
        {
            this->contentFluid->addWidget(dsw);
            break;
        }
        case Enums::EPageType::ASSET:
        {
            amw->updatePage();
            this->contentFluid->addWidget(amw);
            break;
        }
        case Enums::EPageType::RECIPIENTS:
        {
            rpw->update();
            this->contentFluid->addWidget(rpw);
            break;
        }
        case Enums::EPageType::INFORMATIONS:
        {
            inw->updatePage();
            this->contentFluid->addWidget(inw);
            break;
        }
        case Enums::EPageType::ALERTS:
        {
            alw->updatePage();
            this->contentFluid->addWidget(alw);
            break;
        }
        case Enums::EPageType::PLUGIN:
        {
            plw->update();
            this->contentFluid->addWidget(plw);
            break;
        }
        case Enums::EPageType::ALIASES:
        {
            rcw->refresh();
            this->contentFluid->addWidget(rcw);
            break;
        }
        case Enums::EPageType::OPTIONS:
        {
            omw->refresh();
            this->contentFluid->addWidget(omw);
            break;
        }
        case Enums::EPageType::UNITS:
        {
            unw->updatePage();
            this->contentFluid->addWidget(unw);
            break;
        }
        case Enums::EPageType::PROBES:
        {
            prb->getResourceList();
            this->contentFluid->addWidget(prb);
            break;
        }
        case Enums::EPageType::ROLES:
        {
            row->getResourceList();
            this->contentFluid->addWidget(row);
            break;
        }
        case Enums::EPageType::MESSAGES:
        {
            msw->update();
            this->contentFluid->addWidget(msw);
            break;
        }
        case Enums::EPageType::HELP:
        {
            hpw->updatePage();
            this->contentFluid->addWidget(hpw);
            break;
        }
        default:
            break;
    }
 
}

void MainWidget::createContainerFluid()
{
    contentFluid = new Wt::WContainerWidget();
    contentFluid->setStyleClass("container-fluid");
    contentContainer->addWidget(contentFluid);
}

void MainWidget::createContentDiv()
{
    contentContainer->addWidget(createContentHeader());
    contentContainer->addWidget(breadCrumbsContainer);
}

void MainWidget::doActionMenu(int index)
{
    if ((*m_indexFromPageType)[index] < (unsigned)menu->count())
    {
        menu->itemAt((*m_indexFromPageType)[index])->setFromInternalPath(Wt::WApplication::instance()->internalPath());
        // ToDo open the menu if necessary
        updateTitle(index);
        updateBreadcrumbs();
        updateContainerFluid(index);
    }
    else
    {
        Wt::WApplication::instance()->setInternalPath("/messages",  false);
        menu->itemAt(0)->setFromInternalPath("/messages");
        updateTitle(0);
        updateBreadcrumbs();
        updateContainerFluid(0);
    }
    
    
}

string MainWidget::getIconName(Enums::EPageType enumPT)
{
    string res = "home";
    switch (enumPT.index())
    {
        /*case Enums::EPageType::WELCOME:
        {
            res = "home";
            break;
        }*/
        case Enums::EPageType::DASHBOARD:
        {
            res = "list";
            break;
        }
        case Enums::EPageType::ASSET:
        {
            res = "hdd";
            break;
        }
        case Enums::EPageType::RECIPIENTS:
        {
            res = "globe"; //group
            break;
        }
        case Enums::EPageType::INFORMATIONS:
        {
            res = "eye-open"; 
            break;
        }
        case Enums::EPageType::ALERTS:
        {
            res = "bell"; 
            break;
        }
        case Enums::EPageType::PLUGIN:
        {
            res = "pencil";
            break;
        }
        case Enums::EPageType::ALIASES:
        {
            res = "film";
            break;
        }
        case Enums::EPageType::OPTIONS:
        {
            res = "check";
            break;
        }
        case Enums::EPageType::UNITS:
        {
            res = "tasks";
            break;
        }
        case Enums::EPageType::PROBES:
        {
            res = "search";
            break ;
        }
        case Enums::EPageType::ROLES:
        {
            res = "user";
            break ;
        }
        case Enums::EPageType::MESSAGES:
        {
            res = "envelope";
            break ;
        }
        case Enums::EPageType::HELP:
        {
            res = "book";
            break ;
        }
        default:
            res = "home";
            break;
    }
    return res;
}

Wt::WMenu * MainWidget::getMenu()
{
    return this->menu;
}

Wt::WContainerWidget * MainWidget::getSideBarContainer()
{
    return this->sideBarContainer;
}

Wt::WContainerWidget * MainWidget::getFooterContainer()
{
    return this->footerContainer;
}

void MainWidget::close()
{
    delete this;
}

void MainWidget::refresh()
{
//    this->aew->refresh();
//    this->amw->refresh();
}

void MainWidget::setApiUrl(string apiUrl)
{
    _apiUrl = apiUrl;
    return;
}

string MainWidget::getApiUrl() const
{
    return _apiUrl;
}

vector<Enums::EPageType> *MainWidget::getPageDisplayVector()
{
    return m_pageDisplayRights;
}

map<unsigned int,unsigned int> *MainWidget::getMenuIndexFromPageType()
{
    return m_indexFromPageType;
}

std::map<unsigned int,string> *MainWidget::getValueFromMenuIndex()
{
    return m_valueFromMenuIndex;
}
