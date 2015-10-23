/* 
 * File:   HelpWidget.h
 * Author: cde
 *
 * Created on 19 octobre 2015, 11:58
 */

#ifndef HELPWIDGET_H
#define	HELPWIDGET_H

#include <tools/Enums.h>

#include "AbstractPage.h"

class AbstractPage;

class HelpWidget :
public AbstractPage
{

public:
    HelpWidget(Echoes::Dbo::Session *session, std::string apiUrl);
protected:
    
private:
    void                        refresh();

};


#endif	/* HELPWIDGET_H */

