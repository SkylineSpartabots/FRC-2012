// 3rd party modules
#include "target.h"
#include "Vision/ImageBase.h"
/*
TestThread::TestThread(
		const char *threadName,
		int *sharedNumber) :	
		Task("TestThread", (FUNCPTR)TestThread::TaskWrapper)
{
	mThreadName = threadName;
	CRITICAL_REGION(SEM_ID);
		mSharedNumber = sharedNumber;
	END_REGION;
}

void TestThread::TaskWrapper(void *thisObject)
{
	TestThread *self = (TestThread *) thisObject;
	self->Run();
}

void TestThread::Run()
{
	while (this->Verify()) {
		mCachedNumber += CalcNum();
		CRITICAL_REGION(SEM_ID);
			*mSharedNumber = mCachedNumber;
		END_REGION;
	}
}

int TestThread::CalcNum()
{
	Wait(10.0);
	return 1;
}

TestThreadListener::TestThreadListener() :
		BaseComponent(),
		mTestThread("TestThread1", mSharedNumber)
{
	CRITICAL_REGION(SEM_ID);
		mSharedNumber = new int(2);
	END_REGION;
	mLonelyNumber = 0;
	bool result = mTestThread.Start();
	SmartDashboard::GetInstance()->Log(result, "Listener::Result");
}

void TestThreadListener::Update()
{
	CRITICAL_REGION(SEM_ID);
		mCachedNumber = *mSharedNumber;
	END_REGION;
	mLonelyNumber += 1;
}

int TestThreadListener::GetSharedNumber()
{
	return mCachedNumber;
}

int TestThreadListener::GetLonelyNumber()
{
	return mLonelyNumber;
}


TestThreadController::TestThreadController(TestThreadListener *testThreadListener) :
		BaseController()
{
	mTestThreadListener = testThreadListener;
}

void TestThreadController::Run()
{
	mTestThreadListener->Update();
	SmartDashboard::GetInstance()->Log(mTestThreadListener->GetSharedNumber(), "TestThread::SharedNumber");
	SmartDashboard::GetInstance()->Log(mTestThreadListener->GetLonelyNumber(), "TestThread::LonelyNumber");
}

*/









TargetUtils::Coordinate::Coordinate()
{
	Coordinate::X = 0;
	Coordinate::Y = 0;
}

void TargetUtils::Coordinate::Set(float x, float y)
{
	Coordinate::X = x;
	Coordinate::Y = y;
}

TargetUtils::SaneBinaryImage::SaneBinaryImage(void) : BinaryImage()
{
	// Empty
}

TargetUtils::SaneBinaryImage::~SaneBinaryImage(void)
{
	// Empty
}

vector<RectangleMatch> *TargetUtils::SaneBinaryImage::DetectRectangles(
		RectangleDescriptor *rectangleDescriptor,
		CurveOptions *curveOptions,
		ShapeDetectionOptions *shapeDetectionOptions,
		ROI *roi)
{
	int numberOfMatches;
	
	RectangleMatch *rectangleMatch = imaqDetectRectangles(
			m_imaqImage,			// Inherited member value
			rectangleDescriptor,
			curveOptions,
			shapeDetectionOptions,
			roi,
			&numberOfMatches		// Is modified by function.
	);
	vector<RectangleMatch> *rectangles = new vector<RectangleMatch>;
	
	SmartDashboard::GetInstance()->Log(numberOfMatches, "DetectRectangles");
	
	if (rectangleMatch == NULL) {
		return rectangles;
	}
	for (int i = 0; i < numberOfMatches; i++) {
		rectangles->push_back(rectangleMatch[i]);
	}
	imaqDispose(rectangleMatch);
	return rectangles;
}

vector<RectangleMatch> *TargetUtils::SaneBinaryImage::DetectRectangles(
		RectangleDescriptor *rectangleDescriptor)
{
	vector<RectangleMatch> *rectangles = DetectRectangles(
			rectangleDescriptor,
			NULL,
			NULL,
			NULL);
	return rectangles;
}


static RectangleDescriptor rectangleDescriptor = {
		10,		// minWidth
		400,	// maxWidth
		10,		// minHeight
		400,	// maxHeight
};
static CurveOptions curveOptions = {
		IMAQ_NORMAL_IMAGE,	// Extraction mode
		1,					// Edge threshold
		IMAQ_NORMAL, 		// Filter size
		1,					// Min length
		7,					// Row step size
		6,					// Column step size
		23,					// Max end point gap
		0,					// Detect only closed curves?
		1					// Identify curves with subpixel accuracy?
};
static ShapeDetectionOptions shapeDetectionOptions = {
		IMAQ_GEOMETRIC_MATCH_ROTATION_INVARIANT,	// Detection mode
		NULL,				// Angle ranges (all)
		0,					// Num angle ranges
		{0, 100},			// Scale range
		0					// Minimum score
};







