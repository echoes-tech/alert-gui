/* 
 * File:   SummaryBoard.cpp
 * Author: tsa
 * 
 * Created on 12 mai 2013, 16:22
 */

#include "SummaryBoard.h"

SummaryBoard::SummaryBoard(Echoes::Dbo::Session *session)
: Wt::WContainerWidget()
{
    created_ = false;
    this->session = session;
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use(AbstractPage::xmlDirectory + "summary",false);
    mainTemplate = new Wt::WTemplate(Wt::WString::tr("Alert.summary.board.template"));
    mainTemplate->addFunction("tr", &Wt::WTemplate::Functions::tr);
    
    this->addWidget(mainTemplate);
    
    //this->alertSentWidget = new MonitoringWidget(session);
    this->welcomeText = new Wt::WText(tr("welcome-text"));
    
    //mainTemplate->bindWidget("alerts-sent",this->alertSentWidget);
    mainTemplate->bindWidget("welcome", this->welcomeText);
    
}

SummaryBoard::SummaryBoard(const SummaryBoard& orig) {
}

SummaryBoard::~SummaryBoard() {
}

