#ifndef TABMANAGER_H
#define TABMANAGER_H

#include <QTabWidget>
#include <QSplitter>

class TabManager : public QTabWidget
{
public:
    TabManager(QSplitter *parent);
private:
    void onCloseRequested(int index);
};

#endif // TABMANAGER_H