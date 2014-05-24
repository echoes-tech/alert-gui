/* 
 * File:   TestWidget.h
 * Author: tsa
 *
 * Created on 30 nov 2012, 11:50
 */

#ifndef TESTWIDGET_H
#define	TESTWIDGET_H

#include "GlobalIncludeFile.h"

#include <Wt/WBreak>
#include <Wt/WContainerWidget>
#include <Wt/WMenu>
#include <Wt/WStackedWidget>
#include <Wt/WTextArea>
#include <Wt/WText>

#include <memory>
#include <iostream>
#include <fstream>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>

#include <Wt/WApplication>
#include <Wt/WDate>
#include <Wt/WEnvironment>
#include <Wt/WItemDelegate>
#include <Wt/WStandardItemModel>
#include <Wt/WText>

#include <Wt/WBorderLayout>
#include <Wt/WFitLayout>

#include <Wt/WStandardItem>
#include <Wt/WTableView>

#include <Wt/Chart/WCartesianChart>
#include <Wt/Chart/WPieChart>

#include "RecipientsWidget.h"
#include "InformationsWidget.h"
#include "AlertsWidget.h"
#include "Association.h"
#include "AbstractPage.h"
#include "AssetManagementWidget.h"
#include "UnitsWidget.h"

class RecipientsWidget;
class InformationsWidget;
class AlertsWidget;
class Association;

class MainWidget : public Wt::WContainerWidget
{
public:

  MainWidget(Echoes::Dbo::Session *session, const std::string &apiUrl);
  void doActionMenu(int index = -1, Enums::EMenuRoot menuRoot = Enums::EMenuRoot::main);
  
  Wt::WMenu *getMenu();
  Wt::WContainerWidget *getSideBarContainer();
  Wt::WContainerWidget *getFooterContainer();
  
//  AlertEditionWidget *aew;
  void  initFooter(void);
  void  initMenus();
  void  createUI();
  virtual void refresh();
  std::string getApiUrl() const;
  void reset(Echoes::Dbo::Session *session);

protected:
    
  virtual void close();

  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);
  
  Wt::WContainerWidget *createContentHeader();
  void updateTitle(unsigned int index, Enums::EMenuRoot menuRoot = Enums::EMenuRoot::main);
  void updateBreadcrumbs(Enums::EMenuRoot menuRoot = Enums::EMenuRoot::main);
  std::string getBreadcrumbsClass(int pathSize, int level);
  void createContainerFluid();
  void updateContainerFluid(int type, Enums::EMenuRoot menuRoot = Enums::EMenuRoot::main);
  void createContentDiv();
  
  void createSubMenu(Enums::EPageType enumPT);
  void createPage(Enums::EPageType enumPT);
  std::string getIconName(Enums::EPageType enumPT);
  void createAccountPage(Enums::EAccountSubmenu enumSAC);
  
  template <class C>
  void createMenuItem(C enumC, Wt::WMenu *submenu, std::string iconStr);
 
  PluginEditionWidget *pew;
//  AssetManagementModel *amm;
  AssetManagementWidget *amw;
  OptionManagementModel *omm;
  OptionManagementWidget *omw;
  UnitsWidget *unw;
//  AlertEditionModel *aem;
//  AlertListWidget *alw;
  RoleCustomizationWidget *rcw;
  
  UserEditionWidget *uew;
  
  RecipientsWidget *rpw;
  InformationsWidget *inw;
  AlertsWidget *alw;
  Association *act;
  
  Wt::WContainerWidget *wcw;
  

private:
  Echoes::Dbo::Session * session;
  std::string _apiUrl;
 
  bool created_;
  
  Wt::WText *titleText;
  Wt::WContainerWidget *sideBarContainer;
  Wt::WContainerWidget *footerContainer;
  Wt::WContainerWidget *contentContainer;
  Wt::WContainerWidget *contentFluid;
  Wt::WContainerWidget *breadCrumbsContainer;
  Wt::WAnchor *breadCrumbsAnchor0;
  Wt::WAnchor *breadCrumbsAnchor1;
  Wt::WAnchor *breadCrumbsAnchor2;
  Wt::WMenu *menu;
  Wt::WMenu *accountSubmenu;

  void setApiUrl(std::string _apiUrl);
  
};


template <class C>
void MainWidget::createMenuItem(C enumC, Wt::WMenu *submenu, std::string iconStr)
{
    Wt::WMenuItem *newMenuItem = new Wt::WMenuItem(tr("Alert.admin." + boost::lexical_cast<std::string>(enumC.value()) + "-tab"));
    
    // Todo : renommer alertCount, voir comment indiquer la présence de sous-menus
    Wt::WText *alertCount = new Wt::WText("</span><i class='icon icon-" + iconStr + "'></i><span>",Wt::XHTMLUnsafeText);
//    alertCount->setAttributeValue("class","label label-important");
    Wt::WAnchor *anchorTemp = (Wt::WAnchor*)newMenuItem->widget(0);
    anchorTemp->insertWidget(0,alertCount);

    
    newMenuItem->setPathComponent(enumC.value());
    submenu->addItem(newMenuItem);
}





#endif	/* TESTWIDGET_H */
