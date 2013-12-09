/* 
 * Gui CreatePageWidget.h
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 23/09/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#ifndef CREATEPAGEWIDGET_H
#define	CREATEPAGEWIDGET_H

// Lib c++
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <typeinfo>
#include <string>
// Lib c
#include <stdlib.h>
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
#include <Wt/Json/Value>
#include <Wt/WRegExpValidator>

// Lib boost
#include <boost/concept_check.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <Enums.h>

// Test
#include <Wt/WBoxLayout>
#include <Wt/WButtonGroup>
#include <Wt/WGroupBox>
#include <Wt/WRadioButton>
#include <Wt/WTableRow>
#include <Wt/WNavigationBar>
/** gkr.\n
 * Max Size Name of resource
 */
#define SIZE_NAME_RESOURCE (40)

/** gkr.\n
 * vector &lsaquo; WInteractWidget* &rsaquo;
 */
typedef std::vector<Wt::WInteractWidget*> vector_type;
/** gkr.\n
 * vector &lsaquo; pair &lsaquo; int, WObject* &rsaquo; &rsaquo;
 */
typedef std::vector<std::pair<int, Wt::WObject*>> vector_pair;
/** gkr.\n
 * vector &lsaquo; pair &lsaquo; string, WLength &rsaquo; &rsaquo;
 */
typedef std::vector<std::pair<std::string, Wt::WLength>> pair_type;

class CreatePageWidget : 
public Wt::WTemplateFormView
{
public:

                        CreatePageWidget(std::string namePage);
    virtual             ~CreatePageWidget();

    /**
     * This call for fill in popup (WDialog).\n
     * tabW can 
     * @param tabW
     */
    virtual void        popupAddWidget(Wt::WDialog *dialog) {};
    virtual void        update();

    virtual int         checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage);

    /**
     * Add button modification.
     * @param check \n
     * True of false.
     */
    void                setButtonModif(bool check);
    /**
     * Add button delete.
     * @param check \n
     * True or false.
     */
    void                setButtonSup(bool check);
    /**
     * Table in window = True.\n
     * Table in popup (WDialog) = False.
     * @param background \n 
     * True or False.
     */
    void                setLocalTable(bool background);
    /**
     * Set Number resource type WText.
     * @param nbResource \n
     * Number of resource WText.
     */
    void                setResourceNumberAdd(int nbResource);
    void                setUpdate(bool update);
    void                setNameSpecial(std::string nameResourcePageSpe);


protected:

    Wt::WContainerWidget                *createHeaderTable();
    Wt::WContainerWidget                *createBodyTable();
    Wt::WContainerWidget                *createFooterTable();

    virtual void                        addResource(std::vector<Wt::WInteractWidget*> argument) {created_ = false;};
    virtual Wt::WDialog                 *deleteResource(long long id);
    virtual void                        modifResource(std::vector<Wt::WInteractWidget*> argument, long long id) {created_ = false;};
    virtual Wt::WValidator              *editValidator(int who) {return (new Wt::WValidator());};
    virtual void                        closePopup() {};

    virtual std::vector<std::string>    getTitlesTableText() {std::vector<std::string> res; return res;};
    virtual std::vector<std::string>    getTitlesTableWidget() {std::vector<std::string> res; return res;};
    virtual std::vector<long long>      getIdsTable() {std::vector<long long> res; return res;};
    virtual vector_type                 getResourceRowTable(long long id) {vector_type res; return res;};

    void                                buttonInDialogFooter(Wt::WDialog *dialog);
    
private:

    void                addInputForAffix(int rowBodyTable);
    void                showInputForAdd();
    void                checkAdd(std::vector<Wt::WText*> errorMessage);
    void                checkModif(vector_type inputs, long long id, std::vector<Wt::WText*> errorMessage);
    void                popupForModif(long long id);
    void                inputForModif(long long id, int rowTable, int columnTable);

    void                addResourceInHeaderTable();
    void                addButtons(long long id, int rowTable, int columnTable);  
    void                popupWindow();
    void                 popupCheck(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage, Wt::WDialog *dialog, long long id);
    void                fillInTable();
    void                popupComplete(Wt::WDialog *dialog);
    void                searchName(Wt::WLineEdit *arg);

    void                resourceBeAff();
    Wt::WComboBox       *getComboBox();
    void                initPaginatePage(Wt::WNavigationBar *navBar);
    void                paginatePage();
    void                builtPaginate(Wt::WNavigationBar *navBar);
    int                 sizeAff();

    void                switchPage(int rst);
    bool                checkResource();

    std::string         nameResourcePage;
    std::string         nameResourcePageSpec_;
    vector_type         inputs_;
    vector_type         butPaginate_;
    vector_type         butPaginateExt_;
    vector_pair       resources_;
    Wt::WTable          *mediaTable_;
    bool                created_;
    bool                butModif_;
    bool                butSup_;
    bool                mainPage_;
    bool                update_;
    int                 nbResource_;
    int                 nbAff_;
    int                 nbAffBegin_;
    int                 nbAffResource_;
};

#endif	/* CREATEPAGEWIDGET_H */

