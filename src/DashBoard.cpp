/**
 * @file DashBoard.cpp
 * @author Thomas Saquet, Florent Poinsaut
 * @date 
 * @brief File containing example of doxygen usage for quick reference.
 *
 * Alert - GUI is a part of the Alert software
 * Copyright (C) 2013-2017
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301  USA
 * 
 */

#include "DashBoard.h"

using namespace std;

class NumericItem : public Wt::WStandardItem {
public:
virtual NumericItem *clone() const {
    return new NumericItem();
}

virtual void setData(const boost::any &data, int role = Wt::UserRole) 
{
    boost::any dt;

    if (role == Wt::EditRole) {
    double d = Wt::asNumber(data);

    if (d != d)
        dt = data;
    else
        dt = boost::any(d);
    }

    Wt::WStandardItem::setData(dt, role);
}

};


DashBoard::DashBoard(Echoes::Dbo::Session *session)
: Wt::WContainerWidget()
{
    created_ = false;
    this->session = session;
    Wt::WApplication *app = Wt::WApplication::instance();
    app->messageResourceBundle().use("dashboard",false);   
    
    mainTemplate = new Wt::WTemplate(Wt::WString::tr("Alert.dashboard.board.template"));
    mainTemplate->addFunction("tr", &Wt::WTemplate::Functions::tr);
    
    this->addWidget(mainTemplate);
    
    createUI();
}

void DashBoard::render(Wt::WFlags<Wt::RenderFlag> flags)
{
    if (!created_)
    {
//        update();
        created_ = true;
    }

    Wt::WContainerWidget::render(flags);
}

void DashBoard::createUI()
{
    Wt::WStandardItemModel *modelPie1 = new Wt::WStandardItemModel(this);
    modelPie1->setItemPrototype(new NumericItem());
    
    modelPie1->insertColumns(modelPie1->columnCount(), 2);
    
    modelPie1->insertRows(modelPie1->rowCount(), 2);
    int row = 0;

    modelPie1->setData(  row, 0, Wt::WString::fromUTF8("Mails envoyés"));
//    model->setData(  row, 1, Wt::WString("LIV CFM"), Wt::ToolTipRole);
    modelPie1->setData(  row, 1, 10000);

    modelPie1->setData(++row, 0, Wt::WString::fromUTF8("Mails en attente"));
    modelPie1->setData(  row, 1, 1286);
    
    
    
    Wt::WStandardItemModel *modelPie2 = new Wt::WStandardItemModel(this);
    modelPie2->setItemPrototype(new NumericItem());
    
    modelPie2->insertColumns(modelPie2->columnCount(), 2);
    
    modelPie2->insertRows(modelPie2->rowCount(), 2);
    row = 0;

    modelPie2->setData(  row, 0, Wt::WString::fromUTF8("SMS envoyés"));
//    model->setData(  row, 1, Wt::WString("LIV CFM"), Wt::ToolTipRole);
    modelPie2->setData(  row, 1, 3000);

    modelPie2->setData(++row, 0, Wt::WString::fromUTF8("SMS non reçus"));
    modelPie2->setData(  row, 1, 158);
    
    
    Wt::WStandardItemModel *modelPie3 = new Wt::WStandardItemModel(this);
    modelPie3->setItemPrototype(new NumericItem());

    // Configure the header.
    modelPie3->insertColumns(modelPie3->columnCount(), 2);
    modelPie3->setHeaderData(0, Wt::WString::fromUTF8("Code"));
    modelPie3->setHeaderData(1, Wt::WString::fromUTF8("Quantité"));

    // Set data in the model.
    modelPie3->insertRows(modelPie3->rowCount(), 8);
    row = 0;

    modelPie3->setData(  row, 0, Wt::WString("LIV CFM"));
//    model->setData(  row, 1, Wt::WString("LIV CFM"), Wt::ToolTipRole);
    modelPie3->setData(  row, 1, 120);

    modelPie3->setData(++row, 0, Wt::WString("EPC CFM"));
    modelPie3->setData(  row, 1, 30);

    modelPie3->setData(++row, 0, Wt::WString("EPC APM"));
    modelPie3->setData(  row, 1, 260);

    modelPie3->setData(++row, 0, Wt::WString("EML CFM"));
    modelPie3->setData(  row, 1, 160);

    modelPie3->setData(++row, 0, Wt::WString("ECH CFM"));
    modelPie3->setData(  row, 1, 40);
    
    modelPie3->setData(++row, 0, Wt::WString("PAQ RCA"));
    modelPie3->setData(  row, 1, 40);
    
    modelPie3->setData(++row, 0, Wt::WString("LIV RCA"));
    modelPie3->setData(  row, 1, 40);

    modelPie3->setData(++row, 0, Wt::WString("Other"));
    modelPie3->setData(  row, 1, 120);
    
    Wt::WHBoxLayout *hlay1 = new Wt::WHBoxLayout();
    hlay1->addWidget(addPieChart(modelPie1));
    hlay1->addWidget(addPieChart(modelPie2));
    
    Wt::WContainerWidget *firstRow = new Wt::WContainerWidget(this);
    firstRow->setLayout(hlay1);
    
    Wt::WStandardItemModel *modelCat = csvToModel(Wt::WApplication::appRoot() + "category.csv", this);
    
    Wt::WHBoxLayout *hlay2 = new Wt::WHBoxLayout();
    hlay2->addWidget(addCategoryChart(modelCat));
    hlay2->addWidget(addPieChart(modelPie3));
    
    Wt::WContainerWidget *secondRow = new Wt::WContainerWidget(this);
    secondRow->setLayout(hlay2);
    
    mainTemplate->bindWidget("firstrow",firstRow);
    mainTemplate->bindWidget("secondrow",secondRow);
    
    
//    new ScatterPlot(this);
}


