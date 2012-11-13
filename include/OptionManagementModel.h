/* 
 * File:   OptionManagementModel.h
 * Author: tsa
 *
 * Created on 11 novembre 2012, 13:15
 */

#ifndef OPTIONMANAGEMENTMODEL_H
#define	OPTIONMANAGEMENTMODEL_H

#include "GlobalIncludeFile.h"

class OptionManagementModel : public Wt::WFormModel 
{
public:
    OptionManagementModel();
    virtual ~OptionManagementModel();
    
    static const Field smsQuota;
    static const Field smsAsk;
    virtual bool validateField(Field field);
    virtual void reset();
    void modifyField(Field field, const Wt::WString& info);
private:
    Session *session;
    
    void setView(User *user = NULL);
    virtual bool isVisible(Field field) const;
    
//    Wt::WString validateAssetName(Wt::WString stringToValidate) const;
    Wt::WString validateString(Wt::WString stringToValidate) const;
    
    Wt::WString label(Field field) const;
    void setValid(Field field);
};

#endif	/* OPTIONMANAGEMENTMODEL_H */

