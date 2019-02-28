#include "controller.h"

#include <iostream>
#include <QTimer>

using namespace std;

Controller::Controller(Model* model, View* view)
{
    m_model = model;
    m_view = view;

    model->fps = 1000/m_view->getSlider()->value();

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(applyFilter()));
    connect(view->getSlider(), SIGNAL(valueChanged(int)), this, SLOT(changeFps(int)));
}

Controller::~Controller()
{
    delete m_timer;
}

void Controller::setView(View *view)
{
    m_view = view;
}

void Controller::setModel(Model *model)
{
    m_model = model;
}

Model* Controller::getModel() const
{
    return m_model;
}

View* Controller::getView() const
{
    return m_view;
}

void Controller::initializeView()
{
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
}

int* Controller::computeImageCoordinates(int x, int y)
{
    // Convert click coordinates to cell coordinates
    int coordinates[2];
    int cellCoordX = int(float(x) / (float)m_view->getClickableLabel()->size().width() * (float)m_model->size);
    int cellCoordY = int(float(y) / (float)m_view->getClickableLabel()->size().height() * (float)m_model->size);

    coordinates[0] = cellCoordX;
    coordinates[1] = cellCoordY;

    return coordinates;
}

void Controller::cellPressed(int x, int y)
{
    int * coordinates = computeImageCoordinates(x,y);
    m_model->changePixel(coordinates[0], coordinates[1]);
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::cellLeftMove(int x, int y)
{
    int * coordinates = computeImageCoordinates(x,y);
    m_model->changePixelWhite(coordinates[0], coordinates[1]);
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::cellRightMove(int x, int y)
{
    int * coordinates = computeImageCoordinates(x,y);
    m_model->changePixelBlack(coordinates[0], coordinates[1]);
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::applyFilter()
{
    m_model->filter();
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::changeFps(int fps)
{
    m_model->fps = 1000/m_view->getSlider()->value();
    if(m_timer->isActive())
    {
        m_timer->stop();
        m_timer->start(m_model->fps);
    }
}


void Controller::startLive()
{
    if(m_timer->isActive())
    {
        m_view->getButton()->setText("Start");
        m_timer->stop();
    }
    else
    {
        m_view->getButton()->setText("Stop");
        m_timer->start(m_model->fps);
    }
}
