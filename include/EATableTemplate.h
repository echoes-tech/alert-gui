/* 
 * File:   EATableTemplate.h
 * Author: tsa
 *
 * Created on 30 avril 2014, 17:11
 */

#ifndef EATABLETEMPLATE_H
#define	EATABLETEMPLATE_H

#include <Wt/WApplication>
#include <Wt/WTemplate>
#include <Wt/WPushButton>
#include <Wt/WLineEdit>
#include <Wt/WString>
#include <Wt/WText>
#include <Wt/WTable>
#include <Wt/WComboBox>
#include <Wt/WTemplate>
#include <Wt/WNavigationBar>

class EATableTemplate : public Wt::WTemplate
{
public:
    EATableTemplate(const Wt::WString& tableName, Wt::WContainerWidget* parent);
    EATableTemplate(const EATableTemplate& orig);
    virtual ~EATableTemplate();
private:
    void                        createTable();
//    void                        createEmptyResourceTable();
    Wt::WContainerWidget        *createTableHeader();
    Wt::WContainerWidget        *createTableBody();
    Wt::WContainerWidget        *createTableFooter();
    void                        addTableColumnHeader();
    Wt::WComboBox               *createDisplayedLinesComboBox();
    void                        fillTableBody();
    void                        searchInTable();
    
    virtual void                        headerPlusButtonBehaviour();
    
    Wt::WTable                          *m_mainTable;
    Wt::WString                         m_tableName;
    int                                 m_numberOfLinesToDisplay;
    Wt::WPushButton                     *m_headerPlusButton;
    Wt::WLineEdit                       *m_searchWidget;
    bool                                m_hasModifButton;
    bool                                m_hasDeleteButton;
    Wt::WContainerWidget                *m_footer;
    std::vector<std::pair <int, std::string>>     m_titles;

};

#endif	/* EATABLETEMPLATE_H */

