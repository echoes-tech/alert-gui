/* 
 * File:   UserEditionModel.h
 * Author: tsa
 *
 * Created on 3 septembre 2012, 15:01
 */

#ifndef USEREDITIONMODEL_H
#define	USEREDITIONMODEL_H

#include "tools/Session.h"
#include "WFormModel"

class UserEditionModel : public Wt::WFormModel {
public:
    UserEditionModel(User *user);
    virtual ~UserEditionModel();
    
    /*! \brief Enumeration for view
             */
    enum view 
    {
        Modification, 
        Creation
    };
    
    User *user;
    
    //! \brief Choose Password field
    static const Field FirstName;
    static const Field LastName;
    static const Field Email;
    static const Field ChoosePasswordField;
    static const Field RepeatPasswordField;
    static const Field Role;
    static const Field State;
    static const Field MediaEMail;
    static const Field MediaSMS;
    static const Field MediaMobileApp;
    
    virtual bool validateField(Field field);
private:
    view currentView;
    Session *session;
    virtual void reset();
    void setView(User *user = NULL);
    virtual bool isVisible(Field field) const;
    
    Wt::WString validateString(Wt::WString stringToValidate) const;
    void setValid(Field field);
};

#endif	/* USEREDITIONMODEL_H */