TargetFinder::TargetFinder() :
		BaseComponent()
{
	// Empty
}

vector<TargetUtils::Target> TargetFinder::GetTargets()
{
	vector<TargetUtils::Target> targets;
	
	AxisCamera &camera = GetCamera();
	HSLImage *image = camera.GetImage();
	
	if ((image->GetWidth() == 0) or (image->GetHeight() == 0)) {
		delete image;
		return targets;
	}
	
	BinaryImage *thresholdImage = image->ThresholdRGB(TargetUtils::threshold);    // Get only colors within range
	BinaryImage *bigObjectsImage = thresholdImage->RemoveSmallObjects(false, 1);  // Remove small objects
	BinaryImage *convexHullImage = bigObjectsImage->ConvexHull(false);  		  // Rill in partial and full rectangles
	TargetUtils::SaneBinaryImage *processedImage = (TargetUtils::SaneBinaryImage *) convexHullImage;
	
	vector<RectangleMatch> *rectangles = processedImage->DetectRectangles(
			&rectangleDescriptor,
			&curveOptions,
			&shapeDetectionOptions,
			NULL
	);
	
	delete thresholdImage;
	delete bigObjectsImage;
	delete convexHullImage;
	delete processedImage;
	
	int size = (int) rectangles->size();
	SmartDashboard::GetInstance()->Log(size, "Number of targets");
	
	if (size == 0) {
		SmartDashboard::GetInstance()->Log("None found", "Camera Pics");
		delete rectangles;
		delete image;
		return targets;		// Empty vector
	}
	SmartDashboard::GetInstance()->Log("Found", "Camera Pics");
	
	for (int i=0; i<size; i++) {
		TargetUtils::Target t;
		RectangleMatch r = rectangles->at(i);
		
		t.Width = r.width;
		t.Height = r.height;
		t.Rotation = r.rotation;	// Rotation from camera's horizontal axis.
		
		t.Score = r.score;
		t.TopLeft.Set(r.corner[0].x, r.corner[0].y);
		t.TopRight.Set(r.corner[1].x, r.corner[1].y);
		t.BottomRight.Set(r.corner[2].x, r.corner[2].y);
		t.BottomLeft.Set(r.corner[3].x, r.corner[3].y);
		
		float avgMiddleX = (r.corner[0].x + r.corner[1].x + r.corner[2].x + r.corner[3].x) / 4;
		float avgMiddleY = (r.corner[0].y + r.corner[1].y + r.corner[2].y + r.corner[3].y) / 4;
		t.Middle.Set(avgMiddleX, avgMiddleY);
		
		t.DistanceFromCamera = CalculateDistanceBasedOnWidth(t.Width);
		t.XAngleFromCamera = FindXAngle(avgMiddleX);
		t.YAngleFromCamera = FindYAngle(avgMiddleY);
		targets.push_back(t);
	}
	
	delete image;
	delete rectangles;
	return targets;
}


/**
 * Input:
 *   - Width (in pixels)
 * 
 * Output:
 *   - Distance from the camera to approx the center of the target
 *     (in inches)
 */
double TargetFinder::CalculateDistanceBasedOnWidth(double widthInPixels)
{
	// Based on exeriments we conducted...
	// Axis 206 Network camera
	// 640x480
	// The dial contains a focus -- the groove over the 'ar' in
	// 'Near' should be just a hair to the left of the bump.
	// Original formula: (17490 / widthInPixels) - 6.97
	
	double distance = (17490 / widthInPixels) - 6.97; 
	return distance;
}

double TargetFinder::FindXAngle(double middleXCoordinate) {
	double delta = middleXCoordinate - kMiddleXOfImage;
	double percentage = delta / kMiddleXOfImage;
	double angle = percentage * kMaxXAngleOfCamera;
	return angle;
}

double TargetFinder::FindYAngle(double middleYCoordinate) {
	double delta = middleYCoordinate - kMiddleYOfImage;
	double percentage = delta / kMiddleYOfImage;
	double angle = percentage * kMaxYAngleOfCamera;
	return angle;
}

AxisCamera & TargetFinder::GetCamera()
{
	AxisCamera &camera = AxisCamera::GetInstance("10.29.76.11");
	camera.WriteResolution(AxisCamera::kResolution_640x480);
	camera.WriteCompression(20);
	camera.WriteBrightness(AxisCamera::kWhiteBalance_Automatic);
	return camera;
}




