#ifndef IP_H
#define IP_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>
#include "it.h"

#include <QMouseEvent>
#include <QStatusBar>
#include <QLabel>
#include <QRubberBand>
#include <QPoint>
#include "ie.h"


class IP : public QMainWindow
{
    Q_OBJECT

public:
    IP(QWidget *parent = nullptr);
    ~IP();

    void createActions();
    void createMenus();
    void createToolBars();
    void loadFile(QString filename);


private:
    QWidget     *central;
    QMenu       *fileMenu;
    QMenu       *toolMenu;
    QToolBar    *fileTool;
    QToolBar    *toolBar;
    QImage      img;
    QString     filename;
    QLabel      *imgWin;
    QAction     *openFileAction;
    QAction     *exitAction;
    QAction     *zoomInAction;
    QAction     *zoomOutAction;
    QHBoxLayout *mainLayout;

    IT *gWin;
    QAction *geometryAction;

    QLabel  *statusLabel;
    QLabel  *MousePosLabel;
    
    // Selection rectangle for region capture
    QRubberBand *rubberBand;
    QPoint selectionOrigin;
    bool selecting;

private slots:
    void showOpenFile();
    void windowZoomIn();
    void windowZoomOut();
    void showGeometryTransform();

protected:
    void mouseDoubleClickEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
};
#endif // IP_H
