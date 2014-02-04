#include <SPI.h>
#include <Ethernet.h>

#include "PubSubClient.h"
#include "MQTT_Logic.h"

// Update these with values suitable for your network.
byte mac[]    = {  0x11, 0x11, 0xBA, 0xFE, 0xFE, 0x01 };

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
                }

                //Now get the number and update testCountBack
                //cnt=%d

                //testCountBack=
            }
        }
    }
}

void setup()
{
    Serial.begin(9600);
    testCountOut = 0;
    testCountBack = 0;

    //Config the mTest
    mTest.setTopic(
            "test/hw_EchoTest",
            "test/hw_EchoTest"
            );

    //Start ethernet, if no ip is given then dhcp is used.
    Ethernet.begin(mac);
    if(client.connect(project_name))
    {
        client.publish( mTest.getTopicPublish(), "#BEGIN" );
        client.subscribe( mTest.getTopicSubscribe() );
    }
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
