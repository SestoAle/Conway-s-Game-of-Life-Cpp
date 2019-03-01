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

    app.exec();
    return 0;
}
