/* 
 * File:   PluginsWidget.h
 * Author: gdr
 *
 * Created on 18 mars 2013, 11:57
 */

#ifndef PLUGINSWIDGET_H
#define	PLUGINSWIDGET_H

#include "PluginsTablePluginWidget.h"


class PluginsWidget : 
public Wt::WContainerWidget 
{
    public:
    
        PluginsWidget(Echoes::Dbo::Session *session, const std::string &apiUrl);
        void update();

    private:
        Echoes::Dbo::Session *session_;   
        std::string apiUrl_;
        bool newClass_;
};
#endif	/* PLUGINSWIDGET_H */

