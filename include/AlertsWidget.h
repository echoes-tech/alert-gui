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

#include <boost/function.hpp>
#include <boost/system/error_code.hpp>

#include "tools/Session.h"

#include "GlobalIncludeFile.h"
#include "CreatePageWidget.h"
#include "ApiManagement.h"



typedef std::pair<Wt::WComboBox*, Wt::WComboBox*> PairComboBox;
typedef std::pair<long long, std::string>       Pair;
typedef std::pair<long long, long long>         PairLong;
typedef std::pair<PairLong, std::string>         PairLongString;
typedef std::pair<Wt::WLineEdit*, Wt::WText*>     PairResource;

typedef std::multimap<long long, PairLongString>          MultiMapPairLPLS;

typedef std::multimap<long long, Pair>          MultiMapPair;
typedef std::multimap<long long, long long>     MultiMapLongs;
typedef std::multimap<long long, std::string>     MultiMapLongString;

typedef std::map<long long, std::pair<PairResource, Wt::WComboBox*>>    MapUnitOne;
typedef std::map<long long, std::pair<PairResource, PairComboBox>>     MapUnitTwo;

typedef std::map<int, Wt::WWidget *>            MapIntWWidget;
typedef std::map<int, Wt::WLineEdit*>           MapIntLineEdit;
typedef std::map<int, Wt::WText*>               MapIntText;

typedef std::vector<long long>                  VectorLong;
typedef std::vector<std::string>                VectorString;
typedef std::vector<Wt::WText*>                 VectorText;
typedef std::vector<Wt::WComboBox*>             VectorComboBox;
/** gkr.\n
 * pair &lsaquo; pair &lsaquo; idAsset, idPlugin &rsaquo;, pair &lsaquo; idInfo, idKey &rsaquo; &rsaquo;
 */
typedef std::pair<PairLong, PairLong>           PairTwoPair;

