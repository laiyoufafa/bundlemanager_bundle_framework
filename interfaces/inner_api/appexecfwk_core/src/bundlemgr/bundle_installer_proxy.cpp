/*
 * Copyright (c) 2021-2022 Huawei Device Co., Ltd.
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

#include "bundle_installer_proxy.h"

#include <fcntl.h>
#include <unistd.h>

#include "app_log_wrapper.h"
#include "appexecfwk_errors.h"
#include "bundle_file_util.h"
#include "directory_ex.h"
#include "hitrace_meter.h"
#include "ipc_types.h"
#include "parcel.h"
#include "string_ex.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
const std::string SEPARATOR = "/";
const int32_t DEFAULT_BUFFER_SIZE = 65536;
} // namespace

BundleInstallerProxy::BundleInstallerProxy(const sptr<IRemoteObject> &object) : IRemoteProxy<IBundleInstaller>(object)
{
    APP_LOGI("create bundle installer proxy instance");
}

BundleInstallerProxy::~BundleInstallerProxy()
{
    APP_LOGI("destroy bundle installer proxy instance");
}

bool BundleInstallerProxy::Install(
    const std::string &bundlePath, const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    HITRACE_METER_NAME(HITRACE_TAG_APP, __PRETTY_FUNCTION__);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    PARCEL_WRITE_INTERFACE_TOKEN(data, GetDescriptor());
    PARCEL_WRITE(data, String16, Str8ToStr16(bundlePath));
    PARCEL_WRITE(data, Parcelable, &installParam);

    if (statusReceiver == nullptr) {
        APP_LOGE("fail to install, for statusReceiver is nullptr");
        return false;
    }
    if (!data.WriteObject<IRemoteObject>(statusReceiver->AsObject())) {
        APP_LOGE("write parcel failed");
        return false;
    }

    return SendInstallRequest(BundleInstallerInterfaceCode::INSTALL, data, reply, option);
}

bool BundleInstallerProxy::Install(const std::vector<std::string> &bundleFilePaths, const InstallParam &installParam,
    const sptr<IStatusReceiver> &statusReceiver)
{
    HITRACE_METER_NAME(HITRACE_TAG_APP, __PRETTY_FUNCTION__);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    PARCEL_WRITE_INTERFACE_TOKEN(data, GetDescriptor());
    auto size = bundleFilePaths.size();
    PARCEL_WRITE(data, Int32, size);
    for (uint32_t i = 0; i < size; ++i) {
        PARCEL_WRITE(data, String16, Str8ToStr16(bundleFilePaths[i]));
    }
    PARCEL_WRITE(data, Parcelable, &installParam);

    if (statusReceiver == nullptr) {
        APP_LOGE("fail to install, for statusReceiver is nullptr");
        return false;
    }
    if (!data.WriteObject<IRemoteObject>(statusReceiver->AsObject())) {
        APP_LOGE("write parcel failed");
        return false;
    }

    return SendInstallRequest(BundleInstallerInterfaceCode::INSTALL_MULTIPLE_HAPS, data, reply,
        option);
}

bool BundleInstallerProxy::Recover(const std::string &bundleName,
    const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    PARCEL_WRITE_INTERFACE_TOKEN(data, GetDescriptor());
    PARCEL_WRITE(data, String16, Str8ToStr16(bundleName));
    PARCEL_WRITE(data, Parcelable, &installParam);

    if (statusReceiver == nullptr) {
        APP_LOGE("fail to install, for statusReceiver is nullptr");
        return false;
    }
    if (!data.WriteObject<IRemoteObject>(statusReceiver->AsObject())) {
        APP_LOGE("write parcel failed");
        return false;
    }

    return SendInstallRequest(BundleInstallerInterfaceCode::RECOVER, data, reply,
        option);
}

bool BundleInstallerProxy::Uninstall(
    const std::string &bundleName, const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    HITRACE_METER_NAME(HITRACE_TAG_APP, __PRETTY_FUNCTION__);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    PARCEL_WRITE_INTERFACE_TOKEN(data, GetDescriptor());
    PARCEL_WRITE(data, String16, Str8ToStr16(bundleName));
    PARCEL_WRITE(data, Parcelable, &installParam);
    if (statusReceiver == nullptr) {
        APP_LOGE("fail to uninstall, for statusReceiver is nullptr");
        return false;
    }
    if (!data.WriteObject<IRemoteObject>(statusReceiver->AsObject())) {
        APP_LOGE("write parcel failed");
        return false;
    }

    return SendInstallRequest(BundleInstallerInterfaceCode::UNINSTALL, data, reply, option);
}

bool BundleInstallerProxy::Uninstall(const std::string &bundleName, const std::string &modulePackage,
    const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    HITRACE_METER_NAME(HITRACE_TAG_APP, __PRETTY_FUNCTION__);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    PARCEL_WRITE_INTERFACE_TOKEN(data, GetDescriptor());
    PARCEL_WRITE(data, String16, Str8ToStr16(bundleName));
    PARCEL_WRITE(data, String16, Str8ToStr16(modulePackage));
    PARCEL_WRITE(data, Parcelable, &installParam);
    if (statusReceiver == nullptr) {
        APP_LOGE("fail to uninstall, for statusReceiver is nullptr");
        return false;
    }
    if (!data.WriteObject<IRemoteObject>(statusReceiver->AsObject())) {
        APP_LOGE("write parcel failed");
        return false;
    }

    return SendInstallRequest(BundleInstallerInterfaceCode::UNINSTALL_MODULE, data, reply, option);
}

bool BundleInstallerProxy::Uninstall(const UninstallParam &uninstallParam,
    const sptr<IStatusReceiver> &statusReceiver)
{
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);
    PARCEL_WRITE_INTERFACE_TOKEN(data, GetDescriptor());
    PARCEL_WRITE(data, Parcelable, &uninstallParam);
    if (statusReceiver == nullptr) {
        APP_LOGE("fail to uninstall, for statusReceiver is nullptr");
        return false;
    }
    if (!data.WriteObject<IRemoteObject>(statusReceiver->AsObject())) {
        APP_LOGE("write parcel failed");
        return false;
    }
    return SendInstallRequest(BundleInstallerInterfaceCode::UNINSTALL_BY_UNINSTALL_PARAM, data, reply, option);
}

ErrCode BundleInstallerProxy::InstallSandboxApp(const std::string &bundleName, int32_t dlpType, int32_t userId,
    int32_t &appIndex)
{
    HITRACE_METER_NAME(HITRACE_TAG_APP, __PRETTY_FUNCTION__);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        APP_LOGE("failed to InstallSandboxApp due to write MessageParcel fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR;
    }
    if (!data.WriteString16(Str8ToStr16(bundleName))) {
        APP_LOGE("failed to InstallSandboxApp due to write bundleName fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR;
    }
    if (!data.WriteInt32(dlpType)) {
        APP_LOGE("failed to InstallSandboxApp due to write appIndex fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR;
    }
    if (!data.WriteInt32(userId)) {
        APP_LOGE("failed to InstallSandboxApp due to write userId fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR;
    }

    auto ret =
        SendInstallRequest(BundleInstallerInterfaceCode::INSTALL_SANDBOX_APP, data, reply, option);
    if (!ret) {
        APP_LOGE("install sandbox app failed due to send request fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_SEND_REQUEST_ERROR;
    }

    auto res = reply.ReadInt32();
    if (res == ERR_OK) {
        appIndex = reply.ReadInt32();
    }
    return res;
}

ErrCode BundleInstallerProxy::UninstallSandboxApp(const std::string &bundleName, int32_t appIndex, int32_t userId)
{
    HITRACE_METER_NAME(HITRACE_TAG_APP, __PRETTY_FUNCTION__);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (!data.WriteInterfaceToken(GetDescriptor())) {
        APP_LOGE("failed to InstallSandboxApp due to write MessageParcel fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR;
    }
    if (!data.WriteString16(Str8ToStr16(bundleName))) {
        APP_LOGE("failed to InstallSandboxApp due to write bundleName fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR;
    }
    if (!data.WriteInt32(appIndex)) {
        APP_LOGE("failed to InstallSandboxApp due to write appIndex fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR;
    }
    if (!data.WriteInt32(userId)) {
        APP_LOGE("failed to InstallSandboxApp due to write userId fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR;
    }

    auto ret =
        SendInstallRequest(BundleInstallerInterfaceCode::UNINSTALL_SANDBOX_APP, data, reply, option);
    if (!ret) {
        APP_LOGE("uninstall sandbox app failed due to send request fail");
        return ERR_APPEXECFWK_SANDBOX_INSTALL_SEND_REQUEST_ERROR;
    }
    return reply.ReadInt32();
}

sptr<IBundleStreamInstaller> BundleInstallerProxy::CreateStreamInstaller(const InstallParam &installParam,
    const sptr<IStatusReceiver> &statusReceiver)
{
    APP_LOGD("create stream installer begin");
    HITRACE_METER_NAME(HITRACE_TAG_APP, __PRETTY_FUNCTION__);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    if (statusReceiver == nullptr) {
        APP_LOGE("fail to install, for receiver is nullptr");
        return nullptr;
    }

    bool ret = data.WriteInterfaceToken(GetDescriptor());
    if (!ret) {
        APP_LOGE("fail to write interface token into the parcel!");
        statusReceiver->OnFinished(ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR, "");
        return nullptr;
    }
    ret = data.WriteParcelable(&installParam);
    if (!ret) {
        APP_LOGE("fail to write parameter into the parcel!");
        statusReceiver->OnFinished(ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR, "");
        return nullptr;
    }
    if (!data.WriteObject<IRemoteObject>(statusReceiver->AsObject())) {
        APP_LOGE("write parcel failed");
        statusReceiver->OnFinished(ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR, "");
        return nullptr;
    }
    bool res = SendInstallRequest(BundleInstallerInterfaceCode::CREATE_STREAM_INSTALLER, data, reply, option);
    if (!res) {
        APP_LOGE("CreateStreamInstaller failed due to send request fail");
        statusReceiver->OnFinished(ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR, "");
        return nullptr;
    }
    if (!reply.ReadBool()) {
        APP_LOGE("CreateStreamInstaller failed");
        return nullptr;
    }
    uint32_t streamInstallerId = reply.ReadUint32();
    sptr<IRemoteObject> object = reply.ReadRemoteObject();
    if (object == nullptr) {
        APP_LOGE("CreateStreamInstaller create nullptr remote object");
        return nullptr;
    }
    sptr<IBundleStreamInstaller> streamInstaller = iface_cast<IBundleStreamInstaller>(object);
    if (streamInstaller == nullptr) {
        APP_LOGE("CreateStreamInstaller failed");
        return streamInstaller;
    }
    streamInstaller->SetInstallerId(streamInstallerId);
    APP_LOGD("create stream installer successfully");
    return streamInstaller;
}

bool BundleInstallerProxy::DestoryBundleStreamInstaller(uint32_t streamInstallerId)
{
    APP_LOGD("destory stream installer begin");
    HITRACE_METER_NAME(HITRACE_TAG_APP, __PRETTY_FUNCTION__);
    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);

    PARCEL_WRITE_INTERFACE_TOKEN(data, GetDescriptor());
    PARCEL_WRITE(data, Uint32, streamInstallerId);
    bool res = SendInstallRequest(BundleInstallerInterfaceCode::DESTORY_STREAM_INSTALLER, data, reply, option);
    if (!res) {
        APP_LOGE("CreateStreamInstaller failed due to send request fail");
        return false;
    }
    APP_LOGD("destory stream installer successfully");
    return true;
}

ErrCode BundleInstallerProxy::StreamInstall(const std::vector<std::string> &bundleFilePaths,
    const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    APP_LOGD("stream install start");
    if (statusReceiver == nullptr) {
        APP_LOGE("stream install failed due to nullptr status receiver");
        return ERR_APPEXECFWK_INSTALL_PARAM_ERROR;
    }

    std::vector<std::string> realPaths;
    if (!bundleFilePaths.empty() && !BundleFileUtil::CheckFilePath(bundleFilePaths, realPaths)) {
        APP_LOGE("stream install failed due to check file failed");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }

    sptr<IBundleStreamInstaller> streamInstaller = CreateStreamInstaller(installParam, statusReceiver);
    if (streamInstaller == nullptr) {
        APP_LOGE("stream install failed due to nullptr stream installer");
        return ERR_OK;
    }
    for (const auto &path : realPaths) {
        ErrCode res = WriteFileToStream(streamInstaller, path);
        if (res != ERR_OK) {
            DestoryBundleStreamInstaller(streamInstaller->GetInstallerId());
            APP_LOGE("WriteFileToStream failed due to %{public}d", res);
            return res;
        }
    }

    // write shared bundles
    for (size_t i = 0; i < installParam.sharedBundleDirPaths.size(); ++i) {
        realPaths.clear();
        std::vector<std::string> sharedBundleDir = {installParam.sharedBundleDirPaths[i]};
        if (!BundleFileUtil::CheckFilePath(sharedBundleDir, realPaths)) {
            APP_LOGE("stream install failed due to check shared package files failed");
            return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
        }
        for (const auto &path : realPaths) {
            ErrCode res = WriteSharedFileToStream(streamInstaller, path, i);
            if (res != ERR_OK) {
                DestoryBundleStreamInstaller(streamInstaller->GetInstallerId());
                APP_LOGE("WriteSharedFileToStream(sharedBundleDirPaths) failed due to %{public}d", res);
                return res;
            }
        }
    }

    // start install haps
    if (!streamInstaller->Install()) {
        APP_LOGE("stream install failed");
        DestoryBundleStreamInstaller(streamInstaller->GetInstallerId());
        statusReceiver->OnFinished(ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR, "");
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    APP_LOGD("stream install end");
    return ERR_OK;
}

ErrCode BundleInstallerProxy::WriteFileToStream(sptr<IBundleStreamInstaller> &streamInstaller, const std::string &path)
{
    APP_LOGD("write file stream to service terminal start");
    if (streamInstaller == nullptr) {
        APP_LOGE("write file to stream failed due to nullptr stream installer");
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }

    // find hap file name
    size_t pos = path.find_last_of(SEPARATOR);
    if (pos == std::string::npos) {
        APP_LOGE("write file to stream failed due to invalid file path");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }
    std::string hapName = path.substr(pos + 1);
    if (hapName.empty()) {
        APP_LOGE("write file to stream failed due to invalid file path");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }

    APP_LOGD("write file stream of bundle path %{public}s and hap name %{public}s", path.c_str(), hapName.c_str());
    int32_t outputFd = streamInstaller->CreateStream(hapName);
    if (outputFd < 0) {
        APP_LOGE("write file to stream failed due to invalid file descriptor");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }

    int32_t inputFd = open(path.c_str(), O_RDONLY);
    if (inputFd < 0) {
        close(outputFd);
        APP_LOGE("write file to stream failed due to open the hap file");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }
    char buffer[DEFAULT_BUFFER_SIZE] = {0};
    int offset = -1;
    while ((offset = read(inputFd, buffer, sizeof(buffer))) > 0) {
        if (write(outputFd, buffer, offset) < 0) {
            close(inputFd);
            close(outputFd);
            APP_LOGE("write file to the temp dir failed");
            return ERR_APPEXECFWK_INSTALL_DISK_MEM_INSUFFICIENT;
        }
    }

    close(inputFd);
    fsync(outputFd);
    close(outputFd);

    APP_LOGD("write file stream to service terminal end");
    return ERR_OK;
}

ErrCode BundleInstallerProxy::WriteSharedFileToStream(sptr<IBundleStreamInstaller> &streamInstaller,
    const std::string &path, uint32_t index)
{
    APP_LOGD("write shared file stream to service terminal start");
    if (streamInstaller == nullptr) {
        APP_LOGE("write file to stream failed due to nullptr stream installer");
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }

    // find hap file name
    size_t pos = path.find_last_of(SEPARATOR);
    if (pos == std::string::npos) {
        APP_LOGE("write file to stream failed due to invalid file path");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }
    std::string hspName = path.substr(pos + 1);
    if (hspName.empty()) {
        APP_LOGE("write file to stream failed due to invalid file path");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }

    APP_LOGD("write file stream of bundle path %{public}s and hsp name %{public}s", path.c_str(), hspName.c_str());
    int32_t outputFd = streamInstaller->CreateSharedBundleStream(hspName, index);
    if (outputFd < 0) {
        APP_LOGE("write file to stream failed due to invalid file descriptor");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }

    int32_t inputFd = open(path.c_str(), O_RDONLY);
    if (inputFd < 0) {
        close(outputFd);
        APP_LOGE("write file to stream failed due to open the hap file");
        return ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID;
    }
    char buffer[DEFAULT_BUFFER_SIZE] = {0};
    int offset = -1;
    while ((offset = read(inputFd, buffer, sizeof(buffer))) > 0) {
        if (write(outputFd, buffer, offset) < 0) {
            close(inputFd);
            close(outputFd);
            APP_LOGE("write file to the temp dir failed");
            return ERR_APPEXECFWK_INSTALL_DISK_MEM_INSUFFICIENT;
        }
    }

    close(inputFd);
    fsync(outputFd);
    close(outputFd);

    APP_LOGD("write file stream to service terminal end");
    return ERR_OK;
}

bool BundleInstallerProxy::SendInstallRequest(
    BundleInstallerInterfaceCode code, MessageParcel& data, MessageParcel& reply, MessageOption& option)
{
    sptr<IRemoteObject> remote = Remote();
    if (remote == nullptr) {
        APP_LOGE("fail to uninstall, for Remote() is nullptr");
        return false;
    }

    int32_t ret = remote->SendRequest(static_cast<uint32_t>(code), data, reply, option);
    if (ret != NO_ERROR) {
        APP_LOGE("fail to sendRequest, for transact is failed and error code is: %{public}d", ret);
        return false;
    }
    return true;
}
}  // namespace AppExecFwk
}  // namespace OHOS
