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

#include "installd_client.h"

#include "app_log_wrapper.h"
#include "bundle_constants.h"
#include "installd_death_recipient.h"
#include "system_ability_definition.h"
#include "system_ability_helper.h"

namespace OHOS {
namespace AppExecFwk {
ErrCode InstalldClient::CreateBundleDir(const std::string &bundleDir)
{
    if (bundleDir.empty()) {
        APP_LOGE("bundle dir is empty");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::CreateBundleDir, bundleDir);
}

ErrCode InstalldClient::ExtractModuleFiles(const std::string &srcModulePath, const std::string &targetPath)
{
    if (srcModulePath.empty() || targetPath.empty()) {
        APP_LOGE("src module path or target path is empty");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::ExtractModuleFiles, srcModulePath, targetPath);
}

ErrCode InstalldClient::RenameModuleDir(const std::string &oldPath, const std::string &newPath)
{
    if (oldPath.empty() || newPath.empty()) {
        APP_LOGE("rename path is empty");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::RenameModuleDir, oldPath, newPath);
}

ErrCode InstalldClient::CreateBundleDataDir(const std::string &bundleDir,
    const int userid, const int uid, const int gid, const std::string &apl, bool onlyOneUser)
{
    if (bundleDir.empty() || userid < 0 || uid < 0 || gid < 0) {
        APP_LOGE("params are invalid");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::CreateBundleDataDir, bundleDir, userid, uid, gid, apl, onlyOneUser);
}

ErrCode InstalldClient::RemoveBundleDataDir(
    const std::string &bundleName, const int userid)
{
    if (bundleName.empty() || userid < 0) {
        APP_LOGE("params are invalid");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::RemoveBundleDataDir, bundleName, userid);
}

ErrCode InstalldClient::CreateModuleDataDir(
    const std::string &ModuleDir, const std::vector<std::string> &abilityDirs, const int uid, const int gid)
{
    if (ModuleDir.empty() || uid < 0 || gid < 0) {
        APP_LOGE("params are invalid");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::CreateModuleDataDir, ModuleDir, abilityDirs, uid, gid);
}

ErrCode InstalldClient::RemoveModuleDataDir(const std::string &ModuleName, const int userid)
{
    if (ModuleName.empty() || userid < 0) {
        APP_LOGE("params are invalid");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::RemoveModuleDataDir, ModuleName, userid);
}

ErrCode InstalldClient::RemoveDir(const std::string &dir)
{
    if (dir.empty()) {
        APP_LOGE("dir removed is empty");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::RemoveDir, dir);
}

ErrCode InstalldClient::CleanBundleDataDir(const std::string &bundleDir)
{
    if (bundleDir.empty()) {
        APP_LOGE("bundle dir is empty");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::CleanBundleDataDir, bundleDir);
}

ErrCode InstalldClient::GetBundleStats(
    const std::string &bundleName, const int32_t userId, std::vector<int64_t> &bundleStats)
{
    if (bundleName.empty()) {
        APP_LOGE("bundleName is empty");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::GetBundleStats, bundleName, userId, bundleStats);
}

ErrCode InstalldClient::SetDirApl(const std::string &dir, const std::string &bundleName, const std::string &apl)
{
    if (dir.empty() || bundleName.empty() || apl.empty()) {
        APP_LOGE("params are invalid");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::SetDirApl, dir, bundleName, apl);
}

ErrCode InstalldClient::CopyNativeSo(const std::string &srcLibPath, const std::string &targetLibPath)
{
    if (srcLibPath.empty() || targetLibPath.empty()) {
        APP_LOGE("param invalid");
        return ERR_APPEXECFWK_INSTALLD_PARAM_ERROR;
    }

    return CallService(&IInstalld::CopyNativeSo, srcLibPath, targetLibPath);
}

void InstalldClient::ResetInstalldProxy()
{
    if ((installdProxy_ != nullptr) && (installdProxy_->AsObject() != nullptr)) {
        installdProxy_->AsObject()->RemoveDeathRecipient(recipient_);
    }
    installdProxy_ = nullptr;
}

bool InstalldClient::GetInstalldProxy()
{
    if (!installdProxy_) {
        APP_LOGD("try to get installd proxy");
        std::lock_guard<std::mutex> lock(mutex_);
        if (!installdProxy_) {
            sptr<IInstalld> tempProxy =
                iface_cast<IInstalld>(SystemAbilityHelper::GetSystemAbility(INSTALLD_SERVICE_ID));
            if ((!tempProxy) || (!tempProxy->AsObject())) {
                APP_LOGE("the installd proxy or remote object is null");
                return false;
            }
            recipient_ = new (std::nothrow) InstalldDeathRecipient();
            tempProxy->AsObject()->AddDeathRecipient(recipient_);
            installdProxy_ = tempProxy;
        }
    }
    return true;
}
}  // namespace AppExecFwk
}  // namespace OHOS
