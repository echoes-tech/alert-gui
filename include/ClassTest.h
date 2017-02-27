/**
 * @file ClassTest.h
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

#ifndef CLASSTEST_H
#define	CLASSTEST_H


#include "GlobalIncludeFile.h"

#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>
#include <Wt/WBreak>

#include <Wt/WInteractWidget>
#include <Wt/WDialog>
#include <Wt/WTableView>
#include <Wt/WLengthValidator>
#include <Wt/WContainerWidget>
#include <Wt/WAnchor>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTable>

// Ancien
#include <Wt/Dbo/Query>
#include <fstream>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/algorithm/string.hpp>

#include <Wt/Json/Value>

#include <vector>
#include <map>

#include <Wt/Http/Message>

#include <Wt/WInPlaceEdit>

#include <Wt/WMenuItem>

#include "CreatePageWidget.h"

class CreatePageWidget;

class ClassTest : 
public Wt::WContainerWidget 
{
public:
    ClassTest(Echoes::Dbo::Session *session, std::string apiUrl);

    void    initPage();
    void    update();
protected:
  Echoes::Dbo::Session               *session_;
  std::string           apiUrl_;
  bool                  newClass_;

};

#endif	/* CLASSTEST_H */
