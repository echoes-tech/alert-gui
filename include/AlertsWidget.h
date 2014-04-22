/* 
 * Gui AlertsWidget.h
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 20/11/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef ALERTSWIDGET_H
#define	ALERTSWIDGET_H

//#include <Wt/WSelectionBox>

#include <Wt/WAnimation>
#include <Wt/WPanel>
#include <Wt/WText>

#include <Wt/Json/Value>
#include <Wt/Http/Message>
#include <Wt/WRandom>
#include <Wt/WStandardItemModel>

#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

#include "tools/Session.h"

#include "GlobalIncludeFile.h"
#include "ApiManagement.h"

class AlertsWidget :
public AbstractPage
{
public:
    AlertsWidget(Echoes::Dbo::Session *session, std::string apiUrl);

    void                        popupAddWidget(Wt::WDialog *dialog, long long id);
    void                        popupRecipients(std::string nameAlert, std::string message);
    
    void                        updatePage(bool getResources);
    std::vector<std::string>    getTitlesTableWidget();
    std::vector<std::string>    getTitlesTableText();
    Wt::WValidator              *editValidator(int who);
    
    void                        closePopup();
    void                        close();
    
    void                        getAliasListFromRecipient(long long userRoleId);
    void                        getAliasInfo(boost::system::error_code err, const Wt::Http::Message& response, long long userRoleId, long long mediaType);
    void                        getAliasAsset(boost::system::error_code err, const Wt::Http::Message& response, long long userRoleId, long long mediaType);
    void                        getAliasCriteria(boost::system::error_code err, const Wt::Http::Message& response, long long userRoleId, long long mediaType);
    void                        getAliasPlugin(boost::system::error_code err, const Wt::Http::Message& response, long long userRoleId, long long mediaType);

    int                         checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);
    void                        checkPopupRecipients(std::string message, std::string time, int media);

    void                        addResource(std::vector<Wt::WInteractWidget*> argument);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);
    
    void                        postAlert(boost::system::error_code err, const Wt::Http::Message& response);
    void                        deleteAlert(boost::system::error_code err, const Wt::Http::Message& response);
    
    void                        deleteAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        postAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        putAlerts(boost::system::error_code err, const Wt::Http::Message& response);

    void                        modifRecip(long long id);

    void                        postAlertCallApi(std::string message);
    
protected:
    virtual void                clearStructures();
    virtual void                handleJsonGet(vectors_Json jsonResources);
    long long                   getSelectedIdFromBox(Wt::WSelectionBox * box);
    virtual void                setDisplayedTitlesPopups();

private:

    void                        initAlertValueDefinitionPopup(Wt::WTable *tableBox);
    void                        setBox(Wt::WSelectionBox *box, Wt::WStandardItemModel *model);
    void                        assetSelected();
    void                        pluginSelected();
    void                        informationSelected();
    void                        fillModels();
    void                        fillModel(Wt::WStandardItemModel * model, std::map<long long, std::string> m_mapNames);
    void                        sortModels();

    std::vector<long long>      boxActived(Wt::WSelectionBox *box, std::multimap<long long, std::pair<long long, std::string>>  infoInBox, std::multimap<long long, long long> compId, long long id);


    void                        fillInTabMessage();
    
    void                        cleanBox(Wt::WSelectionBox *box);

    void                        createCompareWidgetBoolean();
    void                        createCompareWidgetCustom();
    void                        showCompareWidget(long long id);
    void                        addCompareLine(Enums::EInformationUnitType type);
    Wt::WComboBox               *createCompareComboBox(Enums::EInformationUnitType type);

    void                        selectAsset(long long id, Wt::WSelectionBox *boxAsset, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxInfo);
    void                        selectPlugin(long long id, Wt::WSelectionBox *boxAsset, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxInfo);
    void                        selectInfo(long long id, Wt::WSelectionBox *boxInfo, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxAsset);

    void                        errorsHideOne(std::map<long long, std::pair<std::pair<Wt::WLineEdit*, Wt::WText*>, Wt::WComboBox*>> error);
    void                        errorsHideTwo(std::map<long long, std::pair<std::pair<Wt::WLineEdit*, Wt::WText*>, std::pair<Wt::WComboBox*,Wt::WComboBox*>>> error);

    
    // validator type
    enum EValidatorType
    {
        VALIDATOR_INT = 0,
        VALIDATOR_FLOAT = 1
    };
    
    // alert setting attributes
    enum EBoxType {
        ASSET = 1,
        PLUGIN = 2,
        INFORMATION = 3
    };
    
    //ids
    std::map<long long, std::vector<long long>> m_mapAssetPlugins;
    std::map<long long, std::vector<long long>> m_mapAssetInfos;
    std::map<long long, std::vector<long long>> m_mapPluginAssets;
    std::map<long long, std::vector<long long>> m_mapPluginInfos;
    std::map<long long, std::vector<long long>> m_mapInfoAssets;
    std::map<long long, std::vector<long long>> m_mapInfoPlugins;
    
    //names
    std::map<long long, std::string> m_mapAssetsNames;
    std::map<long long, std::string> m_mapPluginsNames;
    std::map<long long, std::string> m_mapInformationsNames;
    
    //info unit
    std::map<long long, long long> m_mapInformationsUnits;
    std::map<long long, long long> m_mapInformationsUnitTypes;
    
    // first popup boxes
    Wt::WSelectionBox *m_boxAsset;
    Wt::WSelectionBox *m_boxPlugin;
    Wt::WSelectionBox *m_boxInfo;
    
    Wt::WStandardItemModel * m_assets;
    Wt::WStandardItemModel * m_plugins;
    Wt::WStandardItemModel * m_informations;
    
    // alerts criterion
    std::map<int,Wt::WTable *> m_alertCriteria;
    
    // end alert setting attributes
    
    Echoes::Dbo::Session        *m_session;
    std::string                 m_apiUrl;
    Wt::Json::Value             m_alerts;
    
    int                         time_;
    
    std::multimap<long long, std::pair<long long, std::string>>                         userInfo_;
    std::multimap<long long, std::pair<std::pair<long long, long long>, std::string>>   mediaInfo_;
    Wt::WTabWidget      *m_tabWidgetMessages;
    std::string         m_tabContentMessageMail;
    std::string         m_tabContentMessageSMS;
    std::string         m_tabContentMessageMobileApp;

    
    std::multimap<long long, long long>       unitsIds_; // Link between Info and widgets compare. (text, number, bool)

    // FIXME : to be refactored and included in m_alertCriteria
    Wt::WTable                          *m_booleanCompareWidget; // Bool
    Wt::WTextArea                       *m_customCompareWidget; // Bool
    
    bool                m_contentOfBooleanCheck;  //for check button true/false
    int                 checkAll_;
    /**
     * pair &lsaquo; pair &lsaquo; idAsset, idPlugin &rsaquo;
     * , pair &lsaquo; idInfo, idKey &rsaquo; &rsaquo;
     */
    std::pair<std::pair<long long, long long>, std::pair<long long, long long>>         idAll_;
    
    std::map<long long, std::pair<std::pair<Wt::WLineEdit*, Wt::WText*>, Wt::WComboBox*>>                                       resourcesUnitOne;
    std::map<long long, std::pair<std::pair<Wt::WLineEdit*, Wt::WText*>, std::pair<Wt::WComboBox*,Wt::WComboBox*>>>             resourcesUnitTwo;
    
    //  Errors Messages ------
    Wt::WText           *m_textErrorForAsset;
    Wt::WText           *m_textErrorForPlugin; 
    Wt::WText           *m_textErrorForInformation;
    Wt::WText           *m_textErrorForKeyValue;

    Wt::WLineEdit       *keyValue_;
    
    Wt::WText            *errorBool_;
    //  -------
    
    Wt::WContainerWidget *m_compareWidgetContainer;
    Wt::WContainerWidget *m_compareWidgetContainerBottom;
    Wt::WContainerWidget *m_compareWidgetContainerSequence;
    
    Wt::WPushButton      *m_buttonAddCompareCriteria;
    Wt::WPushButton      *m_buttonAddNumber;
    
    Wt::WLineEdit        *saveLineEditOne_;
    Wt::WLineEdit        *saveLineEditTwo_;
};

#endif	/* ALERTSWIDGET_H */
