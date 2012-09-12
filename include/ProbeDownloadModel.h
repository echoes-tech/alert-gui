/* 
 * File:   ProbeDownloadModel.h
 * Author: tsa
 *
 * Created on 3 septembre 2012, 15:01
 */

#ifndef PROBEDOWNLOADMODEL_H
#define	PROBEDOWNLOADMODEL_H

#include "tools/Session.h"
#include "WFormModel"

class ProbeDownloadModel : public Wt::WFormModel {
public:
    ProbeDownloadModel(User *user,Session *session);
    virtual ~ProbeDownloadModel();
    
    static const Field Os;
    static const Field Arch;
    static const Field LabelProbe;
    
    bool displaySendMail;
    
    /*! \brief Enumeration for view
             */
    enum view 
    {
        Modification, 
        Creation
    };
    
    User *user;
    
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

#endif	/* PROBEDOWNLOADMODEL_H */

