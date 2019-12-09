#include <math.h>
#include <uWS/uWS.h>
#include <iostream>
#include <string>
#include "json.hpp"
#include "PID.h"
#include <vector>
// for convenience
using nlohmann::json;
using std::string;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

#define TWIDDLE_RUN_COUNT 200
// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
string hasData(string s) {
    auto found_null = s.find("null");
    auto b1 = s.find_first_of("[");
    auto b2 = s.find_last_of("]");
    if (found_null != string::npos) {
        return "";
    }
    else if (b1 != string::npos && b2 != string::npos) {
        return s.substr(b1, b2 - b1 + 1);
    }
    return "";
}

int main(int argc, char *argv[]) {
    uWS::Hub h;
    PID pid;
    /**
     * TODO: Initialize the pid variable.
     */
    double err = 0.0;
    int count = 0;
    bool testRun = false;

#ifdef RUN_TWIDDLE
    if (argc == 1) {
        pid.Init(2.1, 0.007, 157.5);
        testRun = true;
    } else {
        if (argc < 4) {
            std::cout << "Please enter valid parameters.\n";
            exit(0);
        } 

        if (argc == 4) {
            testRun = true;
        }
        pid.Init(std::stof(argv[1]), std::stof(argv[2]), std::stof(argv[3]));
    }
#else

    pid.Init(2.1, 0.007, 157.5);
    //pid.Init(0.2, 0.0003, 3.0);
#endif

    h.onMessage([&pid,  &count, &err, &testRun](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, 
                uWS::OpCode opCode) {
            // "42" at the start of the message means there's a websocket message event.
            // The 4 signifies a websocket message
            // The 2 signifies a websocket event
            if (length && length > 2 && data[0] == '4' && data[1] == '2') {
            auto s = hasData(string(data).substr(0, length));

            if (s != "") {
            auto j = json::parse(s);

            string event = j[0].get<string>();

            if (event == "telemetry") {
            std::cout << "Telemetry Begins";
            // j[1] is the data JSON object
            double cte = std::stod(j[1]["cte"].get<string>());
            double speed = std::stod(j[1]["speed"].get<string>());
            double angle = std::stod(j[1]["steering_angle"].get<string>());
            double steer_value;
            /**
             * TODO: Calculate steering value here, remember the steering value is
             *   [-1, 1].
             * NOTE: Feel free to play around with the throttle and speed.
             *   Maybe use another PID controller to control the speed!
             */
#ifdef RUN_TWIDDLE
            if (!testRun) {
                count++;
                if (count > TWIDDLE_RUN_COUNT) {
                    err += (cte * cte);
                }
                if(count == (2 * TWIDDLE_RUN_COUNT)) {
                    err = err / TWIDDLE_RUN_COUNT;
                    std::cout << "The Error in this Run = " << err << "\n";
                    exit(0);
                }
            }
#endif
            pid.UpdateError(cte);

            steer_value = pid.TotalError();
            // DEBUG
            std::cout << "CTE: " << cte << " Steering Value: " << steer_value 
                << "speed: " << speed << std::endl;

            json msgJson;
            msgJson["steering_angle"] = steer_value;
            msgJson["throttle"] = 0.3;
            auto msg = "42[\"steer\"," + msgJson.dump() + "]";
            std::cout << msg << std::endl;
            ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
            }  // end "telemetry" if
            } else {
                // Manual driving
                string msg = "42[\"manual\",{}]";
                ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
            }
            }  // end websocket message if
    }); // end h.onMessage

    h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
            std::cout << "Connected!!!" << std::endl;
            });

    h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, 
                char *message, size_t length) {
            ws.close();
            std::cout << "Disconnected" << std::endl;
            });

    int port = 4567;
    if (h.listen(port)) {
        std::cout << "Listening to port " << port << std::endl;
    } else {
        std::cerr << "Failed to listen to port" << std::endl;
        return -1;
    }

    h.run();
}
