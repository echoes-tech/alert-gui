/* 
 * File:   MonitoringWidget.h
 * Author: tsa
 *
 * Created on 30 nov 2012, 11:50
 */

#ifndef MONITORINGWIDGET_H
#define	MONITORINGWIDGET_H

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

class MonitoringWidget : public Wt::WContainerWidget
{
public:

  MonitoringWidget(Session *session);

protected:

  virtual void close();

  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);
  void createUI();

private:
  Session * session;
 
  bool created_;
  
};

class ScatterPlot: public Wt::WContainerWidget
{
public:
  /*! \brief Creates the scatter plot example
   */
  ScatterPlot(Wt::WContainerWidget *parent);
};


#endif	/* MONITORINGWIDGET_H */

