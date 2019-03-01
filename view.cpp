#include "view.h"
#include <iostream>
#include <QRgb>
#include <QColor>
#include <QResizeEvent>
#include <QEvent>
using namespace std;

View::View(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(500, 700);

    // Create vertical layout container for the image
    m_verticalBox = new QVBoxLayout(this);

    m_startButton = new QPushButton("Start", this);
    m_startButton->setCheckable(true);
    m_verticalBox->addWidget(m_startButton);

    m_clearButton = new QPushButton("Clear", this);
    m_verticalBox->addWidget(m_clearButton);

    m_saveButton = new QPushButton("Save", this);
    m_verticalBox->addWidget(m_saveButton);

    m_loadButton = new QPushButton("Load", this);
    m_verticalBox->addWidget(m_loadButton);

    m_dropList = new QComboBox(this);
    m_dropList->addItem("MyImage");
    m_dropList->addItem("Cannon");
    m_dropList->addItem("Gidan");
    m_verticalBox->addWidget(m_dropList);

    // Create clickable label and set the image for the first time
    m_label = new ClickableLabel(this);
    m_verticalBox->addWidget(m_label);

    m_fpsSlider = new QSlider(Qt::Horizontal,this);
    m_verticalBox->addWidget(m_fpsSlider);
    m_fpsSlider->setRange(10,60);
    m_fpsSlider->setValue(30);

    m_zoomSlider = new QSlider(Qt::Horizontal,this);
    m_verticalBox->addWidget(m_zoomSlider);
    m_zoomSlider->setRange(5,100);
    m_zoomSlider->setValue(zoom/2);

    m_heatmapButton = new QRadioButton(this);
    m_heatmapButton->setChecked(false);
    m_verticalBox->addWidget(m_heatmapButton);
}

View::~View()
{
}

void View::resizeEvent(QResizeEvent* event)
{
    // Resize the image when resize the canvas
    updatePixmap();
}

void View::setPixmap(QPixmap pixmap)
{
    m_pixmap = pixmap;
    updatePixmap();
}

void View::updatePixmap()
{
    int length = m_pixmap.width();
    QRect rect(length/2 - zoom/2, length/2 - zoom/2, zoom, zoom);
    QPixmap cropped = m_pixmap.copy(rect);
    cropped = cropped.scaled(m_label->size());

    m_label->setPixmap(cropped);
}

ClickableLabel* View::getClickableLabel() const
{
    return m_label;
}

QPushButton* View::getStartButton() const
{
    return m_startButton;
}

QPushButton* View::getClearButton() const
{
    return m_clearButton;
}

QPushButton* View::getLoadButton() const
{
    return m_loadButton;
}

QPushButton* View::getSaveButton() const
{
    return m_saveButton;
}

QComboBox* View::getDropList() const
{
    return m_dropList;
}

QSlider* View::getFpsSlider() const
{
    return m_fpsSlider;
}

QSlider* View::getZoomSlider() const
{
    return m_zoomSlider;
}

QRadioButton* View::getHeatmapButton() const
{
    return m_heatmapButton;
}
