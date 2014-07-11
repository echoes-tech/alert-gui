/* 
 * GUI PluginsTableSourceWidget.h
 * 
 * @author ECHOES Technologies (MLA)
 * 
 * @date 11/07/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef PLUGINSTABLESOURCEWIDGET_H
#define	PLUGINSTABLESOURCEWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WAbstractItemModel>

#include "AbstractPage.h"

typedef std::map<long long, std::string>        MapLongString;

class AbstractPage;

class PluginsTableSourceWidget :
public AbstractPage
{
public:
                                PluginsTableSourceWidget(Echoes::Dbo::Session *session, std::string apiUrl, AbstractPage* abstractPage);
    void                        updatePage(bool getResources);
    
protected:    
    virtual std::string         addParameter();
    virtual void                setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*> argument);
    virtual void                setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*> argument);
    
private:
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    AbstractPage*               m_abstractPage;
    long long                   m_selectedPluginID;
};



#endif	/* PLUGINSTABLESOURCEWIDGET_H */

