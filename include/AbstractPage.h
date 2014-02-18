/* 
 * Gui AbstractPage.h
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 21/12/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * AbstractPage
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */
#ifndef ABSTRACTPAGE_H
#define	ABSTRACTPAGE_H

// Lib c++
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <string>
// Lib c
#include <stdlib.h>
// Lib boost
#include <boost/concept_check.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/system/system_error.hpp>
// Lib Witty
#include <Wt/WValidator>
#include <Wt/WInteractWidget>
#include <Wt/WTemplateFormView>
#include <Wt/WTemplate>
#include <Wt/WDialog>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WText>
#include <Wt/WAnchor>
#include <Wt/WContainerWidget>
#include <Wt/WTable>
#include <Wt/WApplication>
#include <Wt/WString>
#include <Wt/WLabel>
#include <Wt/WCssDecorationStyle>

#include <Wt/WTextArea>
#include <Wt/WTabWidget>
#include <Wt/WMenu>
#include <Wt/WMenuItem>
#include <Wt/WStackedWidget>

#include <Wt/WInPlaceEdit>
#include <Wt/WComboBox>
#include <Wt/WRegExpValidator>
#include <Wt/WBoxLayout>
#include <Wt/WMessageBox>
#include <Wt/WNavigationBar>

#include <Wt/Http/Message>
#include <Wt/Json/Parser>
#include <Wt/Json/Value>
#include <Wt/Http/Client>
#include <Wt/Json/Array>
#include <Wt/Json/Object>

#include <Wt/WCheckBox>

#include <Enums.h>
// Lib perso
#include "WebUtils.h"
#include "tools/Session.h"

// DEFINE

#define SIZE_NAME (160)

// TYPEDEF

/** AbstractPage. vector_pair \n
 * list &lsaquo; pair &lsaquo; int, string &rsaquo; &rsaquo;
 */
typedef std::vector<std::pair<int, std::string>>  vector_pair_string;
/** AbstractPage. map_widget \n
 * list &lsaquo; int, string &rsaquo;
 */
typedef std::multimap<int, Wt::WWidget>  map_widget;
/** AbstractPage. multimap_string \n
 * list &lsaquo; int, string &rsaquo;
 */
typedef std::multimap<int, std::string>  multimap_string;
/** AbstractPage. list_string \n
 * list &lsaquo; string &rsaquo;
 */
typedef std::list<std::string>  list_string;
/** AbstractPage. lists_string \n
 * list &lsaquo; list &lsaquo; string &rsaquo; &rsaquo;
 */
typedef std::list<list_string> lists_string;

/** AbstractPage. vector_widget \n
 * vector &lsaquo; WInteractWidget* &rsaquo;
 */
typedef std::vector<Wt::WInteractWidget*> vector_widget;
/** AbstractPage. multimap_long_widgets \n
 * multimap &lsaquo; long long, vector &lsaquo; WInteractWidget* &rsaquo; &rsaquo;
 */
typedef std::multimap<long long, vector_widget> multimap_long_widgets;
/** AbstractPage. vector_pair \n
 * vector &lsaquo; pair &lsaquo; int, WObject* &rsaquo; &rsaquo;
 */
typedef std::vector<std::pair<int, Wt::WObject*>> vector_pair;

typedef std::vector<Wt::Json::Value> vector_Json;

typedef std::vector<vector_Json> vectors_Json;

