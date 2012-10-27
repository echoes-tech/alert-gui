/* 
 * File:   AlertEditionWidget.cpp
 * Author: tsa
 * 
 * Created on 14 août 2012, 11:50
 */

#include "AlertEditionWidget.h"

AlertEditionWidget::AlertEditionWidget()
: Wt::WTemplateFormView(Wt::WString::tr("Alert.alert.Edition.template"))
{
    created_ = false;
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("alert",false);
}

void AlertEditionWidget::setModel(AlertEditionModel *model)
{
    model_ = model;
}

void AlertEditionWidget::setSession(Session *session)
{
    this->session = session;
    this->userId = session->user().id();
}

void AlertEditionWidget::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
        update();
        created_ = true;
    }

    WTemplate::render(flags);
}

Wt::WFormWidget *AlertEditionWidget::createFormWidget(Wt::WFormModel::Field field)
{
    Wt::WFormWidget *result = 0;

    if (field == AlertEditionModel::ThresholdOperator)
    {
        comboAlertCrit = new Wt::WComboBox();
        Wt::WStringListModel *slmOperators = new Wt::WStringListModel();
        
        
        
        Wt::Dbo::collection<Wt::Dbo::ptr<AlertCriteria> > alertCriterias;

        // mediaValues list
        {
            Wt::Dbo::Transaction transaction(*session);
            alertCriterias = session->find<AlertCriteria>();
            int idx = 0;
            Wt::WString tempString;
            for (Wt::Dbo::collection<Wt::Dbo::ptr<AlertCriteria> >::const_iterator i = alertCriterias.begin(); i != alertCriterias.end(); ++i)
            {
                tempString = "Alert.alert.operator." + (*i).get()->name;
                slmOperators->insertString(idx,Wt::WString::tr(tempString.toUTF8()));
                long long idAlertCriteria = (*i).id();
                this->mapAlertCriteriaIdSboxRow[idx] = idAlertCriteria;
                idx++;
            }
        }
        comboAlertCrit->setModel(slmOperators);
        
        result = comboAlertCrit;
        result->changed().connect(boost::bind(&AlertEditionWidget::checkThresholdOperator, this));
    }
    else if (field == AlertEditionModel::ThresholdValue)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&AlertEditionWidget::checkThresholdValue, this));
    }
    else if (field == AlertEditionModel::Snooze)
    {
        result = new Wt::WLineEdit();
        result->changed().connect(boost::bind(&AlertEditionWidget::checkSnoozeValue, this));

    }

    return result;
}

void AlertEditionWidget::checkThresholdOperator()
{
    updateModelField(model_, AlertEditionModel::ThresholdOperator);
    model_->validateField(AlertEditionModel::ThresholdOperator);
    model_->setValidated(AlertEditionModel::ThresholdOperator, false);
    update();
}

void AlertEditionWidget::checkThresholdValue()
{
    updateModelField(model_, AlertEditionModel::ThresholdValue);
    model_->validateField(AlertEditionModel::ThresholdValue);
    model_->setValidated(AlertEditionModel::ThresholdValue, false);
    update();
}

void AlertEditionWidget::checkSnoozeValue()
{
    updateModelField(model_, AlertEditionModel::ThresholdValue);
    updateModelField(model_, AlertEditionModel::ThresholdOperator);
    updateModelField(model_, AlertEditionModel::Snooze);
    model_->validateField(AlertEditionModel::Snooze);
    model_->setValidated(AlertEditionModel::Snooze, false);
    update();
}

