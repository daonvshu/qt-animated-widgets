#include "inputspage.h"
#include "ui_inputspage.h"

#include "animatedwidgets/buttons/hovericonbutton.h"
#include "animatedwidgets/inputs/searchlineedit.h"

#include <QIcon>
#include <QGridLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QPalette>
#include <QTextCursor>
#include <QVector>

InputsPage::InputsPage(QWidget* parent)
    : QWidget(parent)
{
    Ui::InputsPage ui;
    ui.setupUi(this);

    initializeBasicInputs(ui);
    initializeCompositeLineEdit(ui);
    initializeCompositePlainTextEdit(ui);
    initializeNumericInputs(ui);
    initializeSearchLineEdit(ui);
}

void InputsPage::initializeBasicInputs(Ui::InputsPage& ui) {
    const QVector<animatedwidgets::LineEdit*> inputs = {ui.inputNormal,      ui.inputError,        ui.inputDisabled, ui.inputReadOnly,
                                                        ui.inputLeadingIcon, ui.inputTrailingIcon, ui.inputPassword, ui.inputStateColors};

    connect(ui.inputErrorCheck, &QCheckBox::toggled, ui.inputError, &LineEdit::setHasError);
    connect(ui.inputStateErrorCheck, &QCheckBox::toggled, ui.inputStateColors, &LineEdit::setHasError);
    connect(ui.inputStateDisabledCheck, &QCheckBox::toggled, ui.inputStateColors,
            [input = ui.inputStateColors](bool disabled) { input->setEnabled(!disabled); });
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged), this, [inputs](int duration) {
        for (auto* input : inputs)
            input->setAnimationDuration(duration);
    });

    connect(ui.inputMultilineErrorCheck, &QCheckBox::toggled, ui.inputMultiline, &PlainTextEdit::setHasError);
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.inputMultiline, &PlainTextEdit::setAnimationDuration);
    connect(ui.passwordAutoHideCheck, &QCheckBox::toggled, ui.inputPassword, &PasswordLineEdit::setAutoHideEnabled);
    connect(ui.inputTrailingIcon, &IconLineEdit::trailingIconClicked, ui.inputTrailingIcon, &QLineEdit::clear);
}

void InputsPage::initializeCompositeLineEdit(Ui::InputsPage& ui) {
    auto* currencyLabel = new QLabel(QStringLiteral("USD"), ui.inputComposite);
    currencyLabel->setMinimumWidth(40);
    currencyLabel->setAlignment(Qt::AlignCenter);
    ui.inputComposite->setLeadingWidget(currencyLabel);

    const auto updateCurrencyColor = [currencyLabel](const QColor& color) {
        QPalette palette = currencyLabel->palette();
        palette.setColor(QPalette::WindowText, color);
        currencyLabel->setPalette(palette);
    };
    connect(ui.inputComposite, &CompositeLineEdit::currentBorderColorChanged, currencyLabel, updateCurrencyColor);
    updateCurrencyColor(ui.inputComposite->currentBorderColor());

    auto* clearLineButton = new HoverIconButton(ui.inputComposite);
    clearLineButton->setFixedSize(26, 26);
    clearLineButton->setIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/clear.svg")));
    clearLineButton->setIconSize(QSize(16, 16));
    connect(clearLineButton, &QPushButton::clicked, ui.inputComposite->editor(), &QLineEdit::clear);
    ui.inputComposite->setTrailingWidget(clearLineButton);

    ui.inputComposite->setTopWidget(new QLabel(QStringLiteral("Payment amount"), ui.inputComposite));
    ui.inputComposite->setBottomWidget(new QLabel(QStringLiteral("USD account"), ui.inputComposite));
    ui.inputComposite->setDrawInputLeftBorder(true);
    ui.inputComposite->setDrawInputRightBorder(true);
    ui.inputComposite->setDrawInputTopBorder(true);
    ui.inputComposite->setDrawInputBottomBorder(true);

    connect(ui.inputErrorCheck, &QCheckBox::toggled, ui.inputComposite, &CompositeLineEdit::setHasError);
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.inputComposite, &CompositeLineEdit::setAnimationDuration);
}

