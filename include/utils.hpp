//
// Created by swearos on 17-5-1.
//
#ifndef DEMO_DETECT_UTILS_HPP
#define DEMO_DETECT_UTILS_HPP

#include <cstdlib>
#include <string>
#include <sstream>

#include <algorithm>
#include <cstdlib>
#include <cassert>
#include <vector>
#include <map>

#include <glog/logging.h>

#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>

#include "caffe/common.hpp"
#include "caffe/util/rng.hpp"

#include "param.hpp"
#include "BBox.hpp"

using std::vector;
using std::map;
using std::string;


static void get_scale_factor(int width, int height, int short_size, int max_long_size,
                      int multiple, vector<float> &im_scale) {
    float im_size_min = std::min(width, height);
    float im_size_max = std::max(width, height);
    //  float im_scale[2];

    float scale_factor = static_cast<float>(short_size) / im_size_min;
    // Prevent the biggest axis from being more than max_size
    if (scale_factor * im_size_max > max_long_size) {
        scale_factor = static_cast<float>(max_long_size) / im_size_max;
    }

    //int multiple = 32;
    im_scale[0] = floor(width * scale_factor / multiple) * multiple / width;
    im_scale[1] = floor(height * scale_factor / multiple) * multiple / height;
    // std::cout<<"temp_scale "<<int((width * scale_factor) / multiple ) * 32 <<" / "<<width<<std::endl;
    //DLOG(INFO)<< "im_scale_x,im_scale_y " << im_scale[0] << " " << im_scale[1];

}
template <typename Dtype>
Point4f<Dtype> bbox_transform_inv(const Point4f<Dtype> &box, const Point4f<Dtype> &delta) {
    Dtype src_w = box[2] - box[0] + 1;
    Dtype src_h = box[3] - box[1] + 1;
    Dtype src_ctr_x = box[0] + 0.5 * src_w; // box[0] + 0.5*src_w;
    Dtype src_ctr_y = box[1] + 0.5 * src_h; // box[1] + 0.5*src_h;
    Dtype pred_ctr_x = delta[0] * src_w + src_ctr_x;
    Dtype pred_ctr_y = delta[1] * src_h + src_ctr_y;
    Dtype pred_w = exp(delta[2]) * src_w;
    Dtype pred_h = exp(delta[3]) * src_h;
    return Point4f<Dtype>(pred_ctr_x - 0.5 * pred_w, pred_ctr_y - 0.5 * pred_h,
                          pred_ctr_x + 0.5 * pred_w, pred_ctr_y + 0.5 * pred_h);
    // return Point4f<Dtype>(pred_ctr_x - 0.5*(pred_w-1) , pred_ctr_y - 0.5*(pred_h-1) ,
    // pred_ctr_x + 0.5*(pred_w-1) , pred_ctr_y + 0.5*(pred_h-1));
}

template Point4f<float> bbox_transform_inv(const Point4f<float> &box, const Point4f<float> &delta);
template Point4f<double> bbox_transform_inv(const Point4f<double> &box, const Point4f<double> &delta);

template<typename Dtype>
Point4f<Dtype> bbox_transform(const Point4f<Dtype> &ex_roi, const Point4f<Dtype> &gt_roi) {
    Dtype ex_width = ex_roi[2] - ex_roi[0] + 1;
    Dtype ex_height = ex_roi[3] - ex_roi[1] + 1;
    Dtype ex_ctr_x = ex_roi[0] + 0.5 * ex_width;
    Dtype ex_ctr_y = ex_roi[1] + 0.5 * ex_height;
    Dtype gt_widths = gt_roi[2] - gt_roi[0] + 1;
    Dtype gt_heights = gt_roi[3] - gt_roi[1] + 1;
    Dtype gt_ctr_x = gt_roi[0] + 0.5 * gt_widths;
    Dtype gt_ctr_y = gt_roi[1] + 0.5 * gt_heights;
    Dtype targets_dx = (gt_ctr_x - ex_ctr_x) / ex_width;
    Dtype targets_dy = (gt_ctr_y - ex_ctr_y) / ex_height;
    Dtype targets_dw = log(gt_widths / ex_width);
    Dtype targets_dh = log(gt_heights / ex_height);
    return Point4f<Dtype>(targets_dx, targets_dy, targets_dw, targets_dh);
}

