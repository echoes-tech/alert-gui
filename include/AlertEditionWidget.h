/* 
 * File:   RegistrationWidget.h
 * Author: tsa
 *
 * Created on 14 août 2012, 11:50
 */

#ifndef ALERTEDITIONWIDGET_H
#define	ALERTEDITIONWIDGET_H

#include "GlobalIncludeFile.h"

#include <Wt/WButtonGroup>
#include <Wt/WRadioButton>
#include <Wt/WGroupBox>
#include <Wt/WBreak>


#include "Wt/WAnchor"
#include "Wt/WApplication"
#include "Wt/WContainerWidget"
#include "Wt/WDialog"
#include "Wt/WImage"
#include "Wt/WLineEdit"
#include "Wt/WPushButton"
#include "Wt/WText"

#include <Wt/WComboBox>
#include <Wt/WComboBox>
#include <Wt/WStringListModel>
#include <Wt/Dbo/Query>
#include <Wt/WSpinBox>
#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <Wt/WMessageBox>
#include <Wt/WSuggestionPopup>

#include <boost/any.hpp>

#include <memory>
#include <Wt/WTableView>

class AlertEditionWidget : public Wt::WTemplateFormView
{
public:
  /*! \brief Constructor
   *
   * Creates a new authentication.
   */
  AlertEditionWidget();

  /*! \brief Sets the registration model.
   */
  void setModel(AlertEditionModel *model);
  
  /*! \brief Sets the session.
   */
  void setSession(Session *session);

  /*! \brief Returns the registration model.
   *
   * This returns the model that is used by the widget to do the actual
   * registration.
   */
  AlertEditionModel *model() const { return model_; }
  
  void checkThresholdOperator();
  void checkThresholdValueKey();
  void checkThresholdValue();
  void checkSnoozeValue();
  void rememberUnitValue();
  
  /*! \brief Updates the user-interface.
   *
   * This updates the user-interface to reflect the current state of the
   * model.
   */
  void update();
  
  long long userId;
  
  Wt::WLineEdit *valueLineEdit;
  
  Wt::WSuggestionPopup *sp;
  
  // maps needed to keep the link to the base
  std::map <int,long long> mapAssetIdSboxRow;
  Wt::WComboBox *serverSelectionBox;
  std::map<int,long long> mapPluginIdSboxRow;
  Wt::WComboBox *applicationSelectionBox;
  // five maps to keep each primary key element without keeping a dbo ptr open out of a transaction + pos_key_value
  std::map<int,long long> mapInformationSeaIdSboxRow;
  std::map<int,long long> mapInformationSrcIdSboxRow;
  std::map<int,long long> mapInformationPlgIdSboxRow;
  std::map<int, int> mapInformationIvnSboxRow;
  std::map<int,long long> mapInformationInuIdSboxRow;
  std::map<int,int> mapInformationPkvSboxRow;
//  std::map<int,long long> mapInformatioInuIdSboxRow;
  Wt::WComboBox *informationSelectionBox;
  
  std::map<int,long long> mapInformationUnitCombo;
  
  // list of media where the alerts are sent
//  Wt::WTableView *userMediaDestinationTableView;
  Wt::WTable *tableMediaDestination;
  std::map<int,long long> mapAlertMediaSpecializationIdTableView;
  
  Wt::WComboBox *comboAlertCrit;
  
  Wt::WComboBox *comboInformationUnit;
  
  std::map<int,long long> mapUserIdSboxRow;
  Wt::WComboBox *userSelectionBox;
  std::map<int,long long> mapMediaIdSboxRow;
  Wt::WComboBox *mediaSelectionBox;
  std::map<int,long long> mapMediaValueIdSboxRow;
  Wt::WComboBox *mediaValueSelectionBox;
  
  Wt::WLineEdit *snoozeEdit;
  
  std::map<int,long long> mapAlertCriteriaIdSboxRow;
  
  void updateMediaSBFromUserSB();
  void updateMediaValueSBFromMediaSB();
  
  void updateApplicationSBFromServerSB();
  void updateInformationSBFromApplicationSB();
  void updateInformationDetailsFromInformationSB();
  
  void updateMediaValueSelectionBox(int userId, int medId);
  void updateMediaSelectionBox(int userId);
  void updateUserSelectionBox(int userId);
  
  void updateServerSelectionBox(int serverId);
  void updateInformationSelectionBox(int pluginId);
  void updateApplicationSelectionBox(int astId);
  void updateInformationDetails(int index);
  
  Wt::WStringListModel *getMediasForCurrentUser(int mediaType);
  
  
  void addMedia();
  void deleteMedia(Wt::WTableCell *cell);
  
  void addAlert();

  bool isCreated();

protected:
  /*! \brief Validates the current information.
   *
   * The default implementation simply calls
   * RegistrationModel::validate() on the model.
   *
   * You may want to reimplement this method if you've added other
   * information to the registration form that need validation.
   */
  virtual bool validate();


  /*! \brief Closes the registration widget.
   *
   * The default implementation simply deletes the widget.
   */
  virtual void close();


  virtual void render(Wt::WFlags<Wt::RenderFlag> flags);

  virtual Wt::WFormWidget *createFormWidget(Wt::WFormModel::Field field);

private:
  AlertEditionModel *model_;
  Session * session;

  bool created_;
  
  Wt::WGroupBox *organizationContainer;
  Wt::WButtonGroup *organizationGroup;

};



#endif	/* ALERTEDITIONWIDGET_H */

