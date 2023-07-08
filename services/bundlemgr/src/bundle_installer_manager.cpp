/*
 * Copyright (c) 2021-2023 Huawei Device Co., Ltd.
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

#include "bundle_installer_manager.h"

#include <cinttypes>

#include "appexecfwk_errors.h"
#include "app_log_wrapper.h"
#include "bundle_memory_guard.h"
#include "bundle_mgr_service.h"
#include "datetime_ex.h"
#include "ffrt.h"
#include "ipc_skeleton.h"

namespace OHOS {
namespace AppExecFwk {
BundleInstallerManager::BundleInstallerManager()
{
    APP_LOGI("create bundle installer manager instance");
}

BundleInstallerManager::~BundleInstallerManager()
{
    APP_LOGI("destroy bundle installer manager instance");
}

void BundleInstallerManager::CreateInstallTask(
    const std::string &bundleFilePath, const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    auto installer = CreateInstaller(statusReceiver);
    if (installer == nullptr) {
        APP_LOGE("create installer failed");
        return;
    }
    auto task = [installer, bundleFilePath, installParam] {
        BundleMemoryGuard memoryGuard;
        installer->Install(bundleFilePath, installParam);
    };
    AddTask(task);
}

void BundleInstallerManager::CreateRecoverTask(
    const std::string &bundleName, const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    auto installer = CreateInstaller(statusReceiver);
    if (installer == nullptr) {
        APP_LOGE("create installer failed");
        return;
    }
    auto task = [installer, bundleName, installParam] {
        BundleMemoryGuard memoryGuard;
        installer->Recover(bundleName, installParam);
    };
    AddTask(task);
}

void BundleInstallerManager::CreateInstallTask(const std::vector<std::string> &bundleFilePaths,
    const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    auto installer = CreateInstaller(statusReceiver);
    if (installer == nullptr) {
        APP_LOGE("create installer failed");
        return;
    }
    auto task = [installer, bundleFilePaths, installParam] {
        BundleMemoryGuard memoryGuard;
        installer->Install(bundleFilePaths, installParam);
    };
    AddTask(task);
}

void BundleInstallerManager::CreateInstallByBundleNameTask(const std::string &bundleName,
    const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    auto installer = CreateInstaller(statusReceiver);
    if (installer == nullptr) {
        APP_LOGE("create installer failed");
        return;
    }

    auto task = [installer, bundleName, installParam] {
        BundleMemoryGuard memoryGuard;
        installer->InstallByBundleName(bundleName, installParam);
    };
    AddTask(task);
}

void BundleInstallerManager::CreateUninstallTask(
    const std::string &bundleName, const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    auto installer = CreateInstaller(statusReceiver);
    if (installer == nullptr) {
        APP_LOGE("create installer failed");
        return;
    }
    auto task = [installer, bundleName, installParam] {
        BundleMemoryGuard memoryGuard;
        installer->Uninstall(bundleName, installParam);
    };
    AddTask(task);
}

void BundleInstallerManager::CreateUninstallTask(const std::string &bundleName, const std::string &modulePackage,
    const InstallParam &installParam, const sptr<IStatusReceiver> &statusReceiver)
{
    auto installer = CreateInstaller(statusReceiver);
    if (installer == nullptr) {
        APP_LOGE("create installer failed");
        return;
    }
    auto task = [installer, bundleName, modulePackage, installParam] {
        BundleMemoryGuard memoryGuard;
        installer->Uninstall(bundleName, modulePackage, installParam);
    };
    AddTask(task);
}

void BundleInstallerManager::CreateUninstallTask(const UninstallParam &uninstallParam,
    const sptr<IStatusReceiver> &statusReceive)
{
    auto installer = CreateInstaller(statusReceive);
    if (installer == nullptr) {
        APP_LOGE("create installer failed");
        return;
    }
    auto task = [installer, uninstallParam] {
        BundleMemoryGuard memoryGuard;
        installer->Uninstall(uninstallParam);
    };
    AddTask(task);
}

std::shared_ptr<BundleInstaller> BundleInstallerManager::CreateInstaller(const sptr<IStatusReceiver> &statusReceiver)
{
    int64_t installerId = GetMicroTickCount();
    auto installer = std::make_shared<BundleInstaller>(installerId, statusReceiver);
    installer->SetCallingUid(IPCSkeleton::GetCallingUid());
    return installer;
}

void BundleInstallerManager::AddTask(const ThreadPoolTask &task)
{
    APP_LOGD("submit task");
    ffrt::submit(task, {}, {}, ffrt::task_attr().qos(ffrt::qos::qos_user_initiated));
}
}  // namespace AppExecFwk
}  // namespace OHOS