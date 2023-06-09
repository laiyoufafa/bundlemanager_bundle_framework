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

#include "service_center_status_callback.h"

#include "hilog_wrapper.h"

namespace OHOS {
namespace AppExecFwk {
ServiceCenterStatusCallback::ServiceCenterStatusCallback(const std::weak_ptr<BundleConnectAbilityMgr> &server)
    : server_(server)
{
    HILOG_INFO("%{public}s", __func__);
}

void ServiceCenterStatusCallback::OnInstallFinished(std::string installResult)
{
    HILOG_INFO("%{public}s", __func__);
    auto server = server_.lock();
    if (!server) {
        HILOG_ERROR("pointer is nullptr.");
        return;
    }
    server->OnServiceCenterCall(installResult);
}
}  // namespace AppExecFwk
}  // namespace OHOS