void AlertEditionWidget::update()
{

    updateView(model_);

    if (!created_)
    {
        
        // alert definition
        serverSelectionBox = new Wt::WSelectionBox();
        applicationSelectionBox = new Wt::WSelectionBox();
        informationSelectionBox = new Wt::WSelectionBox();
        
        //server list
        bindWidget("server-sbox", serverSelectionBox);
        this->updateServerSelectionBox(this->userId);
        serverSelectionBox->clicked().connect(boost::bind(&AlertEditionWidget::updateApplicationSBFromServerSB, this));
        
        // applications list
        bindWidget("application-sbox", applicationSelectionBox);
        applicationSelectionBox->clicked().connect(boost::bind(&AlertEditionWidget::updateInformationSBFromApplicationSB, this));
        
        // information list
        bindWidget("information-sbox", informationSelectionBox);
        
        // \alert definition
        
        // user receiving definition
        
        userSelectionBox = new Wt::WSelectionBox();
        mediaSelectionBox = new Wt::WSelectionBox();
        mediaValueSelectionBox = new Wt::WSelectionBox();
        
        //user list
        bindWidget("user-sbox", userSelectionBox);
        this->updateUserSelectionBox(this->userId);
        userSelectionBox->clicked().connect(boost::bind(&AlertEditionWidget::updateMediaSBFromUserSB, this));
        
        //media list
        bindWidget("media-sbox", mediaSelectionBox);
        mediaSelectionBox->clicked().connect(boost::bind(&AlertEditionWidget::updateMediaValueSBFromMediaSB, this));
//        
//        // information list
        bindWidget("media-value-sbox", mediaValueSelectionBox);
        
        
        Wt::WPushButton *addButtonMedia = new Wt::WPushButton(tr("Alert.alert.add-media-button"));
        Wt::WPushButton *deleteButtonMedia = new Wt::WPushButton(tr("Alert.alert.delete-media-button"));

        bindWidget("add-button-media", addButtonMedia);
        bindWidget("delete-button-media", deleteButtonMedia);

//        dynamic_cast<Wt::WStringListModel>(userSelectionBox->model())->
        
        
        Wt::WStandardItemModel *sim = new Wt::WStandardItemModel(0, 4, this);
        const std::string userTitle = Wt::WString::tr("Alert.alert.user").toUTF8();
        const std::string mediaTitle = Wt::WString::tr("Alert.alert.media").toUTF8();
        const std::string valueTitle = Wt::WString::tr("Alert.alert.media-value").toUTF8();
        const std::string snoozeTitle = Wt::WString::tr("Alert.alert.snooze").toUTF8();
        sim->setHeaderData(0,boost::any(userTitle));
        sim->setHeaderData(1,boost::any(mediaTitle));
        sim->setHeaderData(2,boost::any(valueTitle));
        sim->setHeaderData(3,boost::any(snoozeTitle));
        
        userMediaDestinationTableView = new Wt::WTableView();
        userMediaDestinationTableView->setModel(sim);
        userMediaDestinationTableView->setSelectionBehavior(Wt::SelectRows);
        userMediaDestinationTableView->setSelectionMode(Wt::SingleSelection);
        
        bindWidget("user-media-destination", userMediaDestinationTableView);
        
        
        addButtonMedia->clicked().connect(this, &AlertEditionWidget::addMedia);
        deleteButtonMedia->clicked().connect(this, &AlertEditionWidget::deleteMedia);
        
        Wt::WPushButton *createAlertButton = new Wt::WPushButton(tr("Alert.alert.edition.add-button"));
        bindWidget("create-alert-button", createAlertButton);
        createAlertButton->clicked().connect(this,&AlertEditionWidget::addAlert);
        
        
        created_ = true;
    }
}

void AlertEditionWidget::updateMediaSBFromUserSB()
{
    if (userSelectionBox->currentIndex() != -1)
    {
        this->updateMediaSelectionBox(mapUserIdSboxRow[userSelectionBox->currentIndex()]);
    }
}

void AlertEditionWidget::updateMediaValueSBFromMediaSB()
{
    if (mediaSelectionBox->currentIndex() != -1)
    {
        this->updateMediaValueSelectionBox(mapUserIdSboxRow[userSelectionBox->currentIndex()],mapMediaIdSboxRow[mediaSelectionBox->currentIndex()]);
    }
}

void AlertEditionWidget::updateApplicationSBFromServerSB()
{
    if (serverSelectionBox->currentIndex() != -1)
    {
        this->updateApplicationSelectionBox(mapAssetIdSboxRow[serverSelectionBox->currentIndex()]);
    }
}

void AlertEditionWidget::updateInformationSBFromApplicationSB()
{
    if (applicationSelectionBox->currentIndex() != -1)
    {
        this->updateInformationSelectionBox(mapPluginIdSboxRow[applicationSelectionBox->currentIndex()]);
    }
}

