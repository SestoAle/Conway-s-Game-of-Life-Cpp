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
    // Change othe color pixel to white/black at position x,y
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
    // Change matrix value representing the life of cells
    m_matrix[x][y] = 0;
    // And update image depending on matrix values
    updateImage();
}

void Model::changePixelWhite(int x, int y)
{
    // Change matrix value representing the life of cells
    m_matrix[x][y] = 1;
    // And update image depending on matrix values
    updateImage();
}

void Model::initializeImage()
{
    // Initialize structures
    // First initialize the matri structure representing the life of all cells
    // Then initialize the image to be all black
    m_matrix = new int* [size];
    for(int i = 0; i < size; i ++)
    {
        m_matrix[i] = new int [size];
    }

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
    // Add padding to an image
    QImage padded{source.width() + 2*padWidth, source.height() + 2*padWidth, source.format()};
    padded.fill(padValue);
    QPainter p{&padded};
    p.drawImage(QPoint(padWidth, padWidth), source);
    return padded;
}

int** imageToBitmapMatrix(const QImage& source, bool withPadding = true)
{
    // Convert an image to a bitmap matrix

    QImage padded;
    // Add/not add padding
    if(withPadding)
    {
        padded = paddedImage(source, 1, 0);
    }
    else
    {
        padded = paddedImage(source, 0, 0);
    }
    // Initialize 2D matrix bitmap
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

int** Model::correlation(int** matrix)
{
    // Matrix correlation
    // Create 2D kernel
    int kernel[3][3] = {
        {2,2,2},
        {2,1,2},
        {2,2,2}
    };

    int** newMatrix = new int* [m_image->width()];
    for(int i = 0; i < m_image->width(); ++i)
        newMatrix[i] = new int[m_image->height()];

    // Make correlation
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
    // If the pixel is even, than the pixel was dead and the value/2 is the number of the neighboours
    // If the pixel is odd, than the pixel was alive and the (value - 1)/2 is the number of the neighbours
    // Map the pixel to the new stage
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
    // Sum of matrices
    for(int i = 0; i < size; i ++)
        for(int j = 0; j < size; j++)
        {
            if(dst[i][j] + source[i][j] < 255)
                dst[i][j] += source[i][j];
        }
}

void matrixMul(int** source, int** dst, int size)
{
    // Multiplication of matrix
    for(int i = 0; i < size; i ++)
        for(int j = 0; j < size; j++)
        {
            if(dst[i][j] * source[i][j] < 255)
                dst[i][j] *= source[i][j];
        }
}

void Model::updateImage()
{
    // Update the image depending on the life of the cells saved in matrix structure
    // Compute the color depending on the heatmap flag
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
    // Create a bitmap matrix from image
    int** matrix = imageToBitmapMatrix(*m_image);

    // Compute the new matrix with the cells new stage
    int** newMatrix = correlation(matrix);

    //  # Map the new image with the try_evolve() method
    newMatrix = tryEvolve(newMatrix);

    // Compute the pixel that was living on the previous frame, add them to the new image and re-render it
    // Multiply the old image with the new image. The result is the pixel that was alive in the previous frame
    matrixMul(newMatrix, m_matrix, size);
    // Add the still living image to the new image.
    // The result is the new state with the values of how long the cells are alive
    matrixSum(newMatrix, m_matrix, size);

    // Update the new values in the image
    updateImage();

    delete [] matrix;
    delete [] newMatrix;
}

void Model::saveImage()
{
    // Save the image saved in m_image structure
    m_image->save("myImage.png", "PNG");
}

bool Model::loadImage(QString name)
{
    // Load an image from resource and save it to m_image structure
    bool loaded = m_image->load(name);
    if(loaded)
    {
        m_matrix = imageToBitmapMatrix(*m_image, false);
        updateImage();
    }
    return loaded;
}




