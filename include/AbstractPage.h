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

// Lib c
#include <stdlib.h>
// Lib c++
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <string>
// Lib boost
#include <boost/concept_check.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string/split.hpp>
#include <boost/algorithm/string/classification.hpp>
#include <boost/system/system_error.hpp>
// Lib Witty
#include <Wt/Utils>

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
#include <Wt/WColor>

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

#include <Wt/WEvent>

#include <Wt/Http/Message>
#include <Wt/Json/Parser>
#include <Wt/Json/Value>
#include <Wt/Http/Client>
#include <Wt/Json/Array>
#include <Wt/Json/Object>

#include <Wt/WCheckBox>
//#include <Wt/WTimer>

#include <Enums.h>
// Lib perso
#include "tools/Session.h"

// DEFINE

#define SIZE_NAME (160)

// TYPEDEF

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
/** AbstractPage. vector_pair \n
 * vector &lsaquo; pair &lsaquo; int, WObject* &rsaquo; &rsaquo;
 */
typedef std::vector<std::pair<int, Wt::WObject*>> vector_pair;

typedef std::vector<std::vector<Wt::Json::Value>> vectors_Json;

class AbstractPage :
public Wt::WTemplateFormView
{
public:
                                AbstractPage(Echoes::Dbo::Session *session, std::string apiUrl, std::string namePage, bool selectable = false);
    virtual                     ~AbstractPage();

    
    virtual void                getResourceList();
    void                        setResources(vector_pair resources);
    vector_pair                 getResources() const;
    void                        setResourceTable(Wt::WTable* resourceTable);
    Wt::WTable*                 getResourceTable() const;
    void                        setSelectedID(long long selectedID);
    long long                   getSelectedID();
    void                        addPageToUpdate(AbstractPage* abstractPage);
    virtual void                updatePage(bool getResources = true);
protected:

    // ENUM

    enum ETypeJson
    {
        widget = -1, // other
        text = 0, //WText
        boolean = 1, //WCheckBox
        integer = 2, // int //WText
        undid = 3, // under id (string) "name" //WComboBox
        object = 4
    };

    virtual void                clearStructures();
    
    void                        createTable();

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
    virtual int                 addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable);
    void                        addButtonsToPopupFooter(Wt::WDialog *dialog);

    // Set/Get attribut to init or option. ------------------------
    void                                        setRowsTable(std::map<long long, vector_widget> rowsTable);
    std::map<long long, vector_widget>          getRowsTable();
    /**
     * Set titles for table.
     * @param titles &lsaquo; type, name &rsaquo; \n
     * type(0) == Wtext || type(1) == Wwidget.
     */
    void                        setUndidName(std::string undidName);
    void                        setTitles(std::multimap<int, std::string> titles);
    virtual void                setDisplayedTitlesPopups();
    void                        setUrl(lists_string listsUrl);
    void                        setButtonModif(bool check);
    void                        setButtonSup(bool check);
//    void                        setLocalTable(bool background);
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
    void                        recursiveGetResources(vectors_Json jsonResource = vectors_Json(), lists_string listsUrl = lists_string());
    int                         handleHttpResponseGet(boost::system::error_code err, const Wt::Http::Message& response,
    lists_string listsUrl, vectors_Json jsonResource, Wt::Http::Client *client);
    virtual std::string         addParameter();
    // ---- ADD MODIF DELETE ----------------------------------------------
    virtual void                addResource(std::vector<Wt::WInteractWidget*> argument);
    virtual void                setAddResourceMessage(Wt::Http::Message *message,std::vector<Wt::WInteractWidget*> argument);
    virtual void                modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id);
    virtual void                setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*> argument);
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
    
    std::map<long long, vector_widget>          m_rowsTable;
    std::multimap<int, std::string>             m_displayedTitlesPopups;
    
    Echoes::Dbo::Session                        *m_session;
    
    bool                getFooterOkButtonStatus();
    void                setFooterOkButtonStatus(bool active);
    void                        tableHandler(long long id);

private:
    // Main attributs ---------------------------
    // main table of the page, used to list resources
    Wt::WTable                  *m_resourceTable;
    /**
     * Set elements to table. \n
     * multimap &lsaquo; id/index, vector &lsaquo; widget &rsaquo; &rsaquo;\n
     * Usage : \n id/index, always use number greader 0 \n
     */
    vector_widget                       m_inputs;
    // Attributs.-------------------------------
    lists_string                        m_listsUrl;
    std::multimap<int, std::string>     m_titles;
    
    std::string                         m_apiUrl;
    std::string                         m_xmlPageName;
    std::string                         m_nameResourcePageSpec;
    std::string                         m_undidName;
    bool                                m_isModifButtonPresent;
    bool                                m_isDeleteButtonPresent;
    bool                                m_selectable;
    long long                           m_selectedID;
    std::vector<AbstractPage*>            m_pagesToUpdate;     
//    bool                                m_isMainPage;
    // select drop + paginate--------------------
    vector_pair                 m_resources;
    
    bool m_footerOkButtonActive;
    
//    Wt::WTimer                  *_timer;
};

#endif	/* ABSTRACTPAGE_H */

