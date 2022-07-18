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

#include "bundle_connect_ability_mgr.h"

#include "ability_manager_client.h"
#include "app_log_wrapper.h"
#include "bundle_mgr_service.h"
#include "free_install_params.h"
#include "json_util.h"
#include "parcel.h"
#include "service_center_connection.h"
#include "service_center_status_callback.h"
#include "string_ex.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
const std::string SERVICE_CENTER_BUNDLE_NAME = "com.ohos.hag.famanager";
const std::string SERVICE_CENTER_ABILITY_NAME = "HapInstallServiceAbility";
const std::string DEFAULT_VERSION = "1";
constexpr uint32_t CALLING_TYPE_HARMONY = 2;
constexpr uint32_t BIT_ZERO_COMPATIBLE = 0;
constexpr uint32_t BIT_ONE_FRONT_MODE = 0;
constexpr uint32_t BIT_ONE_BACKGROUND_MODE = 1;
constexpr uint32_t BIT_TWO_CUSTOM = 0;
constexpr uint32_t BIT_FOUR_AZ_DEVICE = 0;
constexpr uint32_t BIT_SIX_SAME_BUNDLE = 0;
constexpr uint32_t BIT_ONE = 2;
constexpr uint32_t BIT_TWO = 4;
constexpr uint32_t BIT_THREE = 8;
constexpr uint32_t BIT_FOUR = 16;
constexpr uint32_t BIT_FIVE = 32;
constexpr uint32_t BIT_SIX = 64;
constexpr uint32_t OUT_TIME = 30000;

void SendSysEvent(int32_t resultCode, const AAFwk::Want &want, int32_t userId)
{
    EventInfo sysEventInfo;
    ElementName element = want.GetElement();
    sysEventInfo.bundleName = element.GetBundleName();
    sysEventInfo.moduleName = element.GetModuleName();
    sysEventInfo.abilityName = element.GetAbilityName();
    sysEventInfo.isFreeInstallMode = true;
    sysEventInfo.userId = userId;
    sysEventInfo.errCode = resultCode;
    EventReport::SendSystemEvent(BMSEventType::BUNDLE_INSTALL_EXCEPTION, sysEventInfo);
}
}

void BundleConnectAbilityMgr::Init()
{
    runner_ = EventRunner::Create(true);
    if (runner_ == nullptr) {
        APP_LOGE("Create runner failed");
        return;
    }

    handler_ = std::make_shared<AppExecFwk::EventHandler>(runner_);
    if (handler_ == nullptr) {
        APP_LOGE("Create handler failed");
    }
}

BundleConnectAbilityMgr::BundleConnectAbilityMgr()
{
    Init();
}

BundleConnectAbilityMgr::~BundleConnectAbilityMgr()
{
    if (handler_ != nullptr) {
        handler_.reset();
    }
    if (runner_ != nullptr) {
        runner_.reset();
    }
}

bool BundleConnectAbilityMgr::SilentInstall(const TargetAbilityInfo &targetAbilityInfo, const Want &want,
    const FreeInstallParams &freeInstallParams, int32_t userId)
{
    APP_LOGI("SilentInstall");
    if (handler_ == nullptr) {
        SendCallBack(FreeInstallErrorCode::UNDEFINED_ERROR, want,
            userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId);
        APP_LOGE("handler is null");
        return false;
    }
    auto silentInstallFunc = [this, targetAbilityInfo, want, userId, freeInstallParams]() {
        int32_t flag = ServiceCenterFunction::CONNECT_SILENT_INSTALL;
        this->SendRequestToServiceCenter(flag, targetAbilityInfo, want, userId, freeInstallParams);
    };
    handler_->PostTask(silentInstallFunc, targetAbilityInfo.targetInfo.transactId.c_str());
    return true;
}

