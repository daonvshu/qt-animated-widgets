#include "stylepropertyeditorutils.h"

#include "animatedwidgets/comboboxes/combobox.h"
#include "animatedwidgets/inputs/doublespinbox.h"
#include "animatedwidgets/inputs/lineedit.h"
#include "animatedwidgets/inputs/spinbox.h"
#include "animatedwidgets/switches/switch.h"
#include "animatedwidgets/styleeditor/utils/stylealignmentutils.h"
#include "animatedwidgets/styleeditor/utils/stylecodeexpressions.h"

#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QWheelEvent>

#include <limits>

using animatedwidgets::styleeditor::StyleAlignmentUtils;
using animatedwidgets::styleeditor::StyleCodeExpressions;
using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyType;

namespace {

template <typename SpinBoxType>
class PropertySpinBox : public SpinBoxType {
public:
    explicit PropertySpinBox(QWidget* parent)
        : SpinBoxType(parent)
    {
        this->setProperty("compactPropertyEditor", true);
        this->setKeyboardTracking(false);
        this->setSizePolicy(QSizePolicy::Ignored, QSizePolicy::Fixed);
        this->setStepButtonSize(QSize(14, 10));
        this->setStepButtonRightMargin(2);
        this->setStepIconSize(QSize(8, 8));
        this->setIconHint(true);
    }

protected:
    void wheelEvent(QWheelEvent* event) override {
        if (!this->hasFocus()) {
            event->ignore();
            return;
        }
        SpinBoxType::wheelEvent(event);
    }
};

} // namespace

QWidget* StylePropertyEditorUtils::createEditor(const StyleProperty& property, QWidget* parent, const ValueChanged& changed) {
    if (property.type == StylePropertyType::Alignment)
        return createAlignmentEditor(property, parent, changed);
    if (property.type == StylePropertyType::Size)
        return createSizeEditor(property, parent, changed);

    if (property.type == StylePropertyType::Integer) {
        auto* spin = new PropertySpinBox<SpinBox>(parent);
        spin->setRange(std::numeric_limits<int>::lowest(), std::numeric_limits<int>::max());
        spin->setValue(property.value.toInt());
        QObject::connect(spin, QOverload<int>::of(&SpinBox::valueChanged), spin, [changed](int value) { changed(value); });
        return spin;
    }

    if (property.type == StylePropertyType::Real) {
        auto* spin = new PropertySpinBox<DoubleSpinBox>(parent);
        spin->setDecimals(StyleCodeExpressions::realDecimals);
        spin->setRange(std::numeric_limits<double>::lowest(), std::numeric_limits<double>::max());
        spin->setSingleStep(1.0);
        spin->setValue(property.value.toDouble());
        QObject::connect(spin, QOverload<double>::of(&DoubleSpinBox::valueChanged), spin, [changed](double value) {
            changed(StyleCodeExpressions::real(value).toDouble());
        });
        return spin;
    }

    if (property.type == StylePropertyType::Boolean) {
        auto* check = new Switch(parent);
        check->setProperty("compactPropertyEditor", true);
        check->setTrackSize(QSize(32, 18));
        check->setFixedSize(36, 24);
        check->setChecked(property.value.toBool());
        QObject::connect(check, &Switch::toggled, check, [changed](bool value) { changed(value); });
        return check;
    }

    if (property.type == StylePropertyType::Enum) {
        auto* combo = new ComboBox(parent);
        combo->setProperty("compactPropertyEditor", true);
        combo->setArrowIconSize(QSize(10, 10));
        combo->addItems(property.enumNames);
        combo->setCurrentIndex(qMax(0, property.enumNames.indexOf(property.value.toString())));
        QObject::connect(combo, &ComboBox::currentTextChanged, combo, [changed](const QString& value) { changed(value); });
        return combo;
    }

    auto* edit = new LineEdit(parent);
    edit->setProperty("compactPropertyEditor", true);
    if (property.type == StylePropertyType::Color)
        edit->setText(property.value.value<QColor>().name(QColor::HexArgb));
    else
        edit->setText(property.value.toString());

    const StylePropertyType type = property.type;
    QObject::connect(edit, &LineEdit::editingFinished, edit, [type, edit, changed] {
        QVariant value = edit->text();
        if (type == StylePropertyType::Color)
            value = QColor(edit->text());
        changed(value);
    });
    return edit;
}

QWidget* StylePropertyEditorUtils::createSizeEditor(const StyleProperty& property, QWidget* parent, const ValueChanged& changed) {
    auto* editor = new QWidget(parent);
    auto* layout = new QHBoxLayout(editor);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    auto* width = new PropertySpinBox<SpinBox>(editor);
    auto* height = new PropertySpinBox<SpinBox>(editor);
    for (SpinBox* spin : {width, height}) {
        spin->setRange(1, 4096);
        layout->addWidget(spin, 1);
    }
    width->setToolTip(QStringLiteral("Width"));
    height->setToolTip(QStringLiteral("Height"));
    width->setAccessibleName(QStringLiteral("Width"));
    height->setAccessibleName(QStringLiteral("Height"));
    const QSize size = property.value.toSize();
    width->setValue(size.width());
    height->setValue(size.height());

    const auto update = [width, height, changed] {
        changed(QSize(width->value(), height->value()));
    };
    QObject::connect(width, QOverload<int>::of(&SpinBox::valueChanged), editor, update);
    QObject::connect(height, QOverload<int>::of(&SpinBox::valueChanged), editor, update);
    return editor;
}

QWidget* StylePropertyEditorUtils::createAlignmentEditor(const StyleProperty& property, QWidget* parent, const ValueChanged& changed) {
    auto* editor = new QWidget(parent);
    auto* layout = new QVBoxLayout(editor);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(4);

    auto* horizontal = new ComboBox(editor);
    auto* vertical = new ComboBox(editor);
    for (ComboBox* combo : {horizontal, vertical}) {
        combo->setProperty("compactPropertyEditor", true);
        combo->setArrowIconSize(QSize(10, 10));
        combo->setFixedHeight(24);
        layout->addWidget(combo, 1);
    }
    horizontal->setAccessibleName(QStringLiteral("Horizontal alignment"));
    horizontal->setToolTip(QStringLiteral("Horizontal alignment"));
    vertical->setAccessibleName(QStringLiteral("Vertical alignment"));
    vertical->setToolTip(QStringLiteral("Vertical alignment"));

    horizontal->addItem(QStringLiteral("Left"), QStringLiteral("AlignLeft"));
    horizontal->addItem(QStringLiteral("Center"), QStringLiteral("AlignHCenter"));
    horizontal->addItem(QStringLiteral("Right"), QStringLiteral("AlignRight"));
    vertical->addItem(QStringLiteral("Top"), QStringLiteral("AlignTop"));
    vertical->addItem(QStringLiteral("Center"), QStringLiteral("AlignVCenter"));
    vertical->addItem(QStringLiteral("Bottom"), QStringLiteral("AlignBottom"));
    horizontal->setCurrentIndex(horizontal->findData(StyleAlignmentUtils::horizontal(property.value.toString())));
    vertical->setCurrentIndex(vertical->findData(StyleAlignmentUtils::vertical(property.value.toString())));

    const auto update = [horizontal, vertical, changed] {
        changed(StyleAlignmentUtils::combine(horizontal->currentData().toString(), vertical->currentData().toString()));
    };
    QObject::connect(horizontal, QOverload<int>::of(&ComboBox::currentIndexChanged), editor, update);
    QObject::connect(vertical, QOverload<int>::of(&ComboBox::currentIndexChanged), editor, update);
    return editor;
}
