#ifndef CLICKABLELABEL_H
#define CLICKABLELABEL_H

#include <QLabel>

class ClickableLabel : public QLabel
{
    Q_OBJECT
public:
    ClickableLabel(QWidget* parent = nullptr);

protected:
    void mousePressEvent(QMouseEvent* event);
    void mouseMoveEvent(QMouseEvent* event);

signals:
    void clickEvent(int x, int y);
    void moveLeftEvent(int x, int y);
    void moveRightEvent(int x, int y);

public slots:

};

#endif // CLICKABLELABEL_H
