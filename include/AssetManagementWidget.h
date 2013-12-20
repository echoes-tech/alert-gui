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

#include "Conf.h"
#include "CreatePageWidget.h"

#define REG_EXP ("[^\\\\<>/.&;?!§,{}()*|\"]{1,255}")

class CreatePageWidget;

class AssetManagementWidget : 
public CreatePageWidget 
{
public:
  /*! \brief Constructor
   *
   * Creates a new page Asset.
   */
  AssetManagementWidget(Echoes::Dbo::Session *session, std::string apiUrl);

  /**
   * In this methode you can call API for recover resources. \n
   * She is call after initialization of this class.
   */
  void          recoverListAsset();

protected:
  /*! \brief Closes the registration widget. 
   * 
   * The default implementation simply deletes the widget.
   */
  virtual void                          close();

  virtual void                          closePopup();

  virtual void                          update();
  virtual void                          modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);
  virtual void                          addResource(std::vector<Wt::WInteractWidget*> argument);
  virtual Wt::WDialog                   *deleteResource(long long id);
  
  virtual Wt::WValidator                *editValidator(int who);
  virtual void                          popupAddWidget(Wt::WDialog *dialog, bool typeDial);

  virtual std::vector<long long>        getIdsTable();
  virtual std::vector<std::string>      getTitlesTableText();
  virtual std::vector<std::string>      getTitlesTableWidget();
  virtual vector_type                   getResourceRowTable(long long id);

private:
  /*
   * Generate and get script (sonde) for asset.
   */
  Wt::WFileResource     *generateScript(long long astId, Wt::WString assetName);
  std::string           getStringFromFile(std::string resourcePath);
  void                  downloadScript(std::string fileName);

  /*
   * return API after call.
   */
  void                  putAsset(boost::system::error_code err, const Wt::Http::Message& response);
  void                  postAsset(boost::system::error_code err, const Wt::Http::Message& response);
  void                  postProbe(boost::system::error_code err, const Wt::Http::Message& response);
  void                  postPlugin(boost::system::error_code err, const Wt::Http::Message& response);
  void                  checkAlertsInAsset(boost::system::error_code err, const Wt::Http::Message& response, Wt::WDialog *box, long long id);
  void                  deleteAsset(boost::system::error_code err, const Wt::Http::Message& response);
  void                  getAssets(boost::system::error_code err, const Wt::Http::Message& response);
  void                  setSession(Echoes::Dbo::Session *session);
  void                  setApiUrl(std::string apiUrl);
  
  std::string           getApiUrl();
  
  bool                   created_;
  bool                   newClass_;
  Echoes::Dbo::Session   *session_;
  std::vector<long long> idsAlert_;
  Wt::Json::Value        result_;
  std::string            apiUrl_;
};


#endif	/* ASSETMANAGEMENTWIDGET_H */

