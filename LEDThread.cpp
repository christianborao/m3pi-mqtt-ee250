/**
 * Copyright (c) 2017, Autonomous Networks Research Group. All rights reserved.
 * Developed by:
 * Autonomous Networks Research Group (ANRG)
 * University of Southern California
 * http://anrg.usc.edu/
 *
 * Contributors:
 * Jason A. Tran <jasontra@usc.edu>
 * Bhaskar Krishnamachari <bkrishna@usc.edu>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy 
 * of this software and associated documentation files (the "Software"), to deal
 * with the Software without restriction, including without limitation the 
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or 
 * sell copies of the Software, and to permit persons to whom the Software is 
 * furnished to do so, subject to the following conditions:
 * - Redistributions of source code must retain the above copyright notice, this
 *     list of conditions and the following disclaimers.
 * - Redistributions in binary form must reproduce the above copyright notice, 
 *     this list of conditions and the following disclaimers in the 
 *     documentation and/or other materials provided with the distribution.
 * - Neither the names of Autonomous Networks Research Group, nor University of 
 *     Southern California, nor the names of its contributors may be used to 
 *     endorse or promote products derived from this Software without specific 
 *     prior written permission.
 * - A citation to the Autonomous Networks Research Group must be included in 
 *     any publications benefiting from the use of the Software.
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
 * CONTRIBUTORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS WITH 
 * THE SOFTWARE.
 */

/**
 * @file       LEDThread.cpp
 * @brief      Implementation of thread that handles LED requests.
 *
 * @author     Jason Tran <jasontra@usc.edu>
 * @author     Bhaskar Krishnachari <bkrishna@usc.edu>
 */
#include "m3pi.h"
#include "mbed.h"
#include "LEDThread.h"
#include "MQTTmbed.h"
#include "MQTTNetwork.h"

#include "MQTTClient.h"

Mail<MailMsg, LEDTHREAD_MAILBOX_SIZE> LEDMailbox;

static DigitalOut led2(LED2);

static const char *topic = "anrg-pi5/led-thread";
static const char *topic2 = "anrg-pi5/Ultrasonic";

extern void movement(char command, char speed, int delta_t);

int distance;
float voltage;

void LEDThread(void *args) 
{
    MQTT::Client<MQTTNetwork, Countdown> *client = (MQTT::Client<MQTTNetwork, Countdown> *)args;
    MailMsg *msg;
    MQTT::Message message;
    osEvent evt;
    char pub_buf[16];

    //printf("Hello world1");

    while(1) {

        evt = LEDMailbox.get();

       /* AnalogIn Ain(p15);
        voltage = Ain.read();
        distance = voltage * 2.38 / 0.0032;

printf("\tdistance: ", distance, "\t");
        pub_buf[0] = (distance / 10) + 48;
        pub_buf[1] = ((distance / 10) % 10) +48;
        pub_buf[2] = (distance % 10) + 48;

        //printf(pub_buf[2],pub_buf[1],pub_buf[0]);

        message.qos = MQTT::QOS0;
        message.retained = false;
        message.dup = false;
        message.payload = (void*)pub_buf;
        message.payloadlen = 3; //MQTTclient.h takes care of adding null char?
        /* Lock the global MQTT mutex before publishing 
        mqttMtx.lock();
        client->publish(topic, message);
        mqttMtx.unlock();

*/
        if(evt.status == osEventMail) {
            msg = (MailMsg *)evt.value.p;
            //printf("Hello world2");
            //movement('s', 10, 1000);


            


            /* the second byte in the message denotes the action type */
            switch (msg->content[1]) {
                /*case '0':
                    printf("LEDThread: received command to publish to topic"
                           "m3pi-mqtt-example/led-thread\n");
                    
                    break; */
                case '0':
                    movement('s', 25, 1000);
                    printf("\tFORWARD!!!!\t");
                    break;

                case '1':
                    /*printf("LEDThread: received message to turn LED2 on for"
                           "one second...\n");
                    led2 = 1;
                    wait(1);
                    led2 = 0;
                    break; */
                    movement('w', 25, 1000);
                    printf("\tBACKWARD\t");
                    break;

                case '2':
                    /*printf("LEDThread: received message to blink LED2 fast for"
                           "one second...\n");
                    for(int i = 0; i < 10; i++)
                    {
                        led2 = !led2;
                        wait(0.1);
                    }
                    led2 = 0;
                    break; */

                    movement('a', 25, 300);
                    printf("\tLEFT\t");
                    break;

                case '3':
                    movement('d', 25, 300);
                    printf("\tRIGHT\t");
                    break;

                case '4':
                    printf("\tSTOP\t");
                    break;

                // Roomba
                case '5':
                    printf("\tRoomba\t");
                  /*  while(1){
                        if(!(distance < 25)){
                            movement('s',10,50);
                            printf("\tMoving straight\t");
                        }
                        else{
                            movement('a',10,50);
                            printf("\tTurning left\t");
                        }
                    } */

                default:
                    printf("LEDThread: invalid message\n");
                    break;
            }            

            LEDMailbox.free(msg);
        }
    } /* while */

    /* this should never be reached */
}

Mail<MailMsg, LEDTHREAD_MAILBOX_SIZE> *getLEDThreadMailbox() 
{
    return &LEDMailbox;
}


