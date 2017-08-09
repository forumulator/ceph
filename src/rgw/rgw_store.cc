#include "rgw_rados.h"

RGWRadosBackend *RGWStoreFactory::make_rgw_rados() {
  RGWRados *store = RGWStoreManager::get_storage(
      cct, bool use_gc_thread, bool use_lc_thread, 
      bool quota_threads, bool run_sync_thread,
      bool run_reshard_thread);
  RGWRadosBackend *rgw_store = new RGWRadosBackend();
  if (!rgw_store->init(store)) {
    return rgw_store;
  }
  else {
    delete rgw_store;
    return NULL;
  }
}


RGWBackend *RGWStoreFactory::make_rgw_backend(int backend_type) {
  RGWBackend *backend;
  switch (backend_type) {
    case RADOS:
      backend = make_rgw_rados();
      break;
    default:
      derr << "Invalid backend_type";
      backend = nullptr;
  }

  return backend;
}


