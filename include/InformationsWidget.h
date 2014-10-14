/* 
 * Header of Informations Widget
 * @author ECHOES Technologies (MLA)
 * @date 12/12/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef INFORMATIONSWIDGET_H
#define	INFORMATIONSWIDGET_H

#include "GlobalIncludeFile.h"

#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>
#include <Wt/WBreak>

#include <Wt/WInteractWidget>
#include <Wt/WDialog>
#include <Wt/WTableView>
#include <Wt/WLengthValidator>
#include <Wt/WContainerWidget>
#include <Wt/WAnchor>
#include <Wt/WLineEdit>
#include <Wt/WPushButton>
#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WTheme>

// Ancien
#include <Wt/Dbo/Query>
#include <fstream>
#include <boost/lexical_cast.hpp>
#include <boost/random.hpp>
#include <boost/random/random_device.hpp>
#include <boost/random/uniform_int_distribution.hpp>
#include <boost/algorithm/string.hpp>

#include <Wt/Json/Value>

#include <vector>
#include <map>

#include <Wt/Http/Message>

#include <Wt/WInPlaceEdit>

#include <tools/Enums.h>

#include "AbstractPage.h"


class AbstractPage;

class InformationsWidget :
public AbstractPage
{
public:
                                /*! \brief Constructor
                                *
                                * Creates a new authentication.
                                */
                                InformationsWidget(Echoes::Dbo::Session *session, std::string apiUrl);

protected:

    void                        checkAlertsInInformation(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id);

    void                        addResource(std::vector<Wt::WInteractWidget*>* arguments);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);

    virtual void                handleJsonGet(vectors_Json jsonResources);
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
    virtual Wt::WDialog         *deleteResource(long long id);

//    Wt::WValidator              *editValidator(int who);

private:

    Wt::WStandardItemModel      *m_unitModel;
//    std::vector<std::string>    infoUnit_;
    Echoes::Dbo::Session        *session_;
    std::vector<long long>      idsAlert_;
    
    Wt::WComboBox               *m_unitComboBox;
};


#endif	/* INFORMATIONSWIDGET_H */

