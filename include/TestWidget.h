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




class TestWidget : public Wt::WContainerWidget
{
public:

  TestWidget(Session *session);
  void testMenu(Wt::WString name);
protected:

  virtual void close();

  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);
  void createUI();
  Wt::WContainerWidget *createContentHeader();
  void updateBreadcrumbs(Wt::WString content);
  void createContainerFluid(Wt::WString content);
  void createContentDiv(Wt::WString content);
  
  
  
  


private:
  Session * session;
 
  bool created_;
  
  Wt::WContainerWidget *sideBarContainer;
  Wt::WContainerWidget *contentContainer;
  Wt::WContainerWidget *contentFluid;
  Wt::WContainerWidget *breadCrumbsContainer;
  Wt::WAnchor *breadCrumbsAnchor;
  Wt::WMenu *menu;
  
};



#endif	/* TESTWIDGET_H */

