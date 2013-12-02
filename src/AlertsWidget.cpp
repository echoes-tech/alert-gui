/* 
 * Gui AlertsWidget.cpp
 * 
 * @author ECHOES Technologies (TSA)
 * @author Guillaume KRIER
 * 
 * @date 20/11/2013
 * 
 * THIS PROGRAM IS CONFIDENTIAL AND PROPRIETARY TO ECHOES TECHNOLOGIES SAS
 * AND MAY NOT BE REPRODUCED, PUBLISHED OR DISCLOSED TO OTHERS WITHOUT
 * COMPANY AUTHORIZATION.
 * 
 * COPYRIGHT 2012-2013 BY ECHOES TECHNOLGIES SAS
 * 
 */

#include "AlertsWidget.h"

AlertsWidget::AlertsWidget(Echoes::Dbo::Session *session, std::string apiUrl)
: CreatePageWidget("alerts-test")
{
    session_= session;
    apiUrl_ = apiUrl;
    created_ = false;
    newClass_ = false;

    setButtonModif(true);
    setButtonSup(true);
    setLocalTable(true);
}

void    AlertsWidget::update()
{
    CreatePageWidget::update();
   if (!newClass_)
    {
       newClass_ = true;
    }
}

std::vector<std::string>        AlertsWidget::getTitlesTableWidget()
{
    std::vector<std::string>    titleWidget;
    titleWidget.empty();
    titleWidget.push_back("nb-dest");
    titleWidget.push_back("last-out");
    titleWidget.push_back("modif-dest");
    return titleWidget;
}

std::vector<std::string>        AlertsWidget::getTitlesTableText()
{
   std::vector<std::string>     titleText;
   titleText.empty();
   titleText.push_back("name");
   return titleText;
}

std::vector<long long>          AlertsWidget::getIdsTable()
{
    std::vector<long long>      ids;

    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Object obj;
    Wt::Json::Array::const_iterator idx1;
    if (alerts_.isNull() == false)
    {
        result1 = alerts_;
        for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
        {
            tmp = (*idx1);
            obj = tmp.get("alert");
            ids.push_back(obj.get("id"));
        }
    }
    return ids;
}

vector_type     AlertsWidget::getResourceRowTable(long long id)
{
    vector_type    rowTable;
    Wt::Json::Array& result1 = Wt::Json::Array::Empty;
    Wt::Json::Object tmp;
    Wt::Json::Object obj;
    Wt::Json::Array::const_iterator idx1;
    Wt::WString info = "";
    long long i(0); 
    long long infoLong;

    if (alerts_.isNull() == false)
    {
        result1 = alerts_;
        for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
        {
            tmp = (*idx1);
            obj = tmp.get("alert");
            i = obj.get("id");
            if (i == id)
            {
                info = obj.get("name");
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, Wt::WString>(info)));
                infoLong = obj.get("alert_media_specializations");
                rowTable.push_back(new Wt::WText(boost::lexical_cast<std::string, long long>(infoLong)));
                if (obj.get("last_attempt").isNull())
                    info = Wt::WString::Empty;
                else
                    info = obj.get("last_attempt");
                rowTable.push_back(new Wt::WText(info.toUTF8() == "Null" ? "Nothing" : info));  //xml
                Wt::WPushButton *button = new Wt::WPushButton();
                button->setAttributeValue("class","btn btn-inverse");
                button->setTextFormat(Wt::XHTMLUnsafeText);
                button->setText("<span class='input-group-btn'><i class='icon-edit icon-white'></i></span>");
                button->clicked().connect(boost::bind(&AlertsWidget::popupPrecipient, this, id));
                rowTable.push_back(button);
                return (rowTable);
            }
        }
    }
    return rowTable;

}

void    AlertsWidget::popupPrecipient(long long id)
{
    Wt::WDialog *dialog = new Wt::WDialog();
    dialog->setClosable(true);
    CreatePageWidget::buttonInDialogFooter(dialog);
    
    
    Wt::WTable *tablePopup = new Wt::WTable(dialog->contents());
    tablePopup->elementAt(0, 0)->addWidget(new Wt::WText("Alert: "));
    tablePopup->elementAt(0, 1)->addWidget(new Wt::WText("Alert 1"));
    
    tablePopup->elementAt(1, 0)->addWidget(new Wt::WText("Destinataire: ")); //XML
    Wt::WComboBox *box = new Wt::WComboBox(tablePopup->elementAt(1, 1));
    box->addItem("Romain Hinfray");
    box->addItem("Florent Poinsaut");
    box->addItem("Thomas Saquet");

    tablePopup->elementAt(2, 0)->addWidget(new Wt::WText("Media: ")); //XML
    box = new Wt::WComboBox(tablePopup->elementAt(2, 1));
    box->addItem("Medias");
    box->addItem("SMS");
    box->addItem("  0125252525");
    box->addItem("  0125252525");
    box->addItem("Mail");
    box->addItem("  salut@salut.fr");
    box->addItem("  salut@salut.fr");
    box->addItem("  salut@salut.fr");
    box->addItem("App");
    
    Wt::WTable *table = new Wt::WTable(tablePopup->elementAt(2, 2));
    
    table->elementAt(0, 0)->addWidget(new Wt::WLineEdit());
    Wt::WComboBox *comboBox = new Wt::WComboBox();
    comboBox->addItem("min"); 
    comboBox->addItem("hour"); // xml
    comboBox->addItem("fois"); // xml
    comboBox->setWidth(Wt::WLength(61));
    table->elementAt(0, 2)->addWidget(comboBox);
    
    tablePopup->elementAt(3, 0)->addWidget(new Wt::WText("Message: ")); //XML
    Wt::WTabWidget *tabPopup = new Wt::WTabWidget(tablePopup->elementAt(3, 1));
    tabPopup->addTab(new Wt::WTextArea("Your message error by mail"), "MAIL", Wt::WTabWidget::PreLoading);
    tabPopup->addTab(new Wt::WTextArea("Your message error by sms"), "SMS", Wt::WTabWidget::PreLoading);
    tabPopup->addTab(new Wt::WTextArea("Your message error by app"), "APP", Wt::WTabWidget::PreLoading);
//    tabPopup->setStyleClass("tabwidget");
    dialog->show();
}

