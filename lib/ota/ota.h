class OTA
{
public:
    static OTA &GET()
    {
        static OTA instance;
        return instance;
    }
    void handleOta();

private:
    OTA();
};