void AlertEditionWidget::updateMediaValueSelectionBox(int userId,int medId)
{
    Wt::WStringListModel *slmMediaValue = new Wt::WStringListModel;
    Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> > mediaValues;
    Wt::Dbo::Query<Wt::Dbo::ptr<MediaValue> > queryMediaValue;
    std::string queryString = "SELECT mev FROM \"T_MEDIA_VALUE_MEV\" mev "
                                "WHERE \"MEV_MED_MED_ID\" = ? "
                                "AND \"MEV_USR_USR_ID\" = ?";
    
    // mediaValues list
    {
        Wt::Dbo::Transaction transaction(*session);
        queryMediaValue = session->query<Wt::Dbo::ptr<MediaValue> >(queryString).bind(medId).bind(userId);
        mediaValues = queryMediaValue.resultList();
        int idx = 0;
        
        for (Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> >::const_iterator i = mediaValues.begin(); i != mediaValues.end(); ++i)
        {
            slmMediaValue->insertString(idx,(*i).get()->value);
            long long idMediaValue = (*i).id();
            this->mapMediaValueIdSboxRow[idx] = idMediaValue;
            idx++;
        }
    }
    mediaValueSelectionBox->setModel(slmMediaValue);
    mediaValueSelectionBox->refresh();                            
}

void AlertEditionWidget::updateMediaSelectionBox(int userId)
{
    Wt::WStringListModel *slmMedia = new Wt::WStringListModel;
    Wt::Dbo::collection<Wt::Dbo::ptr<Media> > medias;
    Wt::Dbo::Query<Wt::Dbo::ptr<Media> > queryMedia;
    std::string queryString = "SELECT med FROM \"T_MEDIA_MED\" med "
                                "WHERE \"MED_ID\" IN "
                                "("
                                    "SELECT \"MEV_MED_MED_ID\" FROM \"T_MEDIA_VALUE_MEV\" "
                                    "WHERE \"MEV_USR_USR_ID\" = ?"
                                ")";
    // medias list
    {
        Wt::Dbo::Transaction transaction(*session);
        queryMedia = session->query<Wt::Dbo::ptr<Media> >(queryString).bind(userId);
        medias = queryMedia.resultList();
        
        int idx = 0;
        for (Wt::Dbo::collection<Wt::Dbo::ptr<Media> >::const_iterator i = medias.begin(); i != medias.end(); ++i)
        {
            slmMedia->insertString(idx,(*i).get()->name);
            long long idMedia = (*i).id();
            this->mapMediaIdSboxRow[idx] = idMedia;
            idx++;
        }
        
        
    }
                                
    mediaSelectionBox->setModel(slmMedia);
    mediaSelectionBox->refresh();
    
}

void AlertEditionWidget::updateUserSelectionBox(int userId)
{
    Wt::WStringListModel *slmUser = new Wt::WStringListModel;
        Wt::Dbo::collection<Wt::Dbo::ptr<User> > users;
        Wt::Dbo::Query<Wt::Dbo::ptr<User> > queryUser;
        std::string queryString =  "SELECT usr FROM \"T_USER_USR\" usr where \"USR_ID\" IN"
            "("
                "SELECT \"T_USER_USR_USR_ID\" FROM \"TJ_USR_ORG\" where \"T_ORGANIZATION_ORG_ORG_ID\" IN "
                "("
                    "select \"T_ORGANIZATION_ORG_ORG_ID\" from \"TJ_USR_ORG\" where \"T_USER_USR_USR_ID\" = ?"
                ")"
            ");";
        
        // hosts list
        {
            Wt::Dbo::Transaction transaction(*session);
            
            
            queryUser = session->query<Wt::Dbo::ptr<User> >(queryString).bind(userId);
            users = queryUser.resultList();
            
            int idx = 0;
            for (Wt::Dbo::collection<Wt::Dbo::ptr<User> >::const_iterator i = users.begin(); i != users.end(); ++i)
            {
                slmUser->insertString(idx,(*i).get()->firstName + " " + (*i).get()->lastName);
                long long idUser = (*i).id();
                this->mapUserIdSboxRow[idx] = idUser;
                idx++;
            }
        }
        userSelectionBox->setModel(slmUser);
}


