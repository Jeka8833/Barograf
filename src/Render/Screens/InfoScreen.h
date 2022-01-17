//
// Created by Jeka8833 on 07.01.2022.
//

#ifndef BAROGRAFV3_INFOSCREEN_H
#define BAROGRAFV3_INFOSCREEN_H

#include "sensors/DHT.h"

class InfoScreen {
public:
    static DHT dht;

    static void init();

    static void render();
};


#endif //BAROGRAFV3_INFOSCREEN_H
