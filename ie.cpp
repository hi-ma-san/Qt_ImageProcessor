#include "ie.h"
#include <QColorDialog>
#include <QFileDialog>
#include <QPixmap>
#include <QPainter>
#include <QDebug>

ImageEditor::ImageEditor(const QImage &image, QWidget *parent)
    : QWidget(parent), originalImage(image), currentImage(image), penColor(Qt::red), drawing(false)
{
    setWindowTitle(tr("圖片編輯器"));
    setupUI();
    redrawImage();
}

ImageEditor::~ImageEditor() {}

void ImageEditor::setupUI()
{
    mainLayout = new QVBoxLayout(this);
    buttonLayout = new QHBoxLayout();
    
    // Create buttons
    colorButton = new QPushButton(tr("變更顏色"), this);
    rotateLeftButton = new QPushButton(tr("向左旋轉"), this);
    rotateRightButton = new QPushButton(tr("向右旋轉"), this);
    saveButton = new QPushButton(tr("存檔"), this);
    
    // Add buttons to layout
    buttonLayout->addWidget(colorButton);
    buttonLayout->addWidget(rotateLeftButton);
    buttonLayout->addWidget(rotateRightButton);
    buttonLayout->addWidget(saveButton);
    
    // Create image label
    imageLabel = new QLabel(this);
    imageLabel->setScaledContents(false);
    imageLabel->setAlignment(Qt::AlignCenter);
    imageLabel->setMouseTracking(true);
    
    // Add widgets to main layout
    mainLayout->addLayout(buttonLayout);
    mainLayout->addWidget(imageLabel);
    
    // Connect signals
    connect(colorButton, &QPushButton::clicked, this, &ImageEditor::changeColor);
    connect(rotateLeftButton, &QPushButton::clicked, this, &ImageEditor::rotateLeft);
    connect(rotateRightButton, &QPushButton::clicked, this, &ImageEditor::rotateRight);
    connect(saveButton, &QPushButton::clicked, this, &ImageEditor::saveImage);
    
    setMouseTracking(true);
    
    // Set initial size
    resize(originalImage.width() + 50, originalImage.height() + 100);
}

void ImageEditor::drawStrokesToPainter(QPainter &painter, const QVector<QVector<QPoint>> &strokes, const QVector<QColor> &colors)
{
    for (int i = 0; i < strokes.size(); ++i)
    {
        if (strokes[i].size() > 1)
        {
            painter.setPen(QPen(colors[i], 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
            for (int j = 1; j < strokes[i].size(); ++j)
            {
                painter.drawLine(strokes[i][j-1], strokes[i][j]);
            }
        }
    }
}

void ImageEditor::redrawImage()
{
    QImage displayImage = currentImage.copy();
    QPainter painter(&displayImage);
    
    // Draw all previous strokes
    drawStrokesToPainter(painter, allStrokes, strokeColors);
    
    // Draw current stroke
    if (currentStroke.size() > 1)
    {
        painter.setPen(QPen(penColor, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin));
        for (int i = 1; i < currentStroke.size(); ++i)
        {
            painter.drawLine(currentStroke[i-1], currentStroke[i]);
        }
    }
    
    imageLabel->setPixmap(QPixmap::fromImage(displayImage));
}

void ImageEditor::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        QPoint pos = event->pos();
        QPoint labelPos = imageLabel->mapFrom(this, pos);
        
        // Check if click is within image bounds
        QPixmap pixmap = imageLabel->pixmap(Qt::ReturnByValue);
        if (!pixmap.isNull())
        {
            QRect imageRect = pixmap.rect();
            if (imageRect.contains(labelPos))
            {
                drawing = true;
                lastPoint = labelPos;
                currentStroke.clear();
                currentStroke.append(labelPos);
            }
        }
    }
}

void ImageEditor::mouseMoveEvent(QMouseEvent *event)
{
    if (drawing && (event->buttons() & Qt::LeftButton))
    {
        QPoint pos = event->pos();
        QPoint labelPos = imageLabel->mapFrom(this, pos);
        
        // Check if position is within image bounds
        QPixmap pixmap = imageLabel->pixmap(Qt::ReturnByValue);
        if (!pixmap.isNull())
        {
            QRect imageRect = pixmap.rect();
            if (imageRect.contains(labelPos))
            {
                currentStroke.append(labelPos);
                lastPoint = labelPos;
                redrawImage();
            }
        }
    }
}

void ImageEditor::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton && drawing)
    {
        drawing = false;
        if (currentStroke.size() > 0)
        {
            allStrokes.append(currentStroke);
            strokeColors.append(penColor);
            
            // Apply the last stroke to current image
            QPainter painter(&currentImage);
            QVector<QVector<QPoint>> lastStrokeVec;
            QVector<QColor> lastColorVec;
            lastStrokeVec.append(allStrokes.last());
            lastColorVec.append(penColor);
            drawStrokesToPainter(painter, lastStrokeVec, lastColorVec);
            
            currentStroke.clear();
        }
        redrawImage();
    }
}

void ImageEditor::changeColor()
{
    QColor color = QColorDialog::getColor(penColor, this, tr("選擇畫筆顏色"));
    if (color.isValid())
    {
        penColor = color;
    }
}

void ImageEditor::rotateLeft()
{
    // Rotate current image 90 degrees counter-clockwise (-90 degrees)
    QTransform transform;
    transform.rotate(-90);
    currentImage = currentImage.transformed(transform);
    
    // Clear all strokes as they don't match the rotated image
    allStrokes.clear();
    strokeColors.clear();
    currentStroke.clear();
    
    redrawImage();
    
    // Adjust window size
    resize(currentImage.width() + 50, currentImage.height() + 100);
}

void ImageEditor::rotateRight()
{
    // Rotate current image 90 degrees clockwise
    QTransform transform;
    transform.rotate(90);
    currentImage = currentImage.transformed(transform);
    
    // Clear all strokes as they don't match the rotated image
    allStrokes.clear();
    strokeColors.clear();
    currentStroke.clear();
    
    redrawImage();
    
    // Adjust window size
    resize(currentImage.width() + 50, currentImage.height() + 100);
}

void ImageEditor::saveImage()
{
    QString filename = QFileDialog::getSaveFileName(this, 
        tr("儲存圖片"), 
        tr("."), 
        tr("PNG Files (*.png);;JPEG Files (*.jpg);;BMP Files (*.bmp)"));
    
    if (!filename.isEmpty())
    {
        // Create final image with all drawings
        QImage finalImage = currentImage.copy();
        QPainter painter(&finalImage);
        
        // Draw all strokes
        drawStrokesToPainter(painter, allStrokes, strokeColors);
        
        bool success = finalImage.save(filename);
        if (success)
        {
            qDebug() << "Image saved successfully to:" << filename;
        }
        else
        {
            qDebug() << "Failed to save image to:" << filename;
        }
    }
}
