/* 
 * File:   SummaryBoard.cpp
 * Author: tsa
 * 
 * Created on 12 mai 2013, 16:22
 */

#include "SummaryBoard.h"

SummaryBoard::SummaryBoard(Session *session)
: Wt::WContainerWidget()
{
    created_ = false;
    this->session = session;
    this->alertSentWidget = new MonitoringWidget(session);
    this->welcomeText = new Wt::WText(tr("welcome-text"));
    
    Wt::WHBoxLayout *hbLayout = new Wt::WHBoxLayout(this);
    
    
    this->setLayout(hbLayout);
    hbLayout->addWidget(this->alertSentWidget);
    hbLayout->addWidget(this->welcomeText);
    
    
    
}

SummaryBoard::SummaryBoard(const SummaryBoard& orig) {
}

SummaryBoard::~SummaryBoard() {
}

