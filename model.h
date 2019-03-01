#ifndef MODEL_H
#define MODEL_H

#include <QObject>
#include <QImage>

class Model: public QObject
{
    Q_OBJECT
public:
    Model();
    ~Model();

    void setImage(QImage* image);
    void saveImage();
    bool loadImage(QString name);
    QImage* getImage() const;
    void changePixel(int x, int y);
    void changePixelWhite(int x, int y);
    void changePixelBlack(int x, int y);
    void renderImage();
    int size = 300;
    int fps = 30;
    bool heatmap = false;
    void filter();
    void initializeImage();

private:
    int** convolve(int** matrix);
    int** tryEvolve(int** matrix);

    int** m_matrix;
    QImage* m_image;
    QRgb black = qRgb(0,0,0);
    QRgb white = qRgb(255,255,255);
};

#endif // MODEL_H
