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
    QImage* getImage() const;
    void changePixel(int x, int y);
    void changePixelWhite(int x, int y);
    void changePixelBlack(int x, int y);
    int size = 100;
    int fps = 30;
    void filter();

private:
    void initializeImage();
    int** convolve(int** matrix);
    void tryEvolve(int** matrix);

    QImage* m_image;
    QRgb black = qRgb(0,0,0);
    QRgb white = qRgb(255,255,255);
};

#endif // MODEL_H
