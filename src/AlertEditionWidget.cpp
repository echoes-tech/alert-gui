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
    
    // Suggestion popup for value key
        Wt::WSuggestionPopup::Options suggestionOptions;
        suggestionOptions.highlightBeginTag = "<span class=\"highlight\">";
        suggestionOptions.highlightEndTag = "</span>";
        suggestionOptions.whitespace = "";
        suggestionOptions.wordSeparators = " ";
        suggestionOptions.listSeparator = '\0';
        suggestionOptions.appendReplacedText = "";
    
    this->sp =
              new Wt::WSuggestionPopup(Wt::WSuggestionPopup::generateMatcherJS(suggestionOptions),
                       Wt::WSuggestionPopup::generateReplacerJS(suggestionOptions),
                       app->root());
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
        try
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
            transaction.commit();
        }
        catch (Wt::Dbo::Exception e)
        {
            Wt::log("error") << "[AlertEditionWidget] " << e.what();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            return result;
        }
        comboAlertCrit->setModel(slmOperators);
        
        result = comboAlertCrit;
        result->changed().connect(boost::bind(&AlertEditionWidget::checkThresholdOperator, this));
    }
    else if (field == AlertEditionModel::ThresholdValueKey)
    {
        this->valueLineEdit = new Wt::WLineEdit();
        result = valueLineEdit;
        result->changed().connect(boost::bind(&AlertEditionWidget::checkThresholdValueKey, this));
        sp->forEdit(valueLineEdit,Wt::WSuggestionPopup::DropDownIcon);
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
    else if (field == AlertEditionModel::Unit)
    {
        comboInformationUnit = new Wt::WComboBox();
        comboInformationUnit->changed().connect(boost::bind(&AlertEditionWidget::rememberUnitValue, this));
//        comboInformationUnit->().connect(boost::bind(&AlertEditionWidget::rememberUnitValue, this));
        result = comboInformationUnit;
    }

    return result;
}

void AlertEditionWidget::checkThresholdOperator()
{
    updateModelField(model_, AlertEditionModel::ThresholdOperator);
    model_->validateField(AlertEditionModel::ThresholdOperator);
    model_->setValidated(AlertEditionModel::ThresholdOperator, false);
}

void AlertEditionWidget::checkThresholdValueKey()
{
    updateModelField(model_, AlertEditionModel::ThresholdValueKey);
    model_->validateField(AlertEditionModel::ThresholdValueKey);
    model_->setValidated(AlertEditionModel::ThresholdValueKey, false);
}

void AlertEditionWidget::checkThresholdValue()
{
    updateModelField(model_, AlertEditionModel::ThresholdValue);
    model_->validateField(AlertEditionModel::ThresholdValue);
    model_->setValidated(AlertEditionModel::ThresholdValue, false);
}

void AlertEditionWidget::checkSnoozeValue()
{
    updateModelField(model_, AlertEditionModel::ThresholdValue);
    updateModelField(model_, AlertEditionModel::ThresholdOperator);
    updateModelField(model_, AlertEditionModel::Snooze);
    model_->validateField(AlertEditionModel::Snooze);
    model_->setValidated(AlertEditionModel::Snooze, false);
}

void AlertEditionWidget::rememberUnitValue()
{
    updateModelField(model_, AlertEditionModel::Unit);
    model_->validateField(AlertEditionModel::Unit);
    model_->setValidated(AlertEditionModel::Unit, false);
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
        informationSelectionBox->clicked().connect(boost::bind(&AlertEditionWidget::updateInformationDetailsFromInformationSB, this));
        
                
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
    mediaValueSelectionBox->clear();
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
    informationSelectionBox->clear();
    updateView(model_);
}

void AlertEditionWidget::updateInformationSBFromApplicationSB()
{
    if (applicationSelectionBox->currentIndex() != -1)
    {
        this->updateInformationSelectionBox(mapPluginIdSboxRow[applicationSelectionBox->currentIndex()]);
    }
}

