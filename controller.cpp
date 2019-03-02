#include "controller.h"

#include <iostream>
#include <QTimer>
#include <QMessageBox>

using namespace std;

Controller::Controller(Model* model, View* view)
{
    // Initialize controller
    m_model = model;
    m_view = view;

    model->fps = 1000/m_view->getFpsSlider()->value();
    model->heatmap = m_view->getHeatmapButton()->isChecked();
    m_timer = new QTimer();

    // Connect all the slots to all the signal in view class
    connect(m_view->getClickableLabel(), SIGNAL(clickEvent(int, int)), this, SLOT(cellPressed(int, int)));
    connect(m_view->getClickableLabel(), SIGNAL(moveLeftEvent(int, int)), this, SLOT(cellLeftMove(int, int)));
    connect(m_view->getClickableLabel(), SIGNAL(moveRightEvent(int, int)), this, SLOT(cellRightMove(int, int)));
    connect(m_view->getStartButton(), SIGNAL(clicked()), this, SLOT(startLive()));
    connect(m_view->getClearButton(), SIGNAL(clicked()), this, SLOT(clear()));
    connect(m_view->getSaveButton(), SIGNAL(clicked()), this, SLOT(save()));
    connect(m_view->getDropList(), SIGNAL(activated(int)), this, SLOT(load()));
    connect(m_timer, SIGNAL(timeout()), this, SLOT(applyFilter()));
    connect(view->getFpsSlider(), SIGNAL(valueChanged(int)), this, SLOT(changeFps(int)));
    connect(view->getZoomSlider(), SIGNAL(valueChanged(int)), this, SLOT(changeZoom(int)));
    connect(view->getHeatmapButton(), SIGNAL(toggled(bool)), this, SLOT(changeHeatmap(bool)));
    // Initialize the view
    initializeView();
    // Show the view
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
    // Link the m_image in the model to the m_pixmap in the view
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
}

int* Controller::computeImageCoordinates(int x, int y)
{
    // Convert click coordinates to cell coordinates depending on the zoom saved in the view
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
    // Manage the click event
    int * coordinates = computeImageCoordinates(x,y);
    m_model->changePixel(coordinates[0], coordinates[1]);
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::cellLeftMove(int x, int y)
{
    // Manage the click-left-and-mode event
    int * coordinates = computeImageCoordinates(x,y);
    m_model->changePixelWhite(coordinates[0], coordinates[1]);
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::cellRightMove(int x, int y)
{
    // Manage the click-right-and-mode event
    int * coordinates = computeImageCoordinates(x,y);
    m_model->changePixelBlack(coordinates[0], coordinates[1]);
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::applyFilter()
{
    // Apply one step of evolution, get the new image and render it in the view
    m_model->filter();
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::changeFps(int fps)
{
    // Manage the change FPS event
    m_model->fps = 1000/fps;
    QString string = "Fps: " + QString::number(fps);
    m_view->getFpsLabel()->setText(string);
    if(m_timer->isActive())
    {
        m_timer->stop();
        m_timer->start(m_model->fps);
    }
}

void Controller::startLive()
{
    // Manage the start/pause button event
    if(m_timer->isActive())
    {
        m_view->getStartButton()->setText("Start");
        m_timer->stop();
    }
    else
    {
        m_view->getStartButton()->setText("Pause");
        m_timer->start(m_model->fps);
    }
}

void Controller::changeZoom(int zoom)
{
    // Manage the change zoom event
    m_view->zoom = zoom*2;
    QString string = "Cells: " + QString::number(zoom*2) + "x" + QString::number(zoom*2);
    m_view->getZoomLabel()->setText(string);
    m_view->updatePixmap();
}

void Controller::changeHeatmap(bool heatmap)
{
    // Manage the heatmap toggle
    m_model->heatmap = heatmap;
    m_model->updateImage();
    m_view->setPixmap(QPixmap::fromImage(*(m_model->getImage())));
    m_view->updatePixmap();
}

void Controller::clear()
{
    // Manage the clear button event
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
    // Manage the save button event
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
    // Manage the load droplist event
    if(m_timer->isActive())
    {
        m_view->getStartButton()->setText("Start");
        m_view->getStartButton()->setChecked(false);
        m_timer->stop();
    }

    QString loadResource;

    if(m_view->getDropList()->currentIndex() == 1)
    {
        loadResource = "myImage.png";
    }
    else if(m_view->getDropList()->currentIndex() > 1)
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

    m_view->getDropList()->setCurrentIndex(0);

}
