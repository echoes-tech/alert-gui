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

//    setButtonModif(true);
    setButtonModif(false);
    setButtonSup(true);
    setLocalTable(true);

    vector_pair_string listTitles;
    setTitles(listTitles);
    
    lists_string lListUrl;
    std::list<std::string> listUrl;
    
    listUrl.push_back("assets");
    listUrl.push_back("assets/:id");
    lListUrl.push_back(listUrl);
    listUrl.clear();

    listUrl.push_back("informations");
    listUrl.push_back("informations/:id");
    lListUrl.push_back(listUrl);
    listUrl.clear();

    listUrl.push_back("plugins");
    lListUrl.push_back(listUrl);
    listUrl.clear();

    listUrl.push_back("filters");
    listUrl.push_back("filters/:id/parameters");
    lListUrl.push_back(listUrl);
    listUrl.clear(); 

    setUrl(lListUrl);
}

// TABLE(S) FOR MAIN PAGE -------------------------------------

void Association::update()
{
    AbstractPage::update();
    if (!newClass_)
    {
        newClass_ = true;
    }
}

// TABLE(S) FOR POPUP ------------------------------------------

void Association::popupAddWidget(Wt::WDialog *dialog, long long id)
{
    dialog->contents()->clear();
    if (id == 0)
    {
        idsInformations_.clear();
        idsInformations_.push_back((*informations_.begin()).second.first);
        idPlugin_ = (*plugins_.begin()).second.first;
        idHost_ = (*assets_.begin()).second.first;
        MapLongString2::iterator itA;

        new Wt::WText("Plugin", dialog->contents());
        Wt::WComboBox *boxPlugins = new Wt::WComboBox(dialog->contents());
        for (itA = plugins_.begin(); itA != plugins_.end(); itA++)
        {
            boxPlugins->addItem((*itA).second.second);
        }

        boxPlugins->changed().connect(bind([ = ] (){
                                                idPlugin_ = (*plugins_.find(boxPlugins->currentIndex())).second.first;
        }));

        new Wt::WText("Host", dialog->contents());
        Wt::WComboBox *boxAssets = new Wt::WComboBox(dialog->contents());
        boxAssets->setModel(assetsModel);
        boxAssets->setModelColumn(1);

        boxAssets->changed().connect(bind([ = ] (){
            idHost_ = boost::lexical_cast<long long>(assetsModel->item(boxAssets->currentIndex(),0)->text().toUTF8());
        }));

        new Wt::WText("Filters", dialog->contents());

        tableFilters = new Wt::WTable(dialog->contents());
        tableFilters->setHeaderCount(1);
        tableFilters->setWidth("100%");
        tableFilters->setStyleClass("table table-bordered table-striped table-hover data-table dataTable");

        int col = 0;
        Wt::WCheckBox *cb = new Wt::WCheckBox();
        tableFilters->elementAt(0, col++)->addWidget(cb);
        new Wt::WText("Search id",tableFilters->elementAt(0, col++));
        new Wt::WText("Filter id",tableFilters->elementAt(0, col++));
        new Wt::WText("Filter type",tableFilters->elementAt(0, col++));
        new Wt::WText("Filter Index",tableFilters->elementAt(0, col++));
        new Wt::WText("Infos",tableFilters->elementAt(0, col++));
        new Wt::WText("Assets",tableFilters->elementAt(0, col++));


        map<long long, filterValuesStruct>::iterator itF;
        int row = 1;
        for (itF = filterParameterValues_.begin(); itF != filterParameterValues_.end(); itF++)
        {
            for (int i = 1 ; i <= ((*itF).second.nbValue) ; i++)
            {
                bool check(true);
                for (MapFilter::iterator filter = filters_.begin();
                        filter != filters_.end(); filter++)
                {
                    if ((*itF).second.filterId == (*filter).first
                            && i == (*filter).second)
                    {
                        check = false;
                        
                    }
                }
                if (check == true)
                {
                    col = 0;

                    Wt::WCheckBox *cb1 = new Wt::WCheckBox();
                    cb1->setChecked(false);
                    filterCheckBox_[row]= cb1;
                    tableFilters->elementAt(row, col++)->addWidget(cb1);
                    new Wt::WText(boost::lexical_cast<string>((*itF).second.searchId),tableFilters->elementAt(row, col++));
                    new Wt::WText(boost::lexical_cast<string>((*itF).first),tableFilters->elementAt(row, col++));
                    new Wt::WText(boost::lexical_cast<string>((*itF).second.filterType),tableFilters->elementAt(row, col++));
                    new Wt::WText(boost::lexical_cast<string>(i),tableFilters->elementAt(row, col++));
                    filterInfosComboBox_[row] = new Wt::WComboBox(tableFilters->elementAt(row, col++));
                    filterInfosComboBox_[row]->setModel(informationsModel);
                    filterInfosComboBox_[row]->setModelColumn(1);

                    filterAssetsComboBox_[row] = new Wt::WComboBox(tableFilters->elementAt(row, col++));
                    filterAssetsComboBox_[row]->setModel(assetsModel);
                    filterAssetsComboBox_[row]->setModelColumn(1);

                    row++;
                }
            }

        }
    }
    else
    {
        // Modif
//        MapAssetInfos::iterator ResForModif = assetInfos_.find(id);
    }
    dialog->show();
}

