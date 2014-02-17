/* 
 * File:   Association.cpp
 * Author: gkr
 * 
 * Created on 13 décembre 2013, 12:04
 */

#include "Association.h"

using namespace std;

Association::Association(Echoes::Dbo::Session *session, string apiUrl)
: AbstractPage(session, apiUrl, "association")
{
    session_ = session;
    apiUrl_ = apiUrl;
    created_ = false;
    newClass_ = false;

    setButtonModif(false);
    setButtonSup(true);
    setLocalTable(true);
    
    vector_pair_string listTitles;
    listTitles.push_back(std::make_pair(ETypeJson::text, "filter"));
    listTitles.push_back(std::make_pair(ETypeJson::text, "filter_index"));
    listTitles.push_back(std::make_pair(ETypeJson::text, "information"));
    listTitles.push_back(std::make_pair(ETypeJson::text, "asset"));
    setTitles(listTitles);
    
    lists_string lListUrl;
    std::list<std::string> listUrl;
    
    listUrl.push_back("information_datas");
    listUrl.push_back("information_datas/:id");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back("assets");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back("plugins");
    lListUrl.push_back(listUrl);
    listUrl.clear();
    
    listUrl.push_back("informations");
    lListUrl.push_back(listUrl);
    listUrl.clear();

    listUrl.push_back("filters");
    listUrl.push_back("filters/:id");
    lListUrl.push_back(listUrl);
    listUrl.clear();

    setUrl(lListUrl);
}

// TABLE(S) FOR MAIN PAGE -------------------------------------

void Association::updatePage(bool getResources)
{
    AbstractPage::updatePage(getResources);
}

// TABLE(S) FOR POPUP ------------------------------------------

void Association::popupAddWidget(Wt::WDialog *dialog, long long id)
{
    dialog->contents()->clear();
    if (id == 0)
    {
        
        std::map<long long, std::pair<long long, std::string>>::iterator itA;

        new Wt::WText("Plugin", dialog->contents());
        Wt::WComboBox *boxPlugins = new Wt::WComboBox(dialog->contents());
        boxPlugins->setModel(pluginsModel);
        boxPlugins->setModelColumn(2);
        boxPlugins->setCurrentIndex(0);
                
        boxPlugins->changed().connect(bind([ = ] (){
            idPlugin_ = boost::lexical_cast<long long>(pluginsModel->item(boxPlugins->currentIndex(),1)->text().toUTF8());
        }));

        new Wt::WText("Host", dialog->contents());
        Wt::WComboBox *boxAssets = new Wt::WComboBox(dialog->contents());
        boxAssets->setModel(assetsModel);
        boxAssets->setModelColumn(2);
        boxAssets->setCurrentIndex(0);

        boxAssets->changed().connect(bind([ = ] (){
            idHost_ = boost::lexical_cast<long long>(assetsModel->item(boxAssets->currentIndex(),1)->text().toUTF8());
        }));
        
        idPlugin_ = boost::lexical_cast<long long>(pluginsModel->item(boxPlugins->currentIndex(),1)->text().toUTF8());
        idHost_ = boost::lexical_cast<long long>(assetsModel->item(boxPlugins->currentIndex(),1)->text().toUTF8());

        new Wt::WText("Filters", dialog->contents());

        tableFilters = new Wt::WTable(dialog->contents());
        tableFilters->setHeaderCount(1);
        tableFilters->setWidth("100%");
        tableFilters->setStyleClass("table table-bordered table-striped table-hover data-table dataTable");

        int col = 0;
        Wt::WCheckBox *cb = new Wt::WCheckBox();
        tableFilters->elementAt(0, col++)->addWidget(cb);
//        new Wt::WText("Search id",tableFilters->elementAt(0, col++));
        new Wt::WText("Filter id",tableFilters->elementAt(0, col++));
        new Wt::WText("Filter type",tableFilters->elementAt(0, col++));
        new Wt::WText("Filter Index",tableFilters->elementAt(0, col++));
        new Wt::WText("Infos",tableFilters->elementAt(0, col++));
        new Wt::WText("Assets",tableFilters->elementAt(0, col++));

        for (int i = 1 ; i <= filtersModel->rowCount() ; i++ )
        {
            col = 0;
            Wt::WCheckBox *selectLine = new Wt::WCheckBox();
            selectLine->setChecked(false);
            filterCheckBox_[i]= selectLine;
            tableFilters->elementAt(i, col++)->addWidget(selectLine);
            
            new Wt::WText(filtersModel->item(i-1,0)->text(),tableFilters->elementAt(i, col++));
            new Wt::WText(filtersModel->item(i-1,1)->text(),tableFilters->elementAt(i, col++));
            new Wt::WText(filtersModel->item(i-1,2)->text(),tableFilters->elementAt(i, col++));
            cout << "FILTRE AFFICHé : " << filtersModel->item(i-1,2)->text() << endl;
            
            filterInfosComboBox_[i] = new Wt::WComboBox(tableFilters->elementAt(i, col++));
            filterInfosComboBox_[i]->setModel(informationsModel);
            filterInfosComboBox_[i]->setModelColumn(2);

            filterAssetsComboBox_[i] = new Wt::WComboBox(tableFilters->elementAt(i, col++));
            filterAssetsComboBox_[i]->setModel(assetsModel);
            filterAssetsComboBox_[i]->setModelColumn(2); 
            
        }
               
    }
    else
    {
        // Modif
//        MapAssetInfos::iterator ResForModif = assetInfos_.find(id);
    }
    dialog->show();
}

