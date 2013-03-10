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
    
    breadCrumbsContainer = new Wt::WContainerWidget(this);
    breadCrumbsContainer->setId("breadcrumb");
    
    breadCrumbsAnchor = new Wt::WAnchor("");
    breadCrumbsAnchor->setAttributeValue("class","tip-bottom");
    breadCrumbsContainer->addWidget(breadCrumbsAnchor);
    
    menu = new Wt::WMenu(sideBarContainer);
    
    
//    menu->setStyleClass("navbar navbar-inverse nav-tabs nav-stacked");

    // Add menu items using the default lazy loading policy.
    
    Wt::WMenuItem *itemTest1 = new Wt::WMenuItem("Test 1");
    itemTest1->setAttributeValue("name","t1");
    Wt::WMenuItem *itemTest2 = new Wt::WMenuItem("Test 2");
    itemTest2->setAttributeValue("name","t2");
    Wt::WMenuItem *itemTest3 = new Wt::WMenuItem("Test 3");
    itemTest3->setAttributeValue("name","t3");
    menu->addItem(itemTest1);
    menu->addItem(itemTest2);
    menu->addItem(itemTest3);
    
    menu->setInternalPathEnabled();
    
    
    
    Wt::WMenu *subMenu = new Wt::WMenu();
    
    Wt::WMenuItem *itemSubTest1 = new Wt::WMenuItem("Test Sub 1");
    itemTest1->setAttributeValue("name","ts1");
    Wt::WMenuItem *itemSubTest2 = new Wt::WMenuItem("Test Sub 2");
    itemTest2->setAttributeValue("name","ts2");
    subMenu->addItem(itemSubTest1);
    subMenu->addItem(itemSubTest2);
    
    menu->addMenu("test", subMenu);
    
    menu->select(0);
    
    menu->itemSelected().connect(boost::bind(&TestWidget::testMenu, this, menu->currentItem()->attributeValue("name")));
//    this->addWidget(contents);
    
    createContentDiv("test");

    
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

void TestWidget::updateBreadcrumbs(Wt::WString content)
{
    const Wt::WLink *test = new Wt::WLink("test");
    breadCrumbsAnchor->setText(content);
    breadCrumbsAnchor->setLink(*test);
}

void TestWidget::createContainerFluid(Wt::WString content)
{
    if (contentFluid)
    {
        delete contentFluid;
    }
    contentFluid = new Wt::WContainerWidget();
    contentFluid->setStyleClass("container-fluid");
    
    Wt::WLineEdit *edit = new Wt::WLineEdit(contentFluid);
    edit->setEmptyText(content);
    edit->setStyleClass("span8");

    edit = new Wt::WLineEdit(contentFluid);
    edit->setEmptyText(".span1");
    edit->setStyleClass("span1");

    edit = new Wt::WLineEdit(contentFluid);
    edit->setEmptyText(".span2");
    edit->setStyleClass("span2");

    edit = new Wt::WLineEdit(contentFluid);
    edit->setEmptyText(".span3");
    edit->setStyleClass("span3");

    edit = new Wt::WLineEdit(contentFluid);
    edit->setEmptyText(".span2");
    edit->setStyleClass("span2");
    contentContainer->addWidget(contentFluid);
}

void TestWidget::createContentDiv(Wt::WString content)
{
    contentContainer->addWidget(createContentHeader());
    contentContainer->addWidget(breadCrumbsContainer);
    createContainerFluid(content);
}

void TestWidget::testMenu(Wt::WString name)
{
//    if menu->currentItem()->is
    updateBreadcrumbs(name);
    createContainerFluid(menu->currentItem()->attributeValue("name"));
}

void TestWidget::close()
{
    delete this;
}