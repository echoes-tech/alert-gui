/* 
 * File:   ClassTest.h
 * Author: gkr
 *
 * Created on 23 octobre 2013, 14:24
 */

#ifndef CLASSTEST_H
#define	CLASSTEST_H


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

#include "CreatePageWidget.h"

class CreatePageWidget;

class ClassTest : 
public CreatePageWidget 
{
public:
    ClassTest(Session *session, std::string apiUrl);

  //void                          popupAddTables(Wt::WTabWidget *tabW);

  void                          setSession(Session *session);
  void                          setApiUrl(std::string apiUrl);
  
  std::string                   getApiUrl();
  void                          getAsset(boost::system::error_code err, const Wt::Http::Message& response);
  void                          recoverListAsset();

  //std::vector<std::string>      recoverTitles();

Wt::Json::Value                 result_;

  Wt::WTable                    *corpTable;
  std::string                   apiUrl_;
  virtual void                  update();

protected:
//    virtual bool                validate();

    virtual void                close();


    //void                        initPopup();
    //void                        addResourceInPopup(Wt::WDialog *dialog_);



  void modifResource(std::vector<Wt::WInteractWidget*> argument, long long id);
  void addResource(std::vector<Wt::WInteractWidget*> argument);
  void deleteResource(long long id);
  Wt::WFileResource *generateScript(long long i, Wt::WString assetName);
  std::string getStringFromFile(std::string resourcePath);
  
  
  Wt::WValidator    *editValidator(int who);

void    putAsset(boost::system::error_code err, const Wt::Http::Message& response);
void    postAsset(boost::system::error_code err, const Wt::Http::Message& response);

private:
  
    std::vector<long long>          getIdsTable();
    std::vector<std::string>        getHeaderTableTitle();
    vector_type     getResourceRowTable(long long id);

    
    
  
  Wt::WLineEdit *saveEdit;

  Wt::WTable            *saveTable; 
  
  Wt::WLineEdit         *assetName; 
  
  bool                  created_;
  bool                  newClass_;
  Session               *session_;

  void                  downloadScript(std::string fileName);

  Wt::WLineEdit         *assetEdit;
  
};

#endif	/* CLASSTEST_H */