// GET INFO FOR MOTHER ---------------------------------------

vector<string> Association::getTitlesTableWidget()
{
    vector<string> titleWidget;
    return titleWidget;
}

vector<string> Association::getTitlesTableText()
{
    vector<string> titleText;
    titleText.push_back("asset");
    titleText.push_back("information");
    return titleText;
}

vector<long long> Association::getIdsTable()
{
    vector<long long> ids;

    if (assetInfos_.size() > 0)
    {
        for (MapAssetInfos::iterator idsAssets = assetInfos_.begin();
                idsAssets != assetInfos_.end(); idsAssets++)
        {
            ids.push_back((*idsAssets).first);
        }
    }
    return ids;
}

vector_type Association::getResourceRowTable(long long id)
{
    vector_type rowTable;
    
    MapAssetInfos::iterator assetInfos = assetInfos_.find(id);

    if (assetInfos_.size() > 0)
    {
        for (MapLongString2::iterator assetResource = assets_.begin();
                assetResource != assets_.end(); assetResource++)
        {
            if (id == (*assetResource).second.first)
            {
                rowTable.push_back(new Wt::WText((*assetResource).second.second));
            }
        }
        std::string htmlInfo = "<ul>";
        for (std::map<long long, std::string>::iterator infos = (*assetInfos).second.begin();
                infos != (*assetInfos).second.end(); infos++)
        {
            htmlInfo += "<li>|" + (*infos).second + "|</li>";
        }
        htmlInfo += "</ul>";
        rowTable.push_back(new Wt::WText(htmlInfo));
    }
    return rowTable;
}

int Association::checkInput(vector<Wt::WInteractWidget*> inputName, vector<Wt::WText*> errorMessage)
{
    if (idsInformations_.size() == 0)
        return 1;
    return 0;
}

Wt::WValidator *Association::editValidator(int who)
{
    Wt::WRegExpValidator *validator;
    return validator;
}

void Association::closePopup()
{
    recoverListAssociation();
}

void Association::recoverListAssociation()
{
    assets_.clear();
    plugins_.clear();
    filterParameterValues_.clear();
    informations_.clear();
    idsInformations_.clear();
    assetInfos_.clear();

    recursiveGetResources();
}

