#include "pid_drive.h"

EncoderSource::EncoderSource(Encoder *encoder) :
		PIDSource()
{
	mEncoder = encoder;
	mEncoder->SetPIDSourceParameter(mEncoder->kRate);
	mEncoder->Start();
	// Set pulse distance here?
	
	mHistory.push_front(0);
}

EncoderSource::~EncoderSource()
{
	// ignore
}

double EncoderSource::PIDGet() 
{
	// We could have just used the encoder directly and not create
	// a custom class, but until we have a clear idea of what to
	// do, I want to make sure we're as flexible as possible.
	double out = -mEncoder->PIDGet();
	mHistory.push_front(out);
	if (mHistory.size() > kMaxCount) {
		mHistory.pop_back();
	}
	double total = 0;
	for (unsigned int i = 0; i < mHistory.size(); i++) 
	{
		total += mHistory.at(i);
	}
	return total / mHistory.size();
}

Tread::Tread(SpeedController *frontWheel, SpeedController *backWheel) :
		PIDOutput()
{
	mFrontWheel = frontWheel;
	mBackWheel = backWheel;
	mCurrent = 0;
}

void Tread::PIDWrite(float speed)
{
	speed = Tools::Limit(speed, -0.03, 0.03);
	mCurrent += speed;
	mCurrent = Tools::Limit(mCurrent, -1.0, 1.0);
	mFrontWheel->Set(mCurrent);
	mBackWheel->Set(mCurrent);
}

double Tread::Report()
{
	return mCurrent;
}


//class PidDrive : public BaseComponent {
//public:
PidDrive::PidDrive(SpeedController *leftFrontDrive,
				   SpeedController *leftBackDrive,
				   SpeedController *rightFrontDrive,
				   SpeedController *rightBackDrive,
		           Encoder *leftEncoder,
		           Encoder *rightEncoder) :
		   BaseComponent() 
{
	mLeftTread = new Tread(leftFrontDrive, leftBackDrive);
	mRightTread = new Tread(rightFrontDrive, rightBackDrive);
	mLeftEncoderSource = new EncoderSource(leftEncoder);
	mRightEncoderSource = new EncoderSource(rightEncoder);
	
	CalibrateEncoders(0.0003);
	
	// Implementing PIDController:
	// PIDController(proportional, integral, derivative, PIDSource, PIDOutput)
	mLeftPid = new PIDController(0.4, 0, 0, mLeftEncoderSource, mLeftTread);
	mRightPid = new PIDController(0.4, 0, 0, mRightEncoderSource, mRightTread);
	
	mLeftPid->SetOutputRange(-0.03, 0.03);
	mRightPid->SetOutputRange(-0.03, 0.03);
	
	mState = kManual;
	
	mLeftPid->Enable();
	mRightPid->Enable();
}

PidDrive::~PidDrive()
{
	delete mLeftPid;
	delete mRightPid;
	delete mLeftEncoderSource;
	delete mRightEncoderSource;
	delete mLeftTread;
	delete mRightTread;
}

void PidDrive::Enable()
{
	mLeftPid->Enable();
	mRightPid->Enable();
}

void PidDrive::Disable()
{
	mLeftPid->Disable();
	mRightPid->Disable();
}

void PidDrive::Tune(double left_p, double left_i, double left_d,
	 	            double right_p, double right_i, double right_d)
{
	Disable();
	mLeftPid->SetPID(left_p, left_i, left_d);
	mRightPid->SetPID(right_p, right_i, right_d);
	mLeftPid->SetContinuous(true);
	mRightPid->SetContinuous(true);
	Enable();
}

void PidDrive::CalibrateEncoders(double distancePerPulse)
{
	mLeftEncoderSource->mEncoder->SetDistancePerPulse(distancePerPulse);
	mRightEncoderSource->mEncoder->SetDistancePerPulse(distancePerPulse);
}

void PidDrive::CalibrateEncoders(double left, double right)
{
	mLeftEncoderSource->mEncoder->SetDistancePerPulse(left);
	mRightEncoderSource->mEncoder->SetDistancePerPulse(right);
}

void PidDrive::SetState(State state)
{
	mState = state;
}

PidDrive::State PidDrive::GetState() 
{
	return mState;
}

