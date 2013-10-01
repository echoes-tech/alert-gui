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

/** gkr.\n
 * Max Size Name of resource
 */
#define SIZE_NAME_RESOURCE (15)

/** gkr.\n
 * map &lsaquo; vector &lsaquo; string &rsaquo; , vector &lsaquo; WInteractWidget* &rsaquo; &rsaquo;
 */
typedef std::map<std::vector<std::string>, std::vector<Wt::WInteractWidget*>> map_Type;

class CreatePageWidget : 
public Wt::WTemplateFormView
{
public:

                                CreatePageWidget(std::string namePage);
    virtual                     ~CreatePageWidget();
    
    void                popupForModif(std::vector<std::string> resources, long long id);

protected:
    /** gkr.\n
     * createBodyTable is call by you.
     * createBodyTable is call in child
     * For init header table.
     * @return WContainerWidget
     * Bind this widget in child.
     */
    Wt::WContainerWidget        *createHeaderTable();
    /** gkr.\n
     * createBodyTable is call by you.
     * createBodyTable is call in child
     * For init body table.
     * @return WContainerWidget
     * Bind this widget in child.
     */
    Wt::WContainerWidget        *createBodyTable();
    /** gkr.\n
    * addResource is call by popupForAdd(bool)
    * Whether name arg is validated.
    * Overloading in child.
    * @param WLineEdit
    * Contains name arg.
    */
    virtual void                addResource(std::vector<Wt::WInteractWidget*> argument) {};
    /** gkr.\n
    * deleteResource is include in addButtons(long long).
    * Overloading in child.
    * @param long long
    * ID of resource.
    */
    virtual void                deleteResource(long long id) {};
    /** gkr.\n
     * modifResource is call by
     * @param WLineEdit
     */
    virtual void                modifResource(Wt::WLineEdit *saveEdit) {};
    /** gkr.\n
    * editValidator is call by popupForAdd(bool).
    * Overloading in child.
    * @return WValidator
    * All child of WValidator.
    */
    virtual Wt::WValidator      *editValidator() {return (new Wt::WValidator());};
    /** gkr.\n
     * addResourceInHeaderTable is call by you.
     * addResourceInHeaderTable is call in child.
     * Fill in vector by resource for title table.
     * addResourceInHeaderTable add modif and sup in header.
     * @param vector &lsaquo; string &rsaquo;\n
     * Same number resource for header than addColumnInTable map_Type
     * @param int\n 
     * Number of title no button.
     */
    void                        addResourceInHeaderTable(std::vector<std::string> titleHeader, int nb);
    /** gkr.\n
     * addColumnInTable is call by you.
     * addColumnInTable is call in child.\n\n
     * Fill in map_Type by resource for Table.
     * addColumnInTable add modif and sup buttons in table.
     * @param map_Type
     * Same number resource for column than addResourceInHeaderTable vector.
     */
    void                        addColumnInTable(map_Type myTable);

    
private:
    /** gkr.\n
     * addButtons is call by addColumnInTable().
     * addButtons add modif and delete buttons.
     * @param std::vector &lsaquo; std::string &lsaquo; , long long
     * resources and ID.
     */
    void                        addButtons(std::vector<std::string> resources, long long id);  
    /** gkr.\n
     * popupForAdd is call when user click in headerButton.
     * popupForAdd call editValidator() ->
     *      You Overloading this methode in child.
     * @param WString
     * For check in recursive, and add same name of last arg add.
     **/
    void                        popupForAdd();


    void    popupTable(Wt::WDialog *dialog_);

    
    
    std::vector<Wt::WInteractWidget*>    saveTitleHeader;
    std::vector<std::string>            titleHeader_;
    std::string                 nameResourcePage;
//    Wt::WDialog                 *dialog_;
    Wt::WTable                  *mediaTable_;
    int                         columnTable;
    int                         rowTable;
    Wt::WLineEdit               *assetEdit;
    bool                        popupTable_;
};

#endif	/* CREATEPAGEWIDGET_H */

