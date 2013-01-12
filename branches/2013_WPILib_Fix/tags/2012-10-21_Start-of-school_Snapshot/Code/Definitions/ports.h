#ifndef _PORTS_H
#define _PORTS_H

/**
 * @brief a series of typedefs to map the ports on the 
 * robot to names.
 * 
 * @details
 * There are several types of ports on the robot:
 *   - PWM ports
 *   - Digital Input/Output
 *   - Relays
 *   - Analog Breakout
 *   - Solenoid Breakout
 * 
 * Some of these ports can be found on the Digital Sidecar,
 * whereas others can be found on the cRIO.
 * 
 * Details about each can be found besides their respective
 * typedefs and namespaces.
 * 
 * This namespace a typedef for USB ports.  
 * USB ports the ones on the computer that 
 * joysticks plug into.
 * 
 * To use a port, use the double-colon syntax:
 * @code
 * Ports::DigitalSidecar::Pwm1
 * Ports::DigitalSidecar::Pwm2
 * @endcode
 * 
 * For example, to declare a Jaguar, you might do...
 * @code
 * Jaguar *j = new Jaguar(Ports::DigitalSidecar::Pwm2);
 * @endcode
 */
namespace Ports
{
	namespace DigitalSidecar
	{
		/**
		 * @brief Used frequently for speed-controllers (including Jaguars).
		 * 
		 * @details
		 * Stands for 'Pulse-width modulation'.
		 * 
		 * Known uses:
		 *   - Jaguars for the robot drive
		 *   - Other speed controllers for motors
		 */
		enum Pwm
		{
			Pwm1 = 1,
			Pwm2 = 2,
			Pwm3 = 3,
			Pwm4 = 4,
			Pwm5 = 5,
			Pwm6 = 6,
			Pwm7 = 7,
			Pwm8 = 8,
			Pwm9 = 9,
			Pwm10 = 10
		};
		
		/**
		 * @brief Used frequently for sensors.  When used with a jumper, can also
		 * be used to power servos.
		 * 
		 * @details
		 * Stands for 'General Purpose Input/Output'.
		 * 
		 * Known uses:
		 *   - Limit switches
		 *   - Servos
		 *   - Accelerometer/Gyro (?)
		 */
		enum Gpio
		{
			Gpio1 = 1,
			Gpio2 = 2,
			Gpio3 = 3,
			Gpio4 = 4,
			Gpio5 = 5,
			Gpio6 = 6,
			Gpio7 = 7,
			Gpio8 = 8,
			Gpio9 = 9,
			Gpio10 = 10,
			Gpio11 = 11,
			Gpio12 = 12,
			Gpio13 = 13,
			Gpio14 = 14
		};
		
		/**
		 * @brief Used mostly for spikes and pneumatics.
		 * 
		 * @details
		 * Known uses:
		 *    - Spikes
		 */
		enum Relay
		{
			Relay1 = 1,
			Relay2 = 2,
			Relay3 = 3,
			Relay4 = 4,
			Relay5 = 5,
			Relay6 = 6,
			Relay7 = 7,
			Relay8 = 8
		};
	}
	
	/**
	 * @brief Used mostly for joysticks (or other forms of input)
	 * 
	 * @details
	 * Known uses:
	 *   - Joysticks
	 * 
	 * Found on the USB dongle that attaches to the laptop
	 */
	enum Usb
	{
		Usb1 = 1,
		Usb2 = 2,
		Usb3 = 3,
		Usb4 = 4	// Not sure if we actually have a 4th one
	};
	
	namespace Crio
	{
		/**
		 * @brief Used mostly for sensors
		 * 
		 * @details
		 * Known uses:
		 *   - Ultrasound sensor
		 * 
		 * Found on top of the first module in the cRIO.
		 */
		enum AnalogBreakout
		{
			AnalogChannel1 = 1,
			AnalogChannel2 = 2,
			AnalogChannel3 = 3,
			AnalogChannel4 = 4,
			AnalogChannel5 = 5,
			AnalogChannel6 = 6,
			AnalogChannel7 = 7,
			AnalogChannel8 = 8
		};
		
		/**
		 * @brief Used mostly for pneumatics
		 */
		enum SolenoidBreakout
		{
			SolenoidBreakout1 = 1,
			SolenoidBreakout2 = 2,
			SolenoidBreakout3 = 3,
			SolenoidBreakout4 = 4,
			SolenoidBreakout5 = 5,
			SolenoidBreakout6 = 6,
			SolenoidBreakout7 = 7,
			SolenoidBreakout8 = 8
		};
		
		/**
		 * @brief Sections of the cRIO
		 * 
		 * @details
		 * This is a relatively new concept, which is meant
		 * to deal with how some FRC teams have 4-slot cRIOS
		 * while others have 8-slot cRIOS.
		 * 
		 * Slot -- the physical space you can plug a module
		 * in.  Module number -- something FIRST invented.
		 * 
		 * For a 8-slot cRIO:
		 *   - Slot 1: Module 1 {Analog Module 9201)
		 *   - Slot 2: Module 1 {Digital Module 9403)
		 *   - Slot 3: Module 1 {Solenoid Module 9472)
		 *   - Slot 4: Module 2 {empty)
		 *   - Slot 5: Module 2 {Analog Module 9201)
		 *   - Slot 6: Module 2 {Digital Module 9403)
		 *   - Slot 7: Module 2 {Solenoid Module 9472)
		 *   - Slot 8: Module   {empty)
		 */
		enum Module
		{
			Module1 = 1,
			Module2 = 2
		};
	}
}

#endif
