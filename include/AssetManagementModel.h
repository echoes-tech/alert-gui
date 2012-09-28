/* 
 * File:   AssetManagementModel.h
 * Author: tsa
 *
 * Created on 3 septembre 2012, 15:01
 */

#ifndef ASSETMANAGEMENTMODEL_H
#define	ASSETMANAGEMENTMODEL_H

#include "tools/Session.h"
#include "WFormModel"

class AssetManagementModel : public Wt::WFormModel {
public:
    AssetManagementModel(User *user);
    virtual ~AssetManagementModel();
    
    User *user;
    
    static const Field AssetName;
    
    virtual bool validateField(Field field);
    virtual void reset();
private:
    Session *session;
    
    void setView(User *user = NULL);
    virtual bool isVisible(Field field) const;
    
    Wt::WString validateAssetName(Wt::WString stringToValidate) const;
    Wt::WString validateString(Wt::WString stringToValidate) const;
    
    Wt::WString label(Field field) const;
    void setValid(Field field);
};

#endif	/* ASSETMANAGEMENTMODEL_H */

