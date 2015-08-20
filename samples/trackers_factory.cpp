#include "tracker.hpp"

cv::Ptr<Tracker> createTrackerDummy();
// TODO: Declare your implementation here
cv::Ptr<Tracker> createTrackerKuklina();

cv::Ptr<Tracker> createTracker(const std::string &impl_name)
{
    if (impl_name == "dummy")
        return createTrackerDummy();
    // TODO: Add case for your implementation
     else if (impl_name == "kuklina")
        return createTrackerKuklina();

    return 0;
}
