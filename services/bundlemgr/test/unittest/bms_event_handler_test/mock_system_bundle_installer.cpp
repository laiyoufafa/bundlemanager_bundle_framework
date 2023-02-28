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
#include <cstring>
#include "system_bundle_installer.h"

#include "app_log_wrapper.h"
#include "bundle_mgr_service.h"

const std::string CALL_MOCK_BUNDLE_DIR_SUCCESS = "callMockBundleDirSuccess";
const std::string CALL_MOCK_BUNDLE_DIR_FAILED = "callMockBundleDirFailed";
const std::string RETURN_MOCK_BUNDLE_DIR_SUCCESS = "mockSuccess";
const std::string RETURN_MOCK_BUNDLE_DIR_FAILED = "mockFailed";
namespace OHOS {
namespace AppExecFwk {
SystemBundleInstaller::SystemBundleInstaller()
{
    APP_LOGI("system bundle installer instance is created");
}

SystemBundleInstaller::~SystemBundleInstaller()
{
    APP_LOGI("system bundle installer instance is destroyed");
}

bool SystemBundleInstaller::InstallSystemBundle(
    const std::string &filePath,
    InstallParam &installParam,
    Constants::AppType appType)
{
    if (filePath.compare(CALL_MOCK_BUNDLE_DIR_SUCCESS) == 0) {
        auto mockFilePath = const_cast<std::string*>(&filePath);
        *mockFilePath = RETURN_MOCK_BUNDLE_DIR_SUCCESS;
        return true;
    } else if (filePath.compare(CALL_MOCK_BUNDLE_DIR_FAILED) == 0) {
        auto mockFilePath = const_cast<std::string*>(&filePath);
        *mockFilePath = RETURN_MOCK_BUNDLE_DIR_FAILED;
        return false;
    }
    MarkPreBundleSyeEventBootTag(true);
    ErrCode result = InstallBundle(filePath, installParam, appType);
    if (result != ERR_OK) {
        APP_LOGE("install system bundle fail, error: %{public}d", result);
        return false;
    }
    return true;
}

bool SystemBundleInstaller::OTAInstallSystemBundle(
    const std::vector<std::string> &filePaths,
    InstallParam &installParam,
    Constants::AppType appType)
{
    auto dataMgr = DelayedSingleton<BundleMgrService>::GetInstance()->GetDataMgr();
    if (dataMgr == nullptr) {
        APP_LOGE("Get dataMgr shared_ptr nullptr");
        return false;
    }

    for (auto allUserId : dataMgr->GetAllUser()) {
        installParam.userId = allUserId;
        MarkPreBundleSyeEventBootTag(false);
        ErrCode result = InstallBundle(filePaths, installParam, appType);
        if (result != ERR_OK) {
            APP_LOGW("install system bundle fail, error: %{public}d", result);
        }

        ResetInstallProperties();
    }

    return true;
}

bool SystemBundleInstaller::UninstallSystemBundle(const std::string &bundleName)
{
    auto dataMgr = DelayedSingleton<BundleMgrService>::GetInstance()->GetDataMgr();
    if (dataMgr == nullptr) {
        APP_LOGE("Get dataMgr shared_ptr nullptr");
        return false;
    }

    InstallParam installParam;
    for (auto userId : dataMgr->GetAllUser()) {
        installParam.userId = userId;
        installParam.needSavePreInstallInfo = true;
        installParam.isPreInstallApp = true;
        installParam.noSkipsKill = false;
        installParam.needSendEvent = false;
        MarkPreBundleSyeEventBootTag(false);
        ErrCode result = UninstallBundle(bundleName, installParam);
        if (result != ERR_OK) {
            APP_LOGW("uninstall system bundle fail, error: %{public}d", result);
        }

        ResetInstallProperties();
    }
    return true;
}

bool SystemBundleInstaller::UninstallSystemBundle(const std::string &bundleName, bool isKeepData)
{
    auto dataMgr = DelayedSingleton<BundleMgrService>::GetInstance()->GetDataMgr();
    if (dataMgr == nullptr) {
        APP_LOGE("Get dataMgr shared_ptr nullptr!");
        return false;
    }

    InstallParam installParam;
    for (auto userId : dataMgr->GetAllUser()) {
        installParam.userId = userId;
        installParam.needSavePreInstallInfo = true;
        installParam.isPreInstallApp = true;
        installParam.noSkipsKill = false;
        installParam.needSendEvent = false;
        installParam.isKeepData = isKeepData;
        MarkPreBundleSyeEventBootTag(false);
        ErrCode result = UninstallBundle(bundleName, installParam);
        if (result != ERR_OK) {
            APP_LOGW("uninstall system bundle fail, error: %{public}d", result);
        }

        ResetInstallProperties();
    }
    return true;
}

bool SystemBundleInstaller::UninstallSystemBundle(const std::string &bundleName, const std::string &modulePackage)
{
    auto dataMgr = DelayedSingleton<BundleMgrService>::GetInstance()->GetDataMgr();
    if (dataMgr == nullptr) {
        APP_LOGE("Get dataMgr shared_ptr nullptr");
        return false;
    }

    InstallParam installParam;
    for (auto userId : dataMgr->GetAllUser()) {
        installParam.userId = userId;
        installParam.needSavePreInstallInfo = true;
        installParam.isPreInstallApp = true;
        installParam.noSkipsKill = false;
        installParam.needSendEvent = false;
        MarkPreBundleSyeEventBootTag(false);
        ErrCode result = UninstallBundle(bundleName, modulePackage, installParam);
        if (result != ERR_OK) {
            APP_LOGW("uninstall system bundle fail, error: %{public}d", result);
        }

        ResetInstallProperties();
    }

    return true;
}
}  // namespace AppExecFwk
}  // namespace OHOS