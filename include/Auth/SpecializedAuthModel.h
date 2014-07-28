/* 
 * File:   SpecializedAuthModel.h
 * Author: tsa
 *
 * Created on 28 juillet 2014, 20:44
 */

#ifndef SPECIALIZEDAUTHMODEL_H
#define	SPECIALIZEDAUTHMODEL_H

class SpecializedAuthModel : public Wt::Auth::AuthModel
{
    public:

        SpecializedAuthModel (const Wt::Auth::AuthService &baseAuth, Wt::Auth::AbstractUserDatabase &users, Wt::WObject* parent=0)
        : Wt::Auth::AuthModel(baseAuth, users, parent)
        {
        }

        virtual void reset()
        {
            Wt::Auth::AuthModel::reset();
            this->setValue(Wt::Auth::AuthModel::RememberMeField, true);
        }
};

#endif	/* SPECIALIZEDAUTHMODEL_H */