bool BundleConnectAbilityMgr::UpgradeCheck(const TargetAbilityInfo &targetAbilityInfo, const Want &want,
    const FreeInstallParams &freeInstallParams, int32_t userId)
{
    APP_LOGI("UpgradeCheck");
    if (handler_ == nullptr) {
        SendCallBack(FreeInstallErrorCode::UNDEFINED_ERROR, want,
            userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId);
        APP_LOGE("handler is null");
        return false;
    }
    auto upgradeCheckFunc = [this, targetAbilityInfo, want, userId, freeInstallParams]() {
        int32_t flag = ServiceCenterFunction::CONNECT_UPGRADE_CHECK;
        this->SendRequestToServiceCenter(flag, targetAbilityInfo, want, userId, freeInstallParams);
    };
    handler_->PostTask(upgradeCheckFunc, targetAbilityInfo.targetInfo.transactId.c_str());
    return true;
}

bool BundleConnectAbilityMgr::UpgradeInstall(const TargetAbilityInfo &targetAbilityInfo, const Want &want,
    const FreeInstallParams &freeInstallParams, int32_t userId)
{
    APP_LOGI("UpgradeInstall");
    if (handler_ == nullptr) {
        SendCallBack(FreeInstallErrorCode::UNDEFINED_ERROR, want,
            userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId);
        APP_LOGE("handler is null");
        return false;
    }
    auto upgradeInstallFunc = [this, targetAbilityInfo, want, userId, freeInstallParams]() {
        int32_t flag = ServiceCenterFunction::CONNECT_UPGRADE_INSTALL;
        this->SendRequestToServiceCenter(flag, targetAbilityInfo, want, userId, freeInstallParams);
    };
    handler_->PostTask(upgradeInstallFunc, targetAbilityInfo.targetInfo.transactId.c_str());
    return true;
}

bool BundleConnectAbilityMgr::SendRequestToServiceCenter(int32_t flag,
    const TargetAbilityInfo &targetAbilityInfo, const Want &want,
    int32_t userId, const FreeInstallParams &freeInstallParams)
{
    APP_LOGI("SendRequestToServiceCenter");
    Want serviceCenterWant;
    serviceCenterWant.SetElementName(SERVICE_CENTER_BUNDLE_NAME, SERVICE_CENTER_ABILITY_NAME);
    bool isConnectSuccess = ConnectAbility(serviceCenterWant, nullptr);
    if (!isConnectSuccess) {
        APP_LOGE("Fail to connect ServiceCenter");
        SendCallBack(FreeInstallErrorCode::CONNECT_ERROR, want,
            userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::CONNECT_ERROR, want, userId);
        return false;
    } else {
        SendRequest(flag, targetAbilityInfo, want, userId, freeInstallParams);
        return true;
    }
}

void BundleConnectAbilityMgr::DisconnectAbility()
{
    if (serviceCenterConnection_ != nullptr) {
        APP_LOGI("DisconnectAbility");
        int result = AbilityManagerClient::GetInstance()->DisconnectAbility(serviceCenterConnection_);
        if (result != ERR_OK) {
            APP_LOGE("BundleConnectAbilityMgr::DisconnectAbility fail, resultCode: %{public}d", result);
        }
    }
}

void BundleConnectAbilityMgr::WaitFromConnecting(std::unique_lock<std::mutex> &lock)
{
    APP_LOGI("ConnectAbility await start CONNECTING");
    while (connectState_ == ServiceCenterConnectState::CONNECTING) {
        cv_.wait(lock);
    }
    APP_LOGI("ConnectAbility await end CONNECTING");
}

void BundleConnectAbilityMgr::WaitFromConnected(std::unique_lock<std::mutex> &lock)
{
    APP_LOGI("ConnectAbility await start CONNECTED");
    while (connectState_ != ServiceCenterConnectState::CONNECTED) {
        if (connectState_ == ServiceCenterConnectState::DISCONNECTED) {
            break;
        }
        cv_.wait(lock);
    }
    APP_LOGI("ConnectAbility await end CONNECTED");
}