int Association::checkInput(vector<Wt::WInteractWidget*> inputName, vector<Wt::WText*> errorMessage)
{
    return 0;
}

void Association::closePopup()
{
    getResourceList();
}



void Association::clearStructures()
{
    AbstractPage::clearStructures();
    assets_.clear();
    plugins_.clear();
    assetInfos_.clear();
}

void Association::handleJsonGet(vectors_Json jsonResources)
{
    m_rowsTable.clear();
    vector_Json jsonResource = jsonResources.at(0);
    cout << "taille resources : " << jsonResources.size() << endl;
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                informationDatasModel = new Wt::WStandardItemModel(0, 4, this);
//                assetsModel = new Wt::WStandardItemModel(0, 2, this);
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonIda = jsonArray.at(cpt);
                    Wt::Json::Object jsonDetailedIda = jsonResource.at(cpt + 1);
                    
                    Wt::WStandardItem *filterIdItem = new Wt::WStandardItem();
                    Wt::WStandardItem *filterIndexItem = new Wt::WStandardItem();
                    Wt::WStandardItem *informationIdItem = new Wt::WStandardItem();
                    Wt::WStandardItem *assetIdItem = new Wt::WStandardItem();
                    
                    Wt::Json::Object filterJsonObject = jsonDetailedIda.get("filter");
                    long long filterId = filterJsonObject.get("id");
                    filterIdItem->setText(boost::lexical_cast<string>(filterId));
                    
                    int filterIndex = jsonIda.get("filter_field_index");
                    filterIndexItem->setText(boost::lexical_cast<string>(filterIndex));
                    
                    Wt::Json::Object informationJsonObject = jsonDetailedIda.get("information");
                    long long informationId = informationJsonObject.get("id");
                    string informationName = informationJsonObject.get("name");
                    informationIdItem->setText(boost::lexical_cast<string>(informationId));
                    
                    Wt::Json::Object assetJsonObject = jsonDetailedIda.get("asset");
                    long long assetId = assetJsonObject.get("id");
                    string assetName = assetJsonObject.get("name");
                    assetIdItem->setText(boost::lexical_cast<string>(assetId));
                    
                    vector<Wt::WStandardItem*> rowVector;
                    rowVector.push_back(filterIdItem);
                    rowVector.push_back(filterIndexItem);
                    rowVector.push_back(informationIdItem);
                    rowVector.push_back(assetIdItem);
                    informationDatasModel->insertRow(cpt, rowVector);
                    
                    vector<Wt::WInteractWidget *> nameW;
                    nameW.push_back(new Wt::WText(boost::lexical_cast<string>(filterId)));
                    nameW.push_back(new Wt::WText(boost::lexical_cast<string>(filterIndex)));
                    nameW.push_back(new Wt::WText(informationName));
                    nameW.push_back(new Wt::WText(assetName));
                    
                    long long idaId = jsonIda.get("id");
                    
                    m_rowsTable.insert(make_pair(idaId, nameW));
                    
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Association][InfoData] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Association][InfoData] JSON Type Exception";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    // assets
    jsonResource = jsonResources.at(1);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                assetsModel = new Wt::WStandardItemModel(0, 3, this);
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonAsset = jsonArray.at(cpt);
                    
                    Wt::WStandardItem *cptItem = new Wt::WStandardItem();
                    Wt::WStandardItem *assetIdItem = new Wt::WStandardItem();
                    Wt::WStandardItem *assetNameItem = new Wt::WStandardItem();
                                    
                    cptItem->setText(boost::lexical_cast<string>(cpt));
                    
                    long long assetId = jsonAsset.get("id");
                    string assetName = jsonAsset.get("name");
                    assetIdItem->setText(boost::lexical_cast<string>(assetId));
                    assetNameItem->setText(boost::lexical_cast<string>(assetName));
                    
                    
                    vector<Wt::WStandardItem*> rowVector;
                    rowVector.push_back(cptItem);
                    rowVector.push_back(assetIdItem);
                    rowVector.push_back(assetNameItem);
                    assetsModel->insertRow(cpt, rowVector);
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Association][Assets] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Association][Assets] JSON Type Exception";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    // plugins
    jsonResource = jsonResources.at(2);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                pluginsModel = new Wt::WStandardItemModel(0, 3, this);
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonPlugin = jsonArray.at(cpt);
                    
                    Wt::WStandardItem *cptItem = new Wt::WStandardItem();
                    Wt::WStandardItem *pluginIdItem = new Wt::WStandardItem();
                    Wt::WStandardItem *pluginNameItem = new Wt::WStandardItem();
                                    
                    cptItem->setText(boost::lexical_cast<string>(cpt));
                    
                    long long pluginId = jsonPlugin.get("id");
                    string pluginName = jsonPlugin.get("name");
                    pluginIdItem->setText(boost::lexical_cast<string>(pluginId));
                    pluginNameItem->setText(boost::lexical_cast<string>(pluginName));
                    
                    vector<Wt::WStandardItem*> rowVector;
                    rowVector.push_back(cptItem);
                    rowVector.push_back(pluginIdItem);
                    rowVector.push_back(pluginNameItem);
                    pluginsModel->insertRow(cpt, rowVector);
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Association][Plugins] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Association][Plugins] JSON Type Exception";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    // information
    jsonResource = jsonResources.at(3);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                informationsModel = new Wt::WStandardItemModel(0, 4, this);
                for (unsigned int cpt = 0; cpt < jsonArray.size(); cpt++)
                {
                    Wt::Json::Object jsonInformation = jsonArray.at(cpt);
                    
                    Wt::WStandardItem *cptItem = new Wt::WStandardItem();
                    Wt::WStandardItem *informationIdItem = new Wt::WStandardItem();
                    Wt::WStandardItem *informationNameItem = new Wt::WStandardItem();
                    Wt::WStandardItem *informationUnitIdItem = new Wt::WStandardItem();
                                    
                    cptItem->setText(boost::lexical_cast<string>(cpt));
                    
                    long long informationId = jsonInformation.get("id");
                    string informationName = jsonInformation.get("name");
                    Wt::Json::Object jsonInfoUnit = jsonInformation.get("information_unit");
                    long long informationUnitId = jsonInfoUnit.get("id");
                    informationIdItem->setText(boost::lexical_cast<string>(informationId));
                    informationNameItem->setText(informationName);
                    informationUnitIdItem->setText(boost::lexical_cast<string>(informationUnitId));
                    
                    vector<Wt::WStandardItem*> rowVector;
                    rowVector.push_back(cptItem);
                    rowVector.push_back(informationIdItem);
                    rowVector.push_back(informationNameItem);
                    rowVector.push_back(informationUnitIdItem);
                    informationsModel->insertRow(cpt, rowVector);
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Association][Information] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Association][Information] JSON Type Exception";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    // filters
    jsonResource = jsonResources.at(4);
    try
    {
        if (jsonResource.size() > 0)
        {
            Wt::Json::Array& jsonArray = (*jsonResource.begin());
            if (!jsonArray.empty())
            {
                filtersModel = new Wt::WStandardItemModel(0, 3, this);
                int row = 0;
                for (int cpt(0); cpt < (int) jsonArray.size(); cpt++)
                {
                    
//                    Wt::Json::Object jsonFilter = jsonArray.at(cpt);
                    Wt::Json::Object jsonDetailedFilter = jsonResource.at(cpt + 1);
                    
                    Wt::WStandardItem *filterNbValueItem = new Wt::WStandardItem();
                    
                    
                    
                    set<int> indexAlreadyAssociated;
                    
                    Wt::Json::Array idaJsonArray = jsonDetailedFilter.get("information_datas");
                    indexAlreadyAssociated.clear();
                    for (int cptIda(0); cptIda < (int) idaJsonArray.size(); cptIda++)
                    {
                        
                        Wt::Json::Object jsonIda = idaJsonArray.at(cptIda);
                        int index = jsonIda.get("filter_field_index");
                        indexAlreadyAssociated.insert(index);
                    }
                    
                    int nbValue = jsonDetailedFilter.get("nb_value");
                    filterNbValueItem->setText(boost::lexical_cast<string>(nbValue));
                    
                    for (int i = 1 ; i <= nbValue ; i++)
                    {
                        
                        
                        if (indexAlreadyAssociated.find(i) == indexAlreadyAssociated.end())
                        {
                            Wt::WStandardItem *filterIdItem = new Wt::WStandardItem();
                            Wt::WStandardItem *filterTypeItem = new Wt::WStandardItem();
                            Wt::WStandardItem *filterIndexItem = new Wt::WStandardItem();
                            long long filterId = jsonDetailedFilter.get("id");
                            cout << "FILTRES : " << filterId << " INDEX : " << i << endl;
                            filterIdItem->setText(boost::lexical_cast<string>(filterId));

                            Wt::Json::Object filterType = jsonDetailedFilter.get("filter_type");
                            string filterTypeName = filterType.get("name");
                            filterTypeItem->setText(filterTypeName);

                            filterIndexItem->setText(boost::lexical_cast<string>(i));

                            vector<Wt::WStandardItem*> rowVector;
                            rowVector.push_back(filterIdItem);
                            rowVector.push_back(filterTypeItem);
                            rowVector.push_back(filterIndexItem);
                            cout << "index ?? " << filterIndexItem->text() << endl;
                            filtersModel->insertRow(row++, rowVector);
                        }
                    }
                }
            }
        }
    }
    catch (Wt::Json::ParseError const& e)
    {
        Wt::log("warning") << "[Association][Filters] Problems parsing JSON";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    catch (Wt::Json::TypeException const& e)
    {
        Wt::log("warning") << "[Association][Filters] JSON Type Exception";
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.asset.database-error"), Wt::Ok);
    }
    
    updatePage(false);
}

