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

#ifndef FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_APP_RUNNING_CONTROL_RULE_PARAM_H
#define FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_APP_RUNNING_CONTROL_RULE_PARAM_H

#include <string>

#include "parcel.h"
#include "want.h"

namespace OHOS {
namespace AppExecFwk {
enum class AppRunningControlRuleType {
    DISALLOWED_RUNNING_NOW = 0, // L1 edm
    DISALLOWED_RUNNING_NEXT, // L2
    UNSPECIFIED,
};
struct AppRunningControlRuleParam : public Parcelable {
    std::string controlMessage;
    std::shared_ptr<AAFwk::Want> controlWant = nullptr;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    static AppRunningControlRuleParam *Unmarshalling(Parcel &parcel);
};
} // AppExecFwk
} // OHOS
#endif // FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_APP_RUNNING_CONTROL_RULE_PARAM_H