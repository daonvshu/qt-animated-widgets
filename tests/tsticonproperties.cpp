#include "animatedwidgets/buttons/variantbuttons.h"
#include "animatedwidgets/calendars/calendarstyle.h"
#include "animatedwidgets/comboboxes/cascadercombobox.h"
#include "animatedwidgets/comboboxes/compositecombobox.h"
#include "animatedwidgets/comboboxes/creatablecombobox.h"
#include "animatedwidgets/comboboxes/multiselectcombobox.h"
#include "animatedwidgets/comboboxes/treecombobox.h"
#include "animatedwidgets/inputs/compositedoublespinbox.h"
#include "animatedwidgets/inputs/compositelineedit.h"
#include "animatedwidgets/inputs/compositespinbox.h"
#include "animatedwidgets/inputs/doublespinbox.h"
#include "animatedwidgets/inputs/passwordlineedit.h"
#include "animatedwidgets/inputs/spinbox.h"
#include "animatedwidgets/popups/cascaderpopupmenu.h"
#include "animatedwidgets/popups/defaultpopupmenuliststyle.h"
#include "animatedwidgets/popups/searchpopupmenu.h"
#include "animatedwidgets/popups/treepopupmenu.h"
#include "animatedwidgets/switches/switch.h"
#include "animatedwidgets/tags/tagwidget.h"

#include <QMetaProperty>
#include <QtTest>

class IconPropertiesTest : public QObject {
    Q_OBJECT

private slots:
    void discoverableProperties();
    void originalColorsByDefault();
};

void IconPropertiesTest::discoverableProperties() {
    using namespace animatedwidgets;
    const QMetaObject* controls[] = {
        &FilledButton::staticMetaObject, &OutlineButton::staticMetaObject,
        &BorderedButton::staticMetaObject, &HoverButton::staticMetaObject,
        &FilledIconButton::staticMetaObject, &OutlineIconButton::staticMetaObject,
        &BorderedIconButton::staticMetaObject, &HoverIconButton::staticMetaObject,
        &CheckBox::staticMetaObject, &RadioButton::staticMetaObject, &Switch::staticMetaObject,
        &IconLineEdit::staticMetaObject, &PasswordLineEdit::staticMetaObject,
        &SpinBox::staticMetaObject, &DoubleSpinBox::staticMetaObject,
        &CompositeLineEdit::staticMetaObject, &CompositeSpinBox::staticMetaObject,
        &CompositeDoubleSpinBox::staticMetaObject, &ComboBox::staticMetaObject,
        &SearchComboBox::staticMetaObject, &CreatableComboBox::staticMetaObject,
        &TreeComboBox::staticMetaObject, &CascaderComboBox::staticMetaObject,
        &MultiSelectComboBox::staticMetaObject, &CompositeComboBox::staticMetaObject,
        &TagWidget::staticMetaObject, &PopupMenu::staticMetaObject,
        &SearchPopupMenu::staticMetaObject, &TreePopupMenu::staticMetaObject,
        &CascaderPopupMenu::staticMetaObject
    };

    for (const QMetaObject* meta : controls) {
        const int index = meta->indexOfProperty("iconHint");
        QVERIFY2(index >= 0, meta->className());
        const QMetaProperty property = meta->property(index);
        QCOMPARE(property.userType(), int(QMetaType::Bool));
        QVERIFY2(property.isReadable() && property.isWritable(), meta->className());
        QVERIFY2(property.isDesignable() && property.isStored(), meta->className());
    }
}

void IconPropertiesTest::originalColorsByDefault() {
    const animatedwidgets::ButtonContentOptions content;
    const animatedwidgets::TagStyle tag;
    const animatedwidgets::DefaultPopupMenuListStyle list;
    const animatedwidgets::CalendarStyle calendar;
    QVERIFY(!content.iconHint);
    QVERIFY(!tag.iconHint);
    QVERIFY(!list.iconHint);
    QVERIFY(!calendar.iconHint);
}

QTEST_APPLESS_MAIN(IconPropertiesTest)

#include "tsticonproperties.moc"
