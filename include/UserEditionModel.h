/* 
 * File:   UserEditionModel.h
 * Author: tsa
 *
 * Created on 3 septembre 2012, 15:01
 */

#ifndef USEREDITIONMODEL_H
#define	USEREDITIONMODEL_H

#include "GlobalIncludeFile.h"

#include <Wt/WMessageBox>

class UserEditionModel : public Wt::WFormModel {
public:
    UserEditionModel(Echoes::Dbo::User *user);
    virtual ~UserEditionModel();
    
    /*! \brief Enumeration for view
             */
    enum view 
    {
        Modification, 
        Creation
    };
    
    Echoes::Dbo::User *user;
    
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
    void setSession(Echoes::Dbo::Session *session);
private:
    view currentView;
    Echoes::Dbo::Session *session;
    virtual void reset();
    void setView(Echoes::Dbo::User *user = NULL);
    virtual bool isVisible(Field field) const;
    
    Wt::WString validateString(Wt::WString stringToValidate) const;
    void setValid(Field field);
};

#endif	/* USEREDITIONMODEL_H */

