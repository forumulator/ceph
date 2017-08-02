#include "rgw_rados.h"

RGWRados *RGWStoreFactory::make_rgw_rados() {
  return RGWStoreManager::get_storage(
      cct, bool use_gc_thread, bool use_lc_thread, 
      bool quota_threads, bool run_sync_thread,
      bool run_reshard_thread);
}


RGWStore *RGWStoreFactory::make_rgw_store(int backend_type) {
  RGWStore *store;
  switch (backend_type) {
    case RADOS:
      store = make_rgw_rados();
      break;
    default:
      derr << "Invalid backend_type";
      store = nullptr;
  }

  return store;
}


