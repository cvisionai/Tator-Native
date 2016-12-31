add_library(net_removal STATIC partition.cc)
target_link_libraries(
  net_removal
  ${ArrayFire_CUDA_LIBRARIES}
  )
