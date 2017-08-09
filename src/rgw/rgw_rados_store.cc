#include "rgw_radods_store.h"
#include "rgw_store.h"
#include "rgw_rados.h"

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

int RGWRadosBackend::ListBucket(RGWBackend::Bucket *target, RGWBackend::ListBucketInfo &info,
                              int64_t max, vector<rgw_bucket_dir_entry> *result, 
                              map<string, bool> *common_prefixes, bool *is_truncated) {
  RGWRados::Bucket target(*target);
  RGWRados::Bucket::List list_op(target);

  list_op.params.prefix = info.prefix;
  list_op.params.delim = info.delimiter;
  list_op.params.marker = info.marker;
  list_op.params.end_marker = info.end_marker;
  list_op.params.list_versions = info.list_versions;

  int op_ret = list_op.list_objects(max, result, common_prefixes, is_truncated);
  info.next_marker = list_op.get_next_marker();
  return op_ret;
}

RGWRados *get_store() {
  return store;
}