void AlertsWidget::aSupprimer()
{
//    Pair pair = std::make_pair(42, "3");
//    assets_.insert(std::make_pair(3, pair));

    std::cout << "A supprimer toujours la !!" << std::endl;
    std::cout << "A supprimer toujours la !!" << std::endl;
    std::cout << "A supprimer toujours la !!" << std::endl;

    Pair pair = std::make_pair(41, "asset1-p1");
    assets_.insert(std::make_pair(4, pair));
    pair = std::make_pair(50, "asset2-p1-p2");
    assets_.insert(std::make_pair(5, pair));
    pair = std::make_pair(60, "asset3-p3-p4");
    assets_.insert(std::make_pair(6, pair));


    pair = std::make_pair(20, "plugin1-i1");
    plugins_.insert(std::make_pair(4, pair));
    pair = std::make_pair(20, "plugin1-i1");
    plugins_.insert(std::make_pair(5, pair));
    pair = std::make_pair(21, "plugin2-i1");
    plugins_.insert(std::make_pair(5, pair));
    pair = std::make_pair(22, "plugin3-i2");
    plugins_.insert(std::make_pair(6, pair));
    pair = std::make_pair(23, "plugin4-i3-i4");
    plugins_.insert(std::make_pair(6, pair));

    pair = std::make_pair(42, "info1-u1");
    infos_.insert(std::make_pair(4, pair));
    pair = std::make_pair(42, "info1-u1");
    infos_.insert(std::make_pair(5, pair));
    pair = std::make_pair(45, "info2-u2");
    infos_.insert(std::make_pair(6, pair));
    pair = std::make_pair(43, "info3-u3");
    infos_.insert(std::make_pair(6, pair));
    pair = std::make_pair(44, "info4-u2");
    infos_.insert(std::make_pair(6, pair));
    
    assetPlugins_.insert(std::make_pair(41, 20));
    assetPlugins_.insert(std::make_pair(50, 20));
    assetPlugins_.insert(std::make_pair(50, 21));
    assetPlugins_.insert(std::make_pair(60, 22));
    assetPlugins_.insert(std::make_pair(60, 23));
    
    pluginInfos_.insert(std::make_pair(20, 42));
    pluginInfos_.insert(std::make_pair(21, 42));
    pluginInfos_.insert(std::make_pair(22, 45));
    pluginInfos_.insert(std::make_pair(23, 43));
    pluginInfos_.insert(std::make_pair(23, 44));

    unitsIds_.insert(std::make_pair(42, 1));
    unitsIds_.insert(std::make_pair(45, 2));
    unitsIds_.insert(std::make_pair(43, 3));
    unitsIds_.insert(std::make_pair(44, 2));
}

// Work boxs -----------------------------------------------------------------

void    AlertsWidget::initBoxs(Wt::WTable *tableBox)
{
    
    aSupprimer();
    
    fillInMultiMap();
    
    Wt::WSelectionBox *boxAsset = new Wt::WSelectionBox(tableBox->elementAt(0, 0));
    Wt::WSelectionBox *boxPlugin = new Wt::WSelectionBox(tableBox->elementAt(0, 1));
    Wt::WSelectionBox *boxInfo = new Wt::WSelectionBox(tableBox->elementAt(0, 2));

    boxAsset->resize(Wt::WLength(250), Wt::WLength(150));
    boxAsset->setSelectionMode(Wt::ExtendedSelection);
    boxAsset->setSelectable(true);
    fillInBox(boxAsset, assets_);
    
    boxPlugin->resize(Wt::WLength(250), Wt::WLength(150));
    boxPlugin->setSelectionMode(Wt::ExtendedSelection);
    fillInBox(boxPlugin, plugins_);

    
    boxInfo->resize(Wt::WLength(150), Wt::WLength(150));
    boxInfo->setSelectionMode(Wt::ExtendedSelection);
    boxInfo->setSelectable(true);
    fillInBox(boxInfo, infos_);

    assetSelected_.clear();
    pluginSelected_.clear();
    infoSelected_.clear();

    idAsset_ = -1;
    idPlugin_ = -1;
    idInfo_ = -1;

    boxAsset->activated().connect(std::bind([=] () {
        idAsset_ = recoverId(boxAsset, assets_);
        if (idAsset_ >= 0)
        {
            selectAsset(idAsset_, boxAsset, boxPlugin, boxInfo);
        }
        else
        {
            assetSelected_.clear();
            idAsset_ = -1;
            if (pluginSelected_.size() > 0 && infoSelected_.size() > 0)
            {}
            else if (pluginSelected_.size() > 0)
            {
                fillInBox(boxPlugin, plugins_);
                long long id = recoverId(boxPlugin, plugins_);
                selectPlugin(id, boxAsset, boxPlugin, boxInfo);
            }
            else if (infoSelected_.size() > 0)
            {
                fillInBox(boxInfo, infos_);
                long long id = recoverId(boxInfo, infos_);
                selectInfo(id, boxInfo, boxPlugin, boxAsset);
            }
            else
            {
                fillInBox(boxAsset, assets_);
                fillInBox(boxPlugin, plugins_);
                fillInBox(boxInfo, infos_);
            }
        }
    }));

   boxPlugin->activated().connect(std::bind([=] () {
       idPlugin_ = recoverId(boxPlugin, plugins_);
       if (idPlugin_ >= 0)
       {
           selectPlugin(idPlugin_, boxAsset, boxPlugin, boxInfo);
       }
       else
       {
           pluginSelected_.clear();
           idPlugin_ = -1;
           if (assetSelected_.size() > 0 && infoSelected_.size() > 0)
           {}
           else if (assetSelected_.size() > 0)
           {
               fillInBox(boxAsset, assets_);
               long long id = recoverId(boxAsset, assets_);
               selectAsset(id, boxAsset, boxPlugin, boxInfo);
           }
           else if (infoSelected_.size() > 0)
           {
               fillInBox(boxInfo, infos_);
               long long id = recoverId(boxInfo, infos_);
               selectInfo(id, boxInfo, boxPlugin, boxAsset);
           }
           else
           {
               fillInBox(boxAsset, assets_);
               fillInBox(boxPlugin, plugins_);
               fillInBox(boxInfo, infos_);
           }
       }
    }));
    
    boxInfo->activated().connect(std::bind([=] () {
        errorsHide(errorText_);
        errorsHide(errorBool_);
        errorsHide(errorNumb_);
        idInfo_ = recoverId(boxInfo, infos_);
        showUnit(idInfo_);
        if (idInfo_ >= 0)
        {
            selectInfo(idInfo_, boxInfo, boxPlugin, boxAsset);
        }
        else
        {
            infoSelected_.clear();
            idInfo_ = -1;
            if (assetSelected_.size() > 0 && pluginSelected_.size() > 0)
            {}
            else if (assetSelected_.size() > 0)
            {
                fillInBox(boxAsset, assets_);
                long long id = recoverId(boxAsset, assets_);
                selectAsset(id, boxAsset, boxPlugin, boxInfo);
            }
            else if (pluginSelected_.size() > 0)
            {
                fillInBox(boxPlugin, plugins_);
                long long id = recoverId(boxPlugin, plugins_);
                selectPlugin(id, boxAsset, boxPlugin, boxInfo);
            }
            else
            {
                fillInBox(boxAsset, assets_);
                fillInBox(boxPlugin, plugins_);
                fillInBox(boxInfo, infos_);
            }
        }
    }));
}

