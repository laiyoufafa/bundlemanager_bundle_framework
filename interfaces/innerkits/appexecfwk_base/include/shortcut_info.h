/*
 * Copyright (c) 2021 Huawei Device Co., Ltd.
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

#ifndef FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_SHORTCUT_INFO_H
#define FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_SHORTCUT_INFO_H

#include <string>
#include "parcel.h"

namespace OHOS {
namespace AppExecFwk {
struct ShortcutIntent {
    std::string targetBundle;
    std::string targetClass;
};

struct ShortcutInfo : public Parcelable {
    std::string id;
    std::string bundleName;
    std::string hostAbility;
    std::string icon;
    std::string label;
    std::string disableMessage;
    std::vector<ShortcutIntent> intents;
    bool isStatic = false;
    bool isHomeShortcut = false;
    bool isEnables = false;

    bool ReadFromParcel(Parcel &parcel);
    virtual bool Marshalling(Parcel &parcel) const override;
    static ShortcutInfo *Unmarshalling(Parcel &parcel);
};
}  // namespace AppExecFwk
}  // namespace OHOS
#endif  // FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_SHORTCUT_INFO_H