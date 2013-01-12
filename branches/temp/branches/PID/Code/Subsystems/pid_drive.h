#ifndef PID_DRIVE_H_
#define PID_DRIVE_H_

#include <string>
#include <queue>

#include "WPIlib.h"
#include "../Definitions/components.h"
#include "../tools.h"
#include "../Client/xbox.h"

class EncoderSource : public PIDSource {
public:
	EncoderSource(Encoder *);
	virtual ~EncoderSource();
	double PIDGet();
	std::deque<double> mHistory;
	static const unsigned int kMaxCount = 100;
	Encoder *mEncoder;
	// The encoder is publically exposed for now to help 
	// aid in configuration and debugging
};

class Tread : public PIDOutput {
public:
	Tread(SpeedController *frontWheel, SpeedController *backWheel);
	void PIDWrite(float);
	double Report();
	double mCurrent;
protected:
	SpeedController *mFrontWheel;
	SpeedController *mBackWheel;
};

class PidDrive : public BaseComponent {
public:
	PidDrive(SpeedController *leftFrontDrive,
			 SpeedController *leftBackDrive,
			 SpeedController *rightFrontDrive,
			 SpeedController *rightBackDrive,
			 Encoder *leftEncoder,
			 Encoder *rightEncoder);
	~PidDrive();
	
	void Disable();
	void Enable();
	void Tune(double left_p, double left_i, double left_d,
		 	  double right_p, double right_i, double right_d);
	void CalibrateEncoders(double distancePerPulse);
	void CalibrateEncoders(double left, double right);
	
	enum State {
		kStraight,
		kManual,
		kHalt
	};
	void SetState(State);
	State GetState();
	void TankDrive(float left, float right);
protected:
	EncoderSource *mLeftEncoderSource;
	EncoderSource *mRightEncoderSource;
	Tread *mLeftTread;
	Tread *mRightTread;
	PIDController *mLeftPid;
	PIDController *mRightPid;
	State mState;
};

class PidDriveController : public BaseController {
public:
	PidDriveController(PidDrive *, XboxController *xboxController);
	~PidDriveController();
	void TryTuning();
	void TrySetState();
	void Run();
protected:
	PidDrive *mPidDrive;
	XboxController *mXboxController;
	string mPreviousCommand;
};

#endif
