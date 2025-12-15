#ifndef IP_H
#define IP_H

#include <QMainWindow>
#include <QAction>
#include <QMenu>
#include <QToolBar>
#include <QImage>
#include <QLabel>
#include <QHBoxLayout>

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
    QHBoxLayout *mainLayout;   // ★ 存 layout

private slots:
    void showOpenFile();
    void windowZoomIn();
    void windowZoomOut();
};
#endif // IP_H
