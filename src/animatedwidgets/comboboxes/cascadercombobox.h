#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/comboboxes/hierarchicalcombobox.h"

namespace animatedwidgets {

class CascaderComboBox : public HierarchicalComboBox {
    Q_OBJECT
    Q_PROPERTY(int columnWidth READ columnWidth WRITE setColumnWidth)
    Q_PROPERTY(bool showFullPath READ showFullPath WRITE setShowFullPath)
    Q_PROPERTY(QString pathSeparator READ pathSeparator WRITE setPathSeparator)

public:
    explicit CascaderComboBox(QWidget* parent = nullptr);

    int columnWidth() const;
    void setColumnWidth(int width);
    bool showFullPath() const;
    void setShowFullPath(bool enabled);
    QString pathSeparator() const;
    void setPathSeparator(const QString& separator);
    QStringList currentPathTexts() const;

protected:
    ComboListPopup* createPopup() override;
    void configurePopup(ComboListPopup* popup) override;
    QString displayText() const override;

private:
    QString m_pathSeparator = QStringLiteral(" / ");
    int m_columnWidth = 180;
    bool m_showFullPath = true;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using CascaderComboBox = animatedwidgets::CascaderComboBox;
#endif