void AlertEditionWidget::updateServerSelectionBox(int serverId)
{
    Wt::WStringListModel *slmServer = new Wt::WStringListModel;
        Wt::Dbo::collection<Wt::Dbo::ptr<Asset> > assets;
        Wt::Dbo::Query<Wt::Dbo::ptr<Asset> > queryAsset;
        std::string queryString =  "SELECT ast FROM \"T_ASSET_AST\" ast where \"AST_PRB_PRB_ID\" IN"
            "("
                "SELECT \"PRB_ID\" FROM \"T_PROBE_PRB\" where \"PRB_ORG_ORG_ID\" IN "
                "("
                    "select \"T_ORGANIZATION_ORG_ORG_ID\" from \"TJ_USR_ORG\" where \"T_USER_USR_USR_ID\" = ?"
                ")"
            ") "
            " AND \"AST_DELETE\" IS NULL;";
        
        // hosts list
        {
            Wt::Dbo::Transaction transaction(*session);
            
            
            queryAsset = session->query<Wt::Dbo::ptr<Asset> >(queryString).bind(serverId);
            assets = queryAsset.resultList();
            
            int idx = 0;
            for (Wt::Dbo::collection<Wt::Dbo::ptr<Asset> >::const_iterator i = assets.begin(); i != assets.end(); ++i)
            {
                Wt::log("info") << (*i).get()->name;
                slmServer->insertString(idx,(*i)->name);
                long long idAsset = (*i).id();
                this->mapAssetIdSboxRow[idx] = idAsset;
                idx++;
            }
        }
        serverSelectionBox->setModel(slmServer);
}

void AlertEditionWidget::updateInformationSelectionBox(int pluginId)
{
    Wt::log("info") << "[Plg Id] " << pluginId;
        Wt::Dbo::Query<Wt::Dbo::ptr<Information2> > queryInformation;
        std::string queryStringInfos = "SELECT inf FROM \"T_INFORMATION_INF\" inf WHERE \"PLG_ID_PLG_ID\" = ? AND \"SRC_ID\" IN (1,3)";
        Wt::Dbo::collection<Wt::Dbo::ptr<Information2> > infos;
        Wt::WStringListModel *slmInformation = new Wt::WStringListModel;
        {
            Wt::Dbo::Transaction transaction(*session);
            queryInformation = session->query<Wt::Dbo::ptr<Information2> >(queryStringInfos).bind(pluginId).orderBy("\"INF_NAME\"");//.bind(session->user().id());
            infos = queryInformation.resultList();
           
        
            int idx = 0;
            for (Wt::Dbo::collection<Wt::Dbo::ptr<Information2> >::const_iterator k = infos.begin(); k != infos.end(); k++)
            {
                slmInformation->insertString(idx,k->get()->name);
                InformationId infId(k->get()->pk.search, k->get()->pk.subSearchNumber, k->get()->pk.unit);
                
                this->mapInformationIdSboxRow[idx] = infId;
                idx++;
            }
        
        }
        informationSelectionBox->setModel(slmInformation);
}

void AlertEditionWidget::updateApplicationSelectionBox(int astId)
{
    Wt::WStringListModel *slmApplication = new Wt::WStringListModel;
    Wt::Dbo::collection<Wt::Dbo::ptr<Asset> > assets;
    {
        Wt::Dbo::Transaction transaction(*session);
        assets = session->find<Asset>().where("\"AST_ID\" = ?").bind(astId).where("\"AST_DELETE\" IS NULL");


        int idx = 0;
        long long idPlugin;
        for (Wt::Dbo::collection<Wt::Dbo::ptr<Asset> >::const_iterator i = assets.begin(); i != assets.end(); ++i)
        {
            Wt::Dbo::collection<Wt::Dbo::ptr<Plugin> > plugins = (*i).get()->plugins;
            for (Wt::Dbo::collection<Wt::Dbo::ptr<Plugin> >::const_iterator j = plugins.begin(); j != plugins.end(); ++j)
            {
                Wt::log("info") << "plugin : " << (*j).get()->name;
                slmApplication->insertString(idx,(*j).get()->name);
                //CHECK: idPlugin i ?? TBCommented
                idPlugin = (*j).id();
                this->mapPluginIdSboxRow[idx] = idPlugin;
                idx++;
            }
        }
    }
    applicationSelectionBox->setModel(slmApplication);
    applicationSelectionBox->refresh();
    
}