void InputsPage::initializeCompositePlainTextEdit(Ui::InputsPage& ui) {
    constexpr int noteLimit = 120;

    auto* countLabel = new QLabel(ui.inputCompositeMultiline);
    countLabel->setAlignment(Qt::AlignRight | Qt::AlignVCenter);
    ui.inputCompositeMultiline->setFooterWidget(countLabel);
    ui.inputCompositeMultiline->setTopWidget(new QLabel(QStringLiteral("Draft note"), ui.inputCompositeMultiline));
    ui.inputCompositeMultiline->setLeadingWidget(new QLabel(QStringLiteral("TXT"), ui.inputCompositeMultiline));

    auto* clearNoteButton = new HoverIconButton(ui.inputCompositeMultiline);
    clearNoteButton->setFixedSize(26, 26);
    clearNoteButton->setIcon(QIcon(QStringLiteral(":/animatedwidgets/inputs/clear.svg")));
    clearNoteButton->setIconSize(QSize(16, 16));
    connect(clearNoteButton, &QPushButton::clicked, ui.inputCompositeMultiline->editor(), &QPlainTextEdit::clear);
    ui.inputCompositeMultiline->setTrailingWidget(clearNoteButton);

    ui.inputCompositeMultiline->setDrawInputLeftBorder(true);
    ui.inputCompositeMultiline->setDrawInputRightBorder(true);
    ui.inputCompositeMultiline->setDrawInputTopBorder(true);
    ui.inputCompositeMultiline->setDrawInputBottomBorder(true);

    const auto updateCount = [countLabel, noteLimit](int count) { countLabel->setText(QStringLiteral("%1 / %2").arg(count).arg(noteLimit)); };
    const auto updateCountColor = [countLabel](const QColor& color) {
        QPalette palette = countLabel->palette();
        palette.setColor(QPalette::WindowText, color);
        countLabel->setPalette(palette);
    };
    connect(ui.inputCompositeMultiline, &CompositePlainTextEdit::currentBorderColorChanged, countLabel, updateCountColor);
    updateCountColor(ui.inputCompositeMultiline->currentBorderColor());

    auto* noteEditor = ui.inputCompositeMultiline->editor();
    connect(noteEditor, &QPlainTextEdit::textChanged, this,
            [noteEditor, updateCount, noteLimit, previous = noteEditor->toPlainText(), adjusting = false]() mutable {
                if (adjusting)
                    return;

                QString current = noteEditor->toPlainText();
                if (current.size() > noteLimit) {
                    int prefix = 0;
                    while (prefix < previous.size() && prefix < current.size() && previous.at(prefix) == current.at(prefix))
                        ++prefix;
                    int suffix = 0;
                    while (suffix < previous.size() - prefix && suffix < current.size() - prefix &&
                           previous.at(previous.size() - suffix - 1) == current.at(current.size() - suffix - 1))
                        ++suffix;
                    const int removed = previous.size() - prefix - suffix;
                    const int allowed = qMax(0, noteLimit - previous.size() + removed);
                    const int insertedLength = current.size() - prefix - suffix;
                    const int excessStart = prefix + qMin(allowed, insertedLength);

                    adjusting = true;
                    QTextCursor cursor = noteEditor->textCursor();
                    cursor.setPosition(excessStart);
                    cursor.setPosition(prefix + insertedLength, QTextCursor::KeepAnchor);
                    cursor.removeSelectedText();
                    noteEditor->setTextCursor(cursor);
                    adjusting = false;
                    current = noteEditor->toPlainText();
                }
                previous = current;
                updateCount(current.size());
            });
    updateCount(noteEditor->toPlainText().size());

    connect(ui.inputCompositeMultilineErrorCheck, &QCheckBox::toggled, ui.inputCompositeMultiline, &CompositePlainTextEdit::setHasError);
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.inputCompositeMultiline,
            &CompositePlainTextEdit::setAnimationDuration);
}

