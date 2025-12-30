#include "ip.h"
#include <QMenu>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QMenuBar>
#include <QRubberBand>

IP::IP(QWidget *parent)
    : QMainWindow(parent), rubberBand(nullptr), selecting(false)
{
    setWindowTitle(tr("Image Processor"));



    central = new QWidget();
    mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(600, 400);
    gWin = new IT();
    initPixmap -> fill(QColor(255, 255, 255));
    imgWin -> resize(300, 200);
    imgWin -> setScaledContents(true);
    imgWin -> setPixmap(*initPixmap);
    mainLayout -> addWidget(imgWin);
    setCentralWidget(central);
    createActions();
    createMenus();
    createToolBars();

    statusLabel = new QLabel;
    statusLabel -> setFixedWidth(100);
    MousePosLabel = new QLabel;
    MousePosLabel -> setText(tr("指標位置："));
    MousePosLabel -> setFixedWidth(200);
    statusBar() -> addPermanentWidget(statusLabel);
    statusBar() -> addPermanentWidget(MousePosLabel);
    setMouseTracking(true);
    imgWin->setMouseTracking(true);
    central->setMouseTracking(true);

}

IP::~IP() {}

void IP::createActions()
{
    openFileAction = new QAction(tr("開啟檔案&O"), this);
    openFileAction -> setShortcut(tr("Ctrl + O"));
    openFileAction -> setStatusTip(tr("開啟影像檔案"));
    connect(openFileAction, SIGNAL(triggered(bool)), this, SLOT(showOpenFile()));

    exitAction = new QAction(tr("結束&Q"), this);
    exitAction -> setShortcut(tr("ctrl + Q"));
    exitAction -> setStatusTip(tr("退出程式"));
    connect(exitAction, SIGNAL(triggered(bool)), this, SLOT(close()));

    zoomInAction = new QAction(tr("放大"), this);
    zoomInAction -> setShortcut(tr("ctrl + ="));
    connect(zoomInAction, SIGNAL(triggered(bool)), this, SLOT(windowZoomIn()));

    zoomOutAction = new QAction(tr("縮小"), this);
    zoomOutAction -> setShortcut(tr("ctrl + -"));
    connect(zoomOutAction, SIGNAL(triggered(bool)), this, SLOT(windowZoomOut()));

    geometryAction = new QAction(tr("幾何轉換"), this);
    geometryAction -> setShortcut(tr("ctrl + G"));
    geometryAction -> setStatusTip(tr("影像幾何轉換"));
    connect(geometryAction, SIGNAL(triggered(bool)), this, SLOT(showGeometryTransform()));
    connect(exitAction, SIGNAL(triggered(bool)), gWin, SLOT(close()));

}

void IP::createMenus()
{

    fileMenu = menuBar() -> addMenu(tr("檔案"));
    fileMenu -> addAction(openFileAction);
    fileMenu -> addAction(exitAction);
    fileMenu -> addAction(geometryAction);

    toolMenu = menuBar() -> addMenu(tr("工具"));
    toolMenu -> addAction(zoomInAction);
    toolMenu -> addAction(zoomOutAction);
}

void IP::createToolBars()
{
    fileTool = addToolBar("file");
    fileTool -> addAction(openFileAction);
    fileTool -> addAction(geometryAction);

    toolBar = addToolBar("tool");
    toolBar -> addAction(zoomInAction);
    toolBar -> addAction(zoomOutAction);

}

void IP::loadFile(QString filename)
{
    qDebug() << QString("file name:%1").arg(filename);
    QByteArray ba = filename.toLatin1();
    printf("FN:%s\n", (char *) ba.data());
    img.load(filename);
    imgWin -> setPixmap(QPixmap::fromImage(img));
}

void IP::showOpenFile()
{
    filename = QFileDialog::getOpenFileName(this, tr("開啟影像"), tr("."), "bmp(*.bmp);;png(*.png);;Jpeg(*.jpg)");
    if (!filename.isEmpty())
    {
        if(img.isNull())
        {
            loadFile(filename);
        }
        else
        {
            IP *newIPWin = new IP();
            newIPWin -> show();
            newIPWin -> loadFile(filename);
        }
    }
}

void IP::windowZoomOut()
{
    QImage zoomOuted;
    zoomOuted = img.scaled(img.width() / 2, img.height() / 2);
    QLabel *ret = new QLabel();
    ret -> setPixmap(QPixmap::fromImage(zoomOuted));
    ret -> show();
}

void IP::windowZoomIn()
{
    QImage zoomOuted;
    zoomOuted = img.scaled(img.width() * 2, img.height() * 2);
    QLabel *ret = new QLabel();
    ret -> setPixmap(QPixmap::fromImage(zoomOuted));
    ret -> show();
}

void IP::showGeometryTransform()
{
    qDebug()<<"test";
    if (!img.isNull())
    {
        gWin->srcImg = img;
        gWin -> inWin -> setPixmap(QPixmap::fromImage(gWin -> srcImg));
        gWin -> show();
    }
}