void    AlertsWidget::selectAsset(long long id, Wt::WSelectionBox *boxAsset, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxInfo)
{
    boxPlugin->clear();
    VectorLong idsPlugins = boxActived(boxPlugin, plugins_, assetPlugins_, id);
    if (pluginSelected_.size() > 0 && selectItemBox(boxPlugin, pluginSelected_) == 1)
        std::cout << "Problème pour définir l'index" << std::endl;
    if (pluginSelected_.size() == 0)
    {
        boxInfo->clear();
        if (idsPlugins.size() > 0)
        {
            for (VectorLong::iterator it = idsPlugins.begin(); it != idsPlugins.end(); it++)
            {
                boxActived(boxInfo, infos_, pluginInfos_, (*it));
            }
        }
        else
           boxInfo->clear();
        if (infoSelected_.size() > 0 && selectItemBox(boxInfo, infoSelected_) == 1)
            std::cout << "Problème pour définir l'index" << std::endl;
    }
    cleanBox(boxAsset);
    cleanBox(boxPlugin);
    cleanBox(boxInfo);
}

void    AlertsWidget::selectPlugin(long long id, Wt::WSelectionBox *boxAsset, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxInfo)
{
    if (assetSelected_.size() == 0)
    {
        boxInfo->clear();
        boxActived(boxInfo, infos_, pluginInfos_, id);
    }
    if (infoSelected_.size() == 0)
    {
        boxAsset->clear();
        boxActived(boxAsset, assets_, pluginAsset_, id);
    }
    if (assetSelected_.size() > 0 && selectItemBox(boxAsset, assetSelected_) == 1)
        std::cout << "Problème pour définir l'index" << std::endl;
    if (infoSelected_.size() > 0 && selectItemBox(boxInfo, infoSelected_) == 1)
        std::cout << "Problème pour définir l'index" << std::endl;
    cleanBox(boxAsset);
    cleanBox(boxPlugin);
    cleanBox(boxInfo);
}

void    AlertsWidget::selectInfo(long long id, Wt::WSelectionBox *boxInfo, Wt::WSelectionBox *boxPlugin, Wt::WSelectionBox *boxAsset)
{
    boxPlugin->clear();
    VectorLong idsPlugins = boxActived(boxPlugin, plugins_, infoPlugin_, id);
    if (pluginSelected_.size() > 0 && selectItemBox(boxPlugin, pluginSelected_) == 1)
        std::cout << "Problème pour définir l'index" << std::endl;
    if (pluginSelected_.size() == 0)
    {
        boxAsset->clear();
        if (idsPlugins.size() > 0)
            for (VectorLong::iterator it = idsPlugins.begin(); it != idsPlugins.end(); it++)
            {
                boxActived(boxAsset, assets_, pluginAsset_, (*it));
            }
        else
            boxAsset->clear();
        if (assetSelected_.size() > 0 && selectItemBox(boxAsset, assetSelected_) == 1)
            std::cout << "Problème pour définir l'index" << std::endl;
    }
    cleanBox(boxAsset);
    cleanBox(boxPlugin);
    cleanBox(boxInfo);
}

void    AlertsWidget::cleanBox(Wt::WSelectionBox *box)
{
    std::set<int> selection;
    Wt::WString selected;
    VectorString checkString;
    
    for (int cpt(0); cpt < box->count(); cpt++)
    {
        selected = box->itemText(cpt);
        for (VectorString::iterator it = checkString.begin(); it != checkString.end(); it++)
        {
            if ((*it).compare(selected.toUTF8()) == 0)
                box->removeItem(cpt);
        }
        checkString.push_back(selected.toUTF8());
    }

}

int    AlertsWidget::selectItemBox(Wt::WSelectionBox *box, std::string select)
{
    Wt::WString rst = " ";
    for (int cpt(0); cpt < box->count(); cpt++)
    {
        rst = box->itemText(cpt);
        if (rst.toUTF8().compare(select) == 0)
        {
            std::set<int> selection;
            selection.clear();
            selection.insert(cpt);
            box->setSelectedIndexes(selection);
            return 0;
        }
    }
    return 1;
}

