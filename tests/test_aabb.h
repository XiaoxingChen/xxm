#include "mxm/spatial_aabb.h"
#include <string>


using namespace mxm;

void testDistanceToPoint()
{
    AABB box({0.157613084, 0.00478348415}, {0.992881298, 0.297029436});
    Vec pt({0.5, 0.5});

    auto dist = distance(box, pt);
    if(0.39802742004394531250 != dist[0])
        throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
}

void testAABB()
{
    {
        AxisAlignedBoundingBox box({1,1,1}, {2,2,2});
        Ray ray1({0,0,0}, {1,1,1});
        Ray ray2({0,0,0}, {1,1,2.001});

        if(!box.hit(ray1))
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        if(box.hit(ray2))
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    {
        AxisAlignedBoundingBox box({-1,-1}, {1,1});
        Ray ray1({3,0}, {-1,0});
        Ray ray2({3,1.1}, {-1,0});
        if(!box.hit(ray1))
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        if(box.hit(ray2))
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    {
        AABB box(3);
        AABB box1({0,0,0},{1,1,1});
        AABB box2({3,3,3},{5,5,5});
        box.extend(box1).extend(box2);

        if((box.min() - Vec::zeros(3)).norm() > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
        if((box.max() - 5 * Vec::ones(3)).norm() > eps())
            throw std::runtime_error(std::string(__FILE__) + ":" + std::to_string(__LINE__));
    }

    testDistanceToPoint();
}
