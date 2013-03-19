/* 
 * File:   MonitoringWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "TestWidget.h"

TestWidget::TestWidget(Session *session)
: Wt::WContainerWidget()
{
    created_ = false;
    this->session = session;
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("test",false);      
    createUI();
}

void TestWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
//        update();
        created_ = true;
    }

    Wt::WContainerWidget::render(flags);
}

void TestWidget::createUI()
{
//    this->mainStack->addWidget(this);
    
//    this->setId("content");
    
    // Create a stack where the contents will be located.
//    Wt::WStackedWidget *contents = new Wt::WStackedWidget();
//    contents->setId("content");
    
    sideBarContainer = new Wt::WContainerWidget(this);
    sideBarContainer->setId("sidebar");
    
    contentContainer = new Wt::WContainerWidget(this);
    contentContainer->setId("content");
    
    
    // BREADCRUMBS
    breadCrumbsContainer = new Wt::WContainerWidget(this);
    breadCrumbsContainer->setId("breadcrumb");
    
    breadCrumbsAnchor0 = new Wt::WAnchor("");
    breadCrumbsAnchor0->setAttributeValue("class","tip-bottom");
    
    breadCrumbsAnchor1 = new Wt::WAnchor("");
    breadCrumbsAnchor1->setAttributeValue("class","tip-bottom");
    
    breadCrumbsAnchor2 = new Wt::WAnchor("");
    breadCrumbsAnchor2->setAttributeValue("class","tip-bottom");
    
    const Wt::WLink *test = new Wt::WLink("");
    breadCrumbsAnchor0->setText("Accueil");
    breadCrumbsAnchor0->setLink(*test);
    
    breadCrumbsContainer->addWidget(breadCrumbsAnchor0);
//    breadCrumbsContainer->addWidget(breadCrumbsAnchor1);
//    breadCrumbsContainer->addWidget(breadCrumbsAnchor2);
    // END BREADCRUMBS
    
    createContentDiv("test");
    createContainerFluid();
    
    
    menu = new Wt::WMenu(sideBarContainer);
    
    
//    menu->setStyleClass("navbar navbar-inverse nav-tabs nav-stacked");

    
    
    
    createMenuItem(Enums::WELCOME);
    createPage(Enums::WELCOME);
    createMenuItem(Enums::ASSET);
    createPage(Enums::ASSET);
    
    menu->setInternalPathEnabled("/");
    
    
    
    Wt::WMenu *subMenu = new Wt::WMenu();
    
    Wt::WMenuItem *itemSubTest1 = new Wt::WMenuItem("Test Sub 1");
    itemSubTest1->setAttributeValue("name","11");
    Wt::WMenuItem *itemSubTest2 = new Wt::WMenuItem("Test Sub 2");
    itemSubTest2->setAttributeValue("name","21");
    subMenu->addItem(itemSubTest1);
    subMenu->addItem(itemSubTest2);
    
    menu->addMenu("test", subMenu);
    
    std::string internalPath = Wt::WApplication::instance()->internalPath();
    
    if (internalPath == "/assets/")
    {
        UserActionManagement::registerUserAction(Enums::display,"/assets/",0);
        menu->select(Enums::ASSET);
        testMenu(Enums::ASSET);
    }
    if (internalPath == "/welcome/")
    {
        UserActionManagement::registerUserAction(Enums::display,"/welcome/",0);
        menu->select(Enums::WELCOME);
        testMenu(Enums::WELCOME);
    }
    
    if (menu->currentIndex() != -1)
    {
        menu->itemSelected().connect(boost::bind(&TestWidget::testMenu, this,-1));
    }
//    this->addWidget(contents);
    
    
    
    

    
//    new ScatterPlot(this);
}

Wt::WContainerWidget * TestWidget::createContentHeader()
{
    Wt::WContainerWidget *res = new Wt::WContainerWidget();
    Wt::WHBoxLayout *layout = new Wt::WHBoxLayout();
    Wt::WText *text = new Wt::WText("TEST");
    text->setWidth("60%");
    res->setWidth("70%");
    res->setLayout(layout);
    layout->addWidget(text);
    res->setId("content-header");
    return res;
}

void TestWidget::createMenuItem(Enums::EPageType type)
{
    switch (type)
    {
        case Enums::ASSET:
        {
            Wt::WMenuItem *itemTest1 = new Wt::WMenuItem(tr("Alert.admin.asset-tab"));
            itemTest1->setAttributeValue("name",boost::lexical_cast<std::string>(type));
            itemTest1->setPathComponent("assets/");
            menu->addItem(itemTest1);
            break;
        }
        case Enums::WELCOME:
        {
            Wt::WMenuItem *itemTest1 = new Wt::WMenuItem(tr("Alert.admin.welcome-tab"));
            itemTest1->setAttributeValue("name",boost::lexical_cast<std::string>(type));
            itemTest1->setPathComponent("welcome/");
            menu->addItem(itemTest1);
            break;
            break;
        }
        default:
            break;
    }
        
                
    
}

void TestWidget::createPage(Enums::EPageType type)
{
    switch (type)
    {
        case Enums::ASSET:
        {
            try
            {
                Wt::Dbo::Transaction transaction(*(this->session));
                amm = new AssetManagementModel();
                transaction.commit();
            }
            catch (Wt::Dbo::Exception e)
            {
                Wt::log("error") << e.what();
            }
            amw = new AssetManagementWidget(amm,this->session);
            break;
        }
        case Enums::WELCOME:
        {
            wcw = new Wt::WText(tr("welcome-text"));
            break;
        }
        default:
            break;
    }

}


void TestWidget::updateBreadcrumbs()
{
    this->breadCrumbsContainer->removeWidget(breadCrumbsAnchor1);
    this->breadCrumbsContainer->removeWidget(breadCrumbsAnchor2);
    
    std::string internalPath = Wt::WApplication::instance()->internalPath();
    std::vector<std::string> internalPathSplitResult;
    boost::split(internalPathSplitResult, internalPath, boost::is_any_of("/"), boost::token_compress_on);
    
    
    
    for (unsigned int i = 1; i < internalPathSplitResult.size(); i++)
    {
        std::cout << internalPathSplitResult[i] << std::endl;
        if (i >= 4)
        {
            break;
        }
        
        switch (i)
        {
            case 1:
            {
                const Wt::WLink *test = new Wt::WLink(internalPathSplitResult[i]);
                breadCrumbsAnchor0->setText(internalPathSplitResult[i]);
                breadCrumbsAnchor0->setLink(*test);
                break;
            }
            case 2:
            {
                if (internalPathSplitResult[i].compare(""))
                {
                    const Wt::WLink *test = new Wt::WLink(internalPathSplitResult[i-1] + "/" + internalPathSplitResult[i]);
                    breadCrumbsAnchor1->setText(internalPathSplitResult[i]);
                    breadCrumbsAnchor1->setLink(*test);
                    this->breadCrumbsContainer->addWidget(breadCrumbsAnchor1);
                }
                break;
            }
            case 3:
            {
                if (internalPathSplitResult[i].compare(""))
                {
                    const Wt::WLink *test = new Wt::WLink(internalPathSplitResult[i-1] + "/" + internalPathSplitResult[i]);
                    breadCrumbsAnchor2->setText(internalPathSplitResult[i]);
                    breadCrumbsAnchor2->setLink(*test);
                    this->breadCrumbsContainer->addWidget(breadCrumbsAnchor2);
                }
                break;
            }
            default:
            {
                break;
            }
        }

    }

}

void TestWidget::updateContainerFluid(int type)
{
    for (int i = 0 ; i < this->contentFluid->count() ; i++)
    {
        contentFluid->removeWidget(contentFluid->widget(i));
    }
    
    switch (type)
    {
        case Enums::ASSET:
        {
            this->contentFluid->addWidget(amw);
            break;
        }
        case Enums::WELCOME:
        {
            this->contentFluid->addWidget(wcw);
            break;
        }
        default:
            break;
    }

}

void TestWidget::createContainerFluid()
{
    contentFluid = new Wt::WContainerWidget();
    contentFluid->setStyleClass("container-fluid");
    
    
    contentContainer->addWidget(contentFluid);
}

void TestWidget::createContentDiv(Wt::WString content)
{
    contentContainer->addWidget(createContentHeader());
    contentContainer->addWidget(breadCrumbsContainer);
}

void TestWidget::testMenu(int index)
{
//    if menu->currentItem()->is
    updateBreadcrumbs();
    updateContainerFluid(index);
    if ((index != -1) && (this->menu->currentIndex() != index))
    {
        this->menu->select(index);
    }
}

Wt::WMenu * TestWidget::getMenu()
{
    return this->menu;
}

void TestWidget::close()
{
    delete this;
}