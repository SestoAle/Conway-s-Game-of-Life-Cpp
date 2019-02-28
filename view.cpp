#include "view.h"
#include <iostream>
#include <QRgb>
#include <QColor>
#include <QResizeEvent>
#include <QEvent>
using namespace std;

View::View(QWidget *parent) : QWidget(parent)
{
    this->setFixedSize(500, 500);

    // Create vertical layout container for the image
    m_verticalBox = new QVBoxLayout(this);

    m_startButton = new QPushButton("Start", this);
    m_startButton->setCheckable(true);

    m_verticalBox->addWidget(m_startButton);

    // Create clickable label and set the image for the first time
    m_label = new ClickableLabel(this);
    m_verticalBox->addWidget(m_label);

    m_slider = new QSlider(Qt::Horizontal,this);
    m_verticalBox->addWidget(m_slider);
    m_slider->setRange(10,60);
    m_slider->setValue(30);
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
    m_pixmap = m_pixmap.scaled(m_label->size());
    m_label->setPixmap(m_pixmap);
}

ClickableLabel* View::getClickableLabel() const
{
    return m_label;
}

QPushButton* View::getButton() const
{
    return m_startButton;
}

QSlider* View::getSlider() const
{
    return m_slider;
}
