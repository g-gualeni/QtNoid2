// rapidyaml must be included before any Qt header: Qt defines "emit" as a
// macro, and that breaks the ryml::emit*() functions.
#include <ryml.hpp>
#include <ryml_std.hpp>

#include "QtNoidJson/yaml2json.h"

#include <exception>
#include <string>

namespace {

// Replace ryml's default error handlers (which print to stderr) with handlers
// that only throw: the error is then reported through Yaml2Json::errorString().
[[noreturn]] void throwBasic(ryml::csubstr msg, ryml::ErrorDataBasic const& data, void *)
{
    throw ryml::ExceptionBasic(msg, data);
}

[[noreturn]] void throwParse(ryml::csubstr msg, ryml::ErrorDataParse const& data, void *)
{
    throw ryml::ExceptionParse(msg, data);
}

[[noreturn]] void throwVisit(ryml::csubstr msg, ryml::ErrorDataVisit const& data, void *)
{
    throw ryml::ExceptionVisit(msg, data);
}

void installRymlCallbacks()
{
    static const bool installed = [] {
        ryml::Callbacks callbacks = ryml::get_callbacks();
        callbacks.set_error_basic(&throwBasic);
        callbacks.set_error_parse(&throwParse);
        callbacks.set_error_visit(&throwVisit);
        ryml::set_callbacks(callbacks);
        return true;
    }();
    Q_UNUSED(installed)
}

} // namespace

namespace QtNoid {
namespace Json {

Yaml2Json::Yaml2Json(QObject* parent)
    : QObject(parent)
{
}

Yaml2Json::Yaml2Json(const QString& yaml, QObject* parent)
    : QObject(parent)
    , m_yaml(yaml)
{
    update();
}

void Yaml2Json::setYaml(const QString& newYaml)
{
    if (m_yaml == newYaml)
        return;
    m_yaml = newYaml;
    update();
}

QString Yaml2Json::jsonAsString(QJsonDocument::JsonFormat format) const
{
    if (m_doc.isNull())
        return {};
    return QString::fromUtf8(m_doc.toJson(format));
}

QJsonDocument Yaml2Json::convert(const QString& yaml, QString* error)
{
    if (error)
        error->clear();

    if (yaml.trimmed().isEmpty())
        return QJsonDocument(QJsonObject());

    installRymlCallbacks();

    try {
        const std::string src = yaml.toStdString();
        ryml::Tree tree = ryml::parse_in_arena(ryml::to_csubstr(src));
        tree.resolve(); // expand anchors and aliases (&a / *a)

        ryml::ConstNodeRef root = tree.crootref();
        if (root.is_stream() && root.num_children() > 0)
            root = root.first_child(); // multi-document stream: use the first document
        if (!root.is_map() && !root.is_seq()) {
            if (error)
                *error = QStringLiteral("YAML error: the root node must be a map or a sequence");
            return {};
        }

        const std::string json = ryml::emitrs_json<std::string>(root);

        QJsonParseError parseError;
        QJsonDocument doc = QJsonDocument::fromJson(QByteArray::fromStdString(json), &parseError);
        if (parseError.error != QJsonParseError::NoError) {
            if (error)
                *error = QStringLiteral("JSON error: %1").arg(parseError.errorString());
            return {};
        }
        return doc;
    } catch (const ryml::ExceptionParse& e) {
        const ryml::Location& loc = e.errdata_parse.ymlloc;
        if (error)
            *error = QStringLiteral("YAML error at line %1, column %2: %3")
                         .arg(loc.line).arg(loc.col).arg(QString::fromUtf8(e.what()));
        return {};
    } catch (const std::exception& e) {
        if (error)
            *error = QStringLiteral("YAML error: %1").arg(QString::fromUtf8(e.what()));
        return {};
    }
}

void Yaml2Json::update()
{
    QString error;
    m_doc = convert(m_yaml, &error);
    m_error = error;
    m_isValid = error.isEmpty();

    if (!m_isValid)
        emit errorOccurred(m_error);
}

} // namespace Json
} // namespace QtNoid
