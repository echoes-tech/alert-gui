/**
 * @file PluginsWidget.h
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#ifndef PLUGINSWIDGET_H
#define	PLUGINSWIDGET_H

#include "PluginsTablePluginWidget.h"
#include "PluginsTableSourceWidget.h"
#include "PluginsTableSearchWidget.h"
#include "PluginsTableFilterWidget.h"
#include "PluginsTableAssociationWidget.h"


class PluginsWidget : 
public Wt::WContainerWidget 
{
    public:
        PluginsWidget(Echoes::Dbo::Session *session, const std::string apiUrl);
        void update();

    private:
        Echoes::Dbo::Session *session_;   
        std::string apiUrl_;
        bool newClass_;
};
#endif	/* PLUGINSWIDGET_H */