bool BundleConnectAbilityMgr::ConnectAbility(const Want &want, const sptr<IRemoteObject> &callerToken)
{
    APP_LOGI("ConnectAbility start target bundle = %{public}s", want.GetBundle().c_str());
    std::unique_lock<std::mutex> lock(mutex_);
    if (connectState_ == ServiceCenterConnectState::CONNECTING) {
        WaitFromConnecting(lock);
    } else if (connectState_ == ServiceCenterConnectState::DISCONNECTED) {
        connectState_ = ServiceCenterConnectState::CONNECTING;
        serviceCenterConnection_ = new (std::nothrow) ServiceCenterConnection(connectState_,
            cv_, weak_from_this());
        if (serviceCenterConnection_ == nullptr) {
            APP_LOGE("ServiceCenterConnection is nullptr");
            connectState_ = ServiceCenterConnectState::DISCONNECTED;
            cv_.notify_all();
            return false;
        }
        APP_LOGI("ConnectAbility start");
        int result = AbilityManagerClient::GetInstance()->ConnectAbility(want, serviceCenterConnection_, callerToken);
        if (result == ERR_OK) {
            if (connectState_ != ServiceCenterConnectState::CONNECTED) {
                WaitFromConnected(lock);
            }
            serviceCenterRemoteObject_ = serviceCenterConnection_->GetRemoteObject();
        } else {
            APP_LOGE("ConnectAbility fail result = %{public}d", result);
        }
    }

    APP_LOGI("ConnectAbility end");
    if (connectState_ == ServiceCenterConnectState::CONNECTED) {
        return true;
    } else {
        APP_LOGE("ConnectAbility fail");
        connectState_ = ServiceCenterConnectState::DISCONNECTED;
        return false;
    }
}

void BundleConnectAbilityMgr::SendCallBack(
    int32_t resultCode, const AAFwk::Want &want, int32_t userId, const std::string &transactId)
{
    APP_LOGI("SendCallBack");
    sptr<IRemoteObject> amsCallBack = GetAbilityManagerServiceCallBack(transactId);

    freeInstallParamsMap_.erase(transactId);
    if (freeInstallParamsMap_.size() == 0) {
        if (connectState_ == ServiceCenterConnectState::CONNECTED) {
            APP_LOGI("Disconnect Ability.");
            DisconnectAbility();
        }
    }

    if (amsCallBack == nullptr) {
        APP_LOGE("Abilitity manager callback is null");
        DisconnectAbility();
        return;
    }

    MessageParcel data;
    if (!data.WriteInterfaceToken(ATOMIC_SERVICE_STATUS_CALLBACK_TOKEN)) {
        APP_LOGE("Write interface token failed");
        return;
    }
    if (!data.WriteInt32(resultCode)) {
        APP_LOGE("Write result code failed");
        return;
    }
    if (!data.WriteParcelable(&want)) {
        APP_LOGE("Write want failed");
        return;
    }
    if (!data.WriteInt32(userId)) {
        APP_LOGE("Write userId failed");
        return;
    }
    MessageParcel reply;
    MessageOption option;

    if (amsCallBack->SendRequest(FREE_INSTALL_DONE, data, reply, option) != ERR_OK) {
        APP_LOGE("BundleConnectAbilityMgr::SendCallBack SendRequest failed");
    }
}

void BundleConnectAbilityMgr::SendCallBack(const std::string &transactId, const FreeInstallParams &freeInstallParams)
{
    MessageParcel data;
    if (!data.WriteInterfaceToken(ATOMIC_SERVICE_STATUS_CALLBACK_TOKEN)) {
        APP_LOGE("Write interface token failed");
        return;
    }
    if (!data.WriteInt32(FreeInstallErrorCode::SERVICE_CENTER_CRASH)) {
        APP_LOGE("Write result code error");
        return;
    }
    if (!data.WriteParcelable(&(freeInstallParams.want))) {
        APP_LOGE("Write want failed");
        return;
    }
    if (!data.WriteInt32(freeInstallParams.userId)) {
        APP_LOGE("Write userId error");
        return;
    }
    MessageParcel reply;
    MessageOption option;
    if (freeInstallParams.callback->SendRequest(FREE_INSTALL_DONE, data, reply, option) != ERR_OK) {
        APP_LOGE("BundleConnectAbilityMgr::SendCallBack SendRequest failed");
    }
}

void BundleConnectAbilityMgr::DeathRecipientSendCallback()
{
    APP_LOGI("DeathRecipientSendCallback start");
    APP_LOGD("freeInstallParamsMap size = %{public}zu", freeInstallParamsMap_.size());
    for (auto &it : freeInstallParamsMap_) {
        SendCallBack(it.first, it.second);
    }
    freeInstallParamsMap_.clear();

    connectState_ = ServiceCenterConnectState::DISCONNECTED;
    serviceCenterRemoteObject_ = nullptr;
    cv_.notify_all();

    APP_LOGI("DeathRecipientSendCallback end");
}

