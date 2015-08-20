#include <tracker.hpp>
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/tracking.hpp"


class TrackerKuklina : public Tracker
{
 public:
    virtual ~TrackerKuklina() {}

    virtual bool init( const cv::Mat& frame, const cv::Rect& initial_position );
    virtual bool track( const cv::Mat& frame,  const cv::Mat& frame_next, cv::Rect& new_position );

 private:
    cv::Rect position_;
};

bool TrackerKuklina::init( const cv::Mat& frame, const cv::Rect& initial_position )
{
    position_ = initial_position;
	return true;
}

bool TrackerKuklina::track( const cv::Mat& frame, const cv::Mat& frame_next, cv::Rect& new_position )
{

	cv::Mat frame_gray, frame_gray_next;
	std::vector<cv::Point2f> corners, corners_next;
	std::vector<uchar> status;
	std::vector<float> err;
	cv::Size  winSize(31,31);
	cv::TermCriteria termcrit(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 20, 0.03);
	double qualityLevel = 0.01;
	double minDistance = 10;
	int blockSize = 3;
	bool useHarrisDetector = false;
	double k = 0.04;

	cvtColor(frame, frame_gray, cv::COLOR_BGR2GRAY );
	cvtColor(frame_next, frame_gray_next, cv::COLOR_BGR2GRAY );

	cv::goodFeaturesToTrack(frame_gray, corners, 23 /*maxCorners*/, qualityLevel, minDistance, cv::Mat(), blockSize, useHarrisDetector, k );
	cv::calcOpticalFlowPyrLK(frame_gray,frame_gray_next, corners, corners_next, status, err, winSize, 3, termcrit, 0, 0.001);
	/*for(int i = 0; i < status.size(); i++)
	{
		if (status[i] == false)
		{
			
		}
	}*/
	for(int j = 0; j < err.size(); j++)
	{
		for(int i = 0; i < err.size() - j; i++)
		{
			if(err[i] > err[i+1])
			{
				float sort_er;
				cv::Point2f sort;
				cv::Point2f sort_next;

				sort_er = err[i];
				err[i] = err[i+1];
				err[i+1] = sort_er;

				sort = corners[i];
				corners[i] = corners[i+1];
				corners[i+1] = sort;

				sort_next = corners_next[i];
				corners_next[i] = corners_next[i+1];
				corners_next[i+1] = sort_next;
			}

		}
	}

	int size = err.size();
	for(int i = 0; i < int(size)/2; i++)
	{
		corners.pop_back();
		corners_next.pop_back();
		err.pop_back();
	}

	for(int i = 0; i < corners.size(); i++)
	{

	}

	new_position = position_;
	return true;
}

cv::Ptr<Tracker> createTrackerDummy()
{
    return cv::Ptr<Tracker>(new TrackerKuklina());
}
