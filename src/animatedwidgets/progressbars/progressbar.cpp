#include "animatedwidgets/progressbars/progressbar.h"

#include <QPainter>
#include <QStyle>
#include <QStyleOptionProgressBar>

#include <cmath>

ProgressBar::ProgressBar(QWidget* parent)
    : QProgressBar(parent)
    , m_valueTransition(this)
{
    m_lastMinimum = minimum();
    m_lastMaximum = maximum();
    m_realValue = value();
    m_valueTransition.setDuration(450);
    m_valueTransition.setEasingCurve(QEasingCurve::OutCubic);
    m_valueTransition.setTarget(m_realValue, true);
    connect(&m_valueTransition, &animatedwidgets::ValueTransition::valueChanged, this, [this] { update(); });
    connect(this, &QProgressBar::valueChanged, this, [this](int newValue) {
        if (m_settingRealValue)
            return;
        if (m_realValue != newValue) {
            m_realValue = newValue;
            emit realValueChanged(m_realValue);
        }
        if (m_lastMinimum != minimum() || m_lastMaximum != maximum()) {
            synchronizeRange();
        } else {
            m_valueTransition.setTarget(m_realValue, !m_animationEnabled || minimum() == maximum());
        }
    });
}

bool ProgressBar::animationEnabled() const {
    return m_animationEnabled;
}

void ProgressBar::setAnimationEnabled(bool enabled) {
    if (m_animationEnabled == enabled)
        return;
    m_animationEnabled = enabled;
    if (!enabled)
        m_valueTransition.setTarget(m_realValue, true);
}

int ProgressBar::animationDuration() const {
    return m_valueTransition.duration();
}

void ProgressBar::setAnimationDuration(int duration) {
    m_valueTransition.setDuration(duration);
    if (duration <= 0)
        m_valueTransition.setTarget(m_realValue, true);
}

QEasingCurve::Type ProgressBar::animationEasingCurve() const {
    return m_valueTransition.easingCurve().type();
}

void ProgressBar::setAnimationEasingCurve(QEasingCurve::Type type) {
    m_valueTransition.setEasingCurve(QEasingCurve(type));
}

double ProgressBar::realValue() const {
    return m_realValue;
}

void ProgressBar::setRealValue(double value) {
    if (!std::isfinite(value) || minimum() == maximum())
        return;
    const double bounded = qBound(double(minimum()), value, double(maximum()));
    if (m_realValue == bounded)
        return;
    m_realValue = bounded;
    m_settingRealValue = true;
    QProgressBar::setValue(int(qRound64(bounded)));
    m_settingRealValue = false;
    m_valueTransition.setTarget(bounded, !m_animationEnabled);
    emit realValueChanged(bounded);
}

int ProgressBar::valueDecimals() const {
    return m_valueDecimals;
}

void ProgressBar::setValueDecimals(int decimals) {
    m_valueDecimals = qBound(0, decimals, 6);
    update();
}

QColor ProgressBar::trackColor() const {
    return m_trackColor;
}
void ProgressBar::setTrackColor(const QColor& color) {
    m_trackColor = color;
    updateStyleSheet();
}

QColor ProgressBar::progressColor() const {
    return m_progressColor;
}
void ProgressBar::setProgressColor(const QColor& color) {
    m_progressColor = color;
    updateStyleSheet();
}

QColor ProgressBar::borderColor() const {
    return m_borderColor;
}
void ProgressBar::setBorderColor(const QColor& color) {
    m_borderColor = color;
    updateStyleSheet();
}

QColor ProgressBar::textColor() const {
    return m_textColor;
}
void ProgressBar::setTextColor(const QColor& color) {
    m_textColor = color;
    updateStyleSheet();
}

int ProgressBar::borderWidth() const {
    return m_borderWidth;
}
void ProgressBar::setBorderWidth(int width) {
    m_borderWidth = qMax(0, width);
    updateStyleSheet();
}

int ProgressBar::cornerRadius() const {
    return m_cornerRadius;
}
void ProgressBar::setCornerRadius(int radius) {
    m_cornerRadius = qMax(-1, radius);
    updateStyleSheet();
}

void ProgressBar::updateStyleSheet() {
    QString base = styleSheet();
    if (!m_generatedStyleSheet.isEmpty() && base.endsWith(m_generatedStyleSheet)) {
        base.chop(m_generatedStyleSheet.size());
    }

    QString track;
    if (m_trackColor.isValid())
        track += QStringLiteral("background-color: %1;").arg(m_trackColor.name(QColor::HexArgb));
    if (m_textColor.isValid())
        track += QStringLiteral("color: %1;").arg(m_textColor.name(QColor::HexArgb));
    if (m_borderColor.isValid()) {
        track += QStringLiteral("border: %1px solid %2;").arg(m_borderWidth).arg(m_borderColor.name(QColor::HexArgb));
    }
    if (m_cornerRadius >= 0)
        track += QStringLiteral("border-radius: %1px;").arg(m_cornerRadius);

    QString chunk;
    if (m_progressColor.isValid())
        chunk += QStringLiteral("background-color: %1;").arg(m_progressColor.name(QColor::HexArgb));
    if (m_cornerRadius >= 0)
        chunk += QStringLiteral("border-radius: %1px;").arg(m_cornerRadius);

    m_generatedStyleSheet.clear();
    if (!track.isEmpty())
        m_generatedStyleSheet += QStringLiteral("\nProgressBar {%1}").arg(track);
    if (!chunk.isEmpty())
        m_generatedStyleSheet += QStringLiteral("\nProgressBar::chunk {%1}").arg(chunk);
    const QString combined = base + m_generatedStyleSheet;
    if (styleSheet() != combined)
        setStyleSheet(combined);
}

void ProgressBar::synchronizeRange() {
    m_lastMinimum = minimum();
    m_lastMaximum = maximum();
    m_realValue = value();
    m_valueTransition.setTarget(m_realValue, true);
}

QString ProgressBar::displayText(double displayedValue) const {
    const qint64 span = qint64(maximum()) - minimum();
    const double percent = span > 0 ? (displayedValue - minimum()) * 100.0 / double(span) : 0.0;
    QString result = format();
    result.replace(QStringLiteral("%p"), locale().toString(percent, 'f', m_valueDecimals));
    result.replace(QStringLiteral("%v"), locale().toString(displayedValue, 'f', m_valueDecimals));
    result.replace(QStringLiteral("%m"), locale().toString(span));
    return result;
}

void ProgressBar::paintEvent(QPaintEvent*) {
    if (m_lastMinimum != minimum() || m_lastMaximum != maximum())
        synchronizeRange();

    QStyleOptionProgressBar option;
    initStyleOption(&option);
    if (minimum() != maximum() && value() >= minimum()) {
        const double displayed = qBound(double(minimum()), m_valueTransition.current().toDouble(), double(maximum()));
        const double ratio = (displayed - minimum()) / (double(maximum()) - minimum());
        option.minimum = 0;
        option.maximum = 10000;
        option.progress = qRound(ratio * option.maximum);
        if (isTextVisible())
            option.text = displayText(displayed);
    }

    QPainter painter(this);
    style()->drawControl(QStyle::CE_ProgressBar, &option, &painter, this);
}
