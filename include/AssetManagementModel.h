///* 
// * Header of Asset Management Model
// * @author ECHOES Technologies (TSA)
// * @date 03/09/2012
// * 
// * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
// * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
// * COMPANY AUTHORIZATION.
// * 
// * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
// * 
// */
//
//#ifndef ASSETMANAGEMENTMODEL_H
//#define	ASSETMANAGEMENTMODEL_H
//
//#include "GlobalIncludeFile.h"
//
//#include <Wt/WMessageBox>
//
//class AssetManagementModel : 
//public Wt::WFormModel
//{
//public:
//
//    AssetManagementModel();
//    virtual ~AssetManagementModel();
//    
//    static const Field AssetName;
//    
//    void setSession(Echoes::Dbo::Session *session);
//
//    virtual bool isVisible(Field field) const;
//    virtual bool validateField(Field field);
//
//private:
//
//    Echoes::Dbo::Session *session_;
//
//    virtual void reset();
//    
//    Wt::WString validateString(Wt::WString stringToValidate) const;
//
//    void setValid(Field field);
//};
//
//#endif	/* ASSETMANAGEMENTMODEL_H */
//