void AlertEditionWidget::updateInformationDetailsFromInformationSB()
{
    if (informationSelectionBox->currentIndex() != -1)
    {
        this->updateInformationDetails(informationSelectionBox->currentIndex());
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
    try
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
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return;
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
    try
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
        
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return;
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
        try
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
            transaction.commit();
        }
        catch (Wt::Dbo::Exception e)
        {
            Wt::log("error") << "[AlertEditionWidget] " << e.what();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            return;
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
        try
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
            transaction.commit();
        }
        catch (Wt::Dbo::Exception e)
        {
            Wt::log("error") << "[AlertEditionWidget] " << e.what();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            return;
        }
        serverSelectionBox->setModel(slmServer);
}

void AlertEditionWidget::updateInformationSelectionBox(int pluginId)
{
    Wt::log("info") << "[Plg Id] " << pluginId;
        Wt::Dbo::Query<Wt::Dbo::ptr<Information2> > queryInformation;
        std::string queryStringInfos = "SELECT inf FROM \"T_INFORMATION_INF\" inf WHERE \"PLG_ID_PLG_ID\" = ? AND \"INF_DISPLAY\" = TRUE"; // AND \"SRC_ID\" IN (1,3)";
        Wt::Dbo::collection<Wt::Dbo::ptr<Information2> > infos;
        Wt::WStringListModel *slmInformation = new Wt::WStringListModel;
        try
        {
            Wt::Dbo::Transaction transaction(*session);
            queryInformation = session->query<Wt::Dbo::ptr<Information2> >(queryStringInfos).bind(pluginId).orderBy("\"SRC_ID\",\"INF_NAME\"");//.bind(session->user().id());
            infos = queryInformation.resultList();
           
        
            int idx = 0;
            for (Wt::Dbo::collection<Wt::Dbo::ptr<Information2> >::const_iterator k = infos.begin(); k != infos.end(); k++)
            {
                slmInformation->insertString(idx,Wt::WString::tr("Alert.alert.information." + k->get()->name.toUTF8()));
                
                this->mapInformationSeaIdSboxRow[idx] = k->get()->pk.search.get()->pk.id;
                this->mapInformationSrcIdSboxRow[idx] = k->get()->pk.search.get()->pk.source.get()->pk.id;
                this->mapInformationPlgIdSboxRow[idx] = k->get()->pk.search.get()->pk.source.get()->pk.plugin.id();
                this->mapInformationIvnSboxRow[idx] = k->get()->pk.subSearchNumber;
                this->mapInformationInuIdSboxRow[idx] = k->get()->pk.unit.id();
                this->mapInformationPkvSboxRow[idx] = k->get()->pk.search.get()->pos_key_value;
//                this->mapInformatioInuIdSboxRow[idx] = k->get()->pk.unit.id();
                idx++;
            }
            transaction.commit();
        
        }
        catch (Wt::Dbo::Exception e)
        {
            Wt::log("error") << "[AlertEditionWidget] " << e.what();
            Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            return;
        }
        informationSelectionBox->setModel(slmInformation);
//        refresh();
}

void AlertEditionWidget::updateApplicationSelectionBox(int astId)
{
    Wt::WStringListModel *slmApplication = new Wt::WStringListModel;
    Wt::Dbo::collection<Wt::Dbo::ptr<Asset> > assets;
    try
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
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return;
    }
    applicationSelectionBox->setModel(slmApplication);
//    applicationSelectionBox->refresh();
    
}

