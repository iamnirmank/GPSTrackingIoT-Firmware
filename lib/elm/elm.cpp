#include "elm.h"
#include <vector>
#include "shared.h"
#include "handler.h"
#include <string>

ELM::ELM()
{
    mySerial.begin(38400, EspSoftwareSerial::SWSERIAL_8N1, 17, 18);
    initialize();
}

void ELM::initialize()
{

    if (myELM327.connected)
    {
        // Serial.println("Connected");
        return;
    }

    if (!myELM327.begin(mySerial, false, 2000, '0'))
    {
#warning " Don't call 'ELM:initialize' within a task,-> may trigger WDT"
        // Serial.println("Couldn't connect to OBD scanner");
        vTaskDelay(pdMS_TO_TICKS(100)); // dangerous
        // ESP.restart(); // restart if you want to re-initialize or call a different function
    }
}

void ELM::IHandleData()
{
#warning "Put a guard here ; dont run below code if not initialized"
    std::vector<double> vec = {};
    double speed = getData(SPEED);
    float distance = getData(DISTANCE);
    float rpm = getData(ENGINE_RPM);
    float fuelPressure = getData(FUEL_PRESSURE);
    float fuel = getData(FUEL);
    float coolantTemp = getData(ENGINE_COOLANT_TEMP);
    float vechilePerformance = ((speed * getData(THROTTLE_POSITION) * getData(ACCELERATOR_PEDAL_POS)) / (rpm * getData(FUEL_RATE)) / 0.02 * 100);
    float driverPerformance = (((rpm * getData(ENGINE_LOAD) * getData(ACCELERATOR_PEDAL_POS)) /
                                (getData(AIR_INTAKE_TEMPERATURE) * getData(FUEL_AIR_EQUIVALENCE_RATIO) * fuelPressure * getData(INTAKE_MANIFOLD_ABSOLUTE_PRESSURE))) /
                               10 * 100);
    float fuel_trim = (getData(SHORT_TERM_FUEL_TRIM_BANK_1) + getData(LONG_TERM_FUEL_TRIM_BANK_1) + getData(SHORT_TERM_FUEL_TRIM_BANK_2) + getData(LONG_TERM_FUEL_TRIM_BANK_2)) / 4;
    float fuelEfficiency = (100 - (getData(ENGINE_LOAD) + fuel_trim + getData(AIR_INTAKE_TEMPERATURE)) / getData(FUEL_RATE));
    if (speed > 10)
    {
        avgSpeed = ((avgSpeed * (valueCount - 1)) + speed) / valueCount;
        valueCount++;
    }
    if (Handler::GET().tripRequest)
    {
        recordedDistance = getData(DISTANCE);
        recordedFuelLevel = fuel;
        Handler::GET().tripRequest = false;
    }

    Handler::GET()
        .handleRequest(C, data_format({fuel,
                                       fuelPressure,
                                       rpm,
                                       speed,
                                       distance,
                                       getData(FREEZE_DTC),
                                       getData(ENGINE_RUN_TIME),
                                       coolantTemp,
                                       vechilePerformance,
                                       fuelEfficiency,
                                       driverPerformance}));
    Serial.println(data_format(vec).c_str());
    return;
}

double ELM::getData(enum MetricType metricType)
{
    // Serial.println("In Get Data");
    initialize();
    startTime = millis();
    while (millis() - startTime < WAIT_TIME)
    {
        double value = 0;
        switch (metricType)
        {
        case FUEL:
            value = myELM327.fuelLevel();
            break;
        case FUEL_PRESSURE:
            value = myELM327.fuelPressure();
            break;
        case ENGINE_RPM:
            value = myELM327.rpm();
            break;
        case ENGINE_COOLANT_TEMP:
            value = myELM327.engineCoolantTemp();
            break;
        case FUEL_RATE:
            value = myELM327.fuelRate();
            break;
        case THROTTLE_POSITION:
            value = myELM327.throttle();
            break;
        case DISTANCE:
            value = myELM327.distSinceCodesCleared();
            break;
        case SPEED:
            value = myELM327.mph();
            break;
        case ACCELERATOR_PEDAL_POS:
            value = myELM327.relativePedalPos();
            break;
        case INTAKE_MANIFOLD_ABSOLUTE_PRESSURE:
            value = myELM327.manifoldPressure();
            break;
        case AIR_INTAKE_TEMPERATURE:
            value = myELM327.intakeAirTemp();
            break;
        case FUEL_AIR_EQUIVALENCE_RATIO:
            value = myELM327.commandedAirFuelRatio();
            break;
        case ENGINE_LOAD:
            value = myELM327.engineLoad();
            break;

        case SHORT_TERM_FUEL_TRIM_BANK_1:
            value = myELM327.shortTermFuelTrimBank_1();
            break;
        case LONG_TERM_FUEL_TRIM_BANK_1:
            value = myELM327.longTermFuelTrimBank_1();
            break;
        case SHORT_TERM_FUEL_TRIM_BANK_2:
            value = myELM327.shortTermFuelTrimBank_2();
            break;
        case LONG_TERM_FUEL_TRIM_BANK_2:
            value = myELM327.longTermFuelTrimBank_2();
            break;
        case FREEZE_DTC:
            value = myELM327.freezeDTC();
            break;
        case ENGINE_RUN_TIME:
            value = myELM327.runTime();
            break;
        }
        if (myELM327.nb_rx_state == ELM_SUCCESS)
        {
            // Serial.println(value);
            return value;
        }
        else if (myELM327.nb_rx_state != ELM_GETTING_MSG)
        {
            // myELM327.printError();
            vTaskDelay(pdMS_TO_TICKS(100));
            return value;
        }
    }
    // Serial.println("LOG:: ELM Timeout");
    return 0.0f;
}
