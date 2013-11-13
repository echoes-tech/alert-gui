/* 
 * File:   RecipientsWidget.h
 * Author: gkr
 *
 * Created on 13 novembre 2013, 17:03
 */

#ifndef RECIPIENTSWIDGET_H
#define	RECIPIENTSWIDGET_H

#include <Wt/WTabWidget>
#include <Wt/WMenuItem>
#include <Wt/WInteractWidget>

#include "GlobalIncludeFile.h"

#include "CreatePageWidget.h"
#include "UserEditionWidget.h"

class UserEditionWidget;

class RecipientsWidget :
public Wt::WContainerWidget 
{
public:
    RecipientsWidget(Session *session, std::string apiUrl);

    void    update();
private:
    Session               *session_;
    std::string           apiUrl_;
    bool                  newClass_;
};

#endif	/* RECIPIENTSWIDGET_H */

