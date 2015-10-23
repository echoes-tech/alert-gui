/* 
 * GUI HelpWidget.cpp
 * 
 * @author ECHOES Technologies (CDE)
 * 
 * @date 19/09/2015
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "HelpWidget.h"

using namespace std;

HelpWidget::HelpWidget(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "help", true)
{
    
}

void HelpWidget::refresh()
{
    if (!Wt::WApplication::instance()->internalPathMatches("/help"))
    {
        m_autoUpdate = true;
        return;
    }
    Wt::WApplication::instance()->triggerUpdate();
}
