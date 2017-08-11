#ifndef RGW_BACKEND_H
#define RGW_BACKEND_H

#include "rgw_rados.h"
#include <atomic>

/*
 * Enum of all existing backends.
 */
// TODO: Better way of doing this?
enum BackendType {
  RADOS = 1, CDOBS = 2
};

/* 
 * The abstract class RGWBackend. Handles backend storage for 
 * Amazon S3/ Apache Swift type buckets and objects for RGW.
 * To add an alternative backend, subclass this class.
 */
class RGWBackend {
protected:
  RGWRealm realm;
  RGWZoneGroup zonegroup;
  RGWZone zone;

  std::atomic<int64_t> max_req_id = { 0 };
public:
  RGWBackend() {}
  virtual uint64_t get_new_req_id() {
    return ++max_req_id;
  }

  virtual int UpdateContainerStats(map<string, RGWBucketEnt>& m) {
    return -1; // Return -ENIMP(not implemented)
  }

  virtual RGWRealm& get_realm() {
    return realm;
  }

  virtual RGWZoneGroup& get_zonegroup() {
    return zonegroup;
  }

  virtual RGWZone& get_zone() {
    return zone;
  }

  /* For bucket ops ListBucket and UpdateIndex */
  class Bucket {
  public:
    RGWRados *store;
    RGWBucketInfo bucket_info;
    rgw_bucket& bucket;
    int shard_id;

    Bucket(RGWRados *_store, const RGWBucketInfo& _bucket_info) : 
        store(_store), bucket_info(_bucket_info), bucket(bucket_info.bucket),
        shard_id(RGW_NO_SHARD) {}
    RGWRados *get_store() { return store; }
    rgw_bucket& get_bucket() { return bucket; }
    RGWBucketInfo& get_bucket_info() { return bucket_info; }

    int update_bucket_id(const string& new_bucket_id);

    int get_shard_id() { return shard_id; }
    void set_shard_id(int id) {
      shard_id = id;
    }
  };

  struct ListBucketInfo {
    string prefix;
    string delim;
    rgw_obj_key marker;
    rgw_obj_key end_marker;
    string ns;
    bool enforce_ns;
    RGWAccessListFilter *filter;
    bool list_versions;

    rgw_obj_key next_marker; // next marker set by ListBucket

    ListBucketInfo() : enforce_ns(true), filter(NULL), list_versions(false) {}
  };

  virtual int ListBucket(RGWBackend::Bucket *target, RGWBackend::ListBucketInfo &info,
                        int64_t max, vector<rgw_bucket_dir_entry> *result, 
                        map<string, bool> *common_prefixes, bool *is_truncated) = 0;
  ~RGWBackend() {};
};

class CephContext;
class RGWRadosBackend;

/* RGWBackend factory. Has private functions for init of 
 * each backend and one public function that returns an
 * instance of RGWBackend
 */
class RGWStoreFactory {
private:
  CephContext *cct;
  bool use_gc_thread, use_lc_thread, quota_threads;
  bool run_sync_thread, run_reshard_thread;

  // RGWCdobs *MakeCdobs();
  RGWRadosBackend *make_rgw_rados();
public:
  RGWStoreFactory(CephContext *cct, bool use_gc_thread, 
                  bool use_lc_thread, bool quota_threads, bool run_sync_thread,
                  bool run_reshard_thread): 
      cct(cct), use_gc_thread(use_gc_thread), use_lc_thread(use_lc_thread), 
      quota_threads(quota_threads), run_sync_thread(run_sync_thread),
      run_reshard_thread(run_reshard_thread) {}

  // Can't change the existing interface of RGWStoreManager, hence all the args.
  RGWBackend *make_rgw_backend(int backend_type);
};


#endif