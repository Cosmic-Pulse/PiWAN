// LoRaWAN uplink test node: ABP, US915 sub-band 2, SF7, 14 dBm.
// Sends MESSAGES_TO_SEND uplinks, one every SEND_INTERVAL_S, then stops transmitting.
// ABP because the gateway under test is listen-only, so an OTAA join could never complete.
#include <Arduino.h>
#include <Preferences.h>
#include <SPI.h>
#include <lmic.h>
#include <hal/hal.h>
#include "secrets.h"

static const int MESSAGES_TO_SEND = 10;
static const int SEND_INTERVAL_S = 30;

// Keys are given directly with LMIC_setSession, so OTAA callbacks are empty.
void os_getArtEui(u1_t *) {}
void os_getDevEui(u1_t *) {}
void os_getDevKey(u1_t *) {}

const lmic_pinmap lmic_pins = {
    .nss = 18,
    .rxtx = LMIC_UNUSED_PIN,
    .rst = 14,
    .dio = {26, 35, 34},
};

static Preferences prefs;
static osjob_t sendJob;
static int sent = 0;

static void sendNext(osjob_t *) {
    if (sent >= MESSAGES_TO_SEND) {
        Serial.println("DONE: no more transmissions");
        return;
    }
    char msg[32];
    int len = snprintf(msg, sizeof(msg), "hello piwan %lu", (unsigned long)LMIC.seqnoUp);
    // Persist before sending: the network rejects a frame counter that goes backwards.
    prefs.putUInt("fcnt", LMIC.seqnoUp + 1);
    LMIC_setTxData2(1, (uint8_t *)msg, len, 0);
    sent++;
    Serial.printf("QUEUED %d/%d fcnt=%lu \"%s\"\n", sent, MESSAGES_TO_SEND, (unsigned long)LMIC.seqnoUp, msg);
}

void onEvent(ev_t ev) {
    if (ev == EV_TXSTART) {
        Serial.printf("TXSTART freq=%lu sf=%d\n", (unsigned long)LMIC.freq, getSf(LMIC.rps) + 6);
    } else if (ev == EV_TXCOMPLETE) {
        Serial.println("TXCOMPLETE");
        os_setTimedCallback(&sendJob, os_getTime() + sec2osticks(SEND_INTERVAL_S), sendNext);
    }
}

void setup() {
    Serial.begin(115200);
    delay(1000);
    Serial.println("lorawan-test-node start");
    prefs.begin("lorawan");
    os_init();
    LMIC_reset();
    static const u1_t nwkskey[16] = NODE_NWKSKEY;
    static const u1_t appskey[16] = NODE_APPSKEY;
    LMIC_setSession(0x13, NODE_DEVADDR, const_cast<u1_t *>(nwkskey), const_cast<u1_t *>(appskey));
    LMIC_selectSubBand(1);  // Sub-band 2: 903.9-905.3 MHz, what the gateway listens on.
    LMIC_setLinkCheckMode(0);
    LMIC_setAdrMode(0);
    LMIC_setDrTxpow(DR_SF7, 14);
    LMIC.seqnoUp = prefs.getUInt("fcnt", 1);
    sendNext(&sendJob);
}

void loop() {
    os_runloop_once();
}
