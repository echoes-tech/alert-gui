/* 
 * File:   SummaryBoard.h
 * Author: tsa
 *
 * Created on 12 mai 2013, 16:22
 */

#ifndef SUMMARYBOARD_H
#define	SUMMARYBOARD_H

#include "GlobalIncludeFile.h"

#include "Wt/WTemplate"

class SummaryBoard  : public Wt::WContainerWidget {
public:
    SummaryBoard(Session *session);
    SummaryBoard(const SummaryBoard& orig);
    virtual ~SummaryBoard();
private:
    Session *session;
    
    Wt::WTemplate *mainTemplate;
    
    Wt::WText *welcomeText;
    Wt::WContainerWidget *alertSentWidget;
    bool created_;

};

#endif	/* SUMMARYBOARD_H */

