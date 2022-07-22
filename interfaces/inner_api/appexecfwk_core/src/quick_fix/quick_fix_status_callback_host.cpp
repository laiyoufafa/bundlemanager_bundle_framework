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

#include "quick_fix_status_callback_host.h"

#include "app_log_wrapper.h"
#include "appexecfwk_errors.h"
#include "ipc_types.h"

namespace OHOS {
namespace AppExecFwk {
QuickFixStatusCallbackHost::QuickFixStatusCallbackHost()
{
    APP_LOGI("create QuickFixStatusCallbackHost.");
}

QuickFixStatusCallbackHost::~QuickFixStatusCallbackHost()
{
    APP_LOGI("destroy QuickFixStatusCallbackHost.");
}

int QuickFixStatusCallbackHost::OnRemoteRequest(
    uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    APP_LOGI("QuickFixStatusCallbackHost onReceived message, the message code is %{public}u", code);
    std::u16string descripter = QuickFixStatusCallbackHost::GetDescriptor();
    std::u16string remoteDescripter = data.ReadInterfaceToken();
    if (descripter != remoteDescripter) {
        APP_LOGE("fail to write reply message in clean cache host due to the reply is nullptr");
        return OBJECT_NULL;
    }
    return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
}
} // AppExecFwk
} // OHOS
