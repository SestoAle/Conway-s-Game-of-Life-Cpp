#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
#include <QLabel>
#include <QSlider>

#include "clickablelabel.h"

class View : public QWidget
{
    Q_OBJECT
public:
    explicit View(QWidget *parent = nullptr);
    ~View();
    void setPixmap(QPixmap pixmap);
    void updatePixmap();
    ClickableLabel* getClickableLabel() const;
    QPushButton* getButton() const;
    QSlider* getSlider() const;

private:
    QWidget* m_mainWindow;
    QBoxLayout* m_verticalBox;
    QButtonGroup* m_buttonGroup;
    QPixmap m_pixmap;
    QPushButton* m_startButton;
    ClickableLabel* m_label;
    QSlider* m_slider;

protected:
    void resizeEvent(QResizeEvent* event);

signals:

public slots:
};

#endif // VIEW_H
