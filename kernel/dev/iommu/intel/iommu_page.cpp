// Copyright 2017 The Fuchsia Authors
//
// Use of this source code is governed by a MIT-style
// license that can be found in the LICENSE file or at
// https://opensource.org/licenses/MIT

#include "iommu_page.h"

#include <kernel/vm.h>
#include <kernel/vm/vm_aspace.h>
#include <mxalloc/new.h>

namespace intel_iommu {

IommuPage::IommuPage(vm_page_t* page, uintptr_t virt) : page_(page), virt_(virt) {
}

IommuPage::~IommuPage() {
    if (page_) {
        VmAspace::kernel_aspace()->FreeRegion(reinterpret_cast<vaddr_t>(virt_));
        pmm_free_page(page_);
    }
}

status_t IommuPage::AllocatePage(IommuPage* out) {
    paddr_t paddr;
    vm_page_t* page = pmm_alloc_page(0, &paddr);
    if (!page) {
        return ERR_NO_MEMORY;
    }
    page->state = VM_PAGE_STATE_IOMMU;

    void* vaddr;
    auto kernel_aspace = VmAspace::kernel_aspace();
    status_t status = kernel_aspace->AllocPhysical(
            "iommu_ctx_tbl",
            PAGE_SIZE,
            &vaddr,
            PAGE_SIZE_SHIFT,
            paddr,
            0,
            ARCH_MMU_FLAG_PERM_READ | ARCH_MMU_FLAG_PERM_WRITE);
    if (status != NO_ERROR) {
        pmm_free_page(page);
        return status;
    }

    arch_zero_page(vaddr);

    *out = IommuPage(page, reinterpret_cast<uintptr_t>(vaddr));
    return NO_ERROR;
}

} // namespace intel_iommu