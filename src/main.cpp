#include "main.h"
#include "pros/misc.h"
#include "pros/motors.hpp"
#include <math.h>
#include <string>
#include <vector>
#include "okapi/api.hpp"
#include "okapi/api/chassis/controller/chassisControllerPid.hpp"
using namespace okapi;


pros::Motor FrontLeft(18, false);
pros::Motor FrontRight(20, true);
pros::Motor BackLeft(14, true);
pros::Motor BackRight(9, false);
pros::Motor MidRight(15,false);
pros::Motor MidLeft(16,false);
pros::Motor Catapult(7, false);
pros::Motor Arm(17,false); 
pros::Motor Intake(13,false);
pros::Rotation RotationSensor(12);
pros::ADIDigitalOut Piston('A');


//pros::Motor Catapult();   //add port

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.


/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.







//pros::Motor Catapult();   //add port

/**
 * A callback function for LLEMU's center button.
 *
 * When this callback is fired, it will toggle line 2 of the LCD text between
 * "I was pressed!" and nothing.
 */

void on_center_button() {
	static bool pressed = false;
	pressed = !pressed;
	if (pressed) {
		pros::lcd::set_text(2, "I was pressed!");
	} else {
		pros::lcd::clear_line(2);
	}
	
}

/**
 * Runs initialization code. This occurs as soon as the program is started.
 *
 * All other competition modes are blocked by initialize; it is recommended
 * to keep execution time for this mode under a few seconds.
 */
void initialize() {
	pros::lcd::initialize();
	pros::lcd::set_text(1, "Hello PROS User!");

	pros::lcd::register_btn1_cb(on_center_button);

}

/**
 * Runs while the robot is in the disabled state of Field Management System or
 * the VEX Competition Switch, following either autonomous or opcontrol. When
 * the robot is enabled, this task will exit.
 */
void disabled() {}

/**
 * Runs after initialize(), and before autonomous when connected to the Field
 * Management System or the VEX Competition Switch. This is intended for
 * competition-specific initialization routines, such as an autonomous selector
 * on the LCD.
 *
 * This task will exit when the robot is enabled and autonomous or opcontrol
 * starts.
 */
void competition_initialize() {}

/**
 * Runs the user autonomous code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the autonomous
 * mode. Alternatively, this function may be called in initialize or opcontrol
 * for non-competition testing purposes.
 *
 * If the robot is disabled or communications is lost, the autonomous task
 * will be stopped. Re-enabling the robot will restart the task, not re-start it
 * from where it left off.
 * 
 * 
 * 
 */



void autonomous() {

	pros::lcd::set_text(1, "THIS IS AUTON!");

	std::shared_ptr<ChassisController> bot = ChassisControllerBuilder()     
			.withMotors(18, -20, -9, 14)  // front right and back right were reversed in order to go forward   
			// change P then D first then I only if necessary  
			//start with P I and D with zero 
			.withGains( //0.7, 0, 0.1 results: faster, shaking less violently 0
		//0.5 = 
				{0.001, 0, 0}, // Distance controller gains 
				{0.000, 0, 0}, // turn controller gains
				{0.00, 0, 0.0000}	// Angle controller (helps bot drive straight)
				)
			.withMaxVelocity(115)
			// Green gearset, 3 inch wheel diam, 9 inch wheel track

			.withDimensions(AbstractMotor::gearset::green, {{16_in, 10_in}, imev5GreenTPR})
			.build();

	pros::lcd::set_text(1, "THIS IS AUTON!");
	bot->moveDistance(-4_ft); //drive forward 33 inches, turn 90 degrees, 77 inches 
	Arm.tare_position();
	Arm.move_absolute(50,100);
	pros::delay(2000);
	Catapult.tare_position();
	Catapult.move_absolute(50,100);


	
}



	

/**
 * Runs the operator control code. This function will be started in its own task
 * with the default priority and stack size whenever the robot is enabled via
 * the Field Management System or the VEX Competition Switch in the operator
 * control mode.
 *
 * If no competition control is connected, this function will run immediately
 * following initialize().
 *
 * If the robot is disabled or communications is lost, the
 * operator control task will be stopped. Re-enabling the robot will restart the
 * task, not resume it from where it left off.
 */
