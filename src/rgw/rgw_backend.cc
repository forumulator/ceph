#include "rgw_rados.h"
#include "rgw_backend.h"
#include "rgw_rados_backend.h"

RGWRadosBackend *RGWStoreFactory::make_rgw_rados() {
  RGWRados *store = RGWStoreManager::get_storage(
      cct, use_gc_thread, use_lc_thread, 
      quota_threads, run_sync_thread,
      run_reshard_thread);
  RGWRadosBackend *rgw_backend = new RGWRadosBackend();
  if (!rgw_backend->init(store)) {
    return rgw_backend;
  }
  else {
    // delete rgw_backend;
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
      // cerr << "Invalid backend_type";
      backend = nullptr;
  }

  return backend;
}
