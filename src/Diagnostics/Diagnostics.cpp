#include "pins.h"
#include "Diagnostics/Diagnostics.h"
#include <SD.h>
#include <SPI.h>
#include <WiFi.h>
#include "version.h"

const char* diagnosticsFile = "/diagnostics.log";

bool initDiagnostics() {
    #ifdef DIAGNOSTIC_LOGGING
    if (!SD.begin(SD_CS)) {
        Serial.println("Failed to initialize SD card for diagnostics.");
        return false;
    }
    Serial.println("Diagnostics initialized successfully.");
    return true;
    #else
    Serial.println("Diagnostics logging is disabled.");
    return false;
    #endif
}

#ifdef DIAGNOSTIC_LOGGING

void logSystemResources() {
    File file = SD.open(diagnosticsFile, FILE_APPEND);
    if (file) {
        uint32_t totalHeap = ESP.getHeapSize();
        uint32_t freeHeap = ESP.getFreeHeap();
        uint32_t usedHeap = totalHeap - freeHeap;
        float memoryUsagePercent = (float(usedHeap) / float(totalHeap)) * 100;
        unsigned long uptime = millis() / 1000;

        file.printf("System Resources - Version: %s\n", CODE_VERSION);
        file.printf("Uptime: %lus, Memory Used: %lu bytes (%.2f%%)\n", uptime, usedHeap, memoryUsagePercent);
        file.close();
        Serial.println("System resources logged.");
    } else {
        Serial.println("Failed to log system resources.");
    }
}

void logWiFiDiagnostics() {
    File file = SD.open(diagnosticsFile, FILE_APPEND);
    if (file) {
        int8_t rssi = WiFi.RSSI();
        file.printf("Wi-Fi Diagnostics - RSSI: %ddBm\n", rssi);
        file.close();
        Serial.println("Wi-Fi diagnostics logged.");
    } else {
        Serial.println("Failed to log Wi-Fi diagnostics.");
    }
}

void logSDCardDiagnostics() {
    File file = SD.open(diagnosticsFile, FILE_APPEND);
    if (file) {
        uint32_t cardSize = SD.cardSize() / (1024 * 1024);  
        uint32_t usedBytes = cardSize - (SD.totalBytes() / (1024 * 1024));
        file.printf("SD Card Diagnostics - Total: %lu MB, Used: %lu MB\n", cardSize, usedBytes);
        file.close();
        Serial.println("SD card diagnostics logged.");
    } else {
        Serial.println("Failed to log SD card diagnostics.");
    }
}

void runDiagnostics() {
    if (SD.exists(diagnosticsFile) || SD.open(diagnosticsFile, FILE_WRITE)) {
        logSystemResources();
        logWiFiDiagnostics();
        logSDCardDiagnostics();
        Serial.println("Diagnostics run and logged to SD card.");
    } else {
        Serial.println("Failed to create diagnostics log file on SD card.");
    }
}

#endif  // DIAGNOSTIC_LOGGING
