/*
 * Copyright (c) 2022 Huawei Device Co., Ltd.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "installd/installd_permission_mgr.h"

#include "app_log_wrapper.h"
#include "ipc_skeleton.h"

namespace OHOS {
namespace AppExecFwk {
bool InstalldPermissionMgr::VerifyCallingPermission(int32_t uid)
{
    int32_t callingUid = IPCSkeleton::GetCallingUid();
    if (callingUid == uid) {
        return true;
    }
    APP_LOGE("VerifyCallingPermission failed, uid = %{public}d, calling uid = %{public}d", uid, callingUid);
    return false;
}
} // AppExecFwk
} // OHOS