template Point4f<float> bbox_transform(const Point4f<float> &ex_roi, const Point4f<float> &gt_roi);

template Point4f<double> bbox_transform(const Point4f<double> &ex_roi, const Point4f<double> &gt_roi);

template<typename Dtype>
vector<Point4f<Dtype> >
bbox_transform(const vector<Point4f<Dtype> > &ex_rois, const vector<Point4f<Dtype> > &gt_rois) {
    CHECK_EQ(ex_rois.size(), gt_rois.size());
    vector<Point4f<Dtype> > transformed_bbox;
    for (size_t i = 0; i < gt_rois.size(); i++) {
        transformed_bbox.push_back(bbox_transform(ex_rois[i], gt_rois[i]));
    }
    return transformed_bbox;
}

template vector<Point4f<float> >
bbox_transform(const vector<Point4f<float> > &ex_rois, const vector<Point4f<float> > &gt_rois);

template vector<Point4f<double> >
bbox_transform(const vector<Point4f<double> > &ex_rois, const vector<Point4f<double> > &gt_rois);


template <typename Dtype>
Dtype get_iou(const Point4f<Dtype> &A, const Point4f<Dtype> &B) {
    const Dtype xx1 = std::max(A[0], B[0]);
    const Dtype yy1 = std::max(A[1], B[1]);
    const Dtype xx2 = std::min(A[2], B[2]);
    const Dtype yy2 = std::min(A[3], B[3]);
    Dtype inter = std::max(Dtype(0), xx2 - xx1 + 1) * std::max(Dtype(0), yy2 - yy1 + 1);
    Dtype areaA = (A[2] - A[0] + 1) * (A[3] - A[1] + 1);
    Dtype areaB = (B[2] - B[0] + 1) * (B[3] - B[1] + 1);
    return inter / (areaA + areaB - inter);
}
template float get_iou(const Point4f<float> &A, const Point4f<float> &B);
template double get_iou(const Point4f<double> &A, const Point4f<double> &B);

template <typename Dtype>
vector<vector<Dtype> > get_ious(const vector<Point4f<Dtype> > &A, const vector<Point4f<Dtype> > &B) {
    vector<vector<Dtype> >ious;
    for (size_t i = 0; i < A.size(); i++) {
        ious.push_back(get_ious(A[i], B));
    }
    return ious;
}
template vector<vector<float> > get_ious(const vector<Point4f<float> > &A, const vector<Point4f<float> > &B);
template vector<vector<double> > get_ious(const vector<Point4f<double> > &A, const vector<Point4f<double> > &B);

template <typename Dtype>
vector<Dtype> get_ious(const Point4f<Dtype> &A, const vector<Point4f<Dtype> > &B) {
    vector<Dtype> ious;
    for (size_t i = 0; i < B.size(); i++) {
        ious.push_back(get_iou(A, B[i]));
    }
    return ious;
}

template vector<float> get_ious(const Point4f<float> &A, const vector<Point4f<float> > &B);
template vector<double> get_ious(const Point4f<double> &A, const vector<Point4f<double> > &B);


// config
typedef std::map<string, string> str_map;

str_map parse_json_config(const string file_path);

string extract_string(string target_key, str_map& default_map);

float extract_float(string target_key,  str_map& default_map);

int extract_int(string target_key, str_map& default_map);

vector<float> extract_vector(string target_key, str_map& default_map);

// file
vector<string> get_file_list (const string& path, const string& ext);

template <typename Dtype>
void print_vector(vector<Dtype> data);

string anchor_to_string(vector<float> data);

string float_to_string(const vector<float> data);

string float_to_string(const float *data);

#endif //DEMO_DETECT_UTILS_HPP
