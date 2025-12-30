#ifndef IE_H
#define IE_H

#include <QWidget>
#include <QImage>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QPainter>
#include <QPoint>
#include <QVector>
#include <QColor>

class ImageEditor : public QWidget
{
    Q_OBJECT

public:
    ImageEditor(const QImage &image, QWidget *parent = nullptr);
    ~ImageEditor();

private:
    QImage originalImage;
    QImage currentImage;
    QLabel *imageLabel;
    QPushButton *colorButton;
    QPushButton *rotateLeftButton;
    QPushButton *rotateRightButton;
    QPushButton *saveButton;
    QVBoxLayout *mainLayout;
    QHBoxLayout *buttonLayout;
    
    QColor penColor;
    bool drawing;
    QPoint lastPoint;
    QVector<QVector<QPoint>> allStrokes;
    QVector<QColor> strokeColors;
    QVector<QPoint> currentStroke;
    
    void setupUI();
    void redrawImage();
    void drawStrokesToPainter(QPainter &painter, const QVector<QVector<QPoint>> &strokes, const QVector<QColor> &colors);
    
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private slots:
    void changeColor();
    void rotateLeft();
    void rotateRight();
    void saveImage();
};

#endif // IE_H
