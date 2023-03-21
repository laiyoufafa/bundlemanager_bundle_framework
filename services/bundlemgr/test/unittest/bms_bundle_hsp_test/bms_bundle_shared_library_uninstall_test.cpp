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

#include <fstream>
#include <gtest/gtest.h>
#include <sstream>
#include <string>

#include "bundle_info.h"
#include "bundle_installer_host.h"
#include "bundle_mgr_service.h"
#include "bundle_permission_mgr.h"
#include "bundle_verify_mgr.h"
#include "inner_bundle_info.h"
#include "installd/installd_service.h"
#include "installd_client.h"
#include "mock_status_receiver.h"

using namespace testing::ext;
using namespace std::chrono_literals;
using namespace OHOS;
using namespace OHOS::AppExecFwk;
using namespace OHOS::Security;

namespace OHOS {
namespace {

const std::string MODULE_FILE_PATH = "/data/test/resource/bms/sharelibrary/";
const std::string HOST_HAP = "host_hap.hap";
const std::string LIBA_V10001 = "libA_v10001.hsp";
const std::string LIBA_V10002 = "libA_v10002.hsp";
const std::string BUNDLE_NAME = "com.example.host";
const std::string SHARED_BUNDLE_NAME = "com.example.liba";
const int32_t USERID = 100;
const int32_t WAIT_TIME = 5; // init mocked bms

class BmsBundleSharedLibraryUninstallTest : public testing::Test {
public:
    BmsBundleSharedLibraryUninstallTest();
    ~BmsBundleSharedLibraryUninstallTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();
    void StartInstalldService() const;
    void StartBundleService();
    ErrCode InstallBundle(const std::vector<std::string> &bundleFilePaths, const InstallParam &installParam) const;
    ErrCode UnInstallBundle(const std::string &bundleName) const;
    ErrCode UninstallShred(const UninstallParam &uninstallParam) const;
private:
    std::shared_ptr<InstalldService> installdService_;
    std::shared_ptr<BundleMgrService> bundleMgrService_;
};

BmsBundleSharedLibraryUninstallTest::BmsBundleSharedLibraryUninstallTest()
{
    installdService_ = std::make_shared<InstalldService>();
    bundleMgrService_ = DelayedSingleton<BundleMgrService>::GetInstance();
}

BmsBundleSharedLibraryUninstallTest::~BmsBundleSharedLibraryUninstallTest()
{}

void BmsBundleSharedLibraryUninstallTest::SetUpTestCase()
{}

void BmsBundleSharedLibraryUninstallTest::TearDownTestCase()
{}

void BmsBundleSharedLibraryUninstallTest::SetUp()
{
    StartBundleService();
    StartInstalldService();
}

void BmsBundleSharedLibraryUninstallTest::TearDown()
{}

void BmsBundleSharedLibraryUninstallTest::StartInstalldService() const
{
    if (!installdService_->IsServiceReady()) {
        installdService_->Start();
    }
}

void BmsBundleSharedLibraryUninstallTest::StartBundleService()
{
    if (!bundleMgrService_->IsServiceReady()) {
        bundleMgrService_->OnStart();
        std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME));
    }
}

ErrCode BmsBundleSharedLibraryUninstallTest::InstallBundle(
    const std::vector<std::string> &bundleFilePaths, const InstallParam &installParam) const
{
    if (!bundleMgrService_) {
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    auto installer = bundleMgrService_->GetBundleInstaller();
    if (!installer) {
        EXPECT_FALSE(true) << "the installer is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    sptr<MockStatusReceiver> receiver = new (std::nothrow) MockStatusReceiver();
    if (!receiver) {
        EXPECT_FALSE(true) << "the receiver is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    bool result = installer->Install(bundleFilePaths, installParam, receiver);
    EXPECT_TRUE(result);
    return receiver->GetResultCode();
}

ErrCode BmsBundleSharedLibraryUninstallTest::UnInstallBundle(const std::string &bundleName) const
{
    if (!bundleMgrService_) {
        return ERR_APPEXECFWK_UNINSTALL_BUNDLE_MGR_SERVICE_ERROR;
    }
    auto installer = bundleMgrService_->GetBundleInstaller();
    if (!installer) {
        EXPECT_FALSE(true) << "the installer is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    sptr<MockStatusReceiver> receiver = new (std::nothrow) MockStatusReceiver();
    if (!receiver) {
        EXPECT_FALSE(true) << "the receiver is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    InstallParam installParam;
    installParam.installFlag = InstallFlag::NORMAL;
    installParam.userId = USERID;
    bool result = installer->Uninstall(bundleName, installParam, receiver);
    EXPECT_TRUE(result);
    return receiver->GetResultCode();
}

ErrCode BmsBundleSharedLibraryUninstallTest::UninstallShred(const UninstallParam &uninstallParam) const
{
    if (!bundleMgrService_) {
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    auto installer = bundleMgrService_->GetBundleInstaller();
    if (!installer) {
        EXPECT_FALSE(true) << "the installer is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    sptr<MockStatusReceiver> receiver = new (std::nothrow) MockStatusReceiver();
    if (!receiver) {
        EXPECT_FALSE(true) << "the receiver is nullptr";
        return ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR;
    }
    bool result = installer->Uninstall(uninstallParam, receiver);
    EXPECT_TRUE(result);
    return receiver->GetResultCode();
}

/**
 * @tc.number: BmsBundleSharedLibraryUninstall_0100
 * @tc.name: BmsBundleSharedLibraryUninstall
 * @tc.desc: test uninstall shared library
 */
HWTEST_F(BmsBundleSharedLibraryUninstallTest, BmsBundleSharedLibraryUninstall_0100, Function | SmallTest | Level0)
{
    std::string hostPath = MODULE_FILE_PATH + HOST_HAP;
    std::string hspv1 = MODULE_FILE_PATH + LIBA_V10001;
    std::vector<std::string> bundleFilePaths;
    bundleFilePaths.push_back(hostPath);
    std::vector<std::string> sharedBundleDirPaths;
    sharedBundleDirPaths.push_back(hspv1);
    InstallParam installParam;
    installParam.installFlag = InstallFlag::NORMAL;
    installParam.userId = USERID;
    installParam.sharedBundleDirPaths = sharedBundleDirPaths;
    ErrCode installResult = InstallBundle(bundleFilePaths, installParam);
    EXPECT_EQ(installResult, ERR_OK);
    ErrCode unInstallResult = UnInstallBundle(BUNDLE_NAME);
    EXPECT_EQ(unInstallResult, ERR_OK);
    UninstallParam uninstallParam;
    uninstallParam.bundleName = SHARED_BUNDLE_NAME;
    unInstallResult = UninstallShred(uninstallParam);
    EXPECT_EQ(unInstallResult, ERR_OK);
}

}
}