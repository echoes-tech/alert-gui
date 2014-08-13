/* 
 * Gui SpecializedAuthModel.h
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

#ifndef SPECIALIZEDAUTHMODEL_H
#define	SPECIALIZEDAUTHMODEL_H

#include <Wt/Auth/AuthModel>

class SpecializedAuthModel : public Wt::Auth::AuthModel
{
    public:

        SpecializedAuthModel (const Wt::Auth::AuthService &baseAuth, Wt::Auth::AbstractUserDatabase &users, Wt::WObject* parent=0)
        : Wt::Auth::AuthModel(baseAuth, users, parent) {}

        virtual void reset();
};

#endif	/* SPECIALIZEDAUTHMODEL_H */

