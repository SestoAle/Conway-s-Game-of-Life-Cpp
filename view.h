#ifndef VIEW_H
#define VIEW_H

#include <QWidget>
#include <QButtonGroup>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
#include <QLabel>
#include <QSlider>
#include <QRadioButton>
#include <QComboBox>

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
    QPushButton* getStartButton() const;
    QPushButton* getClearButton() const;
    QPushButton* getSaveButton() const;
    QPushButton* getLoadButton() const;
    QComboBox* getDropList() const;
    QSlider* getFpsSlider() const;
    QSlider* getZoomSlider() const;
    QRadioButton* getHeatmapButton() const;
    int zoom = 100;

private:
    QWidget* m_mainWindow;
    QBoxLayout* m_verticalBox;
    QButtonGroup* m_buttonGroup;
    QPixmap m_pixmap;
    QPushButton* m_startButton;
    QPushButton* m_clearButton;
    QPushButton* m_saveButton;
    QPushButton* m_loadButton;
    QComboBox* m_dropList;
    ClickableLabel* m_label;
    QSlider* m_fpsSlider;
    QSlider* m_zoomSlider;
    QRadioButton* m_heatmapButton;

protected:
    void resizeEvent(QResizeEvent* event);

signals:

public slots:
};

#endif // VIEW_H
