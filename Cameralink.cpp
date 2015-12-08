 #include "Cameralink.h"


namespace andreasvh{







//resetting values to their default as in the beggining of the project
void CameraLink::setDefaultValues(){
    IPStart = 100;
    IPEnd = 150;
    username = "admin";
    password = "1234";
    port = "8080";
}




//setrs up the connection to the camera
bool CameraLink::setUpConnection(){

    using namespace std;
    cv::VideoCapture vcap;

    //const std::string videoStreamAddress = "http://admin:1234@192.168.0.101:8080/videostream.asf?user=admin&pwd=1234&resolution=64&rate=0";
    std::string connectionHead = addressStart + username + ":" + password + "@" + ipaddress;
    std::string connectionTail = ":" + port + "/videostream.asf?user=" + username + "&pwd=" + password + "&resolution=64&rate=0";
    //open the video stream and make sure it's opened


    bool succeeded = false;
    for (int i = IPStart; i <= IPEnd; i++){
        connectionAddress = connectionHead + to_string(i) + connectionTail;

        if(!vcap.open(connectionAddress)) {
            std::cout << "Error opening video stream for " << ipaddress << i << std::endl;
        }
        else{
            succeeded = true;
            break;
        }
    }
    if (!succeeded){
        std::cout << "Error opening video stream, exiting program" << std::endl;
        return false;
    }
    return true;
}


int CameraLink::getIPStart(){
    return IPStart;
}

int CameraLink::getIPEnd(){
    return IPEnd;
}


std::string CameraLink::getUsername(){
    return username;
}

std::string CameraLink::getPassword(){
    return password;
}

std::string CameraLink::getPort(){
    return port;
}

std::string CameraLink::getConnectionAddress(){
    return connectionAddress;
}

void CameraLink::setUsername(std::string name){
    username= name;
}
void CameraLink::setPassword(std::string password){
    this->password = password;
}
void CameraLink::setStartIP(int ip){
    IPStart= ip;
}
void CameraLink::setEndIP(int ip){
    IPEnd = ip;
}

void CameraLink::setPort(std::string port){
    this->port = port;
}
void CameraLink::setConnAddress(std::string adr){
    connectionAddress= adr;
}

CameraLink::~CameraLink(){

}
}



