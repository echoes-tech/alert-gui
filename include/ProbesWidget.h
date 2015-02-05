/* 
 * File:   ProbesWidget.h
 * Author: vpl
 *
 * Created on 13 août 2014, 12:03
 */

#ifndef PROBESWIDGET_H
#define	PROBESWIDGET_H

#include <Wt/WStandardItem>
#include <Wt/WStandardItemModel>
#include <tools/Enums.h>

#include "Conf.h"
#include "GlobalIncludeFile.h"
#include "AbstractPage.h"


class AbstractPage;

class ProbesWidget :
public AbstractPage 
{
public:
    ProbesWidget(Echoes::Dbo::Session *session, std::string apiUrl);
    virtual ~ProbesWidget();
    static int m_stateColumn;
    
private:
    Echoes::Dbo::Session        *session_;
    std::string                 apiUrl_;
    
    Wt::WComboBox               *m_unitTypeComboBox;
    Wt::WStandardItemModel      *m_unitTypeModel;
    Wt::WStandardItemModel      *m_probes;
    Wt::WStandardItemModel      *m_assets;
    Wt::WIOService              *m_thread;
    
    std::map<long long, bool>           m_mapProbesAlive;
    std::map<long long, int>            m_mapProbesTimer;
    std::map<long long, int>            m_mapProbesDelay;
    std::map<long long, Wt::WText*>     m_mapProbesWText;
    std::map<long long, Wt::WTimer*>    m_mapProbesWTimer;

    
    boost::mutex                m_mutex;
        
    void fillModel(Wt::WStandardItemModel * model, std::map<long long, std::string> m_mapNames);
    void fillModels();
    void addEnumToModel(Enums::EInformationUnitType enumToAdd, Wt::WString name);
    
    void handleJsonGet(vectors_Json jsonResources);
    void handleJsonGetHeartbeat(vector<Wt::Json::Value> jsonResource);
    void updateText(long long id);
    void updateAliveStates(long long id, int rowTable, int columnTable);
    void threadSafeFunctionCall(long long id, int rowTable, int columnTable);
    int addCustomResourceTable(long long id, int rowTable, int columnTable);
protected:
    void modifResource(vector<Wt::WInteractWidget*>* arguments, long long id);
};

#endif	/* PROBESWIDGET_H */

