// Copyright 2017 The Fuchsia Authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#pragma once

#include <assert.h>
#include <magenta/compiler.h>
#include <stdint.h>

__BEGIN_CDECLS

#define MX_IOMMU_MAX_DESC_LEN 4096

#define MX_IOMMU_TYPE_DUMMY 0
#define MX_IOMMU_TYPE_INTEL 1

typedef struct mx_iommu_desc_dummy {
} mx_iommu_desc_dummy_t;

typedef struct mx_iommu_desc_intel {
    uint64_t register_base; // physical address of registers
} mx_iommu_desc_intel_t;

__END_CDECLS