void BundleConnectAbilityMgr::OnServiceCenterCall(std::string installResultStr)
{
    APP_LOGI("OnServiceCenterCall start, installResultStr = %{public}s", installResultStr.c_str());
    InstallResult installResult;
    if (!ParseInfoFromJsonStr(installResultStr.c_str(), installResult)) {
        APP_LOGE("Parse info from json fail");
        return;
    }
    APP_LOGI("OnServiceCenterCall, retCode = %{public}d", installResult.result.retCode);
    FreeInstallParams freeInstallParams;
    auto node = freeInstallParamsMap_.find(installResult.result.transactId);
    if (node == freeInstallParamsMap_.end()) {
        APP_LOGE("Can not find node in %{public}s function", __func__);
        return;
    }
    handler_->RemoveTask(installResult.result.transactId);
    freeInstallParams = node->second;
    if (installResult.result.retCode == ServiceCenterResultCode::FREE_INSTALL_DOWNLOADING) {
        APP_LOGI("ServiceCenter is downloading, downloadSize = %{public}d, totalSize = %{public}d",
            installResult.progress.downloadSize, installResult.progress.totalSize);
        return;
    }
    APP_LOGI("serviceCenterFunction = %{public}d", freeInstallParams.serviceCenterFunction);
    if (freeInstallParams.serviceCenterFunction == ServiceCenterFunction::CONNECT_UPGRADE_INSTALL
        && installResult.result.retCode == ServiceCenterResultCode::FREE_INSTALL_OK) {
        freeInstallParams.want.SetParam("freeInstallUpgraded", true);
        APP_LOGD("Set want is upgraded.");
    }
    SendCallBack(installResult.result.retCode, freeInstallParams.want, freeInstallParams.userId,
        installResult.result.transactId);
    APP_LOGI("OnServiceCenterCall end");
}

void BundleConnectAbilityMgr::OutTimeMonitor(std::string transactId)
{
    APP_LOGI("BundleConnectAbilityMgr::OutTimeMonitor");
    FreeInstallParams freeInstallParams;
    auto node = freeInstallParamsMap_.find(transactId);
    if (node == freeInstallParamsMap_.end()) {
        APP_LOGE("Can not find node in %{public}s function", __func__);
        return;
    }
    freeInstallParams = node->second;
    if (handler_ == nullptr) {
        APP_LOGE("OutTimeMonitor, handler is nullptr");
        return;
    }
    auto RegisterEventListenerFunc = [this, freeInstallParams, transactId]() {
        this->SendCallBack(FreeInstallErrorCode::SERVICE_CENTER_TIMEOUT,
            freeInstallParams.want, freeInstallParams.userId, transactId);
        APP_LOGI("RegisterEventListenerFunc");
    };
    handler_->PostTask(RegisterEventListenerFunc, transactId, OUT_TIME, AppExecFwk::EventQueue::Priority::LOW);
}

