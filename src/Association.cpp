/* 
 * File:   Association.cpp
 * Author: gkr
 * 
 * Created on 13 décembre 2013, 12:04
 */

#include "Association.h"

using namespace std;

Association::Association(Echoes::Dbo::Session *session, string apiUrl)
: CreatePageWidget("association")
{
    session_ = session;
    apiUrl_ = apiUrl;
    created_ = false;
    newClass_ = false;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);
}

// TABLE(S) FOR MAIN PAGE -------------------------------------

void Association::update()
{
    CreatePageWidget::update();
    if (!newClass_)
    {
        newClass_ = true;
    }
}

// TABLE(S) FOR POPUP ------------------------------------------

void Association::popupAddWidget(Wt::WDialog *dialog, bool typeDial)
{
    idsInformations_.clear();
    idsInformations_.push_back((*informations_.begin()).second.first);
    idPlugin_ = (*plugins_.begin()).second.first;
    idHost_ = (*assets_.begin()).second.first;
    dialog->contents()->clear();
    //    dialog = new Wt::WDialog();
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
        for (int i = 1 ; i <= (*itF).second.nbValue ; i++)
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
    titleText.push_back("plugin");
    titleText.push_back("asset");
    titleText.push_back("information");
    return titleText;
}

vector<long long> Association::getIdsTable()
{
    vector<long long> ids;

    ids.push_back(1);
    ids.push_back(2);
    ids.push_back(3);

    //    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    //    Wt::Json::Object tmp;
    //    Wt::Json::Array::const_iterator idx1;
    //
    //    if (result_.isNull() == false)
    //    {
    //    result1 = result_;
    //    for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
    //    {
    //        tmp = (*idx1);
    //        ids.push_back(tmp.get("id"));
    //    }
    //    }
    return ids;
}

vector_type Association::getResourceRowTable(long long id)
{
    vector_type rowTable;

    // Mettre en place quand les infos sur les liens Asset - Plugin - Information sont récupérer.
    /*
    Recherche les informations lié a un asset et un plugin.
            Mettre en place les informations de la bonne manière
            string = <ul>
                <il> info </il>
            </ul>
     */
    
    //    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    //    Wt::Json::Object tmp;
    //    Wt::Json::Array::const_iterator idx1;
    //    Wt::WFileResource *file = 0;
    //    Wt::WString nameAsset = "";
    //    long long i(0); 
    //
    //    if (result_.isNull() == false)
    //    {
    //        result1 = result_;
    //    for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
    //    {
    //        //gkr: Add name Asset for row, and add id for sort.
    //        tmp = (*idx1);
    //        nameAsset = tmp.get("name");
    //        i = tmp.get("id");
    //        if (i == id)
    //        {
    //            rowTable.push_back(new Wt::WText(boost::lexical_cast<string, Wt::WString>(nameAsset)));
    //
    //            //gkr: Add button for row.
    //            file = generateScript(boost::lexical_cast<long long>(i), nameAsset);
    //            Wt::WAnchor *downloadButton = new Wt::WAnchor(file, "");
    //            downloadButton->setAttributeValue("class","btn btn-info");
    //            downloadButton->setTextFormat(Wt::XHTMLUnsafeText);
    //            downloadButton->setText("<span class='input-group-btn'><i class='icon-download icon-white'></i></span>");
    //            downloadButton->clicked().connect(boost::bind(&Association::downloadScript, this, file->fileName()));
    //            rowTable.push_back(downloadButton);
    //            return (rowTable);
    //        }
    //    }
    //    }
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
    Wt::WRegExpValidator *validator =
            new Wt::WRegExpValidator(REG_EXP);
    validator->setMandatory(true);
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


    /* Connection API */
    string apiAddress = this->getApiUrl() + "/assets"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "Association : [GET] address to call : " << apiAddress;
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&Association::getAssets, this, _1, _2));
    if (client->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }

    apiAddress = this->getApiUrl() + "/plugins"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "Association : [GET] address to call : " << apiAddress;
    Wt::Http::Client *client1 = new Wt::Http::Client(this);
    client1->done().connect(boost::bind(&Association::getPlugins, this, _1, _2));
    if (client1->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }

    apiAddress = this->getApiUrl() + "/informations"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "Association : [GET] address to call : " << apiAddress;
    Wt::Http::Client *client2 = new Wt::Http::Client(this);
    client2->done().connect(boost::bind(&Association::getInformations, this, _1, _2));
    if (client2->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }

    apiAddress = this->getApiUrl() + "/filters"
            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::log("debug") << "Association : [GET] address to call : " << apiAddress;
    Wt::Http::Client *client3 = new Wt::Http::Client(this);
    client3->done().connect(boost::bind(&Association::getFilters, this, _1, _2));
    if (client3->get(apiAddress))
    {
        Wt::WApplication::instance()->deferRendering();
    }
    else
    {
        Wt::log("error") << "Error Client Http";
    }
}

