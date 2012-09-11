/* 
 * File:   AlertEditionModel.h
 * Author: tsa
 *
 * Created on 3 septembre 2012, 15:01
 */

#ifndef ALERTEDITIONMODEL_H
#define	ALERTEDITIONMODEL_H

#include "tools/Session.h"
#include "WFormModel"

class AlertEditionModel : public Wt::WFormModel {
public:
    AlertEditionModel(User *user);
    virtual ~AlertEditionModel();
    
    /*! \brief Enumeration for view
             */
    enum view 
    {
        Modification, 
        Creation
    };
    
    User *user;
    
    static const Field ThresholdOperator;
    static const Field ThresholdValue;
    static const Field Snooze;
    
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

#endif	/* ALERTEDITIONMODEL_H */

