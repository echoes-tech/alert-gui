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
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

#include "tools/Session.h"

#include "GlobalIncludeFile.h"
#include "ApiManagement.h"
#include "BoxInBoxMenu.h"

class AlertsWidget :
public AbstractPage
{
public:
    AlertsWidget(Echoes::Dbo::Session *session, std::string apiUrl);

    void                        popupAddWidget(Wt::WDialog *dialog, long long id);
    void                        popupRecipients(std::string nameAlert, std::string message);
    void                        popupNewRecipientsRework(std::string nameAlert, std::string message);
    
    std::vector<std::string>    getTitlesTableWidget();
    std::vector<std::string>    getTitlesTableText();
    
    void                        closePopup();
    void                        close();
    
    void                        getAliasListFromRecipient(long long userRoleId);

    int                         checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);
    /*void                        checkPopupRecipients(std::string message, std::string time, int media); */
    void                        checkNewPopupRecipientsRework(string initialMessage);

    void                        addResource(std::vector<Wt::WInteractWidget*>* argument);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);
    
    void                        postAlert(boost::system::error_code err, const Wt::Http::Message& response);
    void                        deleteAlert(boost::system::error_code err, const Wt::Http::Message& response);
    
    void                        deleteAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        postAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        putAlerts(boost::system::error_code err, const Wt::Http::Message& response);

    void                        modifRecip(long long id);

    void                        postAlertCallApi(std::string message);
    
    
    // Menu setting
    void addReceiver(long long id, long long index, BoxInBoxMenu *menu);
    void deleteReceiver(long long id, long long index, BoxInBoxMenu *menu);
    void selectReceiver(long long id, long long index, BoxInBoxMenu *menu);
    void addMedia(long long id, long long index, BoxInBoxMenu *menu);
    void deleteMedia(long long id, long long index, BoxInBoxMenu *menu);
    void selectMedia(long long id, long long index, BoxInBoxMenu *menu);
    
protected:
    virtual void                clearStructures();
    virtual void                handleJsonGet(vectors_Json jsonResources);
    long long                   getSelectedIdFromSelectionBox(Wt::WSelectionBox * box);
    long long                   getSelectedIdFromComboBox(Wt::WComboBox * box);
    virtual void                setDisplayedTitlesPopups();