void AlertEditionWidget::updateInformationDetails(int idx)
{
    try
    {
                
        Wt::Dbo::Transaction transaction(*session);
    
        // Value key hint
  
        this->sp->clearSuggestions();
        
        if (this->mapInformationPkvSboxRow[idx] != 0)
        {
            model_->setReadOnly(model_->ThresholdValueKey, false);
            model_->setValue(model_->ThresholdValueKey,boost::any(Wt::WString::Empty));
            Wt::Dbo::ptr<Information2> ptrInfoKey = session->find<Information2>().where("\"SEA_ID\" = ?").bind(this->mapInformationSeaIdSboxRow[idx])
                                        .where("\"SRC_ID\" = ?").bind(this->mapInformationSrcIdSboxRow[idx])
                                        .where("\"PLG_ID_PLG_ID\" = ?").bind(this->mapInformationPlgIdSboxRow[idx])
                                        .where("\"INF_VALUE_NUM\" = ?").bind(this->mapInformationPkvSboxRow[idx])
                                        .limit(1);

            if (ptrInfoKey)
            {
                model_->modifyField(model_->ThresholdValueKey,Wt::WString::tr("Alert.alert.hint." + ptrInfoKey.get()->name.toUTF8()));
            }
            else
            {
                Wt::log("error") << "[AlertEditionWidget] " << "ptr ptrInfoKey empty";
            }

            std::string queryString = 
            "SELECT iva FROM \"T_INFORMATION_VALUE_IVA\" iva WHERE \"IVA_ID\" IN ( SELECT \"IVA_ID\" FROM"
            "("
            "SELECT DISTINCT ON (\"IVA_VALUE\") \"IVA_VALUE\", \"IVA_ID\" FROM"
            "(" "SELECT iva.\"IVA_VALUE\", iva.\"IVA_ID\" FROM \"T_INFORMATION_VALUE_IVA\" iva"
            " WHERE \"SEA_ID\" = " + boost::lexical_cast<std::string>(this->mapInformationSeaIdSboxRow[idx]) + 
            " AND \"SRC_ID\" = " + boost::lexical_cast<std::string>(this->mapInformationSrcIdSboxRow[idx]) + 
            " AND \"PLG_ID_PLG_ID\" = " + boost::lexical_cast<std::string>(this->mapInformationPlgIdSboxRow[idx]) + 
            " AND \"INF_VALUE_NUM\" = " + boost::lexical_cast<std::string>(this->mapInformationPkvSboxRow[idx]) + 
            " AND \"IVA_AST_AST_ID\" = " + boost::lexical_cast<std::string>(this->mapAssetIdSboxRow[serverSelectionBox->currentIndex()]) + " ORDER BY \"IVA_ID\" DESC LIMIT 50) sr"
            " ) sr_sr"
            ");";


            std::string valueExample = "";
            Wt::Dbo::collection<Wt::Dbo::ptr<InformationValue> > collPtrIva = session->query<Wt::Dbo::ptr<InformationValue> >(queryString);
            for (Wt::Dbo::collection<Wt::Dbo::ptr<InformationValue> >::const_iterator k = collPtrIva.begin(); k != collPtrIva.end(); k++)
            {
                Wt::WString res = k->get()->value;
                this->sp->addSuggestion(res,res);

            }
        }
        else
        {
            std::string na = "N/A";
            model_->setReadOnly(model_->ThresholdValueKey, true);
            model_->modifyField(model_->ThresholdValueKey,Wt::WString::tr("Alert.alert.not-applicable"));
            model_->setValue(model_->ThresholdValueKey,boost::any(na));
        }
        updateView(model_);
        transaction.commit();
        
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] key values suggestion : " << e.what();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return;
    }    
    try
    {
                
        Wt::Dbo::Transaction transaction(*session);
        // Units
        Wt::log("info") << this->mapInformationInuIdSboxRow[idx];
        Wt::Dbo::collection<Wt::Dbo::ptr<InformationSubUnit> > ptrInfoSubUnit = session->find<InformationSubUnit>().where("\"ISU_INU_INU_ID\" = ?").bind(this->mapInformationInuIdSboxRow[idx]);
        Wt::Dbo::ptr<InformationUnit> ptrInfoUnit = session->find<InformationUnit>().where("\"INU_ID\" = ?").bind(this->mapInformationInuIdSboxRow[idx]);
        
        
        Wt::WStringListModel *slmInformationSubUnits = new Wt::WStringListModel;
        int idx = 0;
        slmInformationSubUnits->insertString(idx,Wt::WString::tr("Alert.alert.unit." + ptrInfoUnit.get()->name.toUTF8()));
        
        mapInformationUnitCombo[idx]= ptrInfoUnit.id();
        
        idx++;
        for (Wt::Dbo::collection<Wt::Dbo::ptr<InformationSubUnit> >::const_iterator k = ptrInfoSubUnit.begin(); k != ptrInfoSubUnit.end(); k++)
        {
            slmInformationSubUnits->insertString(idx,Wt::WString::tr("Alert.alert.sub-unit." + k->get()->name.toUTF8()));
            mapInformationUnitCombo[idx] = k->id();
            idx++;
        }
        comboInformationUnit->setModel(slmInformationSubUnits);
        Wt::log("info") << "[AlertEditionWidget] " << slmInformationSubUnits->stringList()[0];
        model_->setValue(model_->Unit,boost::any(slmInformationSubUnits->stringList()[0]));
        update();

        
        transaction.commit();
        
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] units : " << e.what();
        Wt::WMessageBox::show(tr("Alert.option.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return;
    }
//    update();
    return;
    
}

