#include "model.h"
#include <iostream>
#include <QPainter>
using namespace std;

Model::Model()
{
    initializeImage();
}

Model::~Model()
{
    delete m_image;
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
    if(m_image->pixel(x,y) == black)
    {
        m_image->setPixel(x, y, white);
    }
    else
    {
        m_image->setPixel(x, y, black);
    }
}

void Model::changePixelBlack(int x, int y)
{
    m_image->setPixel(x, y, black);
}

void Model::changePixelWhite(int x, int y)
{
    m_image->setPixel(x, y, white);
}

void Model::initializeImage()
{
    // Initialize the image to be all black
    m_image = new QImage(size, size, QImage::Format_Grayscale8);

    for(int i = 0; i < m_image->width(); i++)
    {
        for(int j = 0; j < m_image->height(); j++)
        {
            m_image->setPixel(i,j,black);
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

int** imageToBitmapMatrix(const QImage& source)
{
    QImage padded = paddedImage(source, 1, 0);
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

void Model::tryEvolve(int** matrix)
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
            if(pixel == 1)
            {
                m_image->setPixel(i,j, white);
            }
            else
            {
                m_image->setPixel(i,j, black);
            }
        }
}

void Model::filter()
{
    int** matrix = imageToBitmapMatrix(*m_image);

    int** newMatrix = convolve(matrix);

    tryEvolve(newMatrix);

    delete [] matrix;
    delete [] newMatrix;
}


