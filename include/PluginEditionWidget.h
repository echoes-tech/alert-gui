/* 
 * File:   PluginEditionWidget.h
 * Author: gdr
 *
 * Created on 18 mars 2013, 11:57
 */

#ifndef PLUGINEDITIONWIDGET_H
#define	PLUGINEDITIONWIDGET_H

#include <fstream>

#include <tools/Session.h>

#include <Wt/Json/Array>
#include <Wt/Json/Value>
#include <Wt/Json/Object>
#include <Wt/Json/Parser>

#include <Wt/WIntValidator>
#include <Wt/WRegExpValidator>

#include <Wt/WTable>
#include <Wt/WPushButton>
#include <Wt/WApplication>
#include <Wt/WMessageBox>
#include <Wt/WGroupBox>
#include <Wt/WText>

#include <cstdlib>
#include <Wt/Http/Response>
#include <Wt/Http/Client>
#include <Wt/Http/Message>
#include <Wt/WIOService>
#include <Wt/WApplication>

#include <Wt/WTemplateFormView>
#include <Wt/WLineEdit>
#include <Wt/WComboBox>
#include <Wt/WSelectionBox>
#include <Wt/WStringListModel>
#include <Wt/WCheckBox>
#include <Wt/WTableCell>
#include <Wt/WAnchor>
#include <Wt/WFileResource>
#include <Wt/WServer>
#include <boost/algorithm/string.hpp>

#include <tools/Enums.h>

#include "AbstractPage.h"
#include "WebUtils.h"


class PluginEditionWidget : public Wt::WTemplateFormView 
{
    public:
    