void InputsPage::initializeNumericInputs(Ui::InputsPage& ui) {
    auto* unitLabel = new QLabel(QStringLiteral("pcs"), ui.inputCompositeSpin);
    unitLabel->setMinimumWidth(36);
    unitLabel->setAlignment(Qt::AlignCenter);
    ui.inputCompositeSpin->setTrailingWidget(unitLabel);
    ui.inputCompositeSpin->setLeadingWidget(new QLabel(QStringLiteral("Qty"), ui.inputCompositeSpin));
    ui.inputCompositeSpin->setTopWidget(new QLabel(QStringLiteral("Order quantity"), ui.inputCompositeSpin));
    ui.inputCompositeSpin->setBottomWidget(new QLabel(QStringLiteral("Whole units"), ui.inputCompositeSpin));
    ui.inputCompositeSpin->setDrawInputLeftBorder(true);
    ui.inputCompositeSpin->setDrawInputRightBorder(true);
    ui.inputCompositeSpin->setDrawInputTopBorder(true);
    ui.inputCompositeSpin->setDrawInputBottomBorder(true);

    ui.inputCompositeDoubleSpin->setLeadingWidget(new QLabel(QStringLiteral("kg"), ui.inputCompositeDoubleSpin));
    ui.inputCompositeDoubleSpin->setTopWidget(new QLabel(QStringLiteral("Package weight"), ui.inputCompositeDoubleSpin));
    ui.inputCompositeDoubleSpin->setDrawInputLeftBorder(true);
    ui.inputCompositeDoubleSpin->setDrawInputTopBorder(true);

    connect(ui.inputSpinErrorCheck, &QCheckBox::toggled, ui.inputSpin, &SpinBox::setHasError);
    connect(ui.inputStepButtonsCheck, &QCheckBox::toggled, ui.inputSpin, &SpinBox::setStepButtonsVisible);
    connect(ui.inputStepButtonsCheck, &QCheckBox::toggled, ui.inputDoubleSpin, &DoubleSpinBox::setStepButtonsVisible);
    connect(ui.inputStepButtonsCheck, &QCheckBox::toggled, ui.inputCompositeSpin, &CompositeSpinBox::setStepButtonsVisible);
    connect(ui.inputStepButtonsCheck, &QCheckBox::toggled, ui.inputCompositeDoubleSpin, &CompositeDoubleSpinBox::setStepButtonsVisible);
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.inputSpin,
            [input = ui.inputSpin](int duration) { input->setAnimationDuration(duration); });
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.inputDoubleSpin,
            [input = ui.inputDoubleSpin](int duration) { input->setAnimationDuration(duration); });
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.inputCompositeSpin,
            &CompositeSpinBox::setAnimationDuration);
    connect(ui.inputDurationSpin, QOverload<int>::of(&QSpinBox::valueChanged), ui.inputCompositeDoubleSpin,
            &CompositeDoubleSpinBox::setAnimationDuration);
}
void InputsPage::initializeSearchLineEdit(Ui::InputsPage& ui) {
    constexpr int insertRow = 5;
    QGridLayout* grid = ui.inputsGrid;
    // 为新增行腾出位置：把该行及之后的控件整体下移
    for (int i = grid->count() - 1; i >= 0; --i) {
        int row = 0;
        int column = 0;
        int rowSpan = 1;
        int columnSpan = 1;
        grid->getItemPosition(i, &row, &column, &rowSpan, &columnSpan);
        if (row >= insertRow) {
            QLayoutItem* item = grid->takeAt(i);
            grid->addItem(item, row + 1, column, rowSpan, columnSpan);
        }
    }

    auto* label = new QLabel(QStringLiteral("Search suggestions"), ui.inputsScrollContent);

    auto* container = new QWidget(ui.inputsScrollContent);
    auto* layout = new QVBoxLayout(container);
    layout->setContentsMargins(0, 0, 0, 0);
    layout->setSpacing(2);

    auto* search = new animatedwidgets::SearchLineEdit(container);
    search->setPlaceholderText(QStringLiteral("Type to filter suggestions"));
    search->setMaximumVisibleItems(6);
    layout->addWidget(search);

    auto* status = new QLabel(QStringLiteral("No suggestion selected"), container);
    layout->addWidget(status);

    const QStringList candidates = {
        QStringLiteral("Anime"),
        QStringLiteral("Animation"),
        QStringLiteral("Animated widgets"),
        QStringLiteral("Animated buttons"),
        QStringLiteral("Search line edit"),
        QStringLiteral("Search combo box"),
        QStringLiteral("Suggestion popup"),
        QStringLiteral("Fuzzy search"),
    };
    connect(search, &QLineEdit::textEdited, search, [search, candidates](const QString& text) {
        const QString keyword = text.trimmed();
        if (keyword.isEmpty()) {
            search->clearSuggestions();
            return;
        }
        QStringList matches;
        for (const QString& candidate : candidates) {
            if (candidate.contains(keyword, Qt::CaseInsensitive)) {
                matches.append(candidate);
            }
        }
        search->setSuggestions(matches);
    });

    connect(search, &animatedwidgets::SearchLineEdit::suggestionActivated, status,
            [status](const QString& suggestion) {
                status->setText(QStringLiteral("Selected: %1").arg(suggestion));
            });

    grid->addWidget(label, insertRow, 0);
    grid->addWidget(container, insertRow, 1);
}
