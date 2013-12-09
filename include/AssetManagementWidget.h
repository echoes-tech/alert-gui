/* 
 * Header of Asset Management Widget
 * @author ECHOES Technologies (TSA)
 * @date 14/08/2012
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ASSETMANAGEMENTWIDGET_H
#define	ASSETMANAGEMENTWIDGET_H

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

class AssetManagementWidget : 
public CreatePageWidget 
{
public:
  /*! \brief Constructor
   *
   * Creates a new authentication.
   */
  AssetManagementWidget(Echoes::Dbo::Session *session, std::string apiUrl);

  /*! \brief Returns the registration model.
   *
   * This returns the model that is used by the widget to do the actual
   * registration.
   */
//  AssetManagementModel *model() const { return model_; }
  
  void  popupAddWidget(Wt::WDialog *dialog);

  long long userId;
  void                  setSession(Echoes::Dbo::Session *session);
  void                  setApiUrl(std::string apiUrl);
  std::string           getApiUrl();
  void                  getAssets(boost::system::error_code err, const Wt::Http::Message& response);
void    recoverListAsset();

//std::vector<std::string> recoverTitles();

Wt::Json::Value result_;

  Wt::WTable *corpTable;
  std::string apiUrl_;
  CreatePageWidget *AssetPage;
  virtual void                  update();

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
  
  void  closePopup();
  
  Wt::WValidator    *editValidator(int who);

void    putAsset(boost::system::error_code err, const Wt::Http::Message& response);
void    postAsset(boost::system::error_code err, const Wt::Http::Message& response);
void    postPlugin(boost::system::error_code err, const Wt::Http::Message& response);
void    checkAlertsInAsset(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id);
void    deleteAsset(boost::system::error_code err, const Wt::Http::Message& response);

private:
  
    std::vector<long long>          getIdsTable();
    std::vector<std::string>        getTitlesTableText();
    std::vector<std::string>        getTitlesTableWidget();
    vector_type                 getResourceRowTable(long long id);
  void                  downloadScript(std::string fileName);

    
    
  
  Wt::WLineEdit         *saveEdit;
  bool                  created_;
  bool                  newClass_;
//  AssetManagementModel  *model_;
  Echoes::Dbo::Session               *session_;
  Wt::WLineEdit         *assetEdit;
  std::vector<long long> idsAlert_;
};


#endif	/* ASSETMANAGEMENTWIDGET_H */

