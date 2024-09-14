#include <string>
#include <vector>
#include <Arduino.h>

std::string data_format(std::vector<double> vec)
{
    std::string str = "*O,";
    for (auto &it : vec)
    {
        str += std::to_string(it);
        str += ",";
    };
    str += "I#";
    return str;
}

std::vector<std::string> serializedResponse(std::string data)
{
    char del = ',';
    std::vector<std::string> values = {};

    std::string temp = "";
    for (int i = 0; i <= (int)data.size(); i++)
    {
        if (i == data.size() || data[i] == del)
        {
            values.push_back(temp);
            temp = "";
        }
        else
        {
            temp += data[i];
        }
    }
    if (!(values.at(0) == "*O" && values.at(values.size() - 1) == "I#"))
    {
        Serial.println("Invalid Response from mqtt");
        return {};
    }
    // removing *O and I#
    values.erase(values.begin());
    values.erase(values.end());

    // transforming to void
    return values;
}

std::vector<float> convertVecFloatStr(std::vector<std::string> values)
{
    std::vector<float> floatValues;

    for (const std::string &str : values)
    {
        try
        {
            float convertedValue = std::stof(str);
            floatValues.push_back(convertedValue);
        }
        catch (const std::invalid_argument &e)
        {
            Serial.println("Error converting string to float: ");
        }
    }

    return floatValues;
}