void    AlertsWidget::fillInMultiMap()
{
    pluginAsset_.clear();
    infoPlugin_.clear();
    for (MultiMapLongs::iterator it = assetPlugins_.begin(); it != assetPlugins_.end(); it++)
    {
        pluginAsset_.insert(std::make_pair((*it).second, (*it).first));
    }
    for (MultiMapLongs::iterator it = pluginInfos_.begin(); it != pluginInfos_.end(); it++)
    {
        infoPlugin_.insert(std::make_pair((*it).second, (*it).first));
    }
}

long long    AlertsWidget::recoverId(Wt::WSelectionBox *box, MultiMapPair infoInBox)
{
    index_.clear();
    std::set<int> selection = box->selectedIndexes();
    Wt::WString selected;
    for (std::set<int>::iterator it = selection.begin();
            it != selection.end(); ++it)
    {
        selected = box->itemText(*it);
    }
    int cpt(0);
    long long id(-1);
    for (MultiMapPair::iterator it = infoInBox.begin(); it != infoInBox.end(); it++)
    {
        if (selected.toUTF8().compare((*it).second.second) == 0)
        {
            if (infoInBox == assets_)
                assetSelected_ = selected.toUTF8();
            if (infoInBox == plugins_)
                pluginSelected_ = selected.toUTF8();
            if (infoInBox == infos_)
                infoSelected_ = selected.toUTF8();
            index_.push_back((*it).first);            
            id = (*it).second.first;
        }
        cpt++;
    }
    return id;
}

VectorLong    AlertsWidget::boxActived(Wt::WSelectionBox *box, MultiMapPair infoInBox, MultiMapLongs compId, long long id)
{
    VectorLong checkId;
    
    MultiMapLongs::iterator it = compId.find(id);
    for (long unsigned int cpt(0); cpt < compId.size(); cpt++)
    {
        for (MultiMapPair::iterator itP = infoInBox.begin(); itP != infoInBox.end(); itP++)
        {
            if ((*itP).second.first == (*it).second)
            {
                bool check(true);
                for (VectorLong::iterator itV = checkId.begin(); itV != checkId.end(); itV++)
                {
                    if ((*itV) == (*itP).second.first)
                        check = false;
                }
                if (check)
                {
                    for (VectorLong::iterator itI = index_.begin(); itI != index_.end(); itI++)
                    {
                        if ((*itP).first == (*itI))
                        {
                            box->addItem((*itP).second.second);
                            checkId.push_back((*itP).second.first);
                        }
                    }
                }
            }
        }
        it++;
    }
    return checkId;
}

void    AlertsWidget::fillInBox(Wt::WSelectionBox *box, MultiMapPair infoInBox)
{
    std::set<int> selection = box->selectedIndexes();
    Wt::WString selected;
    for (std::set<int>::iterator it = selection.begin();
            it != selection.end(); ++it)
        selected = box->itemText(*it);

    std::vector<long long> checkId;
    box->clear();
    for (MultiMapPair::iterator it = infoInBox.begin(); it != infoInBox.end(); it++)
    {
        bool check(true);
        for (std::vector<long long>::iterator itV = checkId.begin(); itV != checkId.end(); itV++)
        {
            if ((*itV) == (*it).second.first)
                check = false;
        }
        if (check)
        {
            checkId.push_back((*it).second.first);
            box->addItem((*it).second.second);
        }
    }
    if (selection.size() > 0)
    {
        for (int cpt(0); cpt < box->count(); cpt++)
        {
           if (selected.toUTF8().compare(box->itemText(cpt).toUTF8()) == 0)
           {
               selection.clear();
               selection.insert(cpt);
                box->setSelectedIndexes(selection);
           }
        }
    }
}

// ---------------------------------------------------

void    AlertsWidget::popupAddWidget(Wt::WDialog *dialog)
{
    Wt::WPushButton *ButtonSC = new Wt::WPushButton("Save and continu", dialog->footer());
    ButtonSC->clicked().connect(dialog, &Wt::WDialog::accept);    

    dialog->resize(Wt::WLength(750), Wt::WLength(500));

    Wt::WContainerWidget *contain = new Wt::WContainerWidget(dialog->contents());
    new Wt::WText("Asset - Plugin - Info - Key", contain); // A REVOIR ET XML
    
    Wt::WTable *tableBox = new Wt::WTable(contain);
            
    initBoxs(tableBox);
    errorAsset_ = new Wt::WText("Select asset", tableBox->elementAt(1, 0)); //XML
    errorPlugin_ = new Wt::WText("Select plugin", tableBox->elementAt(1, 1)); //XML
    errorInfo_ = new Wt::WText("Select info", tableBox->elementAt(1, 2)); //XML
    errorAsset_->hide();
    errorPlugin_->hide();
    errorInfo_->hide();
    
    new Wt::WText(Wt::WString::fromUTF8("<p>Last info value : 21/12/2012</p>"), contain);  //XML est a revoir quand info

    new Wt::WText(Wt::WString::fromUTF8("<p>Criteres de comparaison</p>"), contain);  //XML est hide show

    errorText_.clear();
    errorNumb_.clear();
    errorBool_.clear();

    textBox_.clear();
    compBox_.clear();
    sizeBox_.clear();
    
    unitOne_.clear();
    unitTwo_.clear();
    unitThree_.clear();
    
    createUnitOne(contain);
    createUnitTwo(contain);
    createUnitThree(contain);
    
//    compareBar(contain);
}

// ------------------ Unit | critères de comparaison ------------------------------

void    AlertsWidget::compareBar(Wt::WContainerWidget *contain)
{
    compareBar_ = new Wt::WContainerWidget(contain);
    compareBar_->addStyleClass("widget-title");
//    compareBar_->setWidth(750);
    /*
    new Wt::WText("Crit1 ", compareBar_);
    Wt::WLineEdit *lineEdit = new Wt::WLineEdit(compareBar_);
    lineEdit->setWidth(61);
    new Wt::WText(" Crit2", compareBar_);
    */
//    compareBar_->hide();
}