void BundleConnectAbilityMgr::SendRequest(int32_t flag, const TargetAbilityInfo &targetAbilityInfo, const Want &want,
    int32_t userId, const FreeInstallParams &freeInstallParams)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_ASYNC);
    if (!data.WriteInterfaceToken(SERVICE_CENTER_TOKEN)) {
        APP_LOGE("failed to WriteInterfaceToken");
        SendCallBack(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId);
        return;
    }
    const std::string dataString = GetJsonStrFromInfo(targetAbilityInfo);
    APP_LOGI("TargetAbilityInfo to JsonString : %{public}s", dataString.c_str());
    if (!data.WriteString16(Str8ToStr16(dataString))) {
        APP_LOGE("%{public}s failed to WriteParcelable targetAbilityInfo", __func__);
        SendCallBack(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId);
        return;
    }
    sptr<ServiceCenterStatusCallback> callback = new(std::nothrow) ServiceCenterStatusCallback(weak_from_this());
    if (callback == nullptr) {
        APP_LOGE("callback is nullptr");
        return;
    }
    if (!data.WriteRemoteObject(serviceCenterCallback)) {
        APP_LOGE("%{public}s failed to WriteRemoteObject callbcak", __func__);
        SendCallBack(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId);
        return;
    }
    serviceCenterRemoteObject_ = serviceCenterConnection_->GetRemoteObject();
    if (serviceCenterRemoteObject_ == nullptr) {
        APP_LOGE("%{public}s failed to get remote object", __func__);
        SendCallBack(FreeInstallErrorCode::CONNECT_ERROR, want, userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::CONNECT_ERROR, want, userId);
        return;
    }
    auto emplaceResult = freeInstallParamsMap_.emplace(targetAbilityInfo.targetInfo.transactId, freeInstallParams);
    if (!emplaceResult.second) {
        APP_LOGE("freeInstallParamsMap emplace error");
        CallAbilityManager(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId, freeInstallParams.callback);
        return;
    }
    int32_t result = serviceCenterRemoteObject_->SendRequest(flag, data, reply, option);
    if (result != ERR_OK) {
        APP_LOGE("Failed to sendRequest, result = %{public}d", result);
        SendCallBack(FreeInstallErrorCode::CONNECT_ERROR, want, userId, targetAbilityInfo.targetInfo.transactId);
        SendSysEvent(FreeInstallErrorCode::CONNECT_ERROR, want, userId);
        return;
    }
    OutTimeMonitor(targetAbilityInfo.targetInfo.transactId);
}

bool BundleConnectAbilityMgr::SendRequest(int32_t code, MessageParcel &data, MessageParcel &reply)
{
    APP_LOGI("BundleConnectAbilityMgr::SendRequest to fa service manager");
    serviceCenterRemoteObject_ = serviceCenterConnection_->GetRemoteObject();
    if (serviceCenterRemoteObject_ == nullptr) {
        APP_LOGE("failed to get remote object");
        return false;
    }
    MessageOption option(MessageOption::TF_ASYNC);
    int32_t result = serviceCenterRemoteObject_->SendRequest(code, data, reply, option);
    if (result != ERR_OK) {
        APP_LOGE("failed to send request code:%{public}d", code);
        return false;
    }
    APP_LOGI("send request code:%{public}d success", code);
    return true;
}

sptr<IRemoteObject> BundleConnectAbilityMgr::GetAbilityManagerServiceCallBack(std::string transactId)
{
    APP_LOGI("GetAbilityManagerServiceCallBack");
    FreeInstallParams freeInstallParams;
    auto node = freeInstallParamsMap_.find(transactId);
    if (node == freeInstallParamsMap_.end()) {
        APP_LOGE("Can not find node in %{public}s function", __func__);
        return nullptr;
    }
    freeInstallParams = node->second;
    return freeInstallParams.callback;
}

void BundleConnectAbilityMgr::GetCallingInfo(int32_t userId,
    std::vector<std::string> &bundleNames, std::vector<std::string> &callingAppIds)
{
    APP_LOGI("enter");
    std::shared_ptr<BundleMgrService> bms = DelayedSingleton<BundleMgrService>::GetInstance();
    std::shared_ptr<BundleDataMgr> bundleDataMgr_ = bms->GetDataMgr();
    if (bundleDataMgr_ == nullptr) {
        APP_LOGE("GetDataMgr failed, bundleDataMgr_ is nullptr");
        return;
    }
    std::string bundleName;
    if (bundleDataMgr_->GetBundleNameForUid(IPCSkeleton::GetCallingUid(), bundleName)) {
        bundleNames.emplace_back(bundleName);
    } else {
        APP_LOGE("GetBundleNameForUid failed");
    }
    BundleInfo bundleInfo;
    if (bundleDataMgr_->GetBundleInfo(bundleName, GET_BUNDLE_DEFAULT, bundleInfo, userId)) {
        callingAppIds.emplace_back(bundleInfo.appId);
    } else {
        APP_LOGE("GetBundleInfo failed");
    }
}

bool ExistBundleNameInCallingBundles(const std::string &bundleName, const std::vector<std::string> &callingBundleNames)
{
    for (auto &bundleNameItem : callingBundleNames) {
        if (bundleNameItem == bundleName) {
            return true;
        }
    }
    return false;
}

