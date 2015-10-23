/* 
 * File:   MessagesWidget.h
 * Author: cde
 *
 * Created on 18 mars 2013, 11:57
 */

#ifndef MESSAGESWIDGET_H
#define	MESSAGESWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WAbstractItemModel>

#include "AbstractPage.h"

#include "MessagesTableAlertWidget.h"
#include "MessagesTableMessageWidget.h"
#include "MessagesTableHistoricWidget.h"


class MessagesWidget : 
public Wt::WContainerWidget 
{
    public:
        MessagesWidget(Echoes::Dbo::Session *session, const std::string apiUrl);
        void update();

    private:
        Echoes::Dbo::Session *session_;   
        std::string apiUrl_;
        bool newClass_;
};
#endif	/* MESSAGESWIDGET_H */