void opcontrol() {


pros::Motor FrontLeft(18, false);
pros::Motor FrontRight(20, true);
pros::Motor BackLeft(14, true);
pros::Motor BackRight(9, false);
pros::Motor MidRight(15,false);
pros::Motor MidLeft(16,false);
pros::Motor Catapult(7, false);
pros::Motor Arm(17,false); 
pros::Motor Intake(13,false);
 pros::Rotation RotationSensor(12);
pros::ADIDigitalOut Piston('A');

	pros::lcd::set_text(1,"READY TO DRIVE");
	int yMotion;
	int xMotion;
	int value; 


	while (true)
	{
		pros::lcd::set_text(1, "Front Left Motor:" +  std::to_string(FrontLeft.get_position()));
		pros::lcd::set_text(2, "Front Right Motor:" +std::to_string(FrontRight.get_position()));
		pros::lcd::set_text(3, "Back Left Motor:" + std::to_string(BackLeft.get_position()));
		pros::lcd::set_text(4, "Back Right Motor:" + std::to_string(BackRight.get_position()));
		pros::lcd::set_text(5, "Rotation Sensor: " + std::to_string(RotationSensor.get_angle()));
		pros::Controller master(pros::E_CONTROLLER_MASTER);

		// driving control code

		yMotion = master.get_analog(pros::E_CONTROLLER_ANALOG_RIGHT_X); // ik this looks wrong, but it works
		xMotion = master.get_analog(pros::E_CONTROLLER_ANALOG_LEFT_Y);


		int right = -xMotion + yMotion; //-power + turn
		int left = xMotion + yMotion;	// power + turn

		FrontLeft.move(left); // Swap negatives if you want the bot to drive in the other direction
		BackLeft.move(-left);
		// MidLeft.move(left);
		BackRight.move(right);
		FrontRight.move(-right);
		// MidRight.move(right);



        if (master.get_digital(DIGITAL_R1))
        { 
			RotationSensor.set_data_rate(0);   	
          //   pros::lcd::set_text(5, std::to_string(RotationSensor.get_angle()));
          //  pros::lcd::set_text(6, std::to_string(Catapult.get_position()));
            Catapult.move_velocity(200); 
          //   pros::lcd::set_text(5, std::to_string(RotationSensor.get_angle()));
         //   pros::lcd::set_text(6, std::to_string(Catapult.get_position()));	 
        }
        else if (RotationSensor.get_angle() <= 34000) { //3050 ->2700(no data rate)-> 3565 (data rate)
		//	RotationSensor.set_data_rate(0);  
            Catapult.move_velocity(200);

        }
        else
        {   
			RotationSensor.set_data_rate(55);         
            Catapult.move_velocity(0);
        }

		if(master.get_digital(DIGITAL_L1))
		{
			Arm.move_velocity(-112);
			pros::lcd::set_text(5,"Arm Velocity:" + std::to_string(Arm.get_actual_velocity()));

		}
		else if(master.get_digital(DIGITAL_L2))
		{
			Arm.move_velocity(112);
			pros::lcd::set_text(5,"Arm Velocity:" + std::to_string(Arm.get_actual_velocity()));
		}
		else{
			Arm.move_velocity(0);                                                        
		}


		if(master.get_digital(DIGITAL_R2))
		{
			Intake.move_velocity(-200);
			pros::lcd::set_text(5,"Intake Velocity:" + std::to_string(Intake.get_actual_velocity()));

		}
		else if(master.get_digital(DIGITAL_DOWN))
		{
			Intake.move_velocity(200);
			pros::lcd::set_text(5,"Intake Velocity:" + std::to_string(Intake.get_actual_velocity()));

		}					
		else 
		{
			Intake.move_velocity(0);
		}



/* 		if(master.get_digital(DIGITAL_DOWN))
		{
			Intake.move_velocity(200);
			pros::lcd::set_text(5,"Intake Velocity:" + std::to_string(Intake.get_actual_velocity()));
		}
		else {
			Intake.move_velocity(0);
		} */


	if (master.get_digital(DIGITAL_A))
		{
			Piston.set_value(false);
		}
	else{
		Piston.set_value(true);
		}



		pros::delay(20);
	}
}