void    AlertsWidget::showUnit(long long id)
{
    VectorWWidget::iterator it;
    hideUnit();
    if (id > 0)
    {
        MultiMapLongs::iterator unit = unitsIds_.find(id);
        if ((*unit).second == Enums::EInformationUnitType::text)
        {
            for (it = unitOne_.begin(); it != unitOne_.end(); it++)
            {
                ((Wt::WTable*)(*it))->show();
            }
        }
        else if ((*unit).second == Enums::EInformationUnitType::number)
        {
            for (it = unitTwo_.begin(); it != unitTwo_.end(); it++)
            {
                ((Wt::WTable*)(*it))->show();
            }
        }
        else if ((*unit).second == 3) //Enums::EInformationUnitType::boolean
        {
            for (it = unitThree_.begin(); it != unitThree_.end(); it++)
            {
                ((Wt::WTable*)(*it))->show();
            }
        }
        else
            std::cout << "Error for show table unit" << std::endl;
    }
}

void    AlertsWidget::hideUnit()
{
    VectorWWidget::iterator it;
    for (it = unitOne_.begin(); it != unitOne_.end(); it++)
    {
        ((Wt::WTable*)(*it))->hide();
    }
    for (it = unitTwo_.begin(); it != unitTwo_.end(); it++)
    {
        ((Wt::WTable*)(*it))->hide();
    }
    for (it = unitThree_.begin(); it != unitThree_.end(); it++)
    {
        ((Wt::WTable*)(*it))->hide();
    }
}

Wt::WTable *AlertsWidget::createUnitOne(Wt::WContainerWidget *contain)
{
    Wt::WTable *table = new Wt::WTable(contain);

    Wt::WLineEdit *textEdit = new Wt::WLineEdit(table->elementAt(0, 1));
    
    textsEdit_.push_back(textEdit);
    
    Wt::WComboBox *comboBox = new Wt::WComboBox();
    comboBox->addItem("is equal");  //xml
    comboBox->addItem("is not equal");//xml
    comboBox->addItem("contains");//xml
    comboBox->addItem("doesn't contain");//xml
    comboBox->addItem("reg-exp");//xml
    
    textBox_.push_back(comboBox);

    table->elementAt(0, 0)->addWidget(comboBox);  
    table->hide();
    
    unitOne_.push_back(table);

    Wt::WText *errorText = new Wt::WText("Invalide name", contain); //XML
    errorText->hide();
    errorText_.push_back(errorText);
    
    Wt::WPushButton *buttonAdd = new Wt::WPushButton("+", contain);
    buttonAdd->clicked().connect(std::bind([=] ()
    {
        createUnitOne(contain)->show();
    }));
    buttonAdd->hide();
    textEdit->changed().connect(std::bind([=] ()
    {
       buttonAdd->show();
    }));
    return table;
}

Wt::WTable *AlertsWidget::createUnitTwo(Wt::WContainerWidget *contain)
{
    Wt::WTable *table = new Wt::WTable(contain);
    Wt::WLineEdit *valeurEdit = new Wt::WLineEdit(table->elementAt(0, 0));
    valeurEdit->setValidator(editValidator(-2));

    valuesEdit_.push_back(valeurEdit);
    
    Wt::WComboBox *comboBox = new Wt::WComboBox();
    compBox_.push_back(comboBox);
    comboBox->addItem("<");
    comboBox->addItem("<=");
    comboBox->addItem("==");
    comboBox->addItem("!=");
    comboBox->addItem(">=");
    comboBox->addItem(">");

    table->elementAt(0, 1)->addWidget(comboBox);
    table->elementAt(0, 2)->addWidget(new Wt::WText("Info"));
    
    comboBox = new Wt::WComboBox();
    sizeBox_.push_back(comboBox);
    comboBox->addItem("Kilo");//xml
    comboBox->addItem("Mega");//xml
    comboBox->addItem("Giga");//xml

    table->elementAt(0, 3)->addWidget(comboBox);
    
    table->hide();

    unitTwo_.push_back(table);

    Wt::WText *errorNumb = new Wt::WText("Invalide number", contain); //XML
    errorNumb->hide();
    errorNumb_.push_back(errorNumb);

    Wt::WPushButton *buttonAdd = new Wt::WPushButton("+", contain);
    buttonAdd->clicked().connect(std::bind([=] ()
    {
        createUnitTwo(contain)->show();      
    }));
    buttonAdd->hide();
    valeurEdit->changed().connect(std::bind([=] ()
    {
       buttonAdd->show();
    }));
    return table;
}

void    AlertsWidget::createUnitThree(Wt::WContainerWidget *contain)
{
    Wt::WTable *table = new Wt::WTable(contain);
    Wt::WText *errorBool = new Wt::WText("choose boolean", contain); //XML
    errorBool->hide();
    errorBool_.push_back(errorBool);

    bool_ = -1;
    
    table->elementAt(0, 0)->addWidget(new Wt::WText("True"));//xml
    Wt::WPushButton *button = new Wt::WPushButton(table->elementAt(0, 0));
    button->setAttributeValue("class","btn btn-dark-green");
    button->setTextFormat(Wt::XHTMLUnsafeText);
    button->setText("<span class='input-group-btn'><i class='icon-ok icon-white'></i></span>");
    button->clicked().connect(std::bind([=] () {
        bool_ = 0;
        errorBool->setText("True");
        errorBool->show();
    }));

    table->elementAt(0, 1)->addWidget(new Wt::WText("False"));//xml
    button = new Wt::WPushButton(table->elementAt(0, 1));
    button->setAttributeValue("class","btn btn-danger");
    button->setTextFormat(Wt::XHTMLUnsafeText);
    button->setText("<span class='input-group-btn'><i class='icon-remove icon-white'></i></span>");
    button->clicked().connect(std::bind([=] () {
        bool_ = 1;
        errorBool->setText("False");
        errorBool->show();
    }));

    table->hide();

    unitThree_.push_back(table);
}

// -------------------------------------------------------

void    AlertsWidget::modifRecip(long long id)
{
    std::cout << "modif recipient" << std::endl;
}

Wt::WValidator    *AlertsWidget::editValidator(int who)
{
    Wt::WRegExpValidator *validator = 0;
    
//    if (who == -1)
//        validator->setRegExp(); // pour regex a voir ! 
    if (who == -2)
    {
        validator = new Wt::WRegExpValidator("-?[0123456789]+");
        validator->setMandatory(true);
    }
    return validator;
}

void  AlertsWidget::closePopup()
{
    recoverListAsset();
}

int     AlertsWidget::checkInput(std::vector<Wt::WInteractWidget*> inputName, std::vector<Wt::WText*> errorMessage)
{    
    int checkAll = CreatePageWidget::checkInput(inputName, errorMessage);
    if (idAsset_ < 0 || idPlugin_ < 0 || idInfo_ < 0)
    {
        idAsset_ < 0 ? errorAsset_->show() : errorAsset_->hide();
        idPlugin_ < 0 ? errorPlugin_->show() : errorPlugin_->hide();
        idInfo_ < 0 ? errorInfo_->show() : errorInfo_->hide();
        checkAll = 1;
    }
    else
    {
        errorAsset_->hide();
        errorPlugin_->hide();
        errorInfo_->hide();
    }

    VectorLineEdit::iterator it;
    MultiMapLongs::iterator unit = unitsIds_.find(idInfo_);
    switch ((*unit).second)
    {
        case Enums::EInformationUnitType::text :
        {
            errorsHide(errorNumb_);
            errorsHide(errorBool_);
            
            for (it = textsEdit_.begin(); it != textsEdit_.end(); it++)
            {
                /* Mettre en place si validator sur l'input texte.
                 if (((Wt::WLineEdit*)(*it))->validate() == Wt::WValidator::Invalid)
                {
                    errorsShow(errorText_);
                    checkAll = 1;
                    break;
                }
             */
                if (((Wt::WLineEdit*)(*it))->text().toUTF8().size() <= 0)
                {
                    errorsShow(errorText_);
                    checkAll = 1;
                    break;
                }
                else
                    errorsHide(errorText_);
            }
            break;
        }
        case Enums::EInformationUnitType::number :
        {
            errorsHide(errorText_);
            errorsHide(errorBool_);
            for (it = valuesEdit_.begin(); it != valuesEdit_.end(); it++)
            {
                 if (((Wt::WLineEdit*)(*it))->validate() == Wt::WValidator::Invalid)
                {
                     errorsShow(errorNumb_);
                     checkAll = 1;
                     break;
                }
                if (((Wt::WLineEdit*)(*it))->text().toUTF8().size() <= 0)
                {
                    errorsShow(errorNumb_);
                    checkAll = 1;
                    break;
                }
                else
                    errorsHide(errorNumb_);
            }
            break;
        }
        case 3 : //Enums::EInformationUnitType::boolean
        {
            errorsHide(errorNumb_);
            errorsHide(errorText_);
            if (bool_ < 0)
            {
                errorsShow(errorBool_);
                checkAll = 1;
            }
            else
                errorsHide(errorBool_);
            break;
        }
    }
    return checkAll;
}

void AlertsWidget::addResource(std::vector<Wt::WInteractWidget*> argument)
{

    std::cout << "ADD RESOURCE" << std::endl;
    
    std::cout << "in argument : " << std::endl;
    for (std::vector<Wt::WInteractWidget*>::iterator it = argument.begin();
            it != argument.end(); it++)
    {
        std::cout << ((Wt::WLineEdit*)(*it))->text() << std::endl;
    }
    std::cout << "Asset , plugin info : " << std::endl;
    std::cout << "Id asset : " << idAsset_ << " name : ";
    for (MultiMapPair::iterator it = assets_.begin(); it != assets_.end(); it++)
    {
        if ((*it).second.first == idAsset_)
            std::cout << (*it).second.second << std::endl;
    }
    std::cout << "Id plugin : " << idPlugin_ << " name : ";
    for (MultiMapPair::iterator it = plugins_.begin(); it != plugins_.end(); it++)
    {
        if ((*it).second.first == idPlugin_)
            std::cout << (*it).second.second << std::endl;
    }
    std::cout << "Id info : " << idInfo_ << " name : ";
    for (MultiMapPair::iterator it = infos_.begin(); it != infos_.end(); it++)
    {
        if ((*it).second.first == idInfo_)
            std::cout << (*it).second.second << std::endl;
    }
    
    std::cout << std::endl << "KEY ?" << std::endl;
    
    
    std::cout << "Textbox size : " << textBox_.size() << std::endl;
    VectorWWidget::iterator it;
    VectorComboBox::iterator itV;
    MultiMapLongs::iterator unit = unitsIds_.find(idInfo_);
    switch ((*unit).second)
    {
        case Enums::EInformationUnitType::text :
        {
            for (it = unitOne_.begin(); it != unitOne_.end(); it++)
            {
                if (!((Wt::WTable*)(*it))->isHidden())
                {
                    for (itV = textBox_.begin(); itV != textBox_.end(); itV++)
                    {
                        std::cout
                                << "comparaison : "
                                << ((Wt::WComboBox*)(*itV))->currentText()
                                << std::endl;
                    }
                    for (VectorLineEdit::iterator itL = textsEdit_.begin(); itL != textsEdit_.end(); itL++)
                    {
                        std::cout
                                << "Text : "
                                << ((Wt::WLineEdit*)(*itL))->text().toUTF8()
                                << std::endl;
                    }
                }
            }
            break;
        }
        case Enums::EInformationUnitType::number :
        {
            for (it = unitTwo_.begin(); it != unitTwo_.end(); it++)
            {
                if (!((Wt::WTable*)(*it))->isHidden())
                {
                    for (itV = compBox_.begin(); itV != compBox_.end(); itV++)
                    {
                        std::cout << "comparaison : " << ((Wt::WComboBox*)(*itV))->currentText() << std::endl;
                    }
                    for (itV = sizeBox_.begin(); itV != sizeBox_.end(); itV++)
                    {
                        std::cout << " size :" << ((Wt::WComboBox*)(*itV))->currentText() << std::endl;
                    }
                    for (VectorLineEdit::iterator itL = valuesEdit_.begin(); itL != valuesEdit_.end(); itL++)
                    {
                        std::cout
                                << "Number : "
                                << ((Wt::WLineEdit*)(*itL))->text().toUTF8()
                                << std::endl;
                    }
                }
            }
            break;
        }
        case 3 : //Enums::EInformationUnitType::boolean
        {
            for (it = unitThree_.begin(); it != unitThree_.end(); it++)
            {
                if (!((Wt::WTable*)(*it))->isHidden())
                {
                        std::cout << "boolean : ";
                        if (bool_ == 0)
                            std::cout << "True" << std::endl;
                        else if (bool_ == 1)
                            std::cout << "False" << std::endl;
                        else
                            std::cout << "Bug" << std::endl;
                }
            }
            break;
        }
    }
    
//    std::transform(data.begin(), data.end(), data.begin(), std::tolower);

    
//    std::vector<Wt::WInteractWidget*>::iterator i = argument.begin();
//    Wt::WLineEdit *assetEdit = (Wt::WLineEdit*)(*i);

    /*
    Wt::Http::Message messageAsset;
    messageAsset.addBodyText("{\n\"med_id\": " + boost::lexical_cast<std::string>(this->type_) +
            ",\n\"mev_value\": \"" + boost::lexical_cast<std::string>(assetEdit->text()) + "\"\n}");
    
    std::string apiAddress = this->getApiUrl() + "/medias/";
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::postMedia, this, _1, _2));
    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
    if (client->post(apiAddress, messageAsset))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
    */
    recoverListAsset();
}

