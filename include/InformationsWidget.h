/**
 * @file InformationsWidget.h
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

#ifndef INFORMATIONSWIDGET_H
#define	INFORMATIONSWIDGET_H

#include "GlobalIncludeFile.h"

#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>
#include <Wt/WBreak>

#include <Wt/WInteractWidget>
#include <Wt/WDialog>
#include <Wt/WTableView>
#include <Wt/WLengthValidator>
#include <Wt/WContainerWidget>
#include <Wt/WAnchor>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WTheme>

// Ancien
#include <Wt/Dbo/Query>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/algorithm/string.hpp>

#include <Wt/Json/Value>

#include <vector>
#include <map>

#include <Wt/Http/Message>

#include <Wt/WInPlaceEdit>

#include <tools/Enums.h>

#include "AbstractPage.h"


class AbstractPage;

class InformationsWidget :
public AbstractPage
{
public:
                                /*! \brief Constructor
                                *
                                * Creates a new authentication.
                                */
                                InformationsWidget(Echoes::Dbo::Session *session, std::string apiUrl);

protected:

    void                        checkAlertsInInformation(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id);

    void                        addResource(std::vector<Wt::WInteractWidget*>* arguments);
    void                        modifResource(std::vector<Wt::WInteractWidget*>* arguments, long long id);

    virtual void                handleJsonGet(vectors_Json jsonResources);
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
    virtual Wt::WDialog         *deleteResource(long long id);

//    Wt::WValidator              *editValidator(int who);

private:

    Wt::WStandardItemModel      *m_unitModel;
//    std::vector<std::string>    infoUnit_;
    Echoes::Dbo::Session        *session_;
    std::vector<long long>      idsAlert_;
    
    Wt::WComboBox               *m_unitComboBox;
};


#endif	/* INFORMATIONSWIDGET_H */
