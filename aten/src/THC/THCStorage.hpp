#pragma once

// STOP!!! Thinking of including this header directly?  Please
// read Note [TH abstraction violation]

#include "THCStorage.h"

#include "ATen/ScalarType.h"
#include "ATen/ScalarTypeUtils.h"
#include "ATen/cuda/CUDATypeConversion.cuh"
#include <atomic>

typedef struct THCStorage
{
    at::ScalarType scalar_type;
    void *data_ptr;
    ptrdiff_t size;
    std::atomic<int> refcount;
    char flag;
    THCDeviceAllocator *allocator;
    void *allocatorContext;
    struct THCStorage *view;
    int device;

    template <typename T>
    inline T * data() const {
      auto scalar_type_T = at::CTypeToScalarType<at::cuda::from_type<T>>::to();
      if (scalar_type != scalar_type_T) {
        AT_ERROR("Attempt to access Storage having data type ", at::toString(scalar_type),
                 " as data type ", at::toString(scalar_type_T));
      }
      return unsafe_data<T>();
    }

    template <typename T>
    inline T * unsafe_data() const {
      return static_cast<T*>(this->data_ptr);
    }
} THCStorage;
