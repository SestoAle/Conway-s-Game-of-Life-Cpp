#include "model.h"
#include <iostream>
#include <QPainter>
#include <QMessageBox>
using namespace std;

Model::Model()
{
    initializeImage();
}

Model::~Model()
{
    delete m_image;
    delete[] m_matrix;
}

void Model::setImage(QImage *image)
{
    m_image = image;
}

QImage* Model::getImage() const
{
    return m_image;
}

void Model::changePixel(int x, int y)
{
    if(m_matrix[x][y] == 0)
    {
        changePixelWhite(x,y);
    }
    else
    {
        changePixelBlack(x,y);
    }
}

void Model::changePixelBlack(int x, int y)
{
    m_matrix[x][y] = 0;
    renderImage();
}

void Model::changePixelWhite(int x, int y)
{
    m_matrix[x][y] = 1;
    renderImage();
}

void Model::initializeImage()
{
    m_matrix = new int* [size];
    for(int i = 0; i < size; i ++)
    {
        m_matrix[i] = new int [size];
    }
    // Initialize the image to be all black
    m_image = new QImage(size, size, QImage::Format_Grayscale8);

    for(int i = 0; i < m_image->width(); i++)
    {
        for(int j = 0; j < m_image->height(); j++)
        {
            m_image->setPixel(i,j,black);
            m_matrix[i][j] = 0;
        }
    }
}

template <typename T>
QImage paddedImage(const QImage & source, int padWidth, T padValue) {
  QImage padded{source.width() + 2*padWidth, source.height() + 2*padWidth, source.format()};
  padded.fill(padValue);
  QPainter p{&padded};
  p.drawImage(QPoint(padWidth, padWidth), source);
  return padded;
}

int** imageToBitmapMatrix(const QImage& source, bool withPadding = true)
{
    QImage padded;
    if(withPadding)
    {
        padded = paddedImage(source, 1, 0);
    }
    else
    {
        padded = paddedImage(source, 0, 0);
    }
    int** matrix = new int* [padded.width()];
    for(int i = 0; i < padded.width(); ++i)
        matrix[i] = new int[padded.height()];

    for(int i = 0; i < padded.width(); i++)
    {
        for(int j = 0; j < padded.width(); j++)
        {
            if(qGray(padded.pixel(i,j)) > 0)
            {
                matrix[i][j] = 1;
            }
            else
            {
                matrix[i][j] = 0;
            }
        }
    }
    return matrix;
}

int** Model::convolve(int** matrix)
{
    // Create 2D kernel
    int kernel[3][3] = {
        {2,2,2},
        {2,1,2},
        {2,2,2}
    };

    int** newMatrix = new int* [m_image->width()];
    for(int i = 0; i < m_image->width(); ++i)
        newMatrix[i] = new int[m_image->height()];

    // Make Convolution
    for(int x = 1; x < m_image->width() + 1; x++)
        for(int y = 1; y < m_image->height() + 1; y++)
        {
            int value = 0;
            for(int i = -1; i <= 1; i ++)
                for (int j = -1; j <= 1; j++)
                {
                    value += matrix[x+i][y+j] * kernel[i+1][j+1];
                }

            newMatrix[x-1][y-1] = value;
        }
    return newMatrix;
}

int** Model::tryEvolve(int** matrix)
{
    for(int i = 0; i < m_image->width(); i++)
        for(int j =  0; j < m_image->height(); j++)
        {
            int pixel = matrix[i][j];
            int status = pixel % 2;
            if(status != 0)
            {
                if((pixel - status)/2 == 2 || (pixel - status) / 2 == 3)
                {
                    pixel = 1;
                }
                else
                {
                    pixel = 0;
                }
            }
            else
            {
                if (pixel / 2 == 3)
                {
                    pixel = 1;
                }
                else
                {
                    pixel = 0;
                }
            }
            matrix[i][j] = pixel;
        }

    return matrix;
}

void matrixSum(int** source, int** dst, int size)
{
    for(int i = 0; i < size; i ++)
        for(int j = 0; j < size; j++)
        {
            if(dst[i][j] + source[i][j] < 255)
                dst[i][j] += source[i][j];
        }
}

void matrixMul(int** source, int** dst, int size)
{
    for(int i = 0; i < size; i ++)
        for(int j = 0; j < size; j++)
        {
            if(dst[i][j] * source[i][j] < 255)
                dst[i][j] *= source[i][j];
        }
}

void Model::renderImage()
{
    for(int i = 0; i < m_image->width(); i ++)
        for(int j = 0; j < m_image->height(); j++)
        {
            int value = m_matrix[i][j];
            if(value >= 1)
            {
                if(heatmap)
                {
                    int grayValue = value*2 + 30;
                    if(grayValue > 255)
                        grayValue = 255;
                    uint grayLevel = qRgb(grayValue, grayValue, grayValue);
                    m_image->setPixel(i,j,grayLevel);
                }
                else
                    m_image->setPixel(i,j,white);
            }
            else
            {
                m_image->setPixel(i,j,black);
            }
        }
}

void Model::filter()
{
    int** matrix = imageToBitmapMatrix(*m_image);

    int** newMatrix = convolve(matrix);

    newMatrix = tryEvolve(newMatrix);

    matrixMul(newMatrix, m_matrix, size);
    matrixSum(newMatrix, m_matrix, size);

    renderImage();

    delete [] matrix;
    delete [] newMatrix;
}

void Model::saveImage()
{
    m_image->save("myImage.png", "PNG");
}

bool Model::loadImage(QString name)
{
    bool loaded = m_image->load(name);
    if(loaded)
    {
        m_matrix = imageToBitmapMatrix(*m_image, false);
        renderImage();
    }
    return loaded;
}