Wt::WStringListModel *AlertEditionWidget::getMediasForCurrentUser(int mediaType)
{
    Wt::WStringListModel *res = new Wt::WStringListModel();
        
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> > medias = session->find<MediaValue>().where("\"MEV_USR_USR_ID\" = ?").bind(model_->user->self().id())
                                                                                            .where("\"MEV_MED_MED_ID\" = ?").bind(mediaType);
        int idx = 0;
        for (Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> >::const_iterator i = medias.begin(); i != medias.end(); ++i)
        {
            res->insertString(idx,(*i)->value);
            idx++;
        }
    }
    return res;
}


void AlertEditionWidget::addMedia()
{
    if ((userSelectionBox->currentIndex() == -1) || (mediaSelectionBox->currentIndex() == -1) || (mediaValueSelectionBox->currentIndex() == -1))
    {
        Wt::WMessageBox::show(tr("Alert.alert.media-selection-missing-title"),tr("Alert.alert.media-selection-missing"),Wt::Ok);
        return;
    }
    checkSnoozeValue();
    if (!(model_->validateField(AlertEditionModel::Snooze)))
    {
        Wt::log("info") << "Snooze value incorrect";
        return;
    }
    
//    long long userMediaIdToRemember = mapAssetIdSboxRow[userSelectionBox->currentIndex()];
    const Wt::WString userMediaTextToDisplay = userSelectionBox->currentText();
    Wt::WStandardItem *itemUserMedia = new Wt::WStandardItem();
    itemUserMedia->setData(boost::any(userMediaTextToDisplay),0);
    
//    long long mediaIdToRemember = mapAssetIdSboxRow[mediaSelectionBox->currentIndex()];
    const Wt::WString mediaTextToDisplay = mediaSelectionBox->currentText();
    Wt::WStandardItem *itemMedia = new Wt::WStandardItem();
    itemMedia->setData(boost::any(mediaTextToDisplay),0);
    
//    long long mediaValueIdToRemember = mapAssetIdSboxRow[userSelectionBox->currentIndex()];
    const Wt::WString mediaValueTextToDisplay = mediaValueSelectionBox->currentText();
    Wt::WStandardItem *itemMediaValue = new Wt::WStandardItem();
    itemMediaValue->setData(boost::any(mediaValueTextToDisplay),0);
    
    updateModelField(model_,AlertEditionModel::Snooze);
    const Wt::WString snoozeValueToDisplay = model_->valueText(model_->Snooze);
    Wt::WStandardItem *itemSnooze = new Wt::WStandardItem();
    itemSnooze->setData(boost::any(snoozeValueToDisplay),0);
    
    // vector of WStandardItem to create the row
    std::vector<Wt::WStandardItem*> *vectItem = new std::vector<Wt::WStandardItem*>;
    vectItem->push_back(itemUserMedia);
    vectItem->push_back(itemMedia);
    vectItem->push_back(itemMediaValue);
    vectItem->push_back(itemSnooze);
    
    // const to pass it to appendRow
    const std::vector<Wt::WStandardItem*> *vectItemFilled = vectItem;
    dynamic_cast<Wt::WStandardItemModel*>(userMediaDestinationTableView->model())->appendRow(*vectItemFilled);
    
    AlertMediaSpecialization *ams = new AlertMediaSpecialization();
    Wt::Dbo::ptr<AlertMediaSpecialization> amsPtr;
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::ptr<MediaValue> mevPtr = session->find<MediaValue>().where("\"MEV_ID\" = ?").bind(mapMediaValueIdSboxRow[mediaValueSelectionBox->currentIndex()]);
        ams->snoozeDuration = boost::lexical_cast<int,Wt::WString>(snoozeValueToDisplay);
        ams->mediaValue = mevPtr;
        ams->notifEndOfAlert = false;
        amsPtr = session->add<AlertMediaSpecialization>(ams);
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return;
    }
    const Wt::WStandardItem *constItemUserMedia = itemUserMedia;
    mapAlertMediaSpecializationIdTableView[
            dynamic_cast<Wt::WStandardItemModel*>(userMediaDestinationTableView->model())
                ->indexFromItem
                (
                    constItemUserMedia
                ).row()
            ] = amsPtr.id(); 
    UserActionManagement::registerUserAction(Enums::add,Constants::T_ALERT_MEDIA_SPECIALIZATION_AMS,amsPtr.id());
}