class AlertsWidget :
public CreatePageWidget
{
public:
    AlertsWidget(Echoes::Dbo::Session *session, std::string apiUrl);

    void                        update();
    std::vector<std::string>    getTitlesTableWidget();
    std::vector<std::string>    getTitlesTableText();
    std::vector<long long>      getIdsTable();
    vector_type                 getResourceRowTable(long long id);
    Wt::WValidator              *editValidator(int who);
    void                        closePopup();
    void                        recoverListAlert();
    void                        recoverListRecipientAlias(long long userRoleId);
    
    void                        getAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getCriterion(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getAssets(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getUsersList(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getMedia(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getPluginsForAsset(boost::system::error_code err, const Wt::Http::Message& response, long long idAsset, long long index);
    void                        getInformations(boost::system::error_code err, const Wt::Http::Message& response, long long idPlugin, long long index);
    void                        getInformation(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getAliasInfo(boost::system::error_code err, const Wt::Http::Message& response, long long userRoleId, long long mediaType);
    void                        getAliasAsset(boost::system::error_code err, const Wt::Http::Message& response, long long userRoleId, long long mediaType);
    void                        getAliasPlugin(boost::system::error_code err, const Wt::Http::Message& response, long long userRoleId, long long mediaType);
    void                        postAlert(boost::system::error_code err, const Wt::Http::Message& response);
    void                        deleteAlert(boost::system::error_code err, const Wt::Http::Message& response);

    int                         checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);
    void                        checkPopupRecipients(std::string message, std::string time, int media);

    void                        addResource(std::vector<Wt::WInteractWidget*> argument);
    Wt::WDialog                 *deleteResource(long long id);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);

    void                        close();
    
    
    void                        deleteAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        postAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        putAlerts(boost::system::error_code err, const Wt::Http::Message& response);

    void                        modifRecip(long long id);

    void                        popupAddWidget(Wt::WDialog *dialog, long long id);
    void                        popupRecipients(std::string nameAlert, std::string message);

    
    long long                   recoverId(Wt::WSelectionBox *box, MultiMapPair infoInBox);

    void                        postAlertCallApi(std::string message);

private:

    void                        initBoxOne(Wt::WTable *tableBox);

    VectorLong                  boxActived(Wt::WSelectionBox *box, MultiMapPair infoInBox, MultiMapLongs compId, long long id);
    void                        fillInBox(Wt::WSelectionBox *box, MultiMapPair infoInBox);


    void                        fillInMultiMap();
    int                         selectItemBox(Wt::WSelectionBox *box, std::string select);
    void                        cleanBox(Wt::WSelectionBox *box);

    Wt::WTable                  *createUnitOne(Wt::WContainerWidget *contain);
    Wt::WTable                  *createUnitTwo(Wt::WContainerWidget *contain);
    void                        createUnitThree(Wt::WContainerWidget *contain);
    void                        showUnit(long long id);
    void                        hideUnit();

    void                        selectAsset(long long id, Wt::WSelectionBox *boxAsset, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxInfo);
    void                        selectPlugin(long long id, Wt::WSelectionBox *boxAsset, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxInfo);
    void                        selectInfo(long long id, Wt::WSelectionBox *boxInfo, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxAsset);

    void                        errorsHideOne(MapUnitOne error);
    void                        errorsHideTwo(MapUnitTwo error);

    void                        fillInTabMessage();

    
    std::vector<std::pair<long long, int>>   idUserPositionMedia_;
    
    bool                created_;
    bool                newClass_;
    Echoes::Dbo::Session             *session_;
    std::string         apiUrl_;
    Wt::Json::Value     alerts_;
    
    int                 time_;
    
    MultiMapPair        userInfo_;
    MultiMapPairLPLS        mediaInfo_;
    Wt::WTabWidget      *tabMessage_;
    std::string         messageMailForTab_;
    std::string         messageSmsForTab_;
    std::string         messagePushForTab_;
    
    MultiMapLongString  criterions_;

    MultiMapPair        assets_; // Assets infomations (<index <idAsset, nameAsset>>)
    MultiMapPair        plugins_; // Plugins infomations (<index <idPlugin, namePlugin>>)
    MultiMapPair        infos_; // Infos infomations (<index <idInfo, nameInfo>>)
    
    MultiMapLongs       assetPlugins_; // nexus between Asset id and Plugins ids
    MultiMapLongs       pluginInfos_; // nexus between Plugin id and Infos ids
    MultiMapLongs       pluginAsset_; // nexus between Plugin id and Asset id
    MultiMapLongs       infoPlugin_; // nexus between Info id and Plugin id

    MultiMapLongs       unitsIds_; // Link between Info and widgets compare. (text, number, bool)

    VectorLong          index_;
    
    MapIntWWidget       unitOne_; // Text
    MapIntWWidget       unitTwo_; // Number
    Wt::WTable          *unitThree_; // Bool
    
    int                 bool_;  //for check button true/false
    int                 checkAll_;
    /**
     * pair &lsaquo; pair &lsaquo; idAsset, idPlugin &rsaquo;
     * , pair &lsaquo; idInfo, idKey &rsaquo; &rsaquo;
     */
    PairTwoPair         idAll_;
    
    std::string         assetSelected_; // Name Asset selected
    std::string         pluginSelected_; // Name Plugin selected
    std::string         infoSelected_; // Name Info selected

    long long           idUnitOne; // html tag for widget Text
    long long           idUnitTwo; // html tag for widget number
    
    MapUnitOne          resourcesUnitOne;
    MapUnitTwo          resourcesUnitTwo;
    
    //  Errors Messages ------
    Wt::WText           *errorAsset_;
    Wt::WText           *errorPlugin_; 
    Wt::WText           *errorInfo_;
    Wt::WText           *errorKey_;

    Wt::WLineEdit       *keyValue_;
    
    Wt::WText            *errorBool_;
    //  -------
    
    Wt::WContainerWidget *compareBarOne_;
    Wt::WContainerWidget *compareBarTwo_;
    Wt::WPushButton      *buttonAddOne_;
    Wt::WPushButton      *buttonAddTwo_;
    Wt::WLineEdit        *saveLineEditOne_;
    Wt::WLineEdit        *saveLineEditTwo_;
};

#endif	/* ALERTSWIDGET_H */
