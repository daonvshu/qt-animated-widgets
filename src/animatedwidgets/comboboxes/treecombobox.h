#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/comboboxes/hierarchicalcombobox.h"

namespace animatedwidgets {

class TreeComboBox : public HierarchicalComboBox {
    Q_OBJECT
    Q_PROPERTY(bool searchable READ searchable WRITE setSearchable)

public:
    explicit TreeComboBox(QWidget* parent = nullptr);

    bool searchable() const;
    void setSearchable(bool enabled);

protected:
    ComboListPopup* createPopup() override;
    void configurePopup(ComboListPopup* popup) override;

private:
    bool m_searchable = true;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using TreeComboBox = animatedwidgets::TreeComboBox;
#endif
