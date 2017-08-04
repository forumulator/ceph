#include "rgw_store.h"


class RGWRadosStore : public RGWStore {
private:
  RGWRados *store;

public:
  RGWRadosStore() {}

  int init(RGWStore *_store);

  int UpdateContainerStats(map<string, RGWBucketEnt>& m);
  int ListBucket(RGWStore::Bucket *target, RGWStore::ListBucketInfo &info,
                int64_t max, vector<rgw_bucket_dir_entry> *result, 
                map<string, bool> *common_prefixes, bool *is_truncated);

  RGWStore *get_store();
}