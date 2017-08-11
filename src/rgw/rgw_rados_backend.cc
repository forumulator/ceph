#include "rgw_rados_backend.h"
#include "rgw_rados.h"
#include "rgw_backend.h"

int RGWRadosBackend::init(RGWRados *_store) {
  if (!_store) {
    return -1;
  }
  store = _store;
  return 0;
}

int RGWRadosBackend::UpdateContainerStats(map<string, RGWBucketEnt>& m) {
  return store->update_containers_stats(m);
}

int RGWRadosBackend::ListBucket(RGWBackend::Bucket *pTarget, RGWBackend::ListBucketInfo &info,
                              int64_t max, vector<rgw_bucket_dir_entry> *result, 
                              map<string, bool> *common_prefixes, bool *is_truncated) {
  RGWRados::Bucket target(*pTarget);
  RGWRados::Bucket::List list_op(&target);

  list_op.params.copy(info);

  int op_ret = list_op.list_objects(max, result, common_prefixes, is_truncated);
  info.next_marker = list_op.get_next_marker();
  return op_ret;
}

RGWRados *RGWRadosBackend::get_rados() {
  return store;
}