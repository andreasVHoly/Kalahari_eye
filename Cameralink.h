#ifndef CAMERALINK_H
#define CAMERALINK_H
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <iostream>
#include <string>


namespace andreasvh{
class CameraLink
{
public:
    CameraLink(std::string un, std::string p, int IPStart, int IPEnd, std::string port):username(un), password(p), IPStart(IPStart), IPEnd(IPEnd), port(port){}
    CameraLink();
    bool setUpConnection();
    ~CameraLink();



    //getters
    int getIPStart();
    int getIPEnd();
    std::string getUsername();
    std::string getPassword();
    std::string getPort();
    std::string getConnectionAddress();
    //setters
    void setUsername(std::string name);
    void setPassword(std::string password);
    void setStartIP(int ip);
    void setEndIP(int ip);
    void setPort(std::string port);
    void setConnAddress(std::string adr);
    void setDefaultValues();

private:
    //VARIABLES


    std::string username;
    std::string password;
    int IPStart;
    int IPEnd;
    std::string port;
    std::string addressStart = "http://";
    std::string connectionAddress;
    std::string ipaddress = "192.168.0.";



};
}//namespace
#endif // CAMERALINK_H
