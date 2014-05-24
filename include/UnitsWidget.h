/* 
 * File:   UnitsWidget.h
 * Author: tsa
 *
 * Created on 23 mai 2014, 16:00
 */

#ifndef UNITSWIDGET_H
#define	UNITSWIDGET_H

#include "GlobalIncludeFile.h"

#include <tools/Enums.h>

#include "Conf.h"
#include "AbstractPage.h"

class AbstractPage;

class UnitsWidget :
public AbstractPage 
{
public:
    UnitsWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    virtual ~UnitsWidget();
private:
    Echoes::Dbo::Session   *session_;
    std::string            apiUrl_;
    
    virtual void setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*> argument);
    
};

#endif	/* UNITSWIDGET_H */

