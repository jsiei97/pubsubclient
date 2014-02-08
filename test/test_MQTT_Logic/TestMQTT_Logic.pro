CONFIG += qtestlib
QT     -= gui
TEMPLATE = app
TARGET =
DEFINES += private=public
DEFINES += protected=public

# Test code
DEPENDPATH += .
INCLUDEPATH += .
SOURCES += TestMQTT_Logic.cpp

# Code to test
DEPENDPATH  += ../../PubSubClient/
INCLUDEPATH += ../../PubSubClient/
SOURCES += MQTT_Logic.cpp

