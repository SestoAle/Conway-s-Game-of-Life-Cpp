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
#include <QCheckBox>

#include "clickablelabel.h"

// This class represent the view the user sees and can interact.
// It has no knowledge of the model and the method saved in it.
// It has a bunch of QWidget and display them.
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
    QComboBox* getDropList() const;
    QSlider* getFpsSlider() const;
    QLabel* getFpsLabel() const;
    QSlider* getZoomSlider() const;
    QLabel* getZoomLabel() const;
    QCheckBox* getHeatmapButton() const;
    int zoom = 100;

private:
    QWidget* m_mainWindow;
    QBoxLayout* m_verticalBox;
    QButtonGroup* m_buttonGroup;
    QPixmap m_pixmap;
    QPushButton* m_startButton;
    QPushButton* m_clearButton;
    QPushButton* m_saveButton;
    QComboBox* m_dropList;
    ClickableLabel* m_label;
    QSlider* m_fpsSlider;
    QLabel* fpsLabel;
    QSlider* m_zoomSlider;
    QLabel* zoomLabel;
    QCheckBox* m_heatmapButton;

protected:
    void resizeEvent(QResizeEvent* event);

signals:

public slots:
};

#endif // VIEW_H
