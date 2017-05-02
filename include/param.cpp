//
// Created by swearos on 17-5-1.
//

#include "utils.hpp"
#include "param.hpp"


std::vector<float> FrcnnParam::scales;
float FrcnnParam::max_size;
float FrcnnParam::batch_size;
float FrcnnParam::scale_multiple_of;

float FrcnnParam::fg_fraction;
float FrcnnParam::fg_thresh;
// Overlap threshold for a ROI to be considered background (class = 0
// ifoverlap in [LO, HI))
float FrcnnParam::bg_thresh_hi;
float FrcnnParam::bg_thresh_lo;
bool FrcnnParam::use_flipped;

// Train bounding-box regressors
bool FrcnnParam::bbox_reg; // Unuse
float FrcnnParam::bbox_thresh;
std::string FrcnnParam::snapshot_infix;
bool FrcnnParam::bbox_normalize_targets;
float FrcnnParam::bbox_inside_weights[4];
float FrcnnParam::bbox_normalize_means[4];
float FrcnnParam::bbox_normalize_stds[4];

// RPN to detect objects
float FrcnnParam::rpn_positive_overlap;
float FrcnnParam::rpn_negative_overlap;
// If an anchor statisfied by positive and negative conditions set to negative
bool FrcnnParam::rpn_clobber_positives;
float FrcnnParam::rpn_fg_fraction;
int FrcnnParam::rpn_batchsize;
float FrcnnParam::rpn_nms_thresh;
int FrcnnParam::rpn_pre_nms_top_n;
int FrcnnParam::rpn_post_nms_top_n;
// Proposal height and width both need to be greater than RPN_MIN_SIZE (at
// orig image scale)
float FrcnnParam::rpn_min_size;
// Deprecated (outside weights)
float FrcnnParam::rpn_bbox_inside_weights[4];
// Give the positive RPN examples weight of p * 1 / {num positives}
// and give negatives a weight of (1 - p)
// Set to -1.0 to use uniform example weighting
float FrcnnParam::rpn_positive_weight;
float FrcnnParam::rpn_allowed_border;

// ======================================== Test
std::vector<float> FrcnnParam::test_scales;
float FrcnnParam::test_max_size;
float FrcnnParam::test_nms;

bool FrcnnParam::test_bbox_reg;
// RPN to detect objects
float FrcnnParam::test_rpn_nms_thresh;
int FrcnnParam::test_rpn_pre_nms_top_n;
int FrcnnParam::test_rpn_post_nms_top_n;
// Proposal height and width both need to be greater than RPN_MIN_SIZE (at
// orig image scale)
float FrcnnParam::test_rpn_min_size;

// ========================================
// Means PIXEL
float FrcnnParam::pixel_means[3]; // BGR
int FrcnnParam::rng_seed;
float FrcnnParam::eps;
float FrcnnParam::inf;

// ========================================
int FrcnnParam::feat_stride;
std::vector<float> FrcnnParam::anchors;
float FrcnnParam::test_score_thresh;
int FrcnnParam::n_classes;
int FrcnnParam::iter_test;


void FrcnnParam::load_param(const std::string default_config_path) {
    std::vector<float> v_tmp;

    str_map default_map = parse_json_config(default_config_path);

    scales = extract_vector("scales", default_map);
    max_size = extract_float("max_size", default_map);
    batch_size = extract_float("batch_size", default_map);
    //添加scale_multiple_of because pvanet model
    scale_multiple_of = extract_float("scale_multiple_of",default_map);

    fg_fraction = extract_float("fg_fraction", default_map);
    fg_thresh = extract_float("fg_thresh", default_map);
    bg_thresh_hi = extract_float("bg_thresh_hi", default_map);
    bg_thresh_lo = extract_float("bg_thresh_lo", default_map);
    use_flipped =
            static_cast<bool>(extract_int("use_flipped", default_map));

    bbox_reg =
            static_cast<bool>(extract_int("bbox_reg", default_map));
    bbox_thresh = extract_float("bbox_thresh", default_map);
    snapshot_infix = extract_string("snapshot_infix", default_map);
    bbox_normalize_targets =
            static_cast<bool>(extract_int("bbox_normalize_targets", default_map));
    v_tmp = extract_vector("bbox_inside_weights", default_map);
    std::copy(v_tmp.begin(), v_tmp.end(), bbox_inside_weights);
    v_tmp = extract_vector("bbox_normalize_means", default_map);
    std::copy(v_tmp.begin(), v_tmp.end(), bbox_normalize_means);
    v_tmp = extract_vector("bbox_normalize_stds", default_map);
    std::copy(v_tmp.begin(), v_tmp.end(), bbox_normalize_stds);

    rpn_positive_overlap = extract_float("rpn_positive_overlap", default_map);
    rpn_negative_overlap = extract_float("rpn_negative_overlap", default_map);
    rpn_clobber_positives =
            static_cast<bool>(extract_int("rpn_clobber_positives", default_map));
    rpn_fg_fraction = extract_float("rpn_fg_fraction", default_map);
    rpn_batchsize = extract_int("rpn_batchsize", default_map);
    rpn_nms_thresh = extract_float("rpn_nms_thresh", default_map);
    rpn_pre_nms_top_n = extract_int("rpn_pre_nms_top_n", default_map);
    rpn_post_nms_top_n = extract_int("rpn_post_nms_top_n", default_map);
    rpn_min_size = extract_float("rpn_min_size", default_map);
    v_tmp = extract_vector("rpn_bbox_inside_weights", default_map);
    std::copy(v_tmp.begin(), v_tmp.end(), rpn_bbox_inside_weights);
    rpn_positive_weight = extract_float("rpn_positive_weight", default_map);
    rpn_allowed_border = extract_float("rpn_allowed_border", default_map);

    // ======================================== Test
    test_scales = extract_vector("test_scales", default_map);
    test_max_size = extract_float("test_max_size", default_map);
    test_nms = extract_float("test_nms", default_map);

    test_bbox_reg = static_cast<bool>(extract_float("test_bbox_reg", default_map));
    test_rpn_nms_thresh = extract_float("test_rpn_nms_thresh", default_map);
    test_rpn_pre_nms_top_n = extract_int("test_rpn_pre_nms_top_n", default_map);
    test_rpn_post_nms_top_n = extract_int("test_rpn_post_nms_top_n", default_map);
    test_rpn_min_size = extract_float("test_rpn_min_size", default_map);

    // ========================================
    v_tmp = extract_vector("pixel_means", default_map);
    std::copy(v_tmp.begin(), v_tmp.end(), pixel_means);
    rng_seed = extract_int("rng_seed", default_map);
    eps = extract_float("eps", default_map);
    inf = extract_float("inf", default_map);

    // ========================================
    feat_stride = extract_int("feat_stride", default_map);
    anchors = extract_vector("anchors", default_map);
    test_score_thresh = extract_float("test_score_thresh", default_map);
    n_classes = extract_int("n_classes", default_map);
    iter_test = extract_int("iter_test", default_map);
}

