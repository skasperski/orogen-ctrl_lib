/* Generated from orogen/lib/orogen/templates/tasks/Task.cpp */

#include "JointPositionController.hpp"
#include <ctrl_lib/ProportionalController.hpp>

using namespace ctrl_lib;

JointPositionController::JointPositionController(std::string const& name)
    : JointPositionControllerBase(name){
}

JointPositionController::JointPositionController(std::string const& name, RTT::ExecutionEngine* engine)
    : JointPositionControllerBase(name, engine){
}

JointPositionController::~JointPositionController(){
}

bool JointPositionController::configureHook(){

    field_names = _field_names.get();
    controller = new ProportionalController(field_names.size());
    control_output.resize(field_names.size());
    control_output.names = field_names;

    if (! JointPositionControllerBase::configureHook())
        return false;

    return true;
}

bool JointPositionController::startHook(){
    if (! JointPositionControllerBase::startHook())
        return false;
    return true;
}

bool JointPositionController::readSetpoint(){
    if(_setpoint.readNewest(setpoint) == RTT::NewData){
        has_setpoint = true;
        extractPositions(setpoint, field_names, ((ProportionalController*)controller)->setpoint);
        extractVelocities(setpoint, field_names, ((ProportionalController*)controller)->feed_forward);
        _current_setpoint.write(setpoint);
    }

    return has_setpoint;
}

bool JointPositionController::readFeedback(){
    if(_feedback.readNewest(feedback) == RTT::NewData){
        has_feedback = true;
        extractPositions(feedback, field_names, ((ProportionalController*)controller)->feedback);
        _current_feedback.write(feedback);
        return true;
    }
    else
        return false;
}

void JointPositionController::writeControlOutput(const base::VectorXd &ctrl_output_raw){
    for(size_t i = 0; i < field_names.size(); i++)
        control_output[i].speed = ctrl_output_raw(i);
    control_output.time = base::Time::now();
    _control_output.write(control_output);
}

void JointPositionController::updateHook(){
    JointPositionControllerBase::updateHook();
}

void JointPositionController::errorHook(){
    JointPositionControllerBase::errorHook();
}

void JointPositionController::stopHook(){
    JointPositionControllerBase::stopHook();
}

void JointPositionController::cleanupHook(){
    JointPositionControllerBase::cleanupHook();
}

void JointPositionController::reset(){
    if(has_feedback){
        setpoint.resize(field_names.size());
        setpoint.names = field_names;
        for(size_t i = 0; i < field_names.size(); i++){
            const base::JointState& state = feedback.getElementByName(field_names[i]);
            setpoint[i].position = state.position;
        }
        has_setpoint = true;
    }
}