Wt::WStringListModel *AlertEditionWidget::getMediasForCurrentUser(int mediaType)
{
    Wt::WStringListModel *res = new Wt::WStringListModel();
     
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> > medias = session->find<MediaValue>().where("\"MEV_USR_USR_ID\" = ?").bind(session->user().id())
                                                                                            .where("\"MEV_MED_MED_ID\" = ?").bind(mediaType);
        int idx = 0;
        for (Wt::Dbo::collection<Wt::Dbo::ptr<MediaValue> >::const_iterator i = medias.begin(); i != medias.end(); ++i)
        {
            res->insertString(idx,(*i)->value);
            idx++;
        }
        transaction.commit();
    }
    catch (Wt::Dbo::Exception e)
    {
        Wt::log("error") << "[AlertEditionWidget] " << e.what();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
        return res;
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
    checkThresholdValueKey();
    if (!(model_->validateField(AlertEditionModel::Snooze)))
    {
        Wt::log("info") << "Snooze value incorrect";
        update();
        return;
    }
    else
    {
        update();
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
        transaction.commit();
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
    Wt::WApplication *app = Wt::WApplication::instance();
    
    if (((serverSelectionBox->currentIndex() == -1) || (applicationSelectionBox->currentIndex() == -1) || (informationSelectionBox->currentIndex() == -1)))
    {
        Wt::WMessageBox::show(tr("Alert.alert.information-missing-title"),tr("Alert.alert.information-missing"),Wt::Ok);
        return;
    }
    
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        
        
        Wt::Dbo::collection<Wt::Dbo::ptr<AlertValue> > avaPtrCollec = session->find<AlertValue>().where("\"SEA_ID\" = ?")
                                                        .bind(mapInformationSeaIdSboxRow[informationSelectionBox->currentIndex()])
                                                        .where("\"SRC_ID\" = ?")
                                                        .bind(mapInformationSrcIdSboxRow[informationSelectionBox->currentIndex()])
                                                        .where("\"PLG_ID_PLG_ID\" = ?")
                                                        .bind(mapInformationPlgIdSboxRow[informationSelectionBox->currentIndex()])
                                                        .where("\"INF_VALUE_NUM\" = ?")
                                                        .bind(mapInformationIvnSboxRow[informationSelectionBox->currentIndex()])
                                                        .where("\"INU_ID_INU_ID\" = ?")
                                                        .bind(mapInformationInuIdSboxRow[informationSelectionBox->currentIndex()]);
        
        if (avaPtrCollec.size() > 0)
        {
            std::string inString = "(";
            for (Wt::Dbo::collection<Wt::Dbo::ptr<AlertValue> >::const_iterator i = avaPtrCollec.begin(); i != avaPtrCollec.end(); i++) 
            {
                Wt::log("debug") << " [Class:AlertEditionWidget] " << " - " << " For ava list : " << (*i).id();
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
        transaction.commit();
        
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
        update();
        return;
    }
    
    Alert *alert = new Alert();
    AlertValue *ava = new AlertValue();
    Wt::WString name = serverSelectionBox->currentText() + " - " + applicationSelectionBox->currentText() + " - " + informationSelectionBox->currentText();
    try
    {
        Wt::Dbo::Transaction transaction(*session);
        
        
        Wt::Dbo::ptr<Information2> infoPtr = session->find<Information2>().where("\"SEA_ID\" = ?")
                                                        .bind(mapInformationSeaIdSboxRow[informationSelectionBox->currentIndex()])
                                                        .where("\"SRC_ID\" = ?")
                                                        .bind(mapInformationSrcIdSboxRow[informationSelectionBox->currentIndex()])
                                                        .where("\"PLG_ID_PLG_ID\" = ?")
                                                        .bind(mapInformationPlgIdSboxRow[informationSelectionBox->currentIndex()])
                                                        .where("\"INF_VALUE_NUM\" = ?")
                                                        .bind(mapInformationIvnSboxRow[informationSelectionBox->currentIndex()])
                                                        .where("\"INU_ID_INU_ID\" = ?")
                                                        .bind(mapInformationInuIdSboxRow[informationSelectionBox->currentIndex()]);
//        
        
//        Wt::log("info") << "SEA_ID : " << mapInformationSrcIdSboxRow[informationSelectionBox->currentIndex()];
//        Wt::log("info") << "SEA_ID : " << mapInformationSrcIdSboxRow[informationSelectionBox->currentIndex()];
        Wt::Dbo::ptr<AlertCriteria> critPtr = session->find<AlertCriteria>().where("\"ACR_ID\" = ?").bind(mapAlertCriteriaIdSboxRow[comboAlertCrit->currentIndex()]);
        ava->information = infoPtr;
        updateModelField(model_,AlertEditionModel::ThresholdValue);

        
        if (comboInformationUnit->currentIndex() != -1)
        {
            Wt::Dbo::ptr<InformationUnit> ptrUnit = session->find<InformationUnit>().where("\"INU_ID\" = ?")
                        .bind(mapInformationUnitCombo[0]);
            
            if (ptrUnit.get()->unitType.id() == Enums::text)
            {
                ava->value = model_->valueText(model_->ThresholdValue);
            }
            else if (ptrUnit.get()->unitType.id() == Enums::number)
            {
                if (comboInformationUnit->currentIndex() == 0)
                {
                    ava->value = model_->valueText(model_->ThresholdValue);
                }
                else
                {
                    Wt::Dbo::ptr<InformationSubUnit> ptrSubUnit = session->find<InformationSubUnit>().where("\"ISU_ID\" = ?")
                            .bind(mapInformationUnitCombo[comboInformationUnit->currentIndex()]);
                    float factorValue = boost::lexical_cast<float>(model_->valueText(model_->ThresholdValue)) * ptrSubUnit.get()->factor;
                    ava->value = boost::lexical_cast<std::string>(factorValue);
                }
            }
            else
            {
                Wt::log("error") << "[AlertEditionWidget] Unknown unit type.";
                Wt::WMessageBox::show(tr("Alert.alert.unknown-unit-title"),tr("Alert.alert.unknown-unit"),Wt::Ok);
            }
        }
        else
        {
            Wt::WMessageBox::show(tr("Alert.alert.choose-unit-title"),tr("Alert.alert.choose-unit"),Wt::Ok);
        }
        
        
        
        ava->alertCriteria = critPtr;
        
        
        ava->keyValue = model_->valueText(model_->ThresholdValueKey);
        
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
    
    app->root()->widget(0)->refresh();
    
    try
    {
        Wt::Dbo::Transaction transaction2(*session);
        
        std::string qryString = "DELETE FROM \"T_ALERT_MEDIA_SPECIALIZATION_AMS\" ams "
                                " WHERE \"AMS_ALE_ALE_ID\" IS NULL"
                                " AND \"AMS_MEV_MEV_ID\" IN (SELECT \"MEV_ID\" FROM \"T_MEDIA_VALUE_MEV\" WHERE \"MEV_USR_USR_ID\" = " + boost::lexical_cast<std::string>(this->userId)  + ");";
//        Wt::Dbo::collection<Wt::Dbo::ptr<AlertMediaSpecialization> listAms = session->query<Wt::Dbo::ptr<AlertMediaSpecialization>(qryString);
        session->execute(qryString);
//        for (Wt::Dbo::collection<Wt::Dbo::ptr<AlertMediaSpecialization> >::const_iterator i = listAms.begin(); i != listAms.end(); i++) 
//        {
//            i->remove();
//        }
        
        transaction2.commit();
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
    
    
    
    app->root()->widget(0)->refresh();
}

void AlertEditionWidget::close()
{
    delete this;
}

bool AlertEditionWidget::isCreated()
{
    return this->created_;
}
