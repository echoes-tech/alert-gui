/* 
 * File:   UnitsWidget.h
 * Author: tsa
 *
 * Created on 23 mai 2014, 16:00
 */

#ifndef UNITSWIDGET_H
#define	UNITSWIDGET_H



#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <tools/Enums.h>

#include "Conf.h"
#include "GlobalIncludeFile.h"
#include "AbstractPage.h"


class AbstractPage;

class UnitsWidget :
public AbstractPage 
{
public:
    UnitsWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    virtual ~UnitsWidget();
protected:
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
private:
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    
    Wt::WComboBox               *m_unitTypeComboBox;
    Wt::WStandardItemModel      *m_unitTypeModel;
    
    virtual void setAddResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    virtual void setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*> argument);
    void fillModel();
};

#endif	/* UNITSWIDGET_H */

