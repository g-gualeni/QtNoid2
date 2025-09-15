#include <QApplication>
#include <QDebug>
#include <QJsonDocument>
#include <QJsonObject>
#include <QtNoidApp/QtNoidApp>

using namespace QtNoid::App;

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    qDebug() << "=== QtNoid App ParameterList Usage Example ===\n";

    // Example 1: Create a ParameterList and add parameters manually
    qDebug() << "1. Creating ParameterList and adding parameters manually:";
    ParameterList* config = new ParameterList("SystemConfiguration");

    // Add individual parameters
    Parameter* temperature = new Parameter("Temperature", "System temperature", 25.0);
    temperature->setUnit("°C");
    temperature->setRange(-50.0, 100.0);
    config->addParameter(temperature);

    Parameter* enabled = new Parameter("Enabled", "System enabled state", true);
    config->addParameter(enabled);

    Parameter* maxConnections = new Parameter("MaxConnections", "Maximum number of connections", 100);
    maxConnections->setRange(1, 1000);
    config->addParameter(maxConnections);

    qDebug() << "  ParameterList name:" << config->name();
    qDebug() << "  Parameter count:" << config->count();
    qDebug() << "  Temperature value:" << config->value("Temperature");
    qDebug() << "  Enabled value:" << config->value("Enabled");
    qDebug() << "  MaxConnections value:" << config->value("MaxConnections");

    // Example 2: Modify parameter values
    qDebug() << "\n2. Modifying parameter values:";
    config->setValue("Temperature", 30.5);
    config->setValue("Enabled", false);
    config->setValue("MaxConnections", 250);

    qDebug() << "  New Temperature value:" << config->value("Temperature");
    qDebug() << "  New Enabled value:" << config->value("Enabled");
    qDebug() << "  New MaxConnections value:" << config->value("MaxConnections");

    // Example 3: Export to JSON
    qDebug() << "\n3. Exporting to JSON:";
    QJsonObject schema = config->toJsonSchema();
    QJsonObject values = config->toJsonValues();

    qDebug() << "  Schema JSON:";
    qDebug() << QJsonDocument(schema).toJson(QJsonDocument::Compact);
    qDebug() << "  Values JSON:";
    qDebug() << QJsonDocument(values).toJson(QJsonDocument::Compact);

    // Example 4: Create ParameterList from JSON
    qDebug() << "\n4. Creating ParameterList from JSON:";

    // Create schema and values JSON objects
    QJsonObject jsonSchema;
    QJsonObject tempSchema;
    tempSchema["description"] = "Ambient temperature sensor";
    tempSchema["unit"] = "°F";
    tempSchema["readOnly"] = false;
    tempSchema["min"] = -58.0;  // -50°C in Fahrenheit
    tempSchema["max"] = 212.0;  // 100°C in Fahrenheit
    jsonSchema["AmbientTemp"] = tempSchema;

    QJsonObject pressureSchema;
    pressureSchema["description"] = "Atmospheric pressure";
    pressureSchema["unit"] = "hPa";
    pressureSchema["readOnly"] = true;
    pressureSchema["min"] = 800.0;
    pressureSchema["max"] = 1100.0;
    jsonSchema["AtmPressure"] = pressureSchema;

    QJsonObject statusSchema;
    statusSchema["description"] = "System status message";
    statusSchema["unit"] = "";
    statusSchema["readOnly"] = false;
    jsonSchema["Status"] = statusSchema;

    QJsonObject jsonValues;
    jsonValues["AmbientTemp"] = 77.0;  // 25°C in Fahrenheit
    jsonValues["AtmPressure"] = 1013.25;
    jsonValues["Status"] = "System Ready";

    // Create ParameterList from JSON
    ParameterList* sensorConfig = new ParameterList(jsonSchema, jsonValues);
    sensorConfig->setName("SensorConfiguration");

    qDebug() << "  Created ParameterList from JSON:";
    qDebug() << "    Name:" << sensorConfig->name();
    qDebug() << "    Parameter count:" << sensorConfig->count();

    for (int i = 0; i < sensorConfig->count(); ++i) {
        Parameter* param = sensorConfig->parameter(i);
        qDebug() << "    Parameter" << i << ":";
        qDebug() << "      Name:" << param->name();
        qDebug() << "      Description:" << param->description();
        qDebug() << "      Value:" << param->value();
        qDebug() << "      Unit:" << param->unit();
        qDebug() << "      ReadOnly:" << param->readOnly();
        qDebug() << "      Range:" << param->min() << "to" << param->max();
    }

    // Example 5: Test parameter access and validation
    qDebug() << "\n5. Testing parameter access and validation:";

    // Try to modify read-only parameter
    qDebug() << "  Attempting to modify read-only AtmPressure:";
    bool success = sensorConfig->setValue("AtmPressure", 950.0);
    qDebug() << "    Success:" << success;
    qDebug() << "    AtmPressure value:" << sensorConfig->value("AtmPressure");

    // Test range validation
    qDebug() << "  Testing range validation on AmbientTemp:";
    qDebug() << "    Setting to 300°F (above max 212°F):";
    sensorConfig->setValue("AmbientTemp", 300.0);
    qDebug() << "    Actual value:" << sensorConfig->value("AmbientTemp");

    qDebug() << "    Setting to -100°F (below min -58°F):";
    sensorConfig->setValue("AmbientTemp", -100.0);
    qDebug() << "    Actual value:" << sensorConfig->value("AmbientTemp");

    // Example 6: Parameter lookup and contains check
    qDebug() << "\n6. Parameter lookup and contains check:";
    qDebug() << "  Contains 'AmbientTemp':" << sensorConfig->contains("AmbientTemp");
    qDebug() << "  Contains 'NonExistent':" << sensorConfig->contains("NonExistent");
    qDebug() << "  Index of 'Status':" << sensorConfig->indexOf("Status");

    // Example 7: Remove parameters
    qDebug() << "\n7. Removing parameters:";
    qDebug() << "  Count before removal:" << sensorConfig->count();
    sensorConfig->removeParameter("AtmPressure");
    qDebug() << "  Count after removing AtmPressure:" << sensorConfig->count();
    qDebug() << "  Contains 'AtmPressure':" << sensorConfig->contains("AtmPressure");

    // Example 8: Load values from JSON into existing ParameterList
    qDebug() << "\n8. Loading new values from JSON:";
    QJsonObject newValues;
    newValues["AmbientTemp"] = 68.0;  // 20°C in Fahrenheit
    newValues["Status"] = "All Systems Normal";

    qDebug() << "  Values before update:";
    qDebug() << "    AmbientTemp:" << sensorConfig->value("AmbientTemp");
    qDebug() << "    Status:" << sensorConfig->value("Status");

    sensorConfig->valuesFromJson(newValues);

    qDebug() << "  Values after update:";
    qDebug() << "    AmbientTemp:" << sensorConfig->value("AmbientTemp");
    qDebug() << "    Status:" << sensorConfig->value("Status");

    qDebug() << "\n=== Example completed successfully ===";

    // Clean up
    delete config;
    delete sensorConfig;

    return 0; // Exit without running the event loop for this console example
}