/* 
 * File:   AssetManagementWidget.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:50
 */

#ifndef ASSETMANAGEMENTWIDGET_H
#define	ASSETMANAGEMENTWIDGET_H

#include "GlobalIncludeFile.h"

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

// Ancier
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
  AssetManagementWidget(Session *session, std::string apiUrl);

  /*! \brief Returns the registration model.
   *
   * This returns the model that is used by the widget to do the actual
   * registration.
   */
  AssetManagementModel *model() const { return model_; }
  
  void  popupAddTables(Wt::WTabWidget *tabW);

  
  long long userId;
  void                  setSession(Session *session);
  void                  setApiUrl(std::string apiUrl);
  std::string           getApiUrl();
  void                  getAsset(boost::system::error_code err, const Wt::Http::Message& response);
void    recoverListAsset();

std::vector<std::string> recoverTitles();

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



void    initPopup();
    void        addResourceInPopup(Wt::WDialog *dialog_);



  void modifResource(std::vector<Wt::WInteractWidget*> argument, long long id);
  void addResource(std::vector<Wt::WInteractWidget*> argument);
  void deleteResource(long long id);
  Wt::WFileResource *generateScript(long long i, Wt::WString assetName);
  std::string getStringFromFile(std::string resourcePath);
  
  
  Wt::WContainerWidget  *infoHeader;
  Wt::WContainerWidget  *infoTable;

  Wt::WValidator    *editValidator(int who);

void    putAsset(boost::system::error_code err, const Wt::Http::Message& response);
void    postAsset(boost::system::error_code err, const Wt::Http::Message& response);

private:
  
    std::vector<long long>          getIdsTable();
    std::vector<std::string>        getHeaderTableTitle();
    vector_type     getResourceRowTable(long long id);

    
    
  
  Wt::WLineEdit *saveEdit;

  Wt::WTable            *saveTable; //supprimer
  
  Wt::WLineEdit         *assetName; //link wit AssetName
  
  bool                  created_;
  bool                  newClass_;
  AssetManagementModel  *model_;
  Session               *session_;

  void                  downloadScript(std::string fileName);

  Wt::WLineEdit         *assetEdit;
};


#endif	/* ASSETMANAGEMENTWIDGET_H */

