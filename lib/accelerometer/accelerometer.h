#include <vector>

class Accelerometer
{
public:
    static Accelerometer &GET()
    {
        static Accelerometer instance;
        return instance;
    }
    void initialize();
    void handleAccelerometer();

private:
    Accelerometer(){};
    std::vector<double> getVal();
};