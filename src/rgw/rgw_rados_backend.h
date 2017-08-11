#ifndef RGW_RADOS_BACKEND_H
#define RGW_RADOS_BACKEND_H

#include "rgw_rados.h"
#include "rgw_backend.h"


class RGWRadosBackend : public RGWBackend {
private:
  RGWRados *store;

public:
  RGWRadosBackend() {}

  uint64_t get_new_req_id() {
    return store->get_new_req_id();
  }

  int init(RGWRados *_store);

  int UpdateContainerStats(map<string, RGWBucketEnt>& m);
  int ListBucket(RGWBackend::Bucket *target, RGWBackend::ListBucketInfo &info,
                int64_t max, vector<rgw_bucket_dir_entry> *result, 
                map<string, bool> *common_prefixes, bool *is_truncated);

  RGWRados *get_rados();

  ~RGWRadosBackend() {
    delete store;
  }
};

#endif