        PluginEditionWidget(Echoes::Dbo::Session *session, const std::string &apiUrl);
        ~PluginEditionWidget();
        void handleHttpResponsePlgList(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseAddPlg(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseDeletePlg(boost::system::error_code err, const Wt::Http::Message& response);
        
        void handleHttpResponseAddonList(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseAddonParameters(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseAddSource(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseModifySource(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseDeleteSource(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseSourceList(boost::system::error_code err, const Wt::Http::Message& response);
       
        void handleHttpResponseSeaTypeParameters(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseAddSearch(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseModifySearch(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseDeleteSearch(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseSearchList(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseSearchParameters(boost::system::error_code err, const Wt::Http::Message& response);
        
        void handleHttpResponseFilTypeParameters(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseAddFilter(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseModifyFilter(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseDeleteFilter(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseFilterList(boost::system::error_code err, const Wt::Http::Message& response);
        void handleHttpResponseFilterParameters(boost::system::error_code err, const Wt::Http::Message& response);
        
        std::string getApiUrl() const;

    private:
        Wt::WFormModel *formPlugin;
        void refresh1();
        void updateData();
        
        Wt::Http::Client *client1;
        Echoes::Dbo::Session *session;   
        std::string _apiUrl;

        void formatValue(std::string& value, int nbBackslash) const;
        
        void fillAddonComboBox();
        void fillSearchTypeComboBox();
        void fillFilterTypeComboBox();
        
        void selectedPlugin();
        void selectedSource();
        void selectedSearch();
        void selectedFilter();
        void createUI();
//        void createTableInformation();
        bool validatePlugin(std::string &badField);
        void addPlugin();
        void deletePlugin(int pluginId);
        void resetSource();
        bool validateSource(std::string &badField);
        void addSource();
        void modifySource();
        void deleteSource();
        void resetSearch();
        bool validateSearch(std::string &badField);
        void addSearch();
        void modifySearch();
        void deleteSearch();
        void resetFilter();
        bool validateFilter(std::string &badField);
        void addFilter();
        void modifyFilter();
        void deleteFilter();
        void addInformation();
//        void createTableUnit();
        void updateStateLECalculateInformation();
        void createFormSourceParameters();
        void completFormSource(); 
        void createFormSearchParameters();
        void completFormSearch();
        void createFormFilterParameters();
        void completFormFilter();
        void addSrcParamValue();
        
        void searchToken(); 
        void fixStyleTable(Wt::WTable *table);
        
        Wt::WString token;
        Wt::WString login;
        
        Wt::WIntValidator *validatorInt;
        Wt::WRegExpValidator *validatorRegExp;
        
        Wt::WPushButton *buttonModifySource;
        Wt::WPushButton *buttonModifySearch;
        Wt::WPushButton *buttonModifyFilter;
        
        //partie add Plugin
        Wt::WComboBox *pluginSelectionBox;
        Wt::WLineEdit *plgNameEdit;
        Wt::WLineEdit *plgDescEdit;
        std::map<int,long long> mapPluginsIdSboxRow;
        std::map<int,Wt::WString> mapPluginsDescrition;
        
        Wt::WTable *tablePlugin;
        
        //partie add Source        
        Wt::WSelectionBox *sourceSelectionBox;
        Wt::WStringListModel *slmAddons;
        std::map<int,long long> mapAddonsIdSboxRow;
        Wt::WComboBox *comboAddon;


        Wt::WTable *tableSrcParam;
        std::map<int,long long> mapAddonParameterIdSboxRow;
        std::map<int,Wt::WString> mapAddonParameterNameSBoxRow;
        std::map<int,Wt::WString> mapAddonParameterValueSBoxRow;
        std::map<Wt::WString,Wt::WLineEdit*> mapLEAddonParam;
                
        std::map<int,long long> mapSourceId;
        
        //partie add Search   
        Wt::WSelectionBox *searchSelectionBox;
        Wt::WStringListModel *slmSearchTypes;
        std::map<int,long long> mapSeaTypeIdSboxRow;
        Wt::WComboBox *comboSearchType;
        
        Wt::WTable *tableSeaParam;
        std::map<int,long long> mapSeaTypeParameterIdSboxRow;
        std::map<int,Wt::WString> mapSeaTypeParameterNameSBoxRow;
        std::map<int,Wt::WString> mapSeaTypeParameterValueSBoxRow;
        std::map<Wt::WString,Wt::WLineEdit*> mapLESeaTypeParam;
        
        std::map<int, long long> mapSearchId;
        std::map<int, int> mapSearchPeriode;

        std::map<int, long long> mapSearchSTYId;
        
        Wt::WLineEdit *periodeLE;

        
//        Wt::WTable *tableSeaUnit;
        
        
        //partie add Filter   
        Wt::WSelectionBox *filterSelectionBox;
        Wt::WStringListModel *slmFilType;
        std::map<int,long long> mapFilTypeIdSboxRow;
        Wt::WComboBox *comboFilType;
        
        Wt::WTable *tableFilTypeParam;
        std::map<int,long long> mapFilTypeParameterIdSboxRow;
        std::map<int,Wt::WString> mapFilTypeParameterNameSBoxRow;
        std::map<int,Wt::WString> mapFilTypeParameterValueSBoxRow;
        std::map<Wt::WString,Wt::WLineEdit*> mapLEFilTypeParam;
        
        std::map<int, long long> mapFilterId;
        std::map<int, bool> mapFilterStatic;
        std::map<int, int> mapFilterPKValue;
        std::map<int, int> mapFilterNbValue;
        std::map<int, long long> mapFilterSTYId;
        
        
        Wt::WCheckBox *staticCB;
        Wt::WLineEdit *pKValueLE ;
        Wt::WLineEdit *nbValueLE;
        
//        Wt::WTable *tableFilUnit;
        
        std::map<int,Wt::WLineEdit*> mapLENameInformation;
        std::map<int,Wt::WCheckBox*> mapCBDisplayInformation;
        std::map<int,Wt::WCheckBox*> mapCBCalculateInformation;
        std::map<int,Wt::WLineEdit*> mapLECalculateInformation;
//        std::map<int,long long> mapUnitIdSboxRow;
//        std::map<int,Wt::WComboBox*> mapCBUnit;
//        Wt::WStringListModel *slmUnit;
        
        int searchId;
        int filterId;
        int currentInformation;

        int status;
        
        // savoir si on créer le formSource en mode vierge ou completer (pour modifier une source existante)
        std::string createFormSourceState;
        std::string createFormSearchState;
        std::string createFormFilterState;

        void setApiUrl(std::string apiUrl);
};
#endif	/* PLUGINEDITIONWIDGET_H */

