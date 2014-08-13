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
