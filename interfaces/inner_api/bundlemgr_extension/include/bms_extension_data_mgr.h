/*
 * Copyright (c) 2023 Huawei Device Co., Ltd.
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

#ifndef FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_BMS_EXTENSION_DATA_MGR_H
#define FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_BMS_EXTENSION_DATA_MGR_H
#include <mutex>
#include <string>

#include "appexecfwk_errors.h"
#include "bms_extension.h"
#include "bundle_info.h"

namespace OHOS {
namespace AppExecFwk {
class BmsExtensionDataMgr {
public:
    BmsExtensionDataMgr();
    bool CheckApiInfo(const BundleInfo &bundleInfo);
    ErrCode Init();
private:
    bool OpenHandler();
    static BmsExtension bmsExtension_;
    static void *handler_;
    mutable std::mutex stateMutex_;
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // FOUNDATION_APPEXECFWK_SERVICES_BUNDLEMGR_INCLUDE_BMS_EXTENSION_DATA_MGR_H