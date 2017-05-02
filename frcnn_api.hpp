#include <vector>
#include <string>

#include <boost/make_shared.hpp>
#include <boost/shared_ptr.hpp>

#include "caffe/blob.hpp"
#include "caffe/common.hpp"
#include "caffe/layer.hpp"
#include "caffe/net.hpp"


#include "utils.hpp"
#include "param.hpp"

using std::vector;

using caffe::Blob;
using caffe::Net;


class Detector {
public:
    Detector(std::string &proto_file, std::string &model_file, std::string default_config) {
        Set_Model(proto_file, model_file, default_config);
    }

    void Set_Model(std::string &proto_file, std::string &model_file, std::string default_config);

    void predict(const cv::Mat &img_in, vector<BBox<float> > &results);

private:
    void preprocess(const cv::Mat &img_in, const int blob_idx);

    void preprocess(const vector<float> &data, const int blob_idx);

    vector<boost::shared_ptr<Blob<float> > > predict(const vector<std::string> blob_names);

    boost::shared_ptr<Net<float> > net_;
    float mean_[3];
};

