/* 
 * File:   ClassTest.h
 * Author: gkr
 *
 * Created on 23 octobre 2013, 14:24
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
    ClassTest(Session *session, std::string apiUrl);

    void    initPage();
    void    update();
protected:
  Session               *session_;
  std::string           apiUrl_;
  bool                  newClass_;

};

#endif	/* CLASSTEST_H */

