//
// Created by swearos on 17-5-1.
//

#ifndef DEMO_DETECT_VIS_HPP
#define DEMO_DETECT_VIS_HPP

#include <sstream>

#include "utils.hpp"



inline std::map<int,string> LoadVocClass(){
    std::string CLASSES[] = {"__background__",
                             "aeroplane", "bicycle", "bird", "boat",
                             "bottle", "bus", "car", "cat", "chair",
                             "cow", "diningtable", "dog", "horse",
                             "motorbike", "person", "pottedplant",
                             "sheep", "sofa", "train", "tvmonitor"};
    std::map<int,string> answer;
    for (int index = 0 ; index < 21; index++) {
        answer[index] = CLASSES[index];
    }
    return answer;
}

inline std::string GetClassName(const std::map<int,std::string> CLASS, int label) {
    if( CLASS.find(label) == CLASS.end() ){
        std::ostringstream text;
        text << "Unknow_Class_" << label;
        return text.str();
    } else {
        return CLASS.find(label)->second;
    }
}

inline std::map<int,string> LoadRpnClass() {
    std::map<int,string> answer;
    answer[0] = "background";
    answer[1] = "proposal";
    return answer;
}

template <typename Dtype>
void vis_detections(cv::Mat &frame, const vector<BBox<Dtype>> &ans,const map<int,string> CLASS);

template <typename Dtype>
void vis_detections(cv::Mat &frame, const BBox<Dtype> ans, const map<int,string> CLASS);


template <typename Dtype>
void vis_detections(cv::Mat &frame,const vector<BBox<Dtype>> &ans,const map<int,string> CLASS)
{
    for (int i = 0; i < ans.size(); ++i) {
        cv::rectangle(frame,cv::Point(ans[i][0],ans[i][1]),cv::Point(ans[i][2],ans[i][3]),cv::Scalar(255,255,255));
        std::ostringstream text;
        text<<GetClassName(CLASS,ans[i].ID())<<" : "<<ans[i].Confidence();
        cv::putText(frame, text.str() , cv::Point(ans[i][0],ans[i][1]-18) , 0 , 0.6 , cv::Scalar(0,255,0) );
    }
}

//实例化
template void vis_detections(cv::Mat & frame, const BBox<float> ans, const std::map<int,std::string> CLASS);
template void vis_detections(cv::Mat & frame, const BBox<double> ans, const std::map<int,std::string> CLASS);

template <typename Dtype>
void vis_detections(cv::Mat & frame, const BBox<Dtype> ans,
                    const std::map<int,std::string> CLASS) {
    std::vector<BBox<Dtype> > vec_ans;
    vec_ans.push_back( ans );
    vis_detections(frame, vec_ans, CLASS);
}


#endif //DEMO_DETECT_VIS_HPP
