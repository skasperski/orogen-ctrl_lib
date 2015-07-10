/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "CartesianPositionController.hpp"
#include <ctrl_lib/PositionControlFeedForward.hpp>

using namespace ctrl_lib;

CartesianPositionController::CartesianPositionController(std::string const& name)
    : CartesianPositionControllerBase(name){
}

CartesianPositionController::CartesianPositionController(std::string const& name, RTT::ExecutionEngine* engine)
    : CartesianPositionControllerBase(name, engine){
}

CartesianPositionController::~CartesianPositionController(){
}

bool CartesianPositionController::configureHook(){
    controller = new PositionControlFeedForward(6);

    if (! CartesianPositionControllerBase::configureHook())
        return false;
    return true;
}

bool CartesianPositionController::startHook(){
    if (! CartesianPositionControllerBase::startHook())
        return false;
    return true;
}

bool CartesianPositionController::readSetpoints(){
    if(_setpoint.read(setpoint) == RTT::NoData)
        return false;
    else
        return true;
}

bool CartesianPositionController::readFeedback(){
    if(_feedback.read(feedback) == RTT::NoData)
        return false;
    else{
        Eigen::VectorXd& xr = ((PositionControlFeedForward*)controller)->xr;
        Eigen::VectorXd& x = ((PositionControlFeedForward*)controller)->x;

        // Compute orientation error using angle-axis representation
        orientationError = setpoint.orientation * feedback.orientation.inverse();

        // Set reference value to control error and actual value to zero, since the controller
        // cannot deal with full poses
        xr.segment(0,3) = setpoint.position - feedback.position;
        xr.segment(3,3) = orientationError.axis()* orientationError.angle();
        x.resize(6,0.0);
        return true;
    }
}

void CartesianPositionController::writeControlOutput(const Eigen::VectorXd &y){
    controlOutput.velocity = y.segment(0,3);
    controlOutput.angular_velocity = y.segment(3,3);
    _ctrlOutput.write(controlOutput);
}

void CartesianPositionController::updateHook(){
    CartesianPositionControllerBase::updateHook();
}

void CartesianPositionController::errorHook(){
    CartesianPositionControllerBase::errorHook();
}

void CartesianPositionController::stopHook(){
    CartesianPositionControllerBase::stopHook();
}

void CartesianPositionController::cleanupHook(){
    delete controller;
    CartesianPositionControllerBase::cleanupHook();
}
