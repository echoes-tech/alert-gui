/**
 * @file DashBoard.h
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
