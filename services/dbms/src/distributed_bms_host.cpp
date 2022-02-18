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

#include "distributed_bms_host.h"

#include "app_log_wrapper.h"
#include "appexecfwk_errors.h"
#include "bundle_constants.h"
#include "remote_ability_info.h"

namespace OHOS {
namespace AppExecFwk {
DistributedBmsHost::DistributedBmsHost()
{
    APP_LOGI("DistributedBmsHost instance is created");
}

DistributedBmsHost::~DistributedBmsHost()
{
    APP_LOGI("DistributedBmsHost instance is destroyed");
}

int DistributedBmsHost::OnRemoteRequest(uint32_t code, MessageParcel &data, MessageParcel &reply, MessageOption &option)
{
    APP_LOGI("DistributedBmsHost receives message from client, code = %{public}d, flags = %{public}d", code,
        option.GetFlags());
    switch (code) {
        case static_cast<uint32_t>(IDistributedBms::Message::GET_REMOTE_ABILITY_INFO): {
            return HandleGetRemoteAbilityInfo(data, reply);
        }
        case static_cast<uint32_t>(IDistributedBms::Message::GET_REMOTE_ABILITY_INFOS): {
            return HandleGetRemoteAbilityInfos(data, reply);
        }
        case static_cast<uint32_t>(IDistributedBms::Message::GET_ABILITY_INFO): {
            return HandleGetAbilityInfo(data, reply);
        }
        case static_cast<uint32_t>(IDistributedBms::Message::GET_ABILITY_INFOS): {
            return HandleGetAbilityInfos(data, reply);
        }
        default:
            APP_LOGW("DistributedBmsHost receives unknown code, code = %{public}d", code);
            return IPCObjectStub::OnRemoteRequest(code, data, reply, option);
    }
    return NO_ERROR;
}

int DistributedBmsHost::HandleGetRemoteAbilityInfo(Parcel &data, Parcel &reply)
{
    APP_LOGI("DistributedBmsHost handle get remote ability info");
    std::unique_ptr<ElementName> elementName(data.ReadParcelable<ElementName>());
    if (!elementName) {
        APP_LOGE("ReadParcelable<elementName> failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    RemoteAbilityInfo remoteAbilityInfo;
    int ret = GetRemoteAbilityInfo(*elementName, remoteAbilityInfo);
    if (ret != NO_ERROR) {
        APP_LOGE("GetRemoteAbilityInfo result:%{public}d", ret);
        return ret;
    }
    if (!reply.WriteBool(true)) {
        APP_LOGE("GetRemoteAbilityInfo write failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    if (!reply.WriteParcelable(&remoteAbilityInfo)) {
        APP_LOGE("GetRemoteAbilityInfo write failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    return NO_ERROR;
}

int DistributedBmsHost::HandleGetRemoteAbilityInfos(Parcel &data, Parcel &reply)
{
    APP_LOGI("DistributedBmsHost handle get remote ability infos");
    std::vector<ElementName> elementNames;
    if (!GetParcelableInfos<ElementName>(data, elementNames)) {
        APP_LOGE("GetRemoteAbilityInfos get parcelable infos failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    std::vector<RemoteAbilityInfo> remoteAbilityInfos;
    int ret = GetRemoteAbilityInfos(elementNames, remoteAbilityInfos);
    if (ret != NO_ERROR) {
        APP_LOGE("GetRemoteAbilityInfos result:%{public}d", ret);
        return ret;
    }
    if (!reply.WriteBool(true)) {
        APP_LOGE("GetRemoteAbilityInfos write failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    if (!WriteParcelableVector<RemoteAbilityInfo>(remoteAbilityInfos, reply)) {
        APP_LOGE("GetRemoteAbilityInfos write failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    return NO_ERROR;
}

int DistributedBmsHost::HandleGetAbilityInfo(Parcel &data, Parcel &reply)
{
    APP_LOGI("DistributedBmsHost handle get ability info");
    std::unique_ptr<ElementName> elementName(data.ReadParcelable<ElementName>());
    if (!elementName) {
        APP_LOGE("ReadParcelable<elementName> failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    RemoteAbilityInfo remoteAbilityInfo;
    int ret = GetAbilityInfo(*elementName, remoteAbilityInfo);
    if (ret != NO_ERROR) {
        APP_LOGE("GetAbilityInfo result:%{public}d", ret);
        return ret;
    }
    if (!reply.WriteBool(true)) {
        APP_LOGE("GetRemoteAbilityInfo write failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    if (!reply.WriteParcelable(&remoteAbilityInfo)) {
        APP_LOGE("GetRemoteAbilityInfo write failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    return NO_ERROR;
}

int DistributedBmsHost::HandleGetAbilityInfos(Parcel &data, Parcel &reply)
{
    APP_LOGI("DistributedBmsHost handle get ability infos");
    std::vector<ElementName> elementNames;
    if (!GetParcelableInfos<ElementName>(data, elementNames)) {
        APP_LOGE("GetRemoteAbilityInfos get parcelable infos failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    std::vector<RemoteAbilityInfo> remoteAbilityInfos;
    int ret = GetAbilityInfos(elementNames, remoteAbilityInfos);
    if (ret != NO_ERROR) {
        APP_LOGE("GetAbilityInfos result:%{public}d", ret);
        return ret;
    }
    if (!reply.WriteBool(true)) {
        APP_LOGE("GetAbilityInfos write failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    if (!WriteParcelableVector<RemoteAbilityInfo>(remoteAbilityInfos, reply)) {
        APP_LOGE("GetAbilityInfos write failed");
        return ERR_APPEXECFWK_PARCEL_ERROR;
    }
    return NO_ERROR;
}

template<typename T>
bool DistributedBmsHost::WriteParcelableVector(std::vector<T> &parcelableVector, Parcel &reply)
{
    if (!reply.WriteInt32(parcelableVector.size())) {
        APP_LOGE("write ParcelableVector failed");
        return false;
    }

    for (auto &parcelable : parcelableVector) {
        if (!reply.WriteParcelable(&parcelable)) {
            APP_LOGE("write ParcelableVector failed");
            return false;
        }
    }
    return true;
}

template<typename T>
bool DistributedBmsHost::GetParcelableInfos(Parcel &data, std::vector<T> &parcelableInfos)
{
    int32_t infoSize = data.ReadInt32();
    for (int32_t i = 0; i < infoSize; i++) {
        std::unique_ptr<T> info(data.ReadParcelable<T>());
        if (!info) {
            APP_LOGE("Read Parcelable infos failed");
            return false;
        }
        parcelableInfos.emplace_back(*info);
    }
    APP_LOGD("get parcelable infos success");
    return true;
}
}  // namespace AppExecFwk
}  // namespace OHOS