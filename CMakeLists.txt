cmake_minimum_required(VERSION 3.22)
project(Intersection_Traffic_Simulation)

set(CMAKE_CXX_STANDARD 14)

add_executable(Intersection_Traffic_Simulation main.cpp HeadClass.cpp HeadClass.h Vehicle.cpp Vehicle.h ApplicationClass.cpp ApplicationClass.h Road.cpp Road.h ControlPanel.cpp ControlPanel.h InputData.cpp InputData.h OutputData.cpp OutputData.h TrafficLight.cpp TrafficLight.h)
