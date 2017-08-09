#include "rgw_store.h"


class RGWRadosBackend : public RGWBackend {
private:
  RGWRados *store;

public:
  RGWRadosBackend() {}

  uint64_t get_new_req_id() {
    return store->get_new_req_id();
  }

  int init(RGWBackend *_store);

  int UpdateContainerStats(map<string, RGWBucketEnt>& m);
  int ListBucket(RGWBackend::Bucket *target, RGWBackend::ListBucketInfo &info,
                int64_t max, vector<rgw_bucket_dir_entry> *result, 
                map<string, bool> *common_prefixes, bool *is_truncated);

  RGWRados *get_store();
}