int32_t GetTargetInfoFlag(const Want &want, const std::string &deviceId, const std::string &bundleName,
    const std::vector<std::string> &callingBundleNames)
{
    // make int from bits.
    int32_t flagZero = BIT_ZERO_COMPATIBLE;
    int32_t flagOne = 0;
    if ((want.GetFlags() & Want::FLAG_INSTALL_WITH_BACKGROUND_MODE) == 0) {
        flagOne = BIT_ONE_FRONT_MODE * BIT_ONE;
    } else {
        flagOne = BIT_ONE_BACKGROUND_MODE * BIT_ONE;
    }
    int32_t flagTwo = BIT_TWO_CUSTOM * BIT_TWO;
    int32_t flagThree = !deviceId.empty() * BIT_THREE;
    int32_t flagFour = BIT_FOUR_AZ_DEVICE * BIT_FOUR;
    int32_t flagFive = !ExistBundleNameInCallingBundles(bundleName, callingBundleNames) * BIT_FIVE;
    int32_t flagSix = BIT_SIX_SAME_BUNDLE * BIT_SIX;
    return flagZero + flagOne + flagTwo + flagThree + flagFour + flagFive + flagSix;
}

void BundleConnectAbilityMgr::GetTargetAbilityInfo(const Want &want, int32_t userId,
    const InnerBundleInfo &innerBundleInfo, sptr<TargetAbilityInfo> &targetAbilityInfo)
{
    ElementName element = want.GetElement();
    std::string bundleName = element.GetBundleName();
    std::string moduleName = element.GetModuleName();
    std::string abilityName = element.GetAbilityName();
    std::string deviceId = element.GetDeviceID();
    std::vector<std::string> callingBundleNames;
    std::vector<std::string> callingAppids;
    auto wantParams = want.GetParams();
    std::map<std::string, std::string> extValues;
    for (auto it : wantParams.GetParams()) {
        int typeId = WantParams::GetDataType(it.second);
        auto info = wantParams.GetParam(it.first);
        std::string value = wantParams.GetStringByType(info, typeId);
        extValues.emplace(it.first, value);
    }

    targetAbilityInfo->targetExtSetting.extValues = extValues;
    targetAbilityInfo->targetInfo.transactId = std::to_string(this->GetTransactId());
    targetAbilityInfo->targetInfo.bundleName = bundleName;
    targetAbilityInfo->targetInfo.moduleName = moduleName;
    targetAbilityInfo->targetInfo.abilityName = abilityName;
    targetAbilityInfo->targetInfo.callingUid = IPCSkeleton::GetCallingUid();
    targetAbilityInfo->targetInfo.callingAppType = CALLING_TYPE_HARMONY;
    this->GetCallingInfo(userId, callingBundleNames, callingAppids);
    targetAbilityInfo->targetInfo.callingBundleNames = callingBundleNames;
    targetAbilityInfo->targetInfo.flags = GetTargetInfoFlag(want, deviceId, bundleName, callingBundleNames);
    targetAbilityInfo->targetInfo.reasonFlag = static_cast<int32_t>(innerBundleInfo.GetModuleUpgradeFlag(moduleName));
    targetAbilityInfo->targetInfo.callingAppIds = callingAppids;
}

void BundleConnectAbilityMgr::CallAbilityManager(
    int32_t resultCode, const Want &want, int32_t userId, const sptr<IRemoteObject> &callBack)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option;
    if (!data.WriteInterfaceToken(ATOMIC_SERVICE_STATUS_CALLBACK_TOKEN)) {
        APP_LOGE("Write interface token failed");
        return;
    }
    if (!data.WriteInt32(resultCode)) {
        APP_LOGE("Write result code failed");
        return;
    }
    if (!data.WriteParcelable(&want)) {
        APP_LOGE("Write want failed");
        return;
    }
    if (!data.WriteInt32(userId)) {
        APP_LOGE("Write userId failed");
        return;
    }

    if (callBack->SendRequest(FREE_INSTALL_DONE, data, reply, option) != ERR_OK) {
        APP_LOGE("BundleConnectAbilityMgr::CallAbilityManager SendRequest failed");
    }
}

bool BundleConnectAbilityMgr::CheckIsModuleNeedUpdate(
    InnerBundleInfo &innerBundleInfo, const Want &want, int32_t userId, const sptr<IRemoteObject> &callBack)
{
    APP_LOGI("CheckIsModuleNeedUpdate called");
    if (innerBundleInfo.GetModuleUpgradeFlag(want.GetModuleName()) != 0) {
        sptr<TargetAbilityInfo> targetAbilityInfo = new(std::nothrow) TargetAbilityInfo();
        if (targetAbilityInfo == nullptr) {
            APP_LOGE("targetAbilityInfo is nullptr");
            return false;
        }
        sptr<TargetInfo> targetInfo = new(std::nothrow) TargetInfo();
        if (targetInfo == nullptr) {
            APP_LOGE("targetInfo is nullptr");
            return false;
        }
        sptr<TargetExtSetting> targetExtSetting = new(std::nothrow) TargetExtSetting();
        if (targetExtSetting == nullptr) {
            APP_LOGE("targetExtSetting is nullptr");
            return false;
        }
        targetAbilityInfo->targetInfo = *targetInfo;
        targetAbilityInfo->targetExtSetting = *targetExtSetting;
        targetAbilityInfo->version = DEFAULT_VERSION;
        this->GetTargetAbilityInfo(want, userId, innerBundleInfo, targetAbilityInfo);
        sptr<FreeInstallParams> freeInstallParams = new(std::nothrow) FreeInstallParams();
        if (freeInstallParams == nullptr) {
            APP_LOGE("freeInstallParams is nullptr");
            return false;
        }
        freeInstallParams->callback = callBack;
        freeInstallParams->want = want;
        freeInstallParams->userId = userId;
        freeInstallParams->serviceCenterFunction = ServiceCenterFunction::CONNECT_UPGRADE_INSTALL;
        this->UpgradeInstall(*targetAbilityInfo, want, *freeInstallParams, userId);
        return true;
    }
    APP_LOGI("Module is not need update");
    return false;
}

bool BundleConnectAbilityMgr::IsObtainAbilityInfo(const Want &want, int32_t flags, int32_t userId,
    AbilityInfo &abilityInfo, const sptr<IRemoteObject> &callBack, InnerBundleInfo &innerBundleInfo)
{
    APP_LOGI("IsObtainAbilityInfo");
    std::string bundleName = want.GetElement().GetBundleName();
    std::string abilityName = want.GetElement().GetAbilityName();
    if (bundleName == "" || abilityName == "") {
        CallAbilityManager(FreeInstallErrorCode::UNDEFINED_ERROR, want, userId, callBack);
        APP_LOGE("bundle name or ability name is null");
        return false;
    }
    std::shared_ptr<BundleMgrService> bms = DelayedSingleton<BundleMgrService>::GetInstance();
    std::shared_ptr<BundleDataMgr> bundleDataMgr_ = bms->GetDataMgr();
    if (bundleDataMgr_ == nullptr) {
        APP_LOGE("GetDataMgr failed, bundleDataMgr_ is nullptr");
        return false;
    }
    bool innerBundleInfoResult = bundleDataMgr_->GetInnerBundleInfoWithFlags(bundleName,
        flags, innerBundleInfo, userId);
    bool abilityInfoResult = bundleDataMgr_->QueryAbilityInfo(want, flags, userId, abilityInfo);
    if (!abilityInfoResult) {
        std::vector<ExtensionAbilityInfo> extensionInfos;
        abilityInfoResult = bundleDataMgr_->QueryExtensionAbilityInfos(want, flags, userId, extensionInfos);
    }
    if (innerBundleInfoResult && abilityInfoResult) {
        bool isModuleNeedUpdate = CheckIsModuleNeedUpdate(innerBundleInfo, want, userId, callBack);
        if (!isModuleNeedUpdate) {
            CallAbilityManager(ServiceCenterResultCode::FREE_INSTALL_OK, want, userId, callBack);
        }
        return true;
    }
    return false;
}