void Association::addResource(vector<Wt::WInteractWidget*> argument)
{
    cout << "Host id : " << idHost_ << endl;
    cout << "Plugin id : " << idPlugin_ << endl;


            
        // Post Asset -------
        Wt::Http::Message messageAsset;
        string message = "[\n";
        bool coma = false;
        for (int i = 1 ; i < tableFilters->rowCount() ; i++)
        {
            Wt::WCheckBox *tempCB = filterCheckBox_[i];
            
            if (tempCB->isChecked())
            {
                if (coma)
                {
                    message += ",";
                }
                else
                {
                    coma = true;
                }
                string assetIdString = assetsModel->item(filterAssetsComboBox_[i]->currentIndex(),1)->text().toUTF8();
                cout << "asset id" << assetIdString << endl;
                string informationIdString = informationsModel->item(filterInfosComboBox_[i]->currentIndex(),1)->text().toUTF8();
                cout << "info id" << informationIdString << endl;
                string informationUnitIdString = informationsModel->item(filterInfosComboBox_[i]->currentIndex(),3)->text().toUTF8();
                cout << "info unit id" << informationUnitIdString << endl;
                string filterIdString = ((Wt::WText*)tableFilters->elementAt(i,1)->widget(0))->text().toUTF8();
                cout << "filter id" << filterIdString << endl;
                string filterFieldIndexString = ((Wt::WText*)tableFilters->elementAt(i,3)->widget(0))->text().toUTF8();
                cout << "filter index id" << filterFieldIndexString << endl;
                message += "{\n"
                        "\"asset_id\": "
                        + assetIdString +",\n"
                        "\"information_id\": "
                        + informationIdString +",\n"
                        "\"information_unit_id\": "
                        + informationUnitIdString +",\n"
                        "\"filter_id\": "
                        + filterIdString +",\n"
                        "\"filter_field_index_id\": "
                        + filterFieldIndexString +"\n"
                        "}";
            }
        }
        message += "]";
                
        messageAsset.addBodyText(message);
    
        string apiAddress = this->getApiUrl() + "/assets/"
                            + boost::lexical_cast<string>(idHost_)
                            + "/plugins"
                            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) 
                            + "&token=" + session_->user()->token.toUTF8();
        Wt::Http::Client *client = new Wt::Http::Client(this);
        client->done().connect(boost::bind(&Association::postAssetCallBack, this, _1, _2));
        
    
