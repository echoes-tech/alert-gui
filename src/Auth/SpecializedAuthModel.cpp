#include "Auth/SpecializedAuthModel.h"

void SpecializedAuthModel::reset()
{
    Wt::Auth::AuthModel::reset();
    this->setValue(Wt::Auth::AuthModel::RememberMeField, true);
}
