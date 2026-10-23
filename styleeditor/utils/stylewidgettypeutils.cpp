#include "stylewidgettypeutils.h"

#include <QHash>

#include <algorithm>
#include <cmath>

using animatedwidgets::styleeditor::StyleEditorWidgetType;

QString StyleWidgetTypeUtils::categoryName(const StyleEditorWidgetType& type) {
    return type.category.trimmed().isEmpty() ? QStringLiteral("Other widgets") : type.category;
}

double StyleWidgetTypeUtils::normalizedOrder(double order) {
    return std::isfinite(order) ? order : 1000.0;
}

void StyleWidgetTypeUtils::sort(QList<StyleEditorWidgetType>& types) {
    QHash<QString, double> categoryOrders;
    for (const StyleEditorWidgetType& type : types) {
        const QString category = categoryName(type);
        const double order = normalizedOrder(type.categoryOrder);
        const auto existing = categoryOrders.constFind(category);
        if (existing == categoryOrders.cend() || order < existing.value())
            categoryOrders.insert(category, order);
    }

    std::stable_sort(types.begin(), types.end(), [&categoryOrders](const StyleEditorWidgetType& left, const StyleEditorWidgetType& right) {
        const QString leftCategory = categoryName(left);
        const QString rightCategory = categoryName(right);
        const double leftCategoryOrder = categoryOrders.value(leftCategory);
        const double rightCategoryOrder = categoryOrders.value(rightCategory);
        if (leftCategoryOrder != rightCategoryOrder)
            return leftCategoryOrder < rightCategoryOrder;
        if (leftCategory != rightCategory)
            return leftCategory < rightCategory;

        const double leftOrder = normalizedOrder(left.order);
        const double rightOrder = normalizedOrder(right.order);
        if (leftOrder != rightOrder)
            return leftOrder < rightOrder;
        return left.id < right.id;
    });
}