TargetSnapshotController::TargetSnapshotController(
		RobotDrive *robotDrive,
		TargetFinder *targetFinder, 
		Joystick *joystick, 
		Watchdog &watchdog,
		Gyro *gyro) :
		BaseController(),
		mWatchdog(watchdog)
{
	mRobotDrive = robotDrive;
	mTargetFinder = targetFinder;
	mJoystick = joystick;
	mGyro = gyro;
}

void TargetSnapshotController::Run()
{
	if (mJoystick->GetRawButton(2)) {
		mWatchdog.SetEnabled(false);
		vector<TargetUtils::Target> targets = mTargetFinder->GetTargets();
		
		SmartDashboard::GetInstance()->Log("Yes", "Snapshot");
		
		if (((int)targets.size()) != 0) {
			TargetUtils::Target t = FindHighestTarget(targets);
			PrintDiagnostics(t);
			
			// When the robot turns clockwise, the gyro (which is upside-down on the robot, btw)
			// decreases the angle it returns.
			// The target returns a positive angle when the robot is pointing to the right of
			// the target.
			float start = -mGyro->GetAngle();
			float finish = start + t.XAngleFromCamera;
			float delta = finish - start;
			float adjustment = kTurnPower;
			if (delta < 0) {
				adjustment = -adjustment;
			}
			float current = start;
			
			SmartDashboard *s = SmartDashboard::GetInstance();
			s->Log(start, "c start");
			s->Log(finish, "c finish");
			s->Log(delta, "c delta");
			s->Log(adjustment, "c adjustment");
			while (!Compare(current, finish)) {
				mRobotDrive->TankDrive(adjustment, -adjustment);
				current = mGyro->GetAngle();
				s->Log(current, "c current");
			}
		}
	} else {
		SmartDashboard::GetInstance()->Log("No", "Snapshot");
	}
	mWatchdog.SetEnabled(true);
}

TargetUtils::Target TargetSnapshotController::FindHighestTarget(std::vector<TargetUtils::Target> targetsList)
{
	float topHeight = 0.0;
	TargetUtils::Target topTarget = targetsList.at(0);
	int size = (int) targetsList.size();
	
	for (int i=0; i < size; i++) {
		double height = targetsList.at(i).Height;
		if (height > topHeight) {
			topHeight = height;
			topTarget = targetsList.at(i);
		}
	}
	return topTarget;
}

void TargetSnapshotController::PrintDiagnostics(TargetUtils::Target t) 
{
	SmartDashboard::GetInstance()->Log(t.Width, "t.Width");
	SmartDashboard::GetInstance()->Log(t.Height, "t.Height");
	SmartDashboard::GetInstance()->Log(t.Rotation, "t.Rotation");

	SmartDashboard::GetInstance()->Log(t.Score, "t.Score");

	SmartDashboard::GetInstance()->Log(t.TopLeft.X, "t.TopLeft.x");
	SmartDashboard::GetInstance()->Log(t.TopLeft.Y, "t.TopLeft.y");
	SmartDashboard::GetInstance()->Log(t.TopRight.X, "t.TopRight.x");
	SmartDashboard::GetInstance()->Log(t.TopRight.Y, "t.TopRight.y");
	SmartDashboard::GetInstance()->Log(t.BottomLeft.X, "t.BottomLeft.x");
	SmartDashboard::GetInstance()->Log(t.BottomLeft.Y, "t.BottomLeft.y");
	SmartDashboard::GetInstance()->Log(t.BottomRight.X, "t.BottomRight.x");
	SmartDashboard::GetInstance()->Log(t.BottomRight.Y, "t.BottomRight.y");

	SmartDashboard::GetInstance()->Log(t.Middle.X, "t.Middle.x");
	SmartDashboard::GetInstance()->Log(t.Middle.Y, "t.Middle.y");
	SmartDashboard::GetInstance()->Log(t.DistanceFromCamera, "t.DistanceFromCamera");
	SmartDashboard::GetInstance()->Log(t.XAngleFromCamera, "t.XAngleFromCamera");
	SmartDashboard::GetInstance()->Log(t.YAngleFromCamera, "t.YAngleFromCamera");
}

bool TargetSnapshotController::Compare(float number, float target)
{
	float lowLimit = target - kTolerance;
	float highLimit = target + kTolerance;
	if (lowLimit <= number and number <= highLimit) {
		return true;
	} else {
		return false;
	}
}