private:

    void                        initAlertValueDefinitionPopup();
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

    void                        changeButtonAddCriteriaState();
    void                        handleAddCriteria();
    void                        addCompareWidget(long long assetID, long long pluginID, long long infoID);
    void                        createItemsCriteriaComboBox(long long id, Wt::WString criterion, Wt::WStandardItemModel *model);
    Wt::WComboBox               *createCompareCriteriaComboBox(long long type);

    void                        selectAsset(long long id, Wt::WSelectionBox *boxAsset, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxInfo);
    void                        selectPlugin(long long id, Wt::WSelectionBox *boxAsset, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxInfo);
    void                        selectInfo(long long id, Wt::WSelectionBox *boxInfo, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxAsset);

    
    // validator type
    enum EValidatorType
    {
        VALIDATOR_STRING = 0,
        VALIDATOR_FLOAT = 1,
        VALIDATOR_INT = 2,
        VALIDATOR_TIMER = 3
    };
    
    // alert setting attributes
    enum EBoxType {
        ASSET = 1,
        PLUGIN = 2,
        INFORMATION = 3
    };
    
    enum ERequestType {
        RTASSET = 1,
        RTPLUGIN = 2,
        RTINFORMATION = 3,
        RTCRITERIA = 4
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
    Wt::WPushButton   *m_buttonAddCriteria;
    
    Wt::WStandardItemModel * m_assets;
    Wt::WStandardItemModel * m_plugins;
    Wt::WStandardItemModel * m_informations;
    
    Wt::WStandardItemModel * m_booleanOperators;
    
    // alerts criterion associated message
    struct CriterionResponse {
        Wt::WString message;
        Wt::WString asset;
        Wt::WString plugin;
        Wt::WString criteria;
        Wt::WString information;
    };
    
    // alerts custom information
    struct AlertCustom {
        long long   timer;
        std::string message;
    };
    
    // alerts criterion
    struct AlertCriterion {
        long long unitTypeID;
        int index;
        long long assetID;
        long long pluginID;
        long long criteriaID;
        long long infoID;
        Wt::WComboBox* operatorComboBox;
        Wt::WLineEdit* lineEditValue;
        Wt::WComboBox* comboBoxCriteria;
        Wt::WValidator* validatorCriteria;
        Wt::WButtonGroup* groupTrueFalse;
        Wt::WTemplate* templateValid;
        CriterionResponse smsRsp;
        CriterionResponse emailRsp;
        CriterionResponse mobileappRsp;
    };
    
    std::vector<AlertCriterion> m_alertCriteria;
    // end alert setting attributes
    
    // Check validity
    Wt::WRegExpValidator *validateCriterionType(long long unitType);
    
    // clear messages
    void clearMessages();
    
    // get criteria
    void getCriteriaSelection();
    
    // get aliases
    void getAliases(long long userRoleId);
    
    // generic http ask get
    void httpAsk(long long userRoleId, long long mediaType, long long requestType, long long criteria);

    // Build message
    void updateTabContent(long long mediaType);
    void updateMessage(std::string &tabContent, Wt::WString &message, unsigned long criteria);
    void getRequestRsp(long long requestType, Wt::WString message, CriterionResponse &response);
    void getMediaRsp(Wt::WString message, long long mediaType, long long requestType, long long criteria);
    
    // Generic http response get
    void handleHttpResponse(Wt::Json::Value result, long long mediaType, long long requestType, long long criteria, long long userRoleId);

    enum ERrowType {
        HEADER = 1,
        ITEM = 2
    };
    
    enum EColor {
        HIGHLIGHT = 1,
        HOVER = 2
    };
    
    enum EInteractions {
        ADD = 0,
        MODIF = 1,
        REMOVE = 2,
        SELECT = 3,
    };
    
    struct Message
    {
        long long       receiverId;
        long long       mediaId;
        long long       timer;
        Wt::WString     *str;
    };
    
    struct Aliases
    {
        Wt::WString *email;
        Wt::WString *sms;
        Wt::WString *mobile;
    };
    
    Echoes::Dbo::Session        *m_session;
    std::string                 m_apiUrl;
    Wt::Json::Value             m_alerts;
    
    int             time_;
    long long       m_rowReceiver;
    long long       m_rowMedia;
    
    /* <media_id, media_name> */
    std::map<long long, std::string>       m_nameFromMedia;
    /* <receiver_id, receiver_name> */
    std::map<long long, std::string>       m_nameFromReceiver;
    /* <media_id, message> */
    std::map<long long, struct Message>    m_messages;
    /* <user_id, aliases> */
    std::map<long long, struct Aliases>    m_userAliases;
    /* <media_id, media type>*/
    std::map<long long, long long>         m_typesFromMedia;
    /* <media_id, user_id> */
    std::map<long long, long long>         m_userFromMedia;
    /* <user_id, media_id> */
    std::multimap<long long, long long>    m_mediasFromUser;
    
    
    Wt::WTextArea   *m_messageArea;
    Wt::WLineEdit   *m_timer;
    Wt::WTable      *m_messageTable;
    Wt::WText       *m_messageReceiver;
    Wt::WText       *m_messageMedia;
    
    std::multimap<long long, long long>                                                 m_mediaUserRelation;
    std::multimap<long long, std::pair<long long, std::string>>                         userInfo_;
    std::multimap<long long, std::pair<std::pair<long long, long long>, std::string>>   mediaInfo_;
    Wt::WComboBox       *m_boxMedias;
    Wt::WTemplate       *m_templateAddMedia;
    Wt::WTabWidget      *m_tabWidgetMessages;
    Wt::WTabWidget      *m_tabMessages;
    std::string         m_tabContentMessageMail;
    std::string         m_tabContentMessageSMS;
    std::string         m_tabContentMessageMobileApp;
    
    Wt::WTable *m_table;
    
    // FIXME : to be refactored and included in m_alertCriteria
    Wt::WTable                          *m_booleanCompareWidget; // Bool
    Wt::WTextArea                       *m_customCompareWidget; // Bool
    
    int                 checkAll_;
    /**
     * pair &lsaquo; pair &lsaquo; idAsset, idPlugin &rsaquo;
     * , pair &lsaquo; idInfo, idKey &rsaquo; &rsaquo;
     */
    std::pair<std::pair<long long, long long>, std::pair<long long, long long>>         idAll_;
  
    //  Errors Messages ------
    Wt::WText           *m_textErrorForAsset;
    Wt::WText           *m_textErrorForPlugin; 
    Wt::WText           *m_textErrorForInformation;
    Wt::WText           *m_textErrorForKeyValue;

    Wt::WLineEdit       *keyValue_;
    
    Wt::WText            *errorBool_;
    //  -------
    
    Wt::WTable              *m_compareTable;
    Wt::WContainerWidget *m_compareWidgetContainerTop;
    Wt::WContainerWidget *m_compareWidgetContainerSequence;
    
    Wt::WPushButton      *m_buttonAddNumber;
    
    Wt::WLineEdit        *saveLineEditOne_;
    Wt::WLineEdit        *saveLineEditTwo_;
};

#endif	/* ALERTSWIDGET_H */