class AbstractPage :
public Wt::WTemplateFormView
{
public:
                                AbstractPage(Echoes::Dbo::Session *session, std::string apiUrl, std::string namePage);
    virtual                     ~AbstractPage();

    void                        recursiveGetResources(vectors_Json jsonResource = vectors_Json(), lists_string listsUrl = lists_string());
    virtual void                getResourceList();
protected:

    // ENUM

    enum ETypeJson
    {
        widget = -1, // other
        text = 0, //WText
        boolean = 1, //WCheckBox
        integer = 2, // int //WText
        undid = 3 // under id (string) "name" //WComboBox
    };

    
    virtual void                updatePage(bool getResources = true);
    virtual void                clearStructures();
    
    void                        createTable();
    void                        createEmptyResourceTable();

    // -------- Creates Elements to table. ------------------------
    Wt::WContainerWidget        *createTableFirstHeader();
    Wt::WContainerWidget        *createTableBody();
    Wt::WContainerWidget        *createTableFooter();
    // Add Resource For Elements ----------------------------------
    void                        addTableSecondHeader();
    void                        fillBodyTable();
    void                        addInputForAffix(int rowBodyTable);
    // POPUP : ----------------------------------------------------
    void                        addResourcePopup();
    void                        modifResourcePopup(long long id);
    void                        popupCheck(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage,
    Wt::WDialog                 *dialog, long long id);
    void                        popupFinalization(Wt::WDialog *dialog, long long id);
    // Methodes useful
    void                        addGenericButtonsToResourceTable(long long id, int rowTable, int columnTable);
    void                        addButtonsToPopupFooter(Wt::WDialog *dialog);

    // Set/Get attribut to init or option. ------------------------
    void                        setRowsTable(multimap_long_widgets rowsTable);
    multimap_long_widgets       getRowsTable();
    /**
     * Set titles for table.
     * @param titles &lsaquo; type, name &rsaquo; \n
     * type(0) == Wtext || type(1) == Wwidget.
     */
    void                        setUndidName(std::string undidName);
    void                        setTitles(vector_pair_string titles);
    void                        setUrl(lists_string listsUrl);
    void                        setButtonModif(bool check);
    void                        setButtonSup(bool check);
    void                        setLocalTable(bool background);
    void                        setUpdate(bool update);
    void                        setNameSpecial(std::string nameResourcePageSpec);
    void                        setApiUrl(std::string apiUrl);
    std::string                 getApiUrl();

    // -------- CALL RETURN API, HANDLE --------------------------
    /**
     * You can overload this methode, if you have only one resource don't overload her.
     * @param jsonResources
     */
    virtual void                handleJsonGet(vectors_Json jsonResources);
    /**
     * recursiveGetResources for go call API with listsUrl_ \n
     * listsUrl_ is set in construtor child in setUrl()
     * @param Send jsonResource whether u want use her after handleJsonGet
     */
    int                         handleHttpResponseGet(boost::system::error_code err, const Wt::Http::Message& response,
    lists_string listsUrl, vectors_Json jsonResource, Wt::Http::Client *client);
    virtual std::string         addParameter();
    // ---- ADD MODIF DELETE ----------------------------------------------
    virtual void                addResource(std::vector<Wt::WInteractWidget*> argument);
    virtual void                modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);
    virtual Wt::WDialog         *deleteResource(long long id);
    // RETURNS API --------------------------------------
    virtual void                postResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    virtual void                putResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    virtual void                apiDeleteResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    // Check input ----------------------------------------------
    void                        checkAdd(std::vector<Wt::WText*> errorMessage);
    void                        checkModif(vector_widget inputs, long long id, std::vector<Wt::WText*> errorMessage);
    int                         checkName(std::string inputText, std::vector<long long> ids);
    virtual int                 checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);
    //  INPUT ---------------------------------------------------
    void                        showInputForAdd();
    void                        inputForModif(long long id, int rowTable, int columnTable);    
    // OVERLOAD -------------------------------------------------
    virtual Wt::WValidator      *editValidator(int who) {return (new Wt::WValidator());};
    virtual void                popupAddWidget(Wt::WDialog *dialog, long long id);
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
    
    multimap_long_widgets       m_rowsTable;

private:
    
    // Methodes disposable --------------------------------------
    Wt::WComboBox               *getNumberOfLineDisplayedComboBox();
    void                        builtPagination(Wt::WNavigationBar *navBar);
    void                        switchPage(int rst);
    void                        initPagination(Wt::WNavigationBar *navBar);
    void                        doThePaginationTrick();
    void                        selectLinesToBeDisplayed();
    void                        searchName(Wt::WLineEdit *arg);
    int                         countResources();

    // Main attributs ---------------------------
    // main table of the page, used to list resources
    Wt::WTable                  *m_resourceTable;
    /**
     * Set elements to table. \n
     * multimap &lsaquo; id/index, vector &lsaquo; widget &rsaquo; &rsaquo;\n
     * Usage : \n id/index, always use number greader 0 \n
     */
    vector_widget               m_inputs;
    // Attributs.-------------------------------
    lists_string                m_listsUrl;
    vector_pair_string          m_titles;
    Echoes::Dbo::Session        *m_session;
    std::string                 m_apiUrl;
    std::string                 m_xmlPageName;
    std::string                 m_nameResourcePageSpec;
    std::string                 m_undidName;
    bool                        m_isCreated;
    bool                        m_isModifButtonPresent;
    bool                        m_isDeleteButtonPresent;
    bool                        m_isMainPage;
    bool                        m_toUpdate;
    // select drop + paginate--------------------
    vector_pair                 m_resources;
    vector_widget               m_butPaginate;
    vector_widget               m_butPaginateExt;
    int                         m_nbAff;
    int                         m_nbAffBegin;
};

#endif	/* ABSTRACTPAGE_H */