void AlertEditionWidget::deleteMedia()
{
    if (userMediaDestinationTableView->selectedIndexes().size() > 0)
    {
        int row = (*(userMediaDestinationTableView->selectedIndexes().begin())).row();
        const Wt::WModelIndex index = userMediaDestinationTableView->rootIndex();
        
        try
        {
            Wt::Dbo::Transaction transaction(*session);
            Wt::Dbo::ptr<AlertMediaSpecialization> amsPtr = session->find<AlertMediaSpecialization>()
                                    .where("\"AMS_ID\" = ?").bind(mapAlertMediaSpecializationIdTableView[row]);
            amsPtr.remove();
            dynamic_cast<Wt::WStandardItemModel*>(userMediaDestinationTableView->model())->removeRow(row,index);
            transaction.commit();
            UserActionManagement::registerUserAction(Enums::del,Constants::T_ALERT_MEDIA_SPECIALIZATION_AMS,mapAlertMediaSpecializationIdTableView[row]);
        }
        catch (Wt::Dbo::Exception e)
        {
            Wt::log("error") << "[AlertEditionWidget] " << e.what();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            return;
        }
    }
}





bool AlertEditionWidget::validate()
{
    return model_->validate();
    update();
}


void AlertEditionWidget::addAlert()
{
    if (((serverSelectionBox->currentIndex() == -1) || (applicationSelectionBox->currentIndex() == -1) || (informationSelectionBox->currentIndex() == -1)))
    {
        Wt::WMessageBox::show(tr("Alert.alert.information-missing-title"),tr("Alert.alert.information-missing"),Wt::Ok);
        return;
    }
    
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        
        
        Wt::Dbo::collection<Wt::Dbo::ptr<AlertValue> > avaPtrCollec = session->find<AlertValue>().where("\"SEA_ID\" = ?")
                                                        .bind(mapInformationIdSboxRow[informationSelectionBox->currentIndex()].search.get()->pk.id)
                                                        .where("\"SRC_ID\" = ?")
                                                        .bind(mapInformationIdSboxRow[informationSelectionBox->currentIndex()].search.get()->pk.source.get()->pk.id)
                                                        .where("\"PLG_ID_PLG_ID\" = ?")
                                                        .bind(mapInformationIdSboxRow[informationSelectionBox->currentIndex()].search.get()->pk.source.get()->pk.plugin.id())
                                                        .where("\"INF_VALUE_NUM\" = ?")
                                                        .bind(mapInformationIdSboxRow[informationSelectionBox->currentIndex()].subSearchNumber);
        
        if (avaPtrCollec.size() > 0)
        {
            std::string inString = "(";
            for (Wt::Dbo::collection<Wt::Dbo::ptr<AlertValue> >::const_iterator i = avaPtrCollec.begin(); i != avaPtrCollec.end(); i++) 
            {
                Wt::log("debug") << " [Class:AlertProcessor] " << " - " << " For iva list : " << (*i).id();
                inString += boost::lexical_cast<std::string,long long>((*i).id()) + ",";
                i->flush();
            }
            inString.replace(inString.size()-1, 1, "");
            inString += ")";
            
            
            std::string queryStr = "SELECT ale FROM \"T_ALERT_ALE\" ale WHERE "
                                    " \"ALE_ID\" IN"
                                    "("
                                        "SELECT \"T_ALERT_ALE_ALE_ID\" FROM \"TJ_AST_ALE\" WHERE \"T_ASSET_AST_AST_ID\" = " 
                                        + boost::lexical_cast<std::string>(mapAssetIdSboxRow[serverSelectionBox->currentIndex()]) +
                                    ")"
                                    "AND ale.\"ALE_DELETE\" IS NULL "
                                    "AND \"ALE_AVA_AVA_ID\" IN" + inString;
            
            Wt::Dbo::Query<Wt::Dbo::ptr<Alert> > queryRes = session->query<Wt::Dbo::ptr<Alert> >(queryStr);
            
            Wt::Dbo::collection<Wt::Dbo::ptr<Alert> > alerts = queryRes.resultList();
            
            if (alerts.size() > 0)
            {
                Wt::log("info") << "[AlertEditionWidget] " << "alert exists";
                Wt::WMessageBox::show(tr("Alert.alert.alert-already-exists-title"),tr("Alert.alert.alert-already-exists"),Wt::Ok);
                return;
            }
            
        }
        
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return;
    }
    
    
    if (userMediaDestinationTableView->model()->rowCount() < 1)
    {
        Wt::WMessageBox::show(tr("Alert.alert.media-missing-title"),tr("Alert.alert.media-missing"),Wt::Ok);
        return;
    }
    
    if (!validate())
    {
        checkThresholdOperator();
        checkThresholdValue();
        return;
    }
    
    Alert *alert = new Alert();
    AlertValue *ava = new AlertValue();
    Wt::WString name = serverSelectionBox->currentText() + " - " + applicationSelectionBox->currentText() + " - " + informationSelectionBox->currentText();
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        
        
        Wt::Dbo::ptr<Information2> infoPtr = session->find<Information2>().where("\"SEA_ID\" = ?")
                                                        .bind(mapInformationIdSboxRow[informationSelectionBox->currentIndex()].search.get()->pk.id)
                                                        .where("\"SRC_ID\" = ?")
                                                        .bind(mapInformationIdSboxRow[informationSelectionBox->currentIndex()].search.get()->pk.source.get()->pk.id)
                                                        .where("\"PLG_ID_PLG_ID\" = ?")
                                                        .bind(mapInformationIdSboxRow[informationSelectionBox->currentIndex()].search.get()->pk.source.get()->pk.plugin.id())
                                                        .where("\"INF_VALUE_NUM\" = ?")
                                                        .bind(mapInformationIdSboxRow[informationSelectionBox->currentIndex()].subSearchNumber);
