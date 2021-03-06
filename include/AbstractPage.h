/**
 * @file AbstractPage.h
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
    void                        setAddButtonEnable(bool enable);
    void                        addPageToUpdate(AbstractPage* abstractPage);
    virtual void                updatePage();
    virtual void                fillTable();
    
    static const std::string xmlDirectory;
protected:

    // ENUM
    enum ETypeJson
    {
        text = 0, //WText
        boolean = 1, //WCheckBox
        number = 2, // int //WText
        undid = 3, // under id (string) "name" //WComboBox
        object = 4,
        widget = 5 // other
    };

    enum ETextSpecial
    {
        normalText = 0,
        mail = 1,
        phone = 2,
        date = 3
    };
    
    enum ENumberSpecial
    {
        normalNumber = 0,
        notnull = 1,
        uns = 2, // unsigned
        flt = 3 // float
    };
    
    enum EMandatory
    {
        isnot = 0,
        is = 1
    };

    virtual void                clearStructures();
    
    void                        createTable();

    // -------- Creates Elements to table. ------------------------
    Wt::WContainerWidget        *createTableFirstHeader();
    virtual void                        addPopupAddHandler(Wt::WInteractWidget* widget);
    Wt::WContainerWidget        *createTableBody();
    Wt::WContainerWidget        *createTableFooter();
    // Add Resource For Elements ----------------------------------
    void                        addTableSecondHeader();
    void                        fillBodyTable();
    void                        addInputForAffix(int rowBodyTable);   
    // POPUP : ----------------------------------------------------
    void                        addResourcePopup();
    void                        modifResourcePopup(long long id);
    void                        popupCheck(std::vector<Wt::WInteractWidget*>* inputName, std::vector<Wt::WText*> errorMessage,
                                    Wt::WDialog *dialog, long long id);
    void                        popupFinalization(Wt::WDialog *dialog, long long id);
    // Methodes useful
    void                        addGenericButtonsToResourceTable(long long id, int rowTable, int columnTable);
    virtual void                addPopupModifHandler(Wt::WInteractWidget* widget, long long id);
    virtual int                 addCustomButtonsToResourceTable(long long id, int rowTable, int columnTable);
    virtual int                 addCustomResourceTable(long long id, int rowTable, int columnTable);
    virtual void                saveButtonFooter(Wt::WDialog *dialog);
    virtual void                cancelButtonFooter(Wt::WDialog *dialog);
    virtual void                customButtonFooter(Wt::WDialog *dialog);
    void                        addButtonsToPopupFooter(Wt::WDialog *dialog);

    // Set/Get attribut to init or option. ------------------------
    void                                        setRowsTable(std::map<long long, vector_widget> rowsTable);
    std::map<long long, vector_widget>          getRowsTable();
    /**
     * Set titles for table.
     * @param titles &lsaquo; type, name &rsaquo; \n
     * type(0) == Wtext || type(1) == Wwidget.
     */
    void                        setUndidName(std::vector<std::string> undidNames);
    void                        setTitles(std::vector<std::pair <int, std::string>> titles);
    virtual void                setDisplayedTitlesPopups();
    void                        setUrl(std::list<std::list<std::pair<std::string, std::vector<std::string>>>> listsUrl); 
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
    virtual std::vector<Wt::WInteractWidget*>   initRowWidgets(Wt::Json::Object jsonObject, std::vector<Wt::Json::Value> jsonResource, int cpt);
    void                        sendHttpRequestGet(std::string resource, std::vector<std::string> listParameter, boost::function<void (Wt::Json::Value)> functor);
    void                        handleHttpResponseGet(boost::system::error_code err, const Wt::Http::Message& response,
                                    Wt::Http::Client *client, boost::function<void (Wt::Json::Value)> functor);
    void                        recursiveGetResources(std::list<std::list<std::pair<std::string, std::vector<std::string>>>> listsUrl,
                                    boost::function<void (vectors_Json)> functorToCallAtEnd, vectors_Json jsonResource = vectors_Json());
    void                        handleRecursiveGetResources(Wt::Json::Value result, std::list<std::list<std::pair<std::string, std::vector<std::string>>>> listsUrl,
                                    boost::function<void (vectors_Json)> functorToCallAtEnd, vectors_Json jsonResource);
    /**
     * recursiveGetResources for go call API with listsUrl_ \n
     * listsUrl_ is set in construtor child in setUrl()
     * @param Send jsonResource whether u want use her after handleJsonGet
     */
    // ---- ADD MODIF DELETE ----------------------------------------------
    virtual void                addResource(std::vector<Wt::WInteractWidget*>* argument);
    virtual void                setAddResourceMessage(Wt::Http::Message *message,std::vector<Wt::WInteractWidget*>* argument);
    virtual void                modifResource(std::vector<Wt::WInteractWidget*>* arguments, long long id);
    virtual void                setModifResourceMessage(Wt::Http::Message *message, std::vector<Wt::WInteractWidget*>* argument);
    virtual Wt::WDialog         *deleteResource(long long id);
    // RETURNS API --------------------------------------
    virtual void                postResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    virtual void                putResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    virtual void                apiDeleteResourceCallback(boost::system::error_code err, const Wt::Http::Message& response, Wt::Http::Client *client);
    // Check input ----------------------------------------------
    int                         checkName(std::string inputText, std::vector<long long> ids);
    virtual int                 checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);
    //  INPUT ---------------------------------------------------
    void                        showInputForAdd(); 
    // OVERLOAD -------------------------------------------------
    Wt::WValidator              *editValidator(int type); // {return (new Wt::WValidator());};
    virtual void                popupAddWidget(Wt::WDialog *dialog, long long id);
    virtual Wt::WComboBox       *popupAdd(Wt::WDialog *dialog);
    
    std::map<long long, vector_widget>          m_rowsTable;
    std::vector<std::pair <int, std::string>>   m_displayedTitlesPopups;
    bool                                        m_autoUpdate;
    bool                                        m_hasAddButton;
    
    Echoes::Dbo::Session                        *m_session;
    
    bool                        getFooterOkButtonStatus();
    void                        setFooterOkButtonStatus(bool active);
    void                        tableHandler(long long id);
    void                        addEnumToModel(Wt::WStandardItemModel* standardItemModel, int enumToAdd, Wt::WString name,
                                    Wt::WString optionalParameter = Wt::WString::Empty);
    int                         setValidatorType(int type, int specialType, int mandatory);

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
    std::list<std::list<std::pair<std::string, std::vector<std::string>>>>      m_listsUrl;
    /* vector == column order | pair: 'type' of column - 'name' of column */
    /* full 'type': 4bits ETypeJson, 4bits ESpecialType and 4bits boolean (first being isMandatory) for a total of 12bits encoded chain */
    std::vector<std::pair<int, std::string>>                                    m_titles;
    
    std::string                         m_apiUrl;
    std::string                         m_xmlPageName;
    std::string                         m_nameResourcePageSpec;
    std::vector<std::string>            m_undidNames;
    bool                                m_isModifButtonPresent;
    bool                                m_isDeleteButtonPresent;
    bool                                m_selectable;
    long long                           m_selectedID;
    std::vector<AbstractPage*>            m_pagesToUpdate;     
//    bool                                m_isMainPage;
    // select drop + paginate--------------------
    vector_pair                         m_resources;
    bool                                m_buttonAddEnable = true;
    
    bool m_footerOkButtonActive;
    
//    Wt::WTimer                  *_timer;
};

#endif	/* ABSTRACTPAGE_H */
