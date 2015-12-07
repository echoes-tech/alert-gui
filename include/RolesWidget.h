/* 
 * File:   RolesWidget.h
 * Author: cde
 *
 * Created on 27 janvier 2015, 15:05
 */

#ifndef ROLESWIDGET_H
#define	ROLESWIDGET_H

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
#include <Wt/WTheme>

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

#include <tools/Enums.h>

#include "Conf.h"
#include "AbstractPage.h"

class AbstractPage;

class RolesWidget :
public AbstractPage
{
public: 
    RolesWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    virtual ~RolesWidget();
    void    recoverListRoles();

protected:
  private:
};

#endif	/* ROLESWIDGET_H */