//        
        Wt::Dbo::ptr<AlertCriteria> critPtr = session->find<AlertCriteria>().where("\"ACR_ID\" = ?").bind(mapAlertCriteriaIdSboxRow[comboAlertCrit->currentIndex()]);
        ava->information = infoPtr;
        updateModelField(model_,AlertEditionModel::ThresholdValue);
        ava->value = model_->valueText(model_->ThresholdValue);
        ava->alertCriteria = critPtr;
        
        Wt::log("info") << critPtr.get()->name;
        Wt::Dbo::ptr<AlertValue> avaPtr = session->add<AlertValue>(ava);   
        
        alert->alertValue = avaPtr;
        alert->threadSleep = 0;
        alert->creaDate = Wt::WDateTime::currentDateTime();
        alert->name = name;

        //asset
        Wt::Dbo::ptr<Asset> assetPtr = session->find<Asset>().where("\"AST_ID\" = ?").bind(mapAssetIdSboxRow[serverSelectionBox->currentIndex()]);
        
        
        Wt::Dbo::ptr<Alert> alePtr = session->add<Alert>(alert);
        
        alePtr.modify()->assets.insert(assetPtr);
        
        for (std::map<int,long long>::const_iterator i = mapAlertMediaSpecializationIdTableView.begin(); i != mapAlertMediaSpecializationIdTableView.end(); ++i)
        {
            Wt::Dbo::ptr<AlertMediaSpecialization> amsPtr = session->find<AlertMediaSpecialization>().where("\"AMS_ID\" = ?").bind((*i).second);
            amsPtr.modify()->alert = alePtr;
        }
        transaction.commit();
        UserActionManagement::registerUserAction(Enums::add,Constants::T_ALERT_ALE,alePtr.id());
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return;
    }
    
    Wt::WMessageBox::show(tr("Alert.alert.alert-created-title"),tr("Alert.alert.alert-created"),Wt::Ok);
    
    created_ = false;
    model_->reset();
    update();
    comboAlertCrit->setCurrentIndex(0);
    
    
    Wt::WApplication *app = Wt::WApplication::instance();
    app->root()->widget(0)->refresh();
}

void AlertEditionWidget::close()
{
    delete this;
}