bool BundleConnectAbilityMgr::QueryAbilityInfo(const Want &want, int32_t flags,
    int32_t userId, AbilityInfo &abilityInfo, const sptr<IRemoteObject> &callBack)
{
    APP_LOGI("QueryAbilityInfo");
    InnerBundleInfo innerBundleInfo;
    if (IsObtainAbilityInfo(want, flags, userId, abilityInfo, callBack, innerBundleInfo)) {
        return true;
    }
    sptr<TargetAbilityInfo> targetAbilityInfo = new(std::nothrow) TargetAbilityInfo();
    if (targetAbilityInfo == nullptr) {
        APP_LOGE("targetAbilityInfo is nullptr");
        return false;
    }
    sptr<TargetInfo> targetInfo = new(std::nothrow) TargetInfo();
    if (targetInfo == nullptr) {
        APP_LOGE("targetInfo is nullptr");
        return false;
    }
    sptr<TargetExtSetting> targetExtSetting = new(std::nothrow) TargetExtSetting();
    if (targetExtSetting == nullptr) {
        APP_LOGE("targetExtSetting is nullptr");
        return false;
    }
    targetAbilityInfo->targetInfo = *targetInfo;
    targetAbilityInfo->targetExtSetting = *targetExtSetting;
    targetAbilityInfo->version = DEFAULT_VERSION;
    this->GetTargetAbilityInfo(want, userId, innerBundleInfo, targetAbilityInfo);
    sptr<FreeInstallParams> freeInstallParams = new(std::nothrow) FreeInstallParams();
    if (freeInstallParams == nullptr) {
        APP_LOGE("freeInstallParams is nullptr");
        return false;
    }
    freeInstallParams->callback = callBack;
    freeInstallParams->want = want;
    freeInstallParams->userId = userId;
    freeInstallParams->serviceCenterFunction = ServiceCenterFunction::CONNECT_SILENT_INSTALL;
    this->SilentInstall(*targetAbilityInfo, want, *freeInstallParams, userId);
    return false;
}

void BundleConnectAbilityMgr::UpgradeAtomicService(const Want &want, int32_t userId)
{
    APP_LOGI("UpgradeAtomicService");
    std::shared_ptr<BundleMgrService> bms = DelayedSingleton<BundleMgrService>::GetInstance();
    std::shared_ptr<BundleDataMgr> bundleDataMgr_ = bms->GetDataMgr();
    if (bundleDataMgr_ == nullptr) {
        APP_LOGE("GetDataMgr failed, bundleDataMgr_ is nullptr");
        return;
    }
    std::string bundleName = want.GetElement().GetBundleName();
    InnerBundleInfo innerBundleInfo;
    bundleDataMgr_->GetInnerBundleInfoWithFlags(bundleName, want.GetFlags(), innerBundleInfo, userId);
    if (!innerBundleInfo.GetEntryInstallationFree()) {
        APP_LOGI("bundleName:%{public}s is atomic application", bundleName.c_str());
        return;
    }
    APP_LOGI("bundleName:%{public}s is atomic service", bundleName.c_str());
    sptr<TargetAbilityInfo> targetAbilityInfo = new(std::nothrow) TargetAbilityInfo();
    if (targetAbilityInfo == nullptr) {
        APP_LOGE("targetAbilityInfo is nullptr");
        return;
    }
    sptr<TargetInfo> targetInfo = new(std::nothrow) TargetInfo();
    if (targetInfo == nullptr) {
        APP_LOGE("targetInfo is nullptr");
        return;
    }
    sptr<TargetExtSetting> targetExtSetting = new(std::nothrow) TargetExtSetting();
    if (targetExtSetting == nullptr) {
        APP_LOGE("targetExtSetting is nullptr");
        return;
    }
    targetAbilityInfo->targetInfo = *targetInfo;
    targetAbilityInfo->targetExtSetting = *targetExtSetting;
    targetAbilityInfo->version = DEFAULT_VERSION;
    this->GetTargetAbilityInfo(want, userId, innerBundleInfo, targetAbilityInfo);
    sptr<FreeInstallParams> freeInstallParams = new(std::nothrow) FreeInstallParams();
    if (freeInstallParams == nullptr) {
        APP_LOGE("freeInstallParams is nullptr");
        return;
    }
    freeInstallParams->want = want;
    freeInstallParams->userId = userId;
    freeInstallParams->serviceCenterFunction = ServiceCenterFunction::CONNECT_UPGRADE_CHECK;
    this->UpgradeCheck(*targetAbilityInfo, want, *freeInstallParams, userId);
}
}  // namespace AppExecFwk
}  // namespace OHOS