void FrcnnParam::print_param(){

    LOG(INFO) << "== Train  Parameters ==";
    LOG(INFO) << "scale             : " << float_to_string(scales);
    LOG(INFO) << "max_size          : " << max_size;
    LOG(INFO) << "batch_size        : " << batch_size;

    LOG(INFO) << "fg_fraction       : " << fg_fraction;
    LOG(INFO) << "fg_thresh         : " << fg_thresh;
    LOG(INFO) << "bg_thresh_hi      : " << bg_thresh_hi;
    LOG(INFO) << "bg_thresh_lo      : " << bg_thresh_lo;
    LOG(INFO) << "use_flipped       : " << (use_flipped ? "yes" : "no");

    LOG(INFO) << "use_bbox_reg      : " << (bbox_reg ? "yes" : "no");
    LOG(INFO) << "bbox_thresh       : " << bbox_thresh;
    LOG(INFO) << "snapshot_infix    : " << snapshot_infix;
    LOG(INFO) << "normalize_targets : " << (bbox_normalize_targets ? "yes" : "no");

    LOG(INFO) << "rpn_pos_overlap   : " << rpn_positive_overlap;
    LOG(INFO) << "rpn_neg_overlap   : " << rpn_negative_overlap;
    LOG(INFO) << "clobber_positives : " << (rpn_clobber_positives ? "yes" : "no");
    LOG(INFO) << "rpn_fg_fraction   : " << rpn_fg_fraction;
    LOG(INFO) << "rpn_batchsize     : " << rpn_batchsize;
    LOG(INFO) << "rpn_nms_thresh    : " << rpn_nms_thresh;
    LOG(INFO) << "rpn_pre_nms_top_n : " << rpn_pre_nms_top_n;
    LOG(INFO) << "rpn_post_nms_top_n: " << rpn_post_nms_top_n;
    LOG(INFO) << "rpn_min_size      : " << rpn_min_size;
    LOG(INFO) << "rpn_bbox_inside_weights :" << float_to_string(rpn_bbox_inside_weights);
    LOG(INFO) << "rpn_positive_weight     :" << rpn_positive_weight;
    LOG(INFO) << "rpn_allowed_border      :" << rpn_allowed_border;

    LOG(INFO) << "== Test   Parameters ==";
    LOG(INFO) << "test_scales          : " << float_to_string(test_scales);
    LOG(INFO) << "test_max_size        : " << test_max_size;
    LOG(INFO) << "test_nms             : " << test_nms;
    LOG(INFO) << "test_bbox_reg        : " << (test_bbox_reg?"yes":"no");
    LOG(INFO) << "test_rpn_nms_thresh  : " << test_rpn_nms_thresh;
    LOG(INFO) << "rpn_pre_nms_top_n    : " << test_rpn_pre_nms_top_n;
    LOG(INFO) << "rpn_post_nms_top_n   : " << test_rpn_post_nms_top_n;
    LOG(INFO) << "test_rpn_min_sizen   : " << test_rpn_min_size;

    LOG(INFO) << "== Global Parameters ==";
    LOG(INFO) << "pixel_means[BGR]     : " << pixel_means[0] <<  " , " << pixel_means[1] << " , " << pixel_means[2];
    LOG(INFO) << "rng_seed             : " << rng_seed;
    LOG(INFO) << "eps                  : " << eps;
    LOG(INFO) << "inf                  : " << inf;
    LOG(INFO) << "feat_stride          : " << feat_stride;
    LOG(INFO) << "anchors_size         : " << anchors.size();
    LOG(INFO) << "test_score_thresh    : " << test_score_thresh;
    LOG(INFO) << "n_classes            : " << n_classes;
    LOG(INFO) << "iter_test            : " << iter_test;
}