//        Wt::log("debug") << "Association : [POST] address to call : " << apiAddress;
//        Wt::log("debug") << "Association : message : " << message;
    
        if (client->post(apiAddress, messageAsset))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
        {
            Wt::log("error") << "Error Client Http";
        }
}

void Association::postAssetCallBack(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << response.status() << " Reponse postAsset : " << std::endl << response.body() << std::endl;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() != Enums::EReturnCode::CREATED)
        {
            Wt::log("error") << "[Associations Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "status", tr("Alert.association.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Associations Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "err", tr("Alert.association.database-error"), Wt::Ok);
    }
    getResourceList();
}

void Association::deleteAsset(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        std::cout << response.status() << std::endl;
        if(response.status() == Enums::EReturnCode::NO_CONTENT)
        {
            if (response.body() != "")
            {
                Wt::log("error") << "[Associations Widget] Response should be empty : " << response.body() << ".";
            }
        }
        else
        {
            Wt::log("error") << "[Associations Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.association.database-error-title"),tr("Alert.association.database-error"),Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Associations Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.association.database-error-title"),tr("Alert.association.database-error"),Wt::Ok);
    }
    getResourceList();
}

Wt::WDialog *Association::deleteResource(long long id)
{
    Wt::WDialog *box = AbstractPage::deleteResource(id);
    box->show();
    box->finished().connect(bind([ = ] (){
        if (box->result() == Wt::WDialog::Accepted)
        {
            Wt::Http::Message message;
            message.addBodyText("");

//            string apiAddress = this->getApiUrl() + "/assets/"
//                                + boost::lexical_cast<string>(id)
//                                + "/plugins"
//                                + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) 
//                                + "&token=" + session_->user()->token.toUTF8();
//            Wt::Http::Client *client = new Wt::Http::Client(this);
//            client->done().connect(boost::bind(&Association::deleteAsset, this, _1, _2));
//
//
//            Wt::log("debug") << "Association : [DELETE] address to call : " << apiAddress;
//            Wt::log("debug") << "Association : message : " << message;
//
//            if (client->deleteRequest(apiAddress))
//            {
//                Wt::WApplication::instance()->deferRendering();
//            }
//            else
//            {
//                Wt::log("error") << "Error Client Http";
//            }
        }
                                      return box;
    }));
    return box;
}

void Association::modifResource(vector<Wt::WInteractWidget*> arguments, long long id)
{
    //    string messageString;
    //
    //    messageString = "{\n\n}";
    //
    //    Wt::Http::Message message;
    //    message.addBodyText(messageString);
    //
    //    string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<string> (id)
    //            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
    //            + "&token=" + session_->user()->token.toUTF8();
    //
    //    Wt::Http::Client *client = new Wt::Http::Client(this);
    //    client->done().connect(boost::bind(&Association::putAsset, this, _1, _2));
    //    Wt::log("debug") << "Association : [PUT] address to call : " << apiAddress;
    //    if (client->put(apiAddress, message))
    //        Wt::WApplication::instance()->deferRendering();
    //    else
    //        Wt::log("error") << "[Association] Error Client Http";
//    recoverListAssociation();
}

void Association::close()
{
    delete this;
}