void    Association::handleJsonGet(vectors_Json jsonResources)
{
    
    vector_Json jsonResource = jsonResources.at(0);
    long long cpt(0);
    if (jsonResource.size() > 0)
    {
        Wt::Json::Array& jsonArray = (*jsonResource.begin());
        if (!jsonArray.empty())
        {
            assetsModel = new Wt::WStandardItemModel(0,2,this);
            for (int cpt(0); cpt < (int)jsonArray.size(); cpt++)
            {
                Wt::Json::Object jsonObject = jsonArray.at(cpt);
                Wt::Json::Object jsonAsset = jsonResource.at(cpt + 1);
                
                Wt::WStandardItem *itemId = new Wt::WStandardItem();
                Wt::WStandardItem *itemName = new Wt::WStandardItem();

                Wt::WString name = jsonObject.get("name");
                long long id = jsonObject.get("id");

                itemId->setText(boost::lexical_cast<string>(id));
                itemName->setText(name);
                vector<Wt::WStandardItem*> rowVector;
                rowVector.push_back(itemId);
                rowVector.push_back(itemName);
                assetsModel->insertRow(cpt,rowVector);
                assets_[cpt] = make_pair(id, name.toUTF8());
                
                
                int information = jsonObject.get("information_datas");
                if (information > 0)
                {
                    Wt::Json::Array jsonInfoData = jsonAsset.get("information_datas");

                    std::map<long long, std::string> saveIdInfo;
                    for (Wt::Json::Object jsonIda : jsonInfoData)
                    {
                        Wt::Json::Object infoResource = jsonIda.get("information");
                        long long idInfo = infoResource.get("id");
                        saveIdInfo[idInfo] = "";
                    }
                    assetInfos_[id] = saveIdInfo;
                }
            }
        }
    }
    
    jsonResource = jsonResources.at(1);
    cpt = 0;
    if (jsonResource.size() > 0)
    {
        Wt::Json::Array& jsonArray = (*jsonResource.begin());
        if (!jsonArray.empty())
        {
            informationsModel = new Wt::WStandardItemModel(0,3,this);
            for (int cpt(0); cpt < (int)jsonArray.size(); cpt++)
            {
                Wt::Json::Object jsonObject = jsonArray.at(cpt);
                Wt::Json::Object jsonAsset = jsonResource.at(cpt + 1);
                
                Wt::Json::Array infosFilters = jsonAsset.get("information_datas");
                
                for (Wt::Json::Object infoFilter : infosFilters)
                {
                    long long indexFilter = infoFilter.get("filter_field_index");
                    Wt::Json::Object filterObj = infoFilter.get("filter");
                    long long filterId = filterObj.get("id");
                    filters_.insert(std::make_pair(filterId, indexFilter));
                }
                
                Wt::WString name = jsonObject.get("name");
                long long id = jsonObject.get("id");

                Wt::WStandardItem *itemId = new Wt::WStandardItem();
                Wt::WStandardItem *itemName = new Wt::WStandardItem();
                Wt::WStandardItem *itemUnitId = new Wt::WStandardItem();

                itemId->setText(boost::lexical_cast<string>(id));
                itemName->setText(name);

                if (assetInfos_.size() > 0)
                {
                    for (MapAssetInfos::iterator assetInfos = assetInfos_.begin();
                            assetInfos != assetInfos_.end() ;assetInfos++)
                    {
                        for (std::map<long long, std::string>::iterator saveIdInfo = (*assetInfos).second.begin();
                                saveIdInfo != (*assetInfos).second.end(); saveIdInfo++)
                        {
                            if ((*saveIdInfo).first == id)
                                (*saveIdInfo).second = name.toUTF8();
                        }
                    }
                }
                Wt::Json::Object infoUnit = jsonObject.get("information_unit");
                long long idUnit = infoUnit.get("id");

                itemUnitId->setText(boost::lexical_cast<string>(idUnit));
                vector<Wt::WStandardItem*> rowVector;
                rowVector.push_back(itemId);
                rowVector.push_back(itemName);
                rowVector.push_back(itemUnitId);
                informationsModel->insertRow(cpt,rowVector);
            }
        }
    }


    jsonResource = jsonResources.at(2);
    cpt = 0;
    if (jsonResource.size() > 0)
    {
        Wt::Json::Array& jsonArray = (*jsonResource.begin());
        if (!jsonArray.empty())
        {
            for (Wt::Json::Object jsonObject : jsonArray)
            {
                Wt::WString name = jsonObject.get("name");
                long long id = jsonObject.get("id");
                plugins_[cpt] = make_pair(id, name.toUTF8());
                cpt++;
            }
        }
    }
    

    jsonResource = jsonResources.at(3);
    cpt = 0;
    if (jsonResource.size() > 0)
    {
        Wt::Json::Array& jsonArray = jsonResource.at(0);
        for (int cpt(0); cpt < (int)jsonArray.size(); ++cpt)
        {
            Wt::Json::Object jsonObject = jsonArray.at(cpt);
            Wt::Json::Array& jsonFilterParam = jsonResource.at(cpt + 1);

            long long filterId = jsonObject.get("id");
            
            Wt::Json::Object tmpFilterType = jsonObject.get("filter_type");
            Wt::Json::Object tmpSearch = jsonObject.get("search");
            for (Wt::Json::Object filJson : jsonFilterParam)
            {
                filterParameterValues_[filterId] = {
                    jsonObject.get("id"),
                    tmpFilterType.get("id"),
                    jsonObject.get("nb_value"),
                    tmpSearch.get("id"),
                    filJson.get("value"),
                    jsonObject.get("information_datas")
                };
            }
        }
    }
    update();
}

