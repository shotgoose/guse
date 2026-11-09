#include "tabmanager.h"

TabManager::TabManager(QSplitter *parent)
{
    setTabsClosable(true);
    setMovable(true);

    connect(this, &QTabWidget::tabCloseRequested, this, &TabManager::onCloseRequested);
}

void TabManager::onCloseRequested(int index)
{
    QWidget *page = widget(index);
    removeTab(index);
    page->deleteLater();
}