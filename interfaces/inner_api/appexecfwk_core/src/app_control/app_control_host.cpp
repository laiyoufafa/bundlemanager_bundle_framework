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

#include "app_control_host.h"

#include "app_control_constants.h"
#include "app_log_wrapper.h"
#include "appexecfwk_errors.h"
#include "ipc_types.h"

namespace OHOS {
namespace AppExecFwk {
AppControlHost::AppControlHost()
{
    APP_LOGD("create AppControlHost.");
}

AppControlHost::~AppControlHost()
{
    APP_LOGD("destroy AppControlHost.");
}

int AppControlHost::OnRemoteRequest(
    uint32_t code, MessageParcel& data, MessageParcel& reply, MessageOption& option)
{
    APP_LOGI("AppControlHost OnRemoteRequest, message code : %{public}u", code);
    std::u16string descriptor = AppControlHost::GetDescriptor();
    std::u16string remoteDescriptor = data.ReadInterfaceToken();
    if (descriptor != remoteDescriptor) {
        APP_LOGE("descriptor invalid.");
        return OBJECT_NULL;
    }

    switch (code) {
        case IAppControlMgr::Message::ADD_APP_INSTALL_CONTROL_RULE:
            return HandleAddAppInstallControlRule(data, reply);
        case IAppControlMgr::Message::DELETE_APP_INSTALL_CONTROL_RULE:
            return HandleDeleteAppInstallControlRule(data, reply);
        case IAppControlMgr::Message::CLEAN_APP_INSTALL_CONTROL_RULE:
            return HandleCleanAppInstallControlRule(data, reply);
        case IAppControlMgr::Message::GET_APP_INSTALL_CONTROL_RULE:
            return HandleGetAppInstallControlRule(data, reply);
        default:
            APP_LOGW("AppControlHost receive unknown code, code = %{public}d", code);
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
}

ErrCode AppControlHost::HandleAddAppInstallControlRule(MessageParcel& data, MessageParcel& reply)
{
    std::vector<std::string> appIds;
    int32_t appIdSize = data.ReadInt32();
    if (appIdSize > AppControlConstants::LIST_MAX_SIZE) {
        APP_LOGE("HandleAddAppInstallControlRule parameter is invalid");
        return ERR_INVALID_VALUE;
    }
    for (size_t i = 0; i < appIdSize; i++) {
        appIds.emplace_back(data.ReadString());
    }
    AppInstallControlRuleType controlRuleType = static_cast<AppInstallControlRuleType>(data.ReadInt32());
    int32_t userId = data.ReadInt32();
    int32_t ret = AddAppInstallControlRule(appIds, controlRuleType, userId);
    if (ret != ERR_OK) {
        APP_LOGE("HandleAddAppInstallControlRule failed");
    }
    return ret;
}

ErrCode AppControlHost::HandleDeleteAppInstallControlRule(MessageParcel& data, MessageParcel& reply)
{
    std::vector<std::string> appIds;
    int32_t appIdSize = data.ReadInt32();
    if (appIdSize > AppControlConstants::LIST_MAX_SIZE) {
        APP_LOGE("HandleDeleteAppInstallControlRule parameter is invalid");
        return ERR_INVALID_VALUE;
    }
    for (size_t i = 0; i < appIdSize; i++) {
        appIds.emplace_back(data.ReadString());
    }
    int32_t userId = data.ReadInt32();
    int32_t ret = DeleteAppInstallControlRule(appIds, userId);
    if (ret != ERR_OK) {
        APP_LOGE("HandleDeleteAppInstallControlRule failed");
    }
    return ret;
}

ErrCode AppControlHost::HandleCleanAppInstallControlRule(MessageParcel& data, MessageParcel& reply)
{
    AppInstallControlRuleType controlRuleType = static_cast<AppInstallControlRuleType>(data.ReadInt32());
    int32_t userId = data.ReadInt32();
    int32_t ret = DeleteAppInstallControlRule(controlRuleType, userId);
    if (ret != ERR_OK) {
        APP_LOGE("HandleCleanAppInstallControlRule failed");
    }
    return ret;
}

ErrCode AppControlHost::HandleGetAppInstallControlRule(MessageParcel& data, MessageParcel& reply)
{
    AppInstallControlRuleType controlRuleType = static_cast<AppInstallControlRuleType>(data.ReadInt32());
    int32_t userId = data.ReadInt32();
    std::vector<std::string> appIds;
    int32_t ret = GetAppInstallControlRule(controlRuleType, userId, appIds);
    if (ret != ERR_OK) {
        APP_LOGE("HandleGetAppInstallControlRule failed");
        return ret;
    }
    if (!WriteParcelableVector(appIds, reply)) {
        APP_LOGE("write appIds failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    return ERR_OK;
}

bool AppControlHost::WriteParcelableVector(const std::vector<std::string> &stringVector, MessageParcel &reply)
{
    if (!reply.WriteInt32(stringVector.size())) {
        APP_LOGE("write ParcelableVector failed");
        return false;
    }

    for (auto &string : stringVector) {
        if (!reply.WriteString(string)) {
            APP_LOGE("write string failed");
            return false;
        }
    }
    return true;
}
} // AppExecFwk
} // OHOS