#pragma once

#include "animatedwidgets/namespacealias.h"

#include "animatedwidgets/core/valuetransition.h"

#include <QColor>
#include <QProgressBar>

namespace animatedwidgets {

class ProgressBar : public QProgressBar {
    Q_OBJECT
    Q_PROPERTY(bool animationEnabled READ animationEnabled WRITE setAnimationEnabled)
    Q_PROPERTY(int animationDuration READ animationDuration WRITE setAnimationDuration)
    Q_PROPERTY(QEasingCurve::Type animationEasingCurve READ animationEasingCurve WRITE setAnimationEasingCurve)
    Q_PROPERTY(double realValue READ realValue WRITE setRealValue NOTIFY realValueChanged)
    Q_PROPERTY(int valueDecimals READ valueDecimals WRITE setValueDecimals)
    Q_PROPERTY(QColor trackColor READ trackColor WRITE setTrackColor)
    Q_PROPERTY(QColor progressColor READ progressColor WRITE setProgressColor)
    Q_PROPERTY(QColor border READ border WRITE setBorder)
    Q_PROPERTY(QColor textColor READ textColor WRITE setTextColor)
    Q_PROPERTY(int borderWidth READ borderWidth WRITE setBorderWidth)
    Q_PROPERTY(int cornerRadius READ cornerRadius WRITE setCornerRadius)

public:
    explicit ProgressBar(QWidget* parent = nullptr);

    bool animationEnabled() const;
    void setAnimationEnabled(bool enabled);
    int animationDuration() const;
    void setAnimationDuration(int duration);
    QEasingCurve::Type animationEasingCurve() const;
    void setAnimationEasingCurve(QEasingCurve::Type type);
    double realValue() const;
    void setRealValue(double value);
    int valueDecimals() const;
    void setValueDecimals(int decimals);
    QColor trackColor() const;
    void setTrackColor(const QColor& color);
    QColor progressColor() const;
    void setProgressColor(const QColor& color);
    QColor border() const;
    void setBorder(const QColor& color);
    QColor textColor() const;
    void setTextColor(const QColor& color);
    int borderWidth() const;
    void setBorderWidth(int width);
    int cornerRadius() const;
    void setCornerRadius(int radius);

signals:
    void realValueChanged(double value);

protected:
    void paintEvent(QPaintEvent* event) override;

private:
    void synchronizeRange();
    QString displayText(double displayedValue) const;
    void updateStyleSheet();

    ValueTransition m_valueTransition;
    bool m_animationEnabled = true;
    bool m_settingRealValue = false;
    double m_realValue = -1.0;
    int m_valueDecimals = 1;
    int m_lastMinimum = 0;
    int m_lastMaximum = 100;
    QColor m_trackColor;
    QColor m_progressColor;
    QColor m_borderColor;
    QColor m_textColor;
    int m_borderWidth = 1;
    int m_cornerRadius = -1;
    QString m_generatedStyleSheet;
};

} // namespace animatedwidgets

#ifndef ANIMATEDWIDGETS_DESIGNER_PLUGIN
using ProgressBar = animatedwidgets::ProgressBar;
#endif
