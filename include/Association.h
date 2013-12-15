/* 
 * File:   Association.h
 * Author: gkr
 *
 * Created on 13 décembre 2013, 12:04
 */

#ifndef ASSOCIATION_H
#define	ASSOCIATION_H

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

#include "CreatePageWidget.h"

typedef std::map<long long, std::pair<long long, std::string>> MapLongString2;

class CreatePageWidget;

class Association : 
public CreatePageWidget
{
public:
  /*! \brief Constructor
   *
   * Creates a new page Association.
   */
  Association(Echoes::Dbo::Session *session, std::string apiUrl);

  /**
   * In this methode you can call API for recover resources. \n
   * She is call after initialization of this class.
   */
  void          recoverListAssociation();

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
  
  virtual int checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);

private:
  /*
   * return API after call.
   */
  void                  getAssets(boost::system::error_code err, const Wt::Http::Message& response);
  void                  getPlugins(boost::system::error_code err, const Wt::Http::Message& response);
  void                  getFilters(boost::system::error_code err, const Wt::Http::Message& response);
  void                  getFilterParameterValues(boost::system::error_code err, const Wt::Http::Message& response, long long filterId, Wt::Json::Object filJson);
  void                  getInformations(boost::system::error_code err, const Wt::Http::Message& response);
  
  void                  postAsset(boost::system::error_code err, const Wt::Http::Message& response);
  
  void                  setSession(Echoes::Dbo::Session *session);
  void                  setApiUrl(std::string apiUrl);
  std::string           getApiUrl();
  
    struct filterValuesStruct
    {
        long long filterId;
        int filterType;
        int nbValue;
        long long searchId;
        Wt::WString filterValue;
    };
  
  MapLongString2                 assets_;
  Wt::WStandardItemModel         *assetsModel;
  MapLongString2                 plugins_;
  MapLongString2                 informations_;
  Wt::WStandardItemModel         *informationsModel;
  std::map<long long, filterValuesStruct> filterParameterValues_;
  std::map<int, Wt::WCheckBox*>       filterCheckBox_;
  std::map<long long, Wt::WComboBox*> filterInfosComboBox_;
  std::map<long long, Wt::WComboBox*> filterAssetsComboBox_;
  long long                     idHost_;
  long long                     idPlugin_;
  std::vector<long long>        idsInformations_;
  bool                          created_;
  bool                          newClass_;
  Echoes::Dbo::Session          *session_;
  std::vector<long long>        idsAlert_;
  std::string                   apiUrl_;
  
  Wt::WTable *tableFilters;
};

#endif	/* ASSOCIATION_H */