void Association::addResource(vector<Wt::WInteractWidget*> argument)
{
    cout << "Host id : " << idHost_ << endl;
    cout << "Plugin id : " << idPlugin_ << endl;
    cout << "informations ids : " << endl;
    for (vector<long long>::iterator it = idsInformations_.begin();
            it != idsInformations_.end(); it++)
    {
        cout << " " << (*it);
    }
    cout << endl << endl;

//                    tmp.get("asset_id"),
//                    tmp.get("information_id"),
//                    tmp.get("information_unit_id"),
//                    tmp.get("filter_id"),
//                    tmp.get("filter_field_index_id")
                    
            
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
                message += "{\n"
                        "\"asset_id\": "
                        + assetsModel->item(filterAssetsComboBox_[i]->currentIndex(),0)->text().toUTF8() +",\n"
                        "\"information_id\": "
                        + informationsModel->item(filterInfosComboBox_[i]->currentIndex(),0)->text().toUTF8() +",\n"
                        "\"information_unit_id\": "
                        + informationsModel->item(filterInfosComboBox_[i]->currentIndex(),2)->text().toUTF8() +",\n"
                        "\"filter_id\": "
                        + ((Wt::WText*)tableFilters->elementAt(i,2)->widget(0))->text().toUTF8() +",\n"
                        "\"filter_field_index_id\": "
                        + ((Wt::WText*)tableFilters->elementAt(i,4)->widget(0))->text().toUTF8() +"\n"
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
        client->done().connect(boost::bind(&Association::postAsset, this, _1, _2));
        
    
        Wt::log("debug") << "Association : [POST] address to call : " << apiAddress;
        Wt::log("debug") << "Association : message : " << message;
    
        if (client->post(apiAddress, messageAsset))
        {
            Wt::WApplication::instance()->deferRendering();
        }
        else
        {
            Wt::log("error") << "Error Client Http";
        }
}

void Association::postAsset(boost::system::error_code err, const Wt::Http::Message& response)
{
    std::cout << response.status() << " Reponse postAsset : " << std::endl << response.body() << std::endl;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                // ToDo : check object
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Associations Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Associations Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title") + "TypeException", tr("Alert.association.database-error"), Wt::Ok);
            }
        }
        else
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
    recoverListAssociation();
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
    recoverListAssociation();
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

