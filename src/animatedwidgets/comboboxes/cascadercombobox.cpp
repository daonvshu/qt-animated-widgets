#include "animatedwidgets/comboboxes/cascadercombobox.h"

#include "animatedwidgets/popups/cascaderpopupmenu.h"

namespace animatedwidgets {

CascaderComboBox::CascaderComboBox(QWidget* parent)
    : HierarchicalComboBox(parent)
{
    setPopupWidth(560);
}

int CascaderComboBox::columnWidth() const {
    return m_columnWidth;
}

void CascaderComboBox::setColumnWidth(int width) {
    m_columnWidth = qMax(80, width);
    if (auto* popup = qobject_cast<CascaderPopupMenu*>(currentPopup()))
        popup->setColumnWidth(m_columnWidth);
}

bool CascaderComboBox::showFullPath() const {
    return m_showFullPath;
}

void CascaderComboBox::setShowFullPath(bool enabled) {
    m_showFullPath = enabled;
    update();
}

QString CascaderComboBox::pathSeparator() const {
    return m_pathSeparator;
}

void CascaderComboBox::setPathSeparator(const QString& separator) {
    m_pathSeparator = separator;
    update();
}

QStringList CascaderComboBox::currentPathTexts() const {
    QStringList texts;
    for (const QModelIndex& index : currentPath())
        texts.append(index.sibling(index.row(), modelColumn()).data().toString());
    return texts;
}

ComboListPopup* CascaderComboBox::createPopup() {
    return new CascaderPopupMenu(this);
}

void CascaderComboBox::configurePopup(ComboListPopup* popup) {
    ComboBox::configurePopup(popup);
    auto* cascader = qobject_cast<CascaderPopupMenu*>(popup);
    if (!cascader)
        return;
    cascader->setLeafOnly(leafOnly());
    cascader->setColumnWidth(m_columnWidth);
    cascader->setCurrentModelIndex(currentModelIndex());
    cascader->setAdaptiveHeight(popupWidth() > 0 ? popupWidth() : width(), maxVisibleItems());
}

QString CascaderComboBox::displayText() const {
    return m_showFullPath && currentModelIndex().isValid()
        ? currentPathTexts().join(m_pathSeparator) : ComboBox::displayText();
}

} // namespace animatedwidgets