Wt::WDialog *AlertsWidget::deleteResource(long long id)
{
    Wt::WDialog *box = CreatePageWidget::deleteResource(id);
    box->show();
    box->finished().connect(std::bind([=] () {
        if (box->result() == Wt::WDialog::Accepted)
        {
            /*
            Wt::Http::Message message;
            message.addBodyText("");
            std::string apiAddress = this->getApiUrl() + "/medias/" + boost::lexical_cast<std::string> (id);
            Wt::Http::Client *client = new Wt::Http::Client(this);
            client->done().connect(boost::bind(&UserEditionWidget::deleteMedia, this, _1, _2));
            apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
            if (client->deleteRequest(apiAddress, message))
                Wt::WApplication::instance()->deferRendering();
            else
                std::cout << "Error Client Http" << std::endl;
            */
        }
        return box;
    }));
    return box;
}

void AlertsWidget::modifResource(std::vector<Wt::WInteractWidget*> arguments, long long id)
{
//    Wt::Http::Message message;
    
//    Wt::WLineEdit *test;
//    test = (Wt::WLineEdit*)(*arguments.begin());
    
    /*
    message.addBodyText("{\n\"med_id\": " + boost::lexical_cast<std::string>(this->type_)
            + ",\n\"mev_value\":\"" + boost::lexical_cast<std::string>(test->text()) + "\"\n}");

    std::string apiAddress = this->getApiUrl() + "/medias/" + boost::lexical_cast<std::string>(id);
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&UserEditionWidget::putMedia, this, _1, _2));
    apiAddress += "?login=" + session_->user()->eMail.toUTF8() + "&token=" + session_->user()->token.toUTF8();
    if (client->put(apiAddress, message))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
    */
}

void AlertsWidget::close()
{
    delete this;
}

void    AlertsWidget::setSession(Echoes::Dbo::Session *session)
{
    session_ = session;
}

void    AlertsWidget::setApiUrl(std::string apiUrl)
{
    apiUrl_ = apiUrl;
}

std::string   AlertsWidget::getApiUrl()
{
    return apiUrl_;
}


// API CALL AND RETURN --------------------------------------------------

void    AlertsWidget::recoverListAsset()
{
    alerts_ = 0;
    std::cout << "CLEAR ALL " << std::endl;
    assets_.clear();
    plugins_.clear();
    infos_.clear();
    assetPlugins_.clear();
    pluginAsset_.clear();
    pluginInfos_.clear();
    infoPlugin_.clear();
    unitsIds_.clear();

//    resultPlugins_.clear();

    /*
    std::cout << "Recoverlist Asset" << std::endl;
    std::string apiAddress = this->getApiUrl() + "/plugins" + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::Http::Client *client = new Wt::Http::Client(this);
    client->done().connect(boost::bind(&AlertsWidget::getPlugins, this, _1, _2));
    if (client->get(apiAddress))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;
    */
    std::string apiAddress = this->getApiUrl() + "/alerts" + "?login=" 
            + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8()) 
            + "&token=" + session_->user()->token.toUTF8();
    Wt::Http::Client *client1 = new Wt::Http::Client(this);
    client1->done().connect(boost::bind(&AlertsWidget::getAlerts, this, _1, _2));
    if (client1->get(apiAddress))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;


    apiAddress = this->getApiUrl() + "/assets" + "?login="
            + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
            + "&token=" + session_->user()->token.toUTF8();
    Wt::Http::Client *client2 = new Wt::Http::Client(this);
    client2->done().connect(boost::bind(&AlertsWidget::getAssets, this, _1, _2));
    if (client2->get(apiAddress))
        Wt::WApplication::instance()->deferRendering();
    else
        std::cout << "Error Client Http" << std::endl;


}

// API RETURN INFOS ------------------------------------------

