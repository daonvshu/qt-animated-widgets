#include "stylecodegenerator.h"
#include "styleresourceutils.h"

using animatedwidgets::styleeditor::StyleProperty;
using animatedwidgets::styleeditor::StylePropertyType;

QString StyleCodeGenerator::generate(const StyleEditorItem& item, const QString& projectDirectory) {
    QString code = QStringLiteral("// %1\nauto* widget = new %2(parent);\n").arg(item.displayName, item.typeName);
    for (const StyleProperty& property : item.properties) {
        if (!property.apply || (!property.codeExpression && !property.codeStatement))
            continue;
        if (property.defaultValue.isValid() && property.value == property.defaultValue)
            continue;

        QString setter = property.name;
        if (!setter.isEmpty())
            setter[0] = setter.at(0).toUpper();
        QVariant value = property.value;
        if (property.type == StylePropertyType::Icon && !value.toString().isEmpty()) {
            const QString reference = StyleResourceUtils::relativePath(projectDirectory, value.toString());
            if (reference.isEmpty())
                continue;
            value = reference;
        }
        if (property.codeStatement)
            code += property.codeStatement(value);
        else
            code += QStringLiteral("widget->set%1(%2);\n").arg(setter, property.codeExpression(value));
    }
    return code;
}
