#include "controller.h"

#include <iostream>
#include <QTimer>
#include <QMessageBox>

using namespace std;

Controller::Controller(Model* model, View* view)
{
    m_model = model;
    m_view = view;

    model->fps = 1000/m_view->getFpsSlider()->value();
    model->heatmap = m_view->getHeatmapButton()->isChecked();

    connect(m_view->getClickableLabel(), SIGNAL(clickEvent(int, int)), this, SLOT(cellPressed(int, int)));
    connect(m_view->getClickableLabel(), SIGNAL(moveLeftEvent(int, int)), this, SLOT(cellLeftMove(int, int)));
    connect(m_view->getClickableLabel(), SIGNAL(moveRightEvent(int, int)), this, SLOT(cellRightMove(int, int)));
    connect(m_view->getStartButton(), SIGNAL(clicked()), this, SLOT(startLive()));
    connect(m_view->getClearButton(), SIGNAL(clicked()), this, SLOT(clear()));
    connect(m_view->getSaveButton(), SIGNAL(clicked()), this, SLOT(save()));
    connect(m_view->getLoadButton(), SIGNAL(clicked()), this, SLOT(load()));

    m_timer = new QTimer();
    connect(m_timer, SIGNAL(timeout()), this, SLOT(applyFilter()));
    connect(view->getFpsSlider(), SIGNAL(valueChanged(int)), this, SLOT(changeFps(int)));
    connect(view->getZoomSlider(), SIGNAL(valueChanged(int)), this, SLOT(changeZoom(int)));
    connect(view->getHeatmapButton(), SIGNAL(toggled(bool)), this, SLOT(changeHeatmap(bool)));

    initializeView();

    view->show();
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
    int zoom = m_view->zoom;
    int totalSize = m_model->size;
    int cellCoordX = int(float(x) / (float)m_view->getClickableLabel()->size().width() * (float)(zoom));
    int cellCoordY = int(float(y) / (float)m_view->getClickableLabel()->size().height() * (float)(zoom));

    coordinates[0] = cellCoordX + totalSize/2 - zoom/2;
    coordinates[1] = cellCoordY + totalSize/2 - zoom/2;

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
    m_model->fps = 1000/fps;
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
        m_view->getStartButton()->setText("Start");
        m_timer->stop();
    }
    else
    {
        m_view->getStartButton()->setText("Stop");
        m_timer->start(m_model->fps);
    }
}

void Controller::changeZoom(int zoom)
{
    m_view->zoom = zoom*2;
    m_view->updatePixmap();
}

void Controller::changeHeatmap(bool heatmap)
{
    m_model->heatmap = heatmap;
    m_model->renderImage();
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::clear()
{
    m_model->initializeImage();
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
    if(m_timer->isActive())
    {
        m_view->getStartButton()->setText("Start");
        m_view->getStartButton()->setChecked(false);
        m_timer->stop();
    }
}

void Controller::save()
{
    if(m_timer->isActive())
    {
        m_view->getStartButton()->setText("Start");
        m_view->getStartButton()->setChecked(false);
        m_timer->stop();
    }

    m_model->saveImage();
}

void Controller::load()
{
    if(m_timer->isActive())
    {
        m_view->getStartButton()->setText("Start");
        m_view->getStartButton()->setChecked(false);
        m_timer->stop();
    }

    QString loadResource;

    if(m_view->getDropList()->currentIndex() == 0)
    {
        loadResource = "myImage.png";
    }
    else
    {
        loadResource = ":img/" + m_view->getDropList()->currentText() + ".png";
    }

    if(m_model->loadImage(loadResource))
    {
        m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
        m_view->updatePixmap();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("Nothing to load!");
        msgBox.exec();
    }

}
