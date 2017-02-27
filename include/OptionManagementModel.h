/**
 * @file OptionManagementModel.h
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

#ifndef OPTIONMANAGEMENTMODEL_H
#define	OPTIONMANAGEMENTMODEL_H

#include "GlobalIncludeFile.h"

class OptionManagementModel : public Wt::WFormModel 
{
public:
    OptionManagementModel();
    virtual ~OptionManagementModel();
    
    static const Field smsQuota;
    static const Field smsAsk;
    static const Field userRoleCombo;
    static const Field userRoleButton;
    virtual bool validateField(Field field);
    virtual void reset();
    void modifyField(Field field, const Wt::WString& info);
private:
    Echoes::Dbo::Session *session;
    
    void setView(Echoes::Dbo::User *user = NULL);
    virtual bool isVisible(Field field) const;
    
//    Wt::WString validateAssetName(Wt::WString stringToValidate) const;
    Wt::WString validateString(Wt::WString stringToValidate) const;
    
    Wt::WString label(Field field) const;
    void setValid(Field field);
};

#endif	/* OPTIONMANAGEMENTMODEL_H */