// Call API - POST(ADD) DELETE PUT(MODIF) ----------------------------------------

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
    std::cout << "Reponse : " << std::endl << response.body() << std::endl;
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 201)
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
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title"), tr("Alert.alert.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Associations Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "TypeException", tr("Alert.alert.database-error"), Wt::Ok);
            }
        }
        else
        {
            Wt::log("error") << "[Associations Widget] " << response.body();
            Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "status", tr("Alert.alert.database-error"), Wt::Ok);
        }
    }
    else
    {
        Wt::log("error") << "[Associations Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.asset.database-error-title") + "err", tr("Alert.alert.database-error"), Wt::Ok);
    }
    
}

Wt::WDialog *Association::deleteResource(long long id)
{
    Wt::WDialog *box = CreatePageWidget::deleteResource(id);
    // a REVOIR !! Récupération des alerts par rapport a id de l'asset a sup
    //    string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<string> (id) + "/alerts/";
    //    Wt::Http::Client *client = new Wt::Http::Client(this);
    //    client->done().connect(boost::bind(&Association::checkAlertsInAsset, this, _1, _2, box, id));
    //    apiAddress += "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
    //    Wt::log("debug") << "Association : [GET] address to call : " << apiAddress;
    //    if (client->get(apiAddress))
    //        Wt::WApplication::instance()->deferRendering();
    //    else
    //        Wt::log("error") << "Error Client Http";
    box->show();
    box->finished().connect(bind([ = ] (){
                                      if (box->result() == Wt::WDialog::Accepted)
        {
                                      //            Wt::Http::Message message;
                                      //            message.addBodyText("");
                                      //            string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<string> (id)
                                      //                    + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) + "&token=" + session_->user()->token.toUTF8();
                                      //
                                      //            Wt::Http::Client *client = new Wt::Http::Client(this);
                                      //            client->done().connect(boost::bind(&Association::deleteAsset, this, _1, _2));
                                      //            Wt::log("debug") << "Association : [DELETE] address to call : " << apiAddress;
                                      //            if (client->deleteRequest(apiAddress, message))
                                      //                Wt::WApplication::instance()->deferRendering();
                                      //            else
                                      //                Wt::log("error") << "Error Client Http";
        }
                                      return box;
    }));
    return box;
}

void Association::modifResource(vector<Wt::WInteractWidget*> arguments, long long id)
{
    //    string messageString;
    //
    //    messageString = "{\n\"name\":\"" + boost::lexical_cast<string>(((Wt::WLineEdit*)(*arguments.begin()))->text()) + "\"\n}";
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
}

void Association::close()
{
    delete this;
}


// API RETURN INFOS ------------------------------------------

void Association::getAssets(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::Object tmp;
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                Wt::Json::Array& result1 = result;
                long long id;
                Wt::WString name;
                long long cpt(0);
                assetsModel = new Wt::WStandardItemModel(0,2,this);
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);
                    name = tmp.get("name");
                    id = tmp.get("id");


                    Wt::WStandardItem *itemId = new Wt::WStandardItem();
                    Wt::WStandardItem *itemName = new Wt::WStandardItem();
                    itemId->setText(boost::lexical_cast<string>(id));
                    itemName->setText(name);
                    vector<Wt::WStandardItem*> rowVector;
                    rowVector.push_back(itemId);
                    rowVector.push_back(itemName);
                    assetsModel->insertRow(cpt,rowVector);

                    assets_[cpt] = make_pair(id, name.toUTF8());
                    cpt++;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Associations Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Associations Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Associations Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
    }
    newClass_ = false;
    created_ = false;
}

