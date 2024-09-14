#pragma once
#include <string>
#include <vector>

enum Topic
{
    G,
    C,
    E,
    A
};

enum ConnectionStatus
{
    CONNECTING,
    CONNECTED,
    NOT_CONNECTED
};

enum ResponseTopic
{
    Response_G,
    Response_O,
    Response_I,
    Response_Q,
    Response_A
};

typedef struct
{
    Topic topic;
    std::string data;
} DataObject;

std::string data_format(std::vector<double>);
std::vector<std::string> serializedResponse(std::string);
