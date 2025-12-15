#include "ip.h"
#include <QMenu>
#include <QFileDialog>
#include <QDebug>
#include <QPixmap>
#include <QMenuBar>

IP::IP(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowTitle(tr("Image Processor"));
    central = new QWidget();
    mainLayout = new QHBoxLayout(central);
    imgWin = new QLabel();
    QPixmap *initPixmap = new QPixmap(300, 200);
    initPixmap -> fill(QColor(255, 255, 255));
    imgWin -> resize(300, 200);
    imgWin -> setScaledContents(true);
    imgWin -> setPixmap(*initPixmap);
    mainLayout -> addWidget(imgWin);
    setCentralWidget(central);
    createActions();
    createMenus();
    createToolBars();
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
}

void IP::createMenus()
{

    fileMenu = menuBar() -> addMenu(tr("檔案"));
    fileMenu -> addAction(openFileAction);
    fileMenu -> addAction(exitAction);

    toolMenu = menuBar() -> addMenu(tr("工具"));
    toolMenu -> addAction(zoomInAction);
    toolMenu -> addAction(zoomOutAction);
}

void IP::createToolBars()
{
    fileTool = addToolBar("file");
    fileTool -> addAction(openFileAction);

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

