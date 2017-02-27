/**
 * @file AssetManagementWidget.h
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
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
#include "AbstractPage.h"

class AbstractPage;

class AssetManagementWidget : 
public AbstractPage 
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
  virtual void                apiDeleteResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
  
//  virtual Wt::WValidator                *editValidator(int cpt);

//  virtual void                           updatePage(bool getResources = true);
  virtual int                            addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable);

  virtual void                           postResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
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
  void                  postProbe(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
  void                  checkAlertsInAsset(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client, Wt::WDialog *box, long long id);
  
  void                  setSession(Echoes::Dbo::Session *session);
  void                  setApiUrl(std::string apiUrl);
  
  bool                   created_;
  bool                   newClass_;

  std::vector<long long> idsAlert_;
  Wt::Json::Value        result_;

};


#endif	/* ASSETMANAGEMENTWIDGET_H */
