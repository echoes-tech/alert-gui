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

#include "CreatePageWidget.h"
#include "ClassTest.h"

#define REG_EXP ("[^\\\\<>/.&;?!§,{}()*|\"]{1,255}")

class CreatePageWidget;

class InformationsWidget :
public CreatePageWidget
{
public:
    /*! \brief Constructor
     *
     * Creates a new authentication.
     */
    InformationsWidget(Echoes::Dbo::Session *session, std::string apiUrl);

    void popupAddWidget(Wt::WDialog *dialog);

    long long userId;
    void setSession(Echoes::Dbo::Session *session);
    void setApiUrl(std::string apiUrl);
    std::string getApiUrl();
    void getInformations(boost::system::error_code err, const Wt::Http::Message& response);
    void getInformation(boost::system::error_code err, const Wt::Http::Message& response);
    void recoverListInformations();
    void recoverInformation(int id);

    Wt::Json::Value result_;
    Wt::Json::Value result_row_;

    Wt::WTable *corpTable;
    std::string apiUrl_;
    virtual void update();

protected:
    /*! \brief Validates the current information.
     *
     * The default implementation simply calls
     * RegistrationModel::validate() on the model.
     *
     * You may want to reimplement this method if you've added other
     * information to the registration form that need validation.
     */
    // virtual bool validate();

    /*! \brief Validates the current information.                                                           
     * 
     * The default implementation simply calls
     * RegistrationModel::validate() on the model.
     * 
     * You may want to reimplement this method if you've added other 
     * information to the registration form that need validation.
     */
    //    virtual bool validate();
    /*! \brief Closes the registration widget. 
     * 
     * The default implementation simply deletes the widget.
     */
    virtual void close();

    //void    initPopup();
    //void        addResourceInPopup(Wt::WDialog *dialog_);

    virtual void modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);
    virtual void addResource(std::vector<Wt::WInteractWidget*> argument);
    virtual Wt::WDialog *deleteResource(long long id);
    Wt::WFileResource *generateScript(long long i, Wt::WString assetName);
    std::string getStringFromFile(std::string resourcePath);

    void closePopup();

    Wt::WValidator *editValidator(int who);

    void putInformation(boost::system::error_code err, const Wt::Http::Message& response);
    void postInformation(boost::system::error_code err, const Wt::Http::Message& response);
    void postPlugin(boost::system::error_code err, const Wt::Http::Message& response);
    void checkAlertsInInformation(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id);
    void deleteInformation(boost::system::error_code err, const Wt::Http::Message& response);

private:

    std::vector<long long> getIdsTable();
    std::vector<std::string> getTitlesTableText();
    std::vector<std::string> getTitlesTableWidget();
    vector_type getResourceRowTable(long long id);
    void downloadScript(std::string fileName);




    Wt::WLineEdit *saveEdit;
    bool created_;
    bool newClass_;
    Echoes::Dbo::Session *session_;
    Wt::WLineEdit *assetEdit;
    std::vector<long long> idsAlert_;
};


#endif	/* INFORMATIONSWIDGET_H */

