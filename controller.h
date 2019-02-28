#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QObject>
#include <QPixmap>
#include "view.h"
#include "model.h"

class Controller: public QObject
{
    Q_OBJECT
public:
    Controller(Model* model, View* view);
    ~Controller();

    void setView(View* view);
    View* getView () const;
    void setModel(Model* model);
    Model* getModel() const;
    void initializeView();

private:
    View* m_view;
    Model* m_model;
    QTimer* m_timer;
    int* computeImageCoordinates(int x, int y);

public slots:
    void cellPressed(int x, int y);
    void cellLeftMove(int x, int y);
    void cellRightMove(int x, int y);
    void applyFilter();
    void startLive();
    void changeFps(int fps);
};

#endif // CONTROLLER_H