void IP::mouseMoveEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event -> x()) + ", " + QString::number(event -> y()) + ")";
    int x = event -> x() - imgWin -> x() - central -> x();
    int y = event -> y() - imgWin -> y() - central -> y();
    if (!img.isNull() && x >= 0 && x < imgWin -> width() && y >= 0 && y < imgWin -> height())
    {
        int gray = qGray(img.pixel(x, y));
        str += (" = " + QString::number(gray));
    }
    MousePosLabel -> setText("指標位置：" + str);
    
    // Update rubber band during selection
    if (selecting && rubberBand)
    {
        rubberBand->setGeometry(QRect(selectionOrigin, event->pos()).normalized());
    }
}


void IP::mousePressEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event -> x()) + ", " + QString::number(event -> y()) + ")";
    if (event -> button() == Qt::LeftButton)
    {
        statusBar() -> showMessage(tr("左鍵：") + str);
        
        // Check if click is within imgWin and image is loaded
        int x = event->x() - imgWin->x() - central->x();
        int y = event->y() - imgWin->y() - central->y();
        
        if (!img.isNull() && x >= 0 && x < imgWin->width() && y >= 0 && y < imgWin->height())
        {
            selecting = true;
            selectionOrigin = event->pos();
            
            if (!rubberBand)
            {
                rubberBand = new QRubberBand(QRubberBand::Rectangle, this);
            }
            rubberBand->setGeometry(QRect(selectionOrigin, QSize()));
            rubberBand->show();
        }
    }
    else if (event -> button() == Qt::RightButton)
    {
        statusBar() -> showMessage(tr("右鍵：") + str);
    }
    else if(event -> button() == Qt::MiddleButton)
    {
        statusBar() -> showMessage(tr("中鍵：") + str);
    }
}

void IP::mouseReleaseEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event -> x()) + ", " + QString::number(event -> y()) + ")";
    statusBar() -> showMessage(tr("釋放：") + str);
    
    if (selecting && rubberBand)
    {
        rubberBand->hide();
        selecting = false;
        
        // Get selection rectangle in window coordinates
        QRect selectionRect = rubberBand->geometry();
        
        // Convert to imgWin coordinates
        int imgWinX = imgWin->x() + central->x();
        int imgWinY = imgWin->y() + central->y();
        
        QRect imgWinRect(imgWinX, imgWinY, imgWin->width(), imgWin->height());
        QRect intersection = selectionRect.intersected(imgWinRect);
        
        // Check if we have a valid selection within the image
        if (!intersection.isEmpty() && intersection.width() > 10 && intersection.height() > 10)
        {
            // Convert to imgWin coordinates
            int x1 = intersection.x() - imgWinX;
            int y1 = intersection.y() - imgWinY;
            int w = intersection.width();
            int h = intersection.height();
            
            // Check bounds against imgWin dimensions (displayed image size)
            if (x1 >= 0 && y1 >= 0 && x1 + w <= imgWin->width() && y1 + h <= imgWin->height())
            {
                qDebug() << "Opening ImageEditor with region (widget coords):" << x1 << y1 << w << h;
                
                // Extract region from the displayed pixmap at display size
                QPixmap pixmap = imgWin->pixmap(Qt::ReturnByValue);
                if (!pixmap.isNull())
                {
                    // Scale pixmap to match the display size of imgWin
                    // This ensures we capture at the displayed resolution, not original resolution
                    QPixmap scaledPixmap = pixmap.scaled(imgWin->width(), imgWin->height(), 
                                                         Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
                    
                    // Ensure coordinates are within scaled pixmap bounds
                    if (x1 >= 0 && y1 >= 0 && x1 + w <= scaledPixmap.width() && y1 + h <= scaledPixmap.height())
                    {
                        // Extract the selected region from scaled pixmap
                        QPixmap croppedPixmap = scaledPixmap.copy(x1, y1, w, h);
                        
                        // Validate that the copy succeeded
                        if (!croppedPixmap.isNull())
                        {
                            QImage croppedImage = croppedPixmap.toImage();
                            
                            // Open ImageEditor with the cropped image
                            ImageEditor *editor = new ImageEditor(croppedImage);
                            editor->setAttribute(Qt::WA_DeleteOnClose);
                            editor->show();
                            
                            qDebug() << "ImageEditor window opened with display-size region:" << w << "x" << h;
                        }
                        else
                        {
                            qDebug() << "Failed to copy pixmap region:" << x1 << y1 << w << h;
                        }
                    }
                    else
                    {
                        qDebug() << "Selection out of scaled pixmap bounds:" << x1 << y1 << w << h << "Scaled size:" << scaledPixmap.width() << scaledPixmap.height();
                    }
                }
            }
            else
            {
                qDebug() << "Selection out of bounds:" << x1 << y1 << w << h << "imgWin size:" << imgWin->width() << imgWin->height();
            }
        }
        else
        {
            qDebug() << "Selection too small or empty:" << intersection.width() << "x" << intersection.height();
        }
    }
}

void IP::mouseDoubleClickEvent(QMouseEvent *event)
{
    QString str = "(" + QString::number(event -> x()) + ", " + QString::number(event -> y()) + ")";
    statusBar() -> showMessage(tr("雙擊") + str);
}

