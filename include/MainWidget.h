/**
 * @file MainWidget.h
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
#include "PluginsWidget.h"
#include "AbstractPage.h"
#include "AssetManagementWidget.h"
#include "UnitsWidget.h"
#include "DashBoard.h"
#include "ProbesWidget.h"

class RecipientsWidget;
class InformationsWidget;
class AlertsWidget;
class ProbesWidget;
class DashBoard;

class MainWidget : public Wt::WContainerWidget
{
public:

  MainWidget(Echoes::Dbo::Session *session, const std::string &apiUrl);
  void doActionMenu(int index = -1);
  
  Wt::WMenu *getMenu();
  Wt::WContainerWidget *getSideBarContainer();
  Wt::WContainerWidget *getFooterContainer();
  
//  AlertEditionWidget *aew;
  void  initFooter(void);
  void  initMenus();
  void  createUI();
  virtual void refresh();
  std::string getApiUrl() const;
  std::vector<Enums::EPageType>  *getPageDisplayVector();
  std::map<unsigned int,unsigned int> *getMenuIndexFromPageType();
  std::map<unsigned int,std::string> *getValueFromMenuIndex();
  
  void reset();

protected:
    
  virtual void close();

  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);
  
  Wt::WContainerWidget *createContentHeader();
  void updateTitle(unsigned int index);
  void updateBreadcrumbs();
  std::string getBreadcrumbsClass(int pathSize, int level);
  void createContainerFluid();
  void updateContainerFluid(int type);
  void createContentDiv();
  
  void createSubMenu(Enums::EPageType enumPT);
  void createPage(Enums::EPageType enumPT);
  std::string getIconName(Enums::EPageType enumPT);
  
  template <class C>
  void createMenuItem(C enumC, Wt::WMenu *submenu, std::string iconStr);
  
  void getRightsFromUser();
 
  PluginsWidget *plw;
//  AssetManagementModel *amm;
  AssetManagementWidget *amw;
  OptionManagementModel *omm;
  OptionManagementWidget *omw;
  UnitsWidget *unw;
//  AlertEditionModel *aem;
//  AlertListWidget *alw;
  RoleCustomizationWidget *rcw;
  ProbesWidget *prb;
  
  UserEditionWidget *uew;
  
  RecipientsWidget *rpw;
  InformationsWidget *inw;
  AlertsWidget *alw;
  DashBoard *dsw;
  
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
  
  std::vector<Enums::EPageType>  *m_pageDisplayRights;
  std::map<unsigned int,unsigned int> *m_indexFromPageType;
  std::map<unsigned int,std::string> *m_valueFromMenuIndex;

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
