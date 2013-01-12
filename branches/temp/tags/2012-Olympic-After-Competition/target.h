/**
 * @file target.h
 * 
 * @brief Classes all about using the camera, analyzing images, and sending
 * data back to the computer.
 * 
 * @warning
 * In progress, do not use.
 */

#ifndef TARGET_H_
#define TARGET_H_

// Standard library
#include <utility>
#include <math.h>
#include <algorithm>

// 3rd party libraries
#include "WPILib.h"
#include "nivision.h"
#include "Vision/HSLImage.h"
#include "Vision/MonoImage.h"

// Program modules
#include "components.h"


/*
class TestThread : public Task
{
protected:
	const char *mThreadName;
	int *mSharedNumber;
	int mCachedNumber;

public:
	TestThread(
			const char *,
			int *);
	static void TaskWrapper(void *);
	virtual void Run();
	int CalcNum();
};


class TestThreadListener : public BaseComponent
{
protected:
	int *mSharedNumber;
	int mCachedNumber;
	int mLonelyNumber;
	TestThread mTestThread;
	
public:
	TestThreadListener();
	void Update();
	int GetSharedNumber();
	int GetLonelyNumber();
};

class TestThreadController : public BaseController
{
protected:
	TestThreadListener *mTestThreadListener;
	
public:
	TestThreadController(TestThreadListener *);
	void Run();
};
*/




/**
 * @brief A collection of structs and classes meant specifically
 * for the target detection code.
 */
namespace TargetUtils {
	// Technically, the contents of a namespace aren't usually indented.
	// I'm deliberately choosing to do so to minimize confusion.

	/**
	 * @brief Used to represent one cartesian coordinate.
	 */
	struct Coordinate {
	public:
		Coordinate();
		void Set(float, float);
		float X;
		float Y;
	};
	
	
	/**
	 * @brief A collection of data symbolizing one target.
	 * 
	 * @details
	 * At maximum, the robot should only return exactly four of these
	 * in a vector.
	 */
	struct Target
	{
	public:
		// The percieved width and height of the target
		double Width;
		double Height;
		double Rotation;
		
		// The score (the higher, the more likely it is a target)
		double Score;
		
		// Corners
		Coordinate TopLeft;
		Coordinate TopRight;
		Coordinate BottomLeft;
		Coordinate BottomRight;
		
		// More data
		Coordinate Middle;
		double DistanceFromCamera;
		double XAngleFromCamera;
		double YAngleFromCamera;
	};
	
	static Threshold threshold = Threshold(
			243,				// Red min
			255,				// Red max
			141,				// Green min
			255,				// Green max
			161,				// Blue min
			255					// Blue max
	);
	
	/**
	 * @brief A subclass of BinaryImage (provided by WPILib)
	 * that provides the ability to detect rectangles.
	 * 
	 * @details
	 * BinaryImage was only capable of detecting ellipses.
	 */
	class SaneBinaryImage : public BinaryImage
	{
	public:
		SaneBinaryImage();
		virtual ~SaneBinaryImage();
		
		vector<RectangleMatch> *DetectRectangles(
				RectangleDescriptor *,
				CurveOptions *,
				ShapeDetectionOptions *,
				ROI *);
	
		vector<RectangleMatch> *DetectRectangles(
				RectangleDescriptor *);
	};

} // End namespace.


/**
 * The TargetFinder class is responsible for interfacing with the
 * camera, and doing any image analysis code.
 * 
 * Note: the image processing settings was actually tested and
 * debugged using the NI Vision Assistant tool, then ported
 * over to code.  See the 2010 and 2012 vision samples for 
 * templates.
 * 
 * See also the 
 */
class TargetFinder : public BaseComponent
{
public:
	TargetFinder();
	vector<TargetUtils::Target> GetTargets();
protected:
	AxisCamera & GetCamera();
	double CalculateDistanceBasedOnWidth(double);
	double FindXAngle(double);
	double FindYAngle(double);
	static const double kTargetWidthInches = 24;
	static const double kTargetHeightInches = 18;
	static const double kMiddleXOfImage = 320.0;
	static const double kMaxXAngleOfCamera = 30;	// In degrees
	static const double kMiddleYOfImage = 240.0;
	static const double kMaxYAngleOfCamera = 20;		// In degrees
};


/**
 * @brief This class is responsible for taking a picture 
 * snapshot.  
 * 
 * @warning This will freeze the robot for half a second.
 */
class TargetSnapshotController : public BaseController
{
protected:
	RobotDrive *mRobotDrive;
	TargetFinder *mTargetFinder;
	Joystick *mJoystick;
	Watchdog &mWatchdog;
	Gyro *mGyro;
	
	static const float kTolerance = 0.5;
	static const float kTurnPower = 0.4;
	
	bool Compare(float, float);
	
public:
	TargetSnapshotController(
			RobotDrive *,
			TargetFinder *, 
			Joystick *, 
			Watchdog &,
			Gyro *);
	void Run();
	TargetUtils::Target FindHighestTarget(vector<TargetUtils::Target>);
	void PrintDiagnostics(TargetUtils::Target);
};


#endif