Wt::WContainerWidget * DashBoard::addPieChart(Wt::WStandardItemModel *model)
{
    

    

    Wt::WContainerWidget *container = new Wt::WContainerWidget();

    
    
    
    // Set all items to be editable.
    for (int row = 0; row < model->rowCount(); ++row)
        for (int col = 0; col < model->columnCount(); ++col)
        model->item(row, col)->setFlags(Wt::ItemIsEditable);

//    Wt::WTableView* table = new Wt::WTableView(container);
//
//    table->setMargin(10, Wt::Top | Wt::Bottom);
//    table->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);
//    table->setSortingEnabled(true);
//    table->setModel(model);
//    table->setColumnWidth(1, 100);
//    table->setRowHeight(28);
//    table->setHeaderHeight(28);
//    table->setWidth(150 + 100 + 14 + 2);
//
//    if (Wt::WApplication::instance()->environment().ajax())
//        table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
//    else
//        table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);

    /*
     * Create the pie chart.
     */
    Wt::Chart::WPieChart *chart = new Wt::Chart::WPieChart(container);
    chart->setModel(model);       // Set the model.
    chart->setLabelsColumn(0);    // Set the column that holds the labels.
    chart->setDataColumn(1);      // Set the column that holds the data.

    // Configure location and type of labels.
    chart->setDisplayLabels(Wt::Chart::Outside |
                            Wt::Chart::TextLabel |
                            Wt::Chart::TextPercentage);

    // Enable a 3D and shadow effect.
    chart->setPerspectiveEnabled(true, 0.2);
    chart->setShadowEnabled(true);

    chart->setExplode(0, 0.3);  // Explode the first item.
    chart->resize(600, 300);    // WPaintedWidget must be given an explicit size.
    chart->setMargin(10, Wt::Top | Wt::Bottom); // Add margin vertically.
    chart->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right); // Center horizontally.
    
    return container;
}

Wt::WContainerWidget * DashBoard::addCategoryChart(Wt::WStandardItemModel *model)
{
    Wt::WContainerWidget *container = new Wt::WContainerWidget();

    if (!model)
    {
        return container;
    }

    /*
     * Configure all model items as selectable and editable.
     */
    for (int row = 0; row < model->rowCount(); ++row)
    {
        for (int col = 0; col < model->columnCount(); ++col)
        {
            model->item(row, col)->setFlags(Wt::ItemIsEditable);
        }
    }

    /*
     * Shows a table, allowing editing of the model
     */
//    Wt::WTableView *table = new Wt::WTableView(container);
//    table->setModel(model);
////    table->setSortingEnabled(true);
//    table->setColumnResizeEnabled(true);
//    table->setAlternatingRowColors(true);
//    table->setHeaderAlignment(0, Wt::AlignCenter);
//    table->setColumnAlignment(0, Wt::AlignLeft);
//    table->setRowHeight(28);
//    table->setHeaderHeight(28);
//    table->setMargin(10, Wt::Top | Wt::Bottom);
//    table->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);
//    table->setWidth(1*120 + 80 + 2*7 + 2);

    /*
     * Editing does not really work without Ajax, it would require an
     * additional button somewhere to confirm the edited value.
     */

//    if (Wt::WApplication::instance()->environment().ajax()) 
//    {
//        table->setEditTriggers(Wt::WAbstractItemView::SingleClicked);
//        table->setEditOptions(table->editOptions() | 
//                  Wt::WAbstractItemView::SaveWhenClosed);
//    } else 
//    {
//        table->setEditTriggers(Wt::WAbstractItemView::NoEditTrigger);
//    }

    /*
     * Use a delegate for the numeric data which rounds values sensibly.
     */
//    Wt::WItemDelegate *delegate = new Wt::WItemDelegate(table);
//    delegate->setTextFormat("%.f");
//    table->setItemDelegate(delegate);
//
//    table->setColumnWidth(0, 80);
//    for (int i = 1; i < model->columnCount(); ++i)
//    {
//        table->setColumnWidth(i, 120);
//    }

    /*
     * Create the category chart.
     */
    Wt::Chart::WCartesianChart *chart = new Wt::Chart::WCartesianChart(container);
    chart->setModel(model);
    chart->setXSeriesColumn(0);
    chart->setLegendEnabled(true);

    /*
     * Provide ample space for the title, the X and Y axis and the legend.
     */
    chart->setPlotAreaPadding(40, Wt::Left | Wt::Top | Wt::Bottom);
    chart->setPlotAreaPadding(120, Wt::Right);

    /*
     * Add all (but first) column as bar series.
     */
    for (int column = 1; column < model->columnCount(); ++column) 
    {
        Wt::Chart::WDataSeries series(column, Wt::Chart::BarSeries);
//        series.setLabelColor(Wt::WColor)
        
        series.setShadow(Wt::WShadow(3, 3, Wt::WColor(0, 0, 0, 127), 3));
        chart->addSeries(series);
    }

    chart->resize(600, 300);
    chart->setMargin(Wt::WLength::Auto, Wt::Left | Wt::Right);
    return container;
}

void DashBoard::close()
{
    delete this;
}
