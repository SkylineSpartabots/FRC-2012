#include "track_silver.h"




ImageTarget::ImageTarget()
{
	// Empty
}

Target ImageTarget::ProcessImage(HSLImage *image) {
	Target t = Target();
	return t;
}

SilverImageTarget::SilverImageTarget() :
	ImageTarget(),
	colorThreshold(
		210,	// Red min
		255,	// Red max
		79,		// Green min
		183,	// Green max
		84,		// Blue min
		160		// Blue max
	)
{
	// Empty constructor
}

Target SilverImageTarget::ProcessImage(HSLImage *image)
{
	BinaryImage *thresholdImage = image->ThresholdRGB(colorThreshold);
	BinaryImage *convexImage = thresholdImage->ConvexHull(false);
	convexImage->GetNumberParticles();
	ParticleAnalysisReport particleReport = convexImage->GetParticleAnalysisReport(0);
	Centroid centroid;
	centroid.x = particleReport.center_mass_x;
	centroid.y = particleReport.center_mass_y;
	
	delete thresholdImage;
	delete convexImage;
	
	return centroid;
}
