#include "stylecodegenerator.h"

using animatedwidgets::styleeditor::StyleProperty;

QString StyleCodeGenerator::generate(const StyleEditorItem& item) {
    QString code = QStringLiteral("// %1\nauto* widget = new %2(parent);\n").arg(item.displayName, item.typeName);
    for (const StyleProperty& property : item.properties) {
        if (!property.apply || !property.codeExpression)
            continue;
        if (property.defaultValue.isValid() && property.value == property.defaultValue)
            continue;

        QString setter = property.name;
        if (!setter.isEmpty())
            setter[0] = setter.at(0).toUpper();
        code += QStringLiteral("widget->set%1(%2);\n").arg(setter, property.codeExpression(property.value));
    }
    return code;
}
