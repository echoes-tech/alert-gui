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
    void                        popupRecipientsRework(std::string nameAlert, std::string message);
    
    std::vector<std::string>    getTitlesTableWidget();
    std::vector<std::string>    getTitlesTableText();
    Wt::WValidator              *editValidator(int who);
    
    void                        closePopup();
    void                        close();
    
    void                        getAliasListFromRecipient(long long userRoleId);

    int                         checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);
    void                        checkPopupRecipients(std::string message, std::string time, int media);

    void                        addResource(std::vector<Wt::WInteractWidget*>* argument);
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
        Wt::WRegExpValidator* validatorCriteria;
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
    void httpResponse(boost::system::error_code err, const Wt::Http::Message& response, long long mediaType, long long requestType, long long criteria);
    
    struct Header {
        Wt::WTemplate   *addButton;
        Wt::WComboBox   *comboBox;
    };
    
    struct Row {
        long long       id;
        Wt::WText       *name;
        Wt::WTemplate   *deleteButton;
    };
    
    struct List {
        long long                   currentRow;
        long long                   currentTable;
        struct Header               header;
        std::vector<struct Row>     rows;
        std::vector<Wt::WTable*>    table;
    };
    
    struct Medias {
        struct List     elt;
        Wt::WString     message;
    };
    
    struct Receivers {
        struct List                 elt;
        std::vector<struct Medias>  medias;
    };
 
        // rework second popup utilities
    void changeRowsColor(Wt::WColor *highlightColor, Wt::WTable *table, long long row);
    void addRowTableToList(struct List &s_list, Wt::WWidget *widget1, Wt::WWidget *widget2);
    void addHeaderTableToList(struct List &s_list, Wt::WWidget *widget0, Wt::WWidget *widget1, Wt::WWidget *widget2);
    void addRowToList(struct List &s_list, long long id, Wt::WText *name, Wt::WTemplate *deleteButton);
    void eraseRowFromList(struct List &s_list, long long index);
    
    struct Media {
        long long id;
        long long index;
        Wt::WText *name;
        Wt::WTemplate *deleteButton;
    };
    
    
    struct Receiver {
        long long id;
        long long index;
        long long indexMedia;
        Wt::WText *name;
        Wt::WTemplate *addMedia;
        Wt::WTemplate *deleteButton;
        Wt::WTableCell *nameCell;
        Wt::WComboBox *mediasChoices;
        Wt::WTable *tableMedias;
        std::map<long long, struct Media> medias;
    };
    
    
    Echoes::Dbo::Session        *m_session;
    std::string                 m_apiUrl;
    Wt::Json::Value             m_alerts;
    
    int             time_;
    long long       m_currentReceiver;
    long long       m_currentMedia;
    long long       m_rowReceiver;
    long long       m_rowMedia;
    
    struct Receiver m_precReceiver;
    
    
    std::map<long long, struct Receiver>                                                m_selectedReceivers;
    std::map<long long, Wt::WText*>                                                     m_selectedMedias;
    std::map<long long, std::map<long long, AlertCustom>>                               m_custom;
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

    
    // FIXME : to be refactored and included in m_alertCriteria
    Wt::WTable                          *m_booleanCompareWidget; // Bool
    Wt::WTextArea                       *m_customCompareWidget; // Bool
    
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
    
    Wt::WTable              *m_compareTable;
    Wt::WContainerWidget *m_compareWidgetContainerTop;
    Wt::WContainerWidget *m_compareWidgetContainerSequence;
    
    Wt::WPushButton      *m_buttonAddNumber;
    
    Wt::WLineEdit        *saveLineEditOne_;
    Wt::WLineEdit        *saveLineEditTwo_;
};

#endif	/* ALERTSWIDGET_H */