void Association::getPlugins(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::Object tmp;
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                Wt::Json::Array& result1 = result;
                long long id;
                Wt::WString name;
                long long cpt(0);
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);
                    name = tmp.get("name");
                    id = tmp.get("id");
                    plugins_[cpt] = make_pair(id, name.toUTF8());
                    cpt++;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Associations Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Associations Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Associations Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
    }
    newClass_ = false;
    created_ = false;
}

void Association::getFilters(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::Object tmp;
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                Wt::Json::Array& result1 = result;
                long long id;
                long long cpt(0);
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);
                    id = tmp.get("id");
                    
                    string apiAddress;
                    apiAddress = this->getApiUrl() + "/filters/"
                            + boost::lexical_cast<string>(id)
                            + "/parameters"
                            + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
                            + "&token=" + session_->user()->token.toUTF8();
                    Wt::log("debug") << "Association : [GET] address to call : " << apiAddress;
                    Wt::Http::Client *client = new Wt::Http::Client(this);
                    client->done().connect(boost::bind(&Association::getFilterParameterValues, this, _1, _2,id,tmp));
                    if (client->get(apiAddress))
                    {
                        Wt::WApplication::instance()->deferRendering();
                    }
                    else
                    {
                        Wt::log("error") << "Error Client Http";
                    }
                    
                    cpt++;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Associations Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Associations Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Associations Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
    }
    newClass_ = false;
    created_ = false;
}

void Association::getFilterParameterValues(boost::system::error_code err, const Wt::Http::Message& response, long long filterId, Wt::Json::Object filJson)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::Object tmp;
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                Wt::Json::Array& result1 = result;

//                long long cpt(0);
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);

                    Wt::Json::Object tmpFilterType = filJson.get("filter_type");
                    Wt::Json::Object tmpSearch = filJson.get("search");
        
                    filterParameterValues_[filterId] = {
                        filJson.get("id"),
                        tmpFilterType.get("id"),
                        filJson.get("nb_value"),
                        tmpSearch.get("id"),
                        tmp.get("value")
                    };
                    
                    break;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Associations Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Associations Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Associations Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
    }
    newClass_ = false;
    created_ = false;
}

void Association::getInformations(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if (response.status() == 200)
        {
            try
            {
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::Object tmp;
                Wt::Json::Value result;
                Wt::Json::parse(response.body(), result);
                Wt::Json::Array& result1 = result;
                long long id;
                Wt::WString name;
                int informationDatas;
                long long idUnit;
                long long cpt(0);
                informationsModel = new Wt::WStandardItemModel(0,3,this);
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);
                    name = tmp.get("name");
                    id = tmp.get("id");
                    Wt::Json::Object infoUnit;
                    infoUnit = tmp.get("information_unit");
                    idUnit = infoUnit.get("id");
                    informationDatas = tmp.get("information_datas");
                    
                    if (informationDatas == 0)
                    {
                        Wt::WStandardItem *itemId = new Wt::WStandardItem();
                        Wt::WStandardItem *itemName = new Wt::WStandardItem();
                        Wt::WStandardItem *itemUnitId = new Wt::WStandardItem();
                        itemId->setText(boost::lexical_cast<string>(id));
                        itemName->setText(name);
                        itemUnitId->setText(boost::lexical_cast<string>(idUnit));
                        vector<Wt::WStandardItem*> rowVector;
                        rowVector.push_back(itemId);
                        rowVector.push_back(itemName);
                        rowVector.push_back(itemUnitId);
                        informationsModel->insertRow(cpt,rowVector);


                        informations_[cpt] = make_pair(id, name.toUTF8());
                        cpt++;
                    }
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Associations Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Associations Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Associations Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.association.database-error-title"), tr("Alert.association.database-error"), Wt::Ok);
    }
    newClass_ = false;
    created_ = false;
    update();
}

void Association::setSession(Echoes::Dbo::Session *session)
{
    session_ = session;
}

void Association::setApiUrl(string apiUrl)
{
    apiUrl_ = apiUrl;
}

string Association::getApiUrl()
{
    return apiUrl_;
}
