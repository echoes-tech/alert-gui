/* 
 * Gui PluginsTablePluginWidget.h
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 15/11/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef PLUGINSTABLEPLUGINWIDGET_H
#define	PLUGINSTABLEPLUGINWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WAbstractItemModel>

#include "AbstractPage.h"

class AbstractPage;

class PluginsTablePluginWidget :
public AbstractPage
{
public:
    PluginsTablePluginWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    
protected:
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
    
private:
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    Wt::WStandardItemModel      *m_assetsStandardItemModel;
    Wt::WComboBox               *m_assetComboBox;
    
    void fillModel(Wt::Json::Value result);
    virtual void setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    virtual void setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
};



#endif	/* PLUGINSTABLEPLUGINWIDGET_H */

