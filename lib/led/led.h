class Led
{
public:
    static Led &GET()
    {
        static Led instance;
        return instance;
    }

private:
    Led();
};