#include <QApplication>
#include <QVBoxLayout>
#include "view.h"
#include "controller.h"
#include "model.h"


int main(int argc, char ** argv)
{
    QApplication app(argc, argv);

    View view;
    Model model;

    Controller controller(&model, &view);
    controller.initializeView();

    QObject::connect(view.getClickableLabel(), SIGNAL(clickEvent(int, int)), &controller, SLOT(cellPressed(int, int)));
    QObject::connect(view.getClickableLabel(), SIGNAL(moveLeftEvent(int, int)), &controller, SLOT(cellLeftMove(int, int)));
    QObject::connect(view.getClickableLabel(), SIGNAL(moveRightEvent(int, int)), &controller, SLOT(cellRightMove(int, int)));
    QObject::connect(view.getButton(), SIGNAL(clicked()), &controller, SLOT(startLive()));


    view.show();

    app.exec();
    return 0;
}
