//
// Created by swearos on 17-5-1.
//
#ifndef DEMO_DETECT_BBOX_HPP
#define DEMO_DETECT_BBOX_HPP

#include <cstring>
#include <string>
using std::string;
// image and box
template <typename Dtype>
class Point4f {
public:
    //添加一个委托构造函数
    Point4f(Dtype x1 = 0, Dtype y1 = 0, Dtype x2 = 0, Dtype y2 = 0) {
        Point[0] = x1; Point[1] = y1;
        Point[2] = x2; Point[3] = y2;
    }
    Point4f(const float data[4]) {
        for (int i=0;i<4;i++) Point[i] = data[i];
    }
    Point4f(const double data[4]) {
        for (int i=0;i<4;i++) Point[i] = data[i];
    }
    Point4f(const Point4f &other) { memcpy(Point, other.Point, sizeof(Point)); }

    Dtype& operator[](const unsigned int id) { return Point[id]; }
    const Dtype& operator[](const unsigned int id) const { return Point[id]; }

    string to_string() const {
        char buff[100];
        snprintf(buff, sizeof(buff), "%.1f %.1f %.1f %.1f", Point[0], Point[1], Point[2], Point[3]);
        return string(buff);
    }
protected:
    Dtype Point[4]; // x1 y1 x2 y2
};

template <typename Dtype>
class BBox : public Point4f<Dtype> {
public:
    BBox(Dtype x1 = 0, Dtype y1 = 0, Dtype x2 = 0, Dtype y2 = 0,
         Dtype confidence = 0, int id = 0)
            : Point4f<Dtype>(x1, y1, x2, y2), confidence(confidence), id(id) {}
    BBox(Point4f<Dtype> box, Dtype confidence_ = 0, int id = 0)
            : Point4f<Dtype>(box), confidence(confidence_), id(id) {}

    BBox &operator=(const BBox &other) {
        memcpy(this->Point, other.Point, sizeof(this->Point));
        confidence = other.confidence;
        id = other.id;
        return *this;
    }

    bool operator<(const class BBox &other) const {
        if (confidence != other.confidence)
            return confidence > other.confidence;
        else
            return id < other.id;
    }

    inline string to_string() const {
        char buff[100];
        snprintf(buff, sizeof(buff), "cls:%3d -- (%.3f): %.2f %.2f %.2f %.2f", id,
                 confidence, this->Point[0], this->Point[1], this->Point[2], this->Point[3]);
        return string(buff);
    }

    inline string to_short_string() const {
        char buff[100];
        snprintf(buff, sizeof(buff), "cls:%1d -- (%.2f)", id, confidence);
        return string(buff);
    }

    inline Dtype Confidence() const{
        return confidence;
    }
    inline int ID() const {
        return id;
    }
protected:
    Dtype confidence;
    int id;
};


#endif //DEMO_DETECT_BBOX_HPP
