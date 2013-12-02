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


typedef std::pair<long long, std::string>       Pair;

typedef std::multimap<long long, Pair>          MultiMapPair;
typedef std::multimap<long long, long long>     MultiMapLongs;

typedef std::vector<long long>                  VectorLong;
typedef std::vector<std::string>                VectorString;
typedef std::vector<Wt::WWidget *>              VectorWWidget;
typedef std::vector<Wt::WText*>                 VectorText;
typedef std::vector<Wt::WComboBox*>             VectorComboBox;
typedef std::vector<Wt::WLineEdit*>             VectorLineEdit;

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
    void                        recoverListAsset();
    
    void                        getAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getAssets(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getPluginsForAsset(boost::system::error_code err, const Wt::Http::Message& response, long long idAsset, long long index);
    void                        getPlugins(boost::system::error_code err, const Wt::Http::Message& response);
    void                        getInformations(boost::system::error_code err, const Wt::Http::Message& response, long long idPlugin, long long index);

    int                         checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);

    void                        addResource(std::vector<Wt::WInteractWidget*> argument);
    Wt::WDialog                 *deleteResource(long long id);
    void                        modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);

    void                        close();
    
    
    void                        deleteAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        postAlerts(boost::system::error_code err, const Wt::Http::Message& response);
    void                        putAlerts(boost::system::error_code err, const Wt::Http::Message& response);

    void                        modifRecip(long long id);

    void                        popupAddWidget(Wt::WDialog *dialog);
    void                        popupPrecipient(long long id);

    
    long long                   recoverId(Wt::WSelectionBox *box, MultiMapPair infoInBox);

private:

    void                        initBoxs(Wt::WTable *tableBox);

    VectorLong                  boxActived(Wt::WSelectionBox *box, MultiMapPair infoInBox, MultiMapLongs compId, long long id);
    void                        fillInBox(Wt::WSelectionBox *box, MultiMapPair infoInBox);


    void                        setSession(Echoes::Dbo::Session *session);
    void                        setApiUrl(std::string apiUrl);
    std::string                 getApiUrl();
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

    void                        compareBar(Wt::WContainerWidget *contain);
    
    void                        errorsShow(VectorText error);
    void                        errorsHide(VectorText error);

    
    void aSupprimer(); // 

    
    bool                created_;
    bool                newClass_;
    Echoes::Dbo::Session             *session_;
    std::string         apiUrl_;
    Wt::Json::Value     alerts_;
    
    MultiMapPair        assets_;
    MultiMapPair        plugins_;
    MultiMapPair        infos_;
    
    MultiMapLongs       assetPlugins_;
    MultiMapLongs       pluginInfos_;
    MultiMapLongs       pluginAsset_;
    MultiMapLongs       infoPlugin_;

    MultiMapLongs       unitsIds_;

    VectorLong          index_;
    
    VectorWWidget       unitOne_;
    VectorWWidget       unitTwo_;
    VectorWWidget       unitThree_;
    
    VectorComboBox      textBox_;  //For check combo text
    VectorComboBox      compBox_;  //For check combo number compare
    VectorComboBox      sizeBox_;  //For check combo number size

    int                 bool_;  //for check button true/false
    
    long long           idAsset_;
    long long           idPlugin_;
    long long           idInfo_;
    
    std::string         assetSelected_;
    std::string         pluginSelected_;
    std::string         infoSelected_;

    VectorLineEdit       textsEdit_;
    VectorLineEdit       valuesEdit_;

    //  Errors Messages ------
    Wt::WText           *errorAsset_;
    Wt::WText           *errorPlugin_; 
    Wt::WText           *errorInfo_;
    Wt::WText           *errorKey_;
    
    VectorText           errorText_;
    VectorText           errorBool_;
    VectorText           errorNumb_;
    //  -------
    
    Wt::WContainerWidget *compareBar_;
};

#endif	/* ALERTSWIDGET_H */

