#include "view.h"
#include <iostream>
#include <QRgb>
#include <QColor>
#include <QResizeEvent>
#include <QEvent>
#include <QCheckBox>
#include <QLineEdit>
using namespace std;

View::View(QWidget *parent) : QWidget(parent)
{
    // Set size and style of main window
    this->setFixedSize(600, 700);
    this->setStyleSheet("View { background : #333333; }");

    // Create vertical layout container for the image
    m_verticalBox = new QVBoxLayout(this);

    // Create clickable label and set the image for the first time
    m_label = new ClickableLabel(this);
    m_label->setStyleSheet("QLabel {border: solid 10px white}");
    m_verticalBox->addWidget(m_label);
    m_verticalBox->addSpacing(10);

    // Create all the horizontal layouts needed
    QHBoxLayout* horizontalLayout = new QHBoxLayout();
    m_verticalBox->addLayout(horizontalLayout);
    horizontalLayout->setSpacing(100);
    QHBoxLayout* horizontalLayout1 = new QHBoxLayout();
    m_verticalBox->addLayout(horizontalLayout1);
    horizontalLayout1->setSpacing(100);
    QHBoxLayout* horizontalLayout2 = new QHBoxLayout();
    m_verticalBox->addLayout(horizontalLayout2);

    // Create Start button and add it to its layout
    m_startButton = new QPushButton("Start", this);
    m_startButton->setCheckable(true);
    horizontalLayout->addSpacing(50);
    horizontalLayout->addWidget(m_startButton);

    // Create Clear button and add it to its layout
    m_clearButton = new QPushButton("Clear", this);
    horizontalLayout->addWidget(m_clearButton);
    horizontalLayout->addSpacing(50);

    // Create Save button and add it to its layout
    m_saveButton = new QPushButton("Save", this);
    horizontalLayout1->addSpacing(50);
    horizontalLayout1->addWidget(m_saveButton);

    // Create DropList saved menu and add it to its layout
    m_dropList = new QComboBox(this);
    m_dropList->setEditable(true);
    m_dropList->lineEdit()->setReadOnly(true);
    m_dropList->lineEdit()->setAlignment(Qt::AlignCenter);
    m_dropList->addItem("Load");
    m_dropList->addItem("MyImage");
    m_dropList->addItem("10 Cell Row");
    m_dropList->addItem("Small Exploder");
    m_dropList->addItem("Exploder");
    m_dropList->addItem("Spaceship");
    m_dropList->addItem("Tumbler");
    m_dropList->addItem("Gosper Cannon");
    horizontalLayout1->addWidget(m_dropList);
    horizontalLayout1->addSpacing(50);

    // Create box layout to contain slider and its label
    QVBoxLayout* fpsSliderBox = new QVBoxLayout();
    QString string = "Fps: " + QString::number(30);
    m_fpsSlider = new QSlider(Qt::Horizontal, this);
    m_fpsSlider->setRange(10,60);
    m_fpsSlider->setValue(30);
    fpsLabel = new QLabel(string, m_fpsSlider);
    fpsLabel->setStyleSheet("QLabel { color : white; }");
    fpsLabel->setAlignment(Qt::AlignCenter);
    fpsSliderBox->addWidget(fpsLabel);
    fpsSliderBox->addWidget(m_fpsSlider);
    // Add the container to its layout
    horizontalLayout2->addSpacing(50);
    horizontalLayout2->addLayout(fpsSliderBox);
    horizontalLayout2-> addStretch(0);

    // Create box layout to contain slider and its label
    QVBoxLayout* zoomSliderBox = new QVBoxLayout();
    string = "Cells: " + QString::number(zoom) + "x" + QString::number(zoom);
    m_zoomSlider = new QSlider(Qt::Horizontal,this);
    m_zoomSlider->setRange(5,100);
    m_zoomSlider->setValue(zoom/2);
    zoomLabel = new QLabel(string, m_zoomSlider);
    zoomLabel->setStyleSheet("QLabel { color : white; }");
    zoomLabel->setAlignment(Qt::AlignCenter);
    zoomSliderBox->addWidget(zoomLabel);
    zoomSliderBox->addWidget(m_zoomSlider);
    // Add the container to its layout
    horizontalLayout2->addLayout(zoomSliderBox);
    horizontalLayout2->addStretch(0);

    // Create CheckBox as heatmap toogle
    m_heatmapButton = new QCheckBox("Heatmap", this);
    m_heatmapButton->setChecked(false);
    m_heatmapButton->setStyleSheet("QCheckBox { color : white; }");
    horizontalLayout2->addWidget(m_heatmapButton);
    horizontalLayout2->addSpacing(50);

    // Set the general layout
    this->setLayout(m_verticalBox);
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
    // Update the rendered pixmap
    int length = m_pixmap.width();
    // Crop the image depending on the zoom
    QRect rect(length/2 - zoom/2, length/2 - zoom/2, zoom, zoom);
    QPixmap cropped = m_pixmap.copy(rect);
    // Scaled the cropped image to fit the label
    cropped = cropped.scaled(m_label->size());
    // Render the pixmap
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

QLabel* View::getFpsLabel() const
{
    return fpsLabel;
}

QSlider* View::getZoomSlider() const
{
    return m_zoomSlider;
}

QLabel* View::getZoomLabel() const
{
    return zoomLabel;
}

QCheckBox* View::getHeatmapButton() const
{
    return m_heatmapButton;
}