void AlertsWidget::getAlerts(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::parse(response.body(), alerts_);
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
        else
        {
            this->alerts_ = Wt::Json::Value::Null;
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
   newClass_ = false;
   created_ = false;
   update();
}


void AlertsWidget::getAssets(boost::system::error_code err, const Wt::Http::Message& response)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::Object tmp;
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::Value result;
                Wt::WString name;
                Wt::Json::parse(response.body(), result);
                long long id;
                if (result.isNull() == false)
                {
                    Wt::Json::Array& result1 = result;
                    long long index(0);
                    for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                    {
                        tmp = (*idx1);
                        id = tmp.get("id");
                        name = tmp.get("name");

                        Pair pair = std::make_pair(id, name.toUTF8());
                        assets_.insert(std::make_pair(index, pair));

                        std::string apiAddress = this->getApiUrl() + "/assets/" + boost::lexical_cast<std::string>(id)
                                + "/plugins" + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
                                + "&token=" + session_->user()->token.toUTF8();
                        Wt::Http::Client *client = new Wt::Http::Client(this);
                        client->done().connect(boost::bind(&AlertsWidget::getPluginsForAsset, this, _1, _2, id, index++));
                        if (client->get(apiAddress))
                        {
                            Wt::WApplication::instance()->deferRendering();
                        }
                        else
                            std::cout << "Error Client Http" << std::endl;
                    }
                }
                else
                    std::cout << "Erreur Data Base" << std::endl;


            }
            catch (Wt::Json::ParseError const& e)
            {
                
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
   newClass_ = false;
   created_ = false;
}

void AlertsWidget::getPlugins(boost::system::error_code err, const Wt::Http::Message& response)
{    
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::Value result;
/*
                Wt::Json::Array& result1 = Wt::Json::Array::Empty;
                Wt::Json::Object tmp;
                Wt::Json::Array::const_iterator idx1;
                long long id;
                Wt::WString name;
                */
                Wt::Json::parse(response.body(), result);
//                result1 = result;
/*
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);
                    id = tmp.get("id");
                    name = tmp.get("name");
                    resultPlugins_.push_back(std::make_pair(id, name.toUTF8()));
                    std::string apiAddress = this->getApiUrl() + "/plugins/" + boost::lexical_cast<std::string>(id)
                            + "/informations" + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
                            + "&token=" + session_->user()->token.toUTF8();
                    Wt::Http::Client *client = new Wt::Http::Client(this);
                    client->done().connect(boost::bind(&AlertsWidget::getInformations, this, _1, _2));
                    if (client->get(apiAddress))
                        Wt::WApplication::instance()->deferRendering();
                    else
                        std::cout << "Error Client Http" << std::endl;
                }
                */
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
   newClass_ = false;
   created_ = false;
}

void AlertsWidget::getPluginsForAsset(boost::system::error_code err, const Wt::Http::Message& response, long long idAsset, long long index)
{    
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::Array& result1 = Wt::Json::Array::Empty;
                Wt::Json::Object tmp;
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::parse(response.body(), result);
                long long id;
                Wt::WString name;
                result1 = result;
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);
                    id = tmp.get("id");
                    name = tmp.get("name");

                    assetPlugins_.insert(std::make_pair(idAsset, id));
                            
                    Pair pair = std::make_pair(id, name.toUTF8());
                    plugins_.insert(std::make_pair(index, pair));

                    std::string apiAddress = this->getApiUrl() + "/plugins/" + boost::lexical_cast<std::string>(id)
                            + "/informations" + "?login=" + Wt::Utils::urlEncode(session_->user()->eMail.toUTF8())
                            + "&token=" + session_->user()->token.toUTF8();
                    Wt::Http::Client *client = new Wt::Http::Client(this);
                    client->done().connect(boost::bind(&AlertsWidget::getInformations, this, _1, _2, id, index));
                    if (client->get(apiAddress))
                        Wt::WApplication::instance()->deferRendering();
                    else
                        std::cout << "Error Client Http" << std::endl;
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
   newClass_ = false;
   created_ = false;
}

void AlertsWidget::getInformations(boost::system::error_code err, const Wt::Http::Message& response, long long idPlugin, long long index)
{
    Wt::WApplication::instance()->resumeRendering();
    if (!err)
    {
        if(response.status() == 200)
        {
            try
            {
                Wt::Json::Value result;
                Wt::Json::Array& result1 = Wt::Json::Array::Empty;
                Wt::Json::Object tmp, obj;
                Wt::Json::Array::const_iterator idx1;
                Wt::Json::parse(response.body(), result);
                long long idUnit;
                Wt::WString name;
                result1 = result;
                for (idx1 = result1.begin(); idx1 != result1.end(); idx1++)
                {
                    tmp = (*idx1);
                    if (tmp.get("display"))
                    {
                        obj = tmp.get("id");
                        idUnit = obj.get("unit_id");
//                        idP = obj.get("plugin_id");
                        name = tmp.get("name");
                        
                        unitsIds_.insert(std::make_pair(idUnit, idUnit));
                        pluginInfos_.insert(std::make_pair(idPlugin, idUnit));
                        
                        Pair pair = std::make_pair(idUnit, name.toUTF8());
                        infos_.insert(std::make_pair(index, pair));
                    }
                }
            }
            catch (Wt::Json::ParseError const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] Problems parsing JSON: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title"),tr("Alert.alert.database-error"),Wt::Ok);
            }
            catch (Wt::Json::TypeException const& e)
            {
                Wt::log("warning") << "[Alert Edition Widget] JSON Type Exception: " << response.body();
                Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "TypeException",tr("Alert.alert.database-error"),Wt::Ok);
            }
        }
    }
    else
    {
        Wt::log("error") << "[Alert Edition Widget] Http::Client error: " << err.message();
        Wt::WMessageBox::show(tr("Alert.alert.database-error-title") + "err",tr("Alert.alert.database-error"),Wt::Ok);
    }
   newClass_ = false;
   created_ = false;
}

// Utility --------------------------------------------------------

void    AlertsWidget::errorsShow(VectorText error)
{
    for (VectorText::iterator it = error.begin(); it != error.end(); it++)
    {
        ((Wt::WText*)(*it))->show();
    }
}

void    AlertsWidget::errorsHide(VectorText error)
{
    for (VectorText::iterator it = error.begin(); it != error.end(); it++)
    {
        ((Wt::WText*)(*it))->hide();
    }
}


