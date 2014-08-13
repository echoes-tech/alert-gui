/* 
 * Gui SpecializedAuthModel.cpp
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
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "Auth/SpecializedAuthModel.h"

void SpecializedAuthModel::reset()
{
    Wt::Auth::AuthModel::reset();
    this->setValue(Wt::Auth::AuthModel::RememberMeField, true);
}

