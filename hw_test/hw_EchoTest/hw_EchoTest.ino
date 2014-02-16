#include <SPI.h>
#include <Ethernet.h>

#include "PubSubClient.h"
#include "MQTT_Logic.h"

// Update these with values suitable for your network.
byte mac[]    = {  0xBA, 0xBE, 0xBA, 0xFE, 0x01, 0x01 };

// The MQTT device name, this must be unique
char project_name[]  = "pubsubclient_hw_EchoTest";

EthernetClient ethClient;
PubSubClient client("mosqhub", 1883, callback, ethClient);

MQTT_Logic mTest;

int testCountOut;
int testCountBack;

void callback(char* topic, byte* payload, unsigned int length)
{
    Serial.print("Topic:");
    Serial.println(topic);
    Serial.print("Payload:");
    Serial.println((char*)payload);
    Serial.print("length:");
    Serial.println(length);

    // handle message arrived
    if(mTest.checkTopicSubscribe(topic))
    {
        Serial.println("Correct topic");
        Serial.print("Topic:");
        Serial.println(topic);
        if(length > 4)
        {
            if(     payload[0] == 'c' &&
                    payload[1] == 'n' &&
                    payload[2] == 't' &&
                    payload[3] == '=' )
            {
                Serial.println("Correct payload");
                for( int f=4 ; f<length ; f++ )
                {
                    Serial.println(payload[f]);
                    payload[f-4] = payload[f];
                    payload[f-3] = '\0';
                }

                Serial.print("Payload nr:");
                Serial.println((char*)payload);

                testCountBack=atoi((char*)payload);
                Serial.print("Payload nr:");
                Serial.println(testCountBack);
            }
        }
    }
}

void setup()
{
    Serial.begin(9600);
    testCountOut = 0;
    testCountBack = 0;

    Serial.println("Setup begin:");

    //Config the mTest
    mTest.setTopic(
            "test/hw_EchoTest",
            "test/hw_EchoTest"
            );

    Serial.println("Setup do eth:");
    //Start ethernet, if no ip is given then dhcp is used.
    Ethernet.begin(mac);

    Serial.println("Setup connect:");
    if(client.connect(project_name))
    {
        client.publish( mTest.getTopicPublish(), "#BEGIN" );
        client.subscribe( mTest.getTopicSubscribe() );
    }

    Serial.println("Setup done:");
}

void loop()
{
    if(false == client.loop())
    {
        client.connect(project_name);
    }
    if(false == client.connected())
    {
        client.connect(project_name);
    }

    //Send number.
    if(testCountOut == testCountBack)
    {
        testCountOut++;
        char str[30];
        snprintf(str, 30, "cnt=%d", testCountOut);

        Serial.print("Send data:");
        Serial.println("str");
        client.publish( mTest.getTopicPublish(), str);
    }
    delay(1000);
}
