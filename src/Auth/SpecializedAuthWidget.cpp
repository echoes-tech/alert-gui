/* 
 * Gui SpecializedAuthWidget.cpp
 * 
 * @author Echoes Technologies
 * @author Thomas SAQUET (TSA)
 * 
 * @date 13/08/2014
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * AbstractPage
 * COPYRIGHT 2012-2014 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "Auth/SpecializedAuthWidget.h"
#include "Auth/SpecializedRegistrationWidget.h"

Wt::WWidget * SpecializedAuthWidget::createRegistrationView(const Wt::Auth::Identity& id)
{
  registrationModel_ = createRegistrationModel();

  if (id.isValid())
    registrationModel_->registerIdentified(id);

  SpecializedRegistrationWidget *w = new SpecializedRegistrationWidget(this);
  w->setModel(registrationModel_);

  return w;
}
