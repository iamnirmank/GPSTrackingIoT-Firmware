#include <SoftwareSerial.h>
#include <ELMduino.h>

static RTC_DATA_ATTR float avgSpeed;
static RTC_DATA_ATTR int valueCount;
static RTC_DATA_ATTR float recordedDistance;
static RTC_DATA_ATTR float recordedFuelLevel;

class ELM
{
public:
    std::string elm_data = "";
    static ELM &GET()
    {
        static ELM instance;
        return instance;
    }
    void initialize();
    static void handleData(void *pvparams)
    {
        while (true)
        {
            GET().IHandleData();
            vTaskDelay(pdTICKS_TO_MS(2000));
        };
        return;
    }
    void IHandleData();

private:
    enum MetricType
    {
        FUEL,
        FUEL_PRESSURE,
        ENGINE_RPM,
        ENGINE_COOLANT_TEMP,
        FUEL_RATE,
        THROTTLE_POSITION,
        DISTANCE,
        SPEED,
        ACCELERATOR_PEDAL_POS,
        INTAKE_MANIFOLD_ABSOLUTE_PRESSURE,
        AIR_INTAKE_TEMPERATURE,
        FUEL_AIR_EQUIVALENCE_RATIO,
        ENGINE_LOAD,
        SHORT_TERM_FUEL_TRIM_BANK_1,
        LONG_TERM_FUEL_TRIM_BANK_1,
        SHORT_TERM_FUEL_TRIM_BANK_2,
        LONG_TERM_FUEL_TRIM_BANK_2,
        FREEZE_DTC,
        ENGINE_RUN_TIME,
    };

    EspSoftwareSerial::UART mySerial;
    ELM327 myELM327;
    unsigned long startTime = millis();
    const int WAIT_TIME = 5000;
    double getData(enum MetricType);
    ELM();
};
