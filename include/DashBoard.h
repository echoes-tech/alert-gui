/* 
 * File:   DashBoard.h
 * Author: tsa
 *
 * Created on 26 jun 2014, 11:50
 */

#ifndef DASHBOARD_H
#define	DASHBOARD_H

#include "GlobalIncludeFile.h"

#include <Wt/WAnchor>
#include <Wt/WApplication>
#include <Wt/WContainerWidget>
#include <Wt/WDialog>
#include <Wt/WImage>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>

#include <Wt/WBreak>
#include <Wt/WDatePicker>
#include <Wt/WSelectionBox>
#include <Wt/WIntValidator>
#include <Wt/WTextArea>
#include <Wt/WFileResource>

#include <Wt/WComboBox>
#include <Wt/WSelectionBox>
#include <Wt/WStringListModel>
#include <Wt/Dbo/Query>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/Mail/Client>
#include <Wt/Mail/Message>

#include <Wt/WMessageBox>

#include <Wt/WVBoxLayout>
#include <Wt/WHBoxLayout>
#include <Wt/WTable>
#include <Wt/WTableCell>
#include <Wt/WTableView>

#include <Wt/Utils>
#include <Wt/WLabel>

#include <Wt/WGroupBox>
#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>

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


    #include <Wt/WApplication>
    #include <Wt/WContainerWidget>
    #include <Wt/WEnvironment>
    #include <Wt/WStandardItemModel>
    #include <Wt/WStandardItem>
    #include <Wt/WTableView>

#include "CsvUtil.h"

class DashBoard : public Wt::WContainerWidget
{
public:

  DashBoard(Echoes::Dbo::Session *session);

protected:

  virtual void close();

  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);
  void createUI();

private:
  Echoes::Dbo::Session * session;
  Wt::WTemplate *mainTemplate;
 
  bool created_;
  
  Wt::WContainerWidget * addPieChart(Wt::WStandardItemModel *model);
  Wt::WContainerWidget * addCategoryChart(Wt::WStandardItemModel *model);
  
};


#endif	/* DASHBOARD_H */