void PidDrive::TankDrive(float left, float right)
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	
	if ((left < 0.2) && (left > -0.2)) {
		left = 0;
	}
	if ((right < 0.2) && (right > -0.2)) {
		right = 0;
	}
	
	if (mState == kManual) {
		s->Log("Manual", "Current PID Drive state");
		mLeftPid->SetSetpoint(left);
		mRightPid->SetSetpoint(right);
	} else if (mState == kStraight) {
		s->Log("Straight", "Current PID Drive state");
		float speed = (left + right) / 2;
		left = 0.05;
		right = 0.05;
		mLeftPid->SetSetpoint(speed);
		mRightPid->SetSetpoint(speed);
	} else if (mState == kHalt) {
		s->Log("Halt", "Current PID Drive state");
		mLeftPid->SetSetpoint(0);
		mRightPid->SetSetpoint(0);
	} else {
		// default state, should never happen.
		mLeftPid->SetSetpoint(0);
		mRightPid->SetSetpoint(0);
	}
	
	s->Log(left, "Input left");
	s->Log(right, "Input right");
	s->Log(mLeftPid->Get(), "Output left");
	s->Log(mRightPid->Get(), "Output right");
	
	s->Log(mLeftEncoderSource->PIDGet(), "Left Encoder Rate");
	s->Log(mRightEncoderSource->PIDGet(), "Right Encoder Rate");
	
	s->Log(mLeftTread->Report(), "Tread left");
	s->Log(mRightTread->Report(), "Tread right");
}



PidDriveController::PidDriveController(PidDrive *pidDrive, XboxController *xboxController) :
		BaseController()
{
	mPidDrive = pidDrive;
	mXboxController = xboxController;
	
	SmartDashboard::GetInstance()->PutString("PidDrive state", "manual");
	mPreviousCommand = "manual";
	
	SmartDashboard *s = SmartDashboard::GetInstance();
	s->PutString("Left P", "0.4");
	s->PutString("Left I", "0.0");
	s->PutString("Left D", "0.0");
	
	s->PutString("Right P", "0.4");
	s->PutString("Right I", "0.0");
	s->PutString("Right D", "0.0");
	
	s->PutString("PID Tune", "disable");
	s->PutString("PID Enabled", "enable");
	
	s->PutString("Left encoder distance per pulse", "0.00059");
	s->PutString("Right encoder distance per pulse", "0.00059");
}

PidDriveController::~PidDriveController()
{
	// empty
}


void PidDriveController::Run()
{
	TryTuning();
	TrySetState();
	
	if (!mXboxController->GetButton(mXboxController->B)) {
		mPidDrive->TankDrive(mXboxController->GetAxis(mXboxController->LeftY), 
							 mXboxController->GetAxis(mXboxController->RightY));
	} else {
		mPidDrive->TankDrive(0, 0);
	}
}

void PidDriveController::TryTuning()
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	
	double left_p = Tools::StringToFloat(s->GetString("Left P"));
	double left_i = Tools::StringToFloat(s->GetString("Left I"));
	double left_d = Tools::StringToFloat(s->GetString("Left D"));
	
	double right_p = Tools::StringToFloat(s->GetString("Right P"));
	double right_i = Tools::StringToFloat(s->GetString("Right I"));
	double right_d = Tools::StringToFloat(s->GetString("Right D"));
	
	double leftDistancePerPulse = Tools::StringToFloat(s->GetString("Left encoder distance per pulse"));
	double rightDistancePerPulse = Tools::StringToFloat(s->GetString("Right encoder distance per pulse"));
	
	if (mXboxController->GetButton(mXboxController->A)) {
		mPidDrive->Tune(left_p, left_i, left_d, right_p, right_i, right_d);
		mPidDrive->CalibrateEncoders(leftDistancePerPulse, rightDistancePerPulse);
	}
}

void PidDriveController::TrySetState()
{
	SmartDashboard *s = SmartDashboard::GetInstance();
	
	std::string command = s->GetString("PidDrive state");
	if (command != mPreviousCommand) {
		if (command == "manual") {
			mPidDrive->SetState(mPidDrive->kManual);
		} else if (command == "straight") {
			mPidDrive->SetState(mPidDrive->kStraight);
		} else if (command == "halt") {
			mPidDrive->SetState(mPidDrive->kHalt);
		}
		mPreviousCommand = command;
	}	
}
