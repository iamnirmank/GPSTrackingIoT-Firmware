class GPIO
{
public:
    static GPIO &GET()
    {
        static GPIO instance;
        return instance;
    }
    enum BuzzerType
    {
        QUEUE,
        ALERT,
        WARNING
    };

    void handleIgnition();
    void handleBuzzer(enum BuzzerType);
    void handleInputs();

private:
    GPIO();
};
