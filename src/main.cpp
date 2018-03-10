#include <uWS/uWS.h>
#include <iostream>
#include "json.hpp"
#include "PID.h"
#include <math.h>

// for convenience
using json = nlohmann::json;

// For converting back and forth between radians and degrees.
constexpr double pi() { return M_PI; }
double deg2rad(double x) { return x * pi() / 180; }
double rad2deg(double x) { return x * 180 / pi(); }

// Checks if the SocketIO event has JSON data.
// If there is data the JSON object in string format will be returned,
// else the empty string "" will be returned.
std::string hasData(std::string s) {
  auto found_null = s.find("null");
  auto b1 = s.find_first_of("[");
  auto b2 = s.find_last_of("]");
  if (found_null != std::string::npos) {
    return "";
  }
  else if (b1 != std::string::npos && b2 != std::string::npos) {
    return s.substr(b1, b2 - b1 + 1);
  }
  return "";
}

int main()
{
  uWS::Hub h;

  PID pid;
  // TODO: Initialize the pid variable.
  // some init value, still need to be twiddle.
  // manual tuning tips: https://discussions.udacity.com/t/pid-instability-manual-twiddle-tuning/334048/2?u=sun.pochin
  // Try to find a value for Kp which keeps you car on the track atleast for 2-3 seconds. The car might oscillate but that is fine.
  // To tune down the oscillation gradually try with increased Kd values.
  // Increasing Kd reduces oscillations, whereas increasing Kp increases 
  // the magnitude of turning. You need to find a sweet spot where the car turns 
  // sufficiently but does not oscillate. 
  // Once you find Kp and Kd values which work well, try very small values of Ki. 
  // Don’t be surprised if Ki is many orders of magnitude lower than Kp or Kd.
  // It may also help to have another PID controller to control the speed. 
  // Having a nearly constant speed makes controlling steering a lot easier.
  
  // turning not enough, failed at the first turning. increase Kp.
  pid.Init(1.0, 0.0, 0.0);

  // Kp : 1, Kd: 1.1 ^ 16
  // oscillates too much after the bridge, tune down by 10.
  // pid.Init(1.0, 0.0, 4.59);

  // (1.1 ^ 8) * 0.1 = 0.214, (1.1 ^ 45)  * 0.1  = 7.2890
  pid.Init(0.214, 0.01, 7.2890);


  h.onMessage([&pid](uWS::WebSocket<uWS::SERVER> ws, char *data, size_t length, uWS::OpCode opCode) {
    // "42" at the start of the message means there's a websocket message event.
    // The 4 signifies a websocket message
    // The 2 signifies a websocket event
    if (length && length > 2 && data[0] == '4' && data[1] == '2')
    {
      auto s = hasData(std::string(data).substr(0, length));
      if (s != "") {
        auto j = json::parse(s);
        std::string event = j[0].get<std::string>();
        if (event == "telemetry") {
          // j[1] is the data JSON object
          double cte = std::stod(j[1]["cte"].get<std::string>());
          double speed = std::stod(j[1]["speed"].get<std::string>());
          double angle = std::stod(j[1]["steering_angle"].get<std::string>());
          double steer_value;
          /*
          * TODO: Calcuate steering value here, remember the steering value is
          * [-1, 1].
          * NOTE: Feel free to play around with the throttle and speed. Maybe use
          * another PID controller to control the speed!
          */
          pid.UpdateError(cte);
          // note: I'm really bad at these degrees/radius conversion. :(
          // https://discussions.udacity.com/t/what-does-steering-value-1-1-mean/478335/4?u=sun.pochin
          // steer_value = deg2rad(pid.TotalError() ) / deg2rad(25) ;
          // steer_value = pid.TotalError() / deg2rad(25) ;
          steer_value = pid.TotalError();
          // DEBUG
          std::cout << "frame_cnt: " << pid._frame_cnt << std::endl;
          // // reset the simulation after 1000 frames, it's for the twiddle algorithm.
          // if (1000 == pid._frame_cnt) {
          //   pid._frame_cnt = 0;
          //   std::string msg = "42[\"reset\",{}]";
          //   ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
          // }
          // std::cout << "CTE: " << cte << " Steering Value: " << steer_value << std::endl;
          // std::cout << "angle : " << angle << ", pid.steer: " << pid._steer << ", steer_value : " << steer_value << std::endl;

          json msgJson;
          msgJson["steering_angle"] = steer_value;
          msgJson["throttle"] = 0.3;

          auto msg = "42[\"steer\"," + msgJson.dump() + "]";
          std::cout << msg << std::endl;
          ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
        }
      } else {
        // Manual driving
        std::string msg = "42[\"manual\",{}]";
        ws.send(msg.data(), msg.length(), uWS::OpCode::TEXT);
      }
    }
  });

  // We don't need this since we're not using HTTP but if it's removed the program
  // doesn't compile :-(
  h.onHttpRequest([](uWS::HttpResponse *res, uWS::HttpRequest req, char *data, size_t, size_t) {
    const std::string s = "<h1>Hello world!</h1>";
    if (req.getUrl().valueLength == 1)
    {
      res->end(s.data(), s.length());
    }
    else
    {
      // i guess this should be done more gracefully?
      res->end(nullptr, 0);
    }
  });

  h.onConnection([&h](uWS::WebSocket<uWS::SERVER> ws, uWS::HttpRequest req) {
    std::cout << "Connected!!!" << std::endl;
  });

  h.onDisconnection([&h](uWS::WebSocket<uWS::SERVER> ws, int code, char *message, size_t length) {
    ws.close();
    std::cout << "Disconnected" << std::endl;
  });

  int port = 4567;
  if (h.listen(port))
  {
    std::cout << "Listening to port " << port << std::endl;
  }
  else
  {
    std::cerr << "Failed to listen to port" << std::endl;
    return -1;
  }
  h.run();
}
