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

#include <gtest/gtest.h>

#include "appexecfwk_errors.h"
#include "bundle_overlay_install_checker.h"
#include "bundle_mgr_service.h"

#include <map>

using namespace testing::ext;
using namespace OHOS::AppExecFwk;
using OHOS::DelayedSingleton;

namespace OHOS {
namespace {
const std::string TEST_MODULE_NAME = "testModuleName";
const std::string TEST_MODULE_NAME_SECOND = "testModuleNameSecond";
const std::string TARGET_MODULE_NAME = "targetModuleName";
const std::string OTHER_TARGET_MODULE_NAME = "targetModuleNameTest";
const std::string TEST_BUNDLE_NAME = "testBundleName";
const std::string TEST_PATH_FIRST = "testPath1";
const std::string TEST_PATH_SECOND = "testPath2";
const int32_t INVALID_TARGET_PRIORITY_FIRST = 0;
const int32_t INVALID_TARGET_PRIORITY_SECOND = 101;
const int32_t DEFAULT_TARGET_PRIORITY_SECOND = 1;
const int32_t TEST_VERSION_CODE = 1000000;
const int32_t LOWER_TEST_VERSION_CODE = 999999;
const int32_t HIGHER_TEST_VERSION_CODE = 1000001;
const int32_t WAIT_TIME = 5; // init mocked bms
} // namespace

class BmsBundleOverlayCheckerTest : public testing::Test {
public:
    BmsBundleOverlayCheckerTest();
    ~BmsBundleOverlayCheckerTest();
    static void SetUpTestCase();
    static void TearDownTestCase();
    void SetUp();
    void TearDown();

    const std::shared_ptr<BundleDataMgr> GetBundleDataMgr() const;
    const std::shared_ptr<BundleOverlayInstallChecker> GetBundleOverlayChecker() const;

private:
    std::shared_ptr<BundleOverlayInstallChecker> overlayChecker_ = std::make_shared<BundleOverlayInstallChecker>();
    std::shared_ptr<BundleMgrService> bundleMgrService_ = DelayedSingleton<BundleMgrService>::GetInstance();
};

BmsBundleOverlayCheckerTest::BmsBundleOverlayCheckerTest()
{}

BmsBundleOverlayCheckerTest::~BmsBundleOverlayCheckerTest()
{}

void BmsBundleOverlayCheckerTest::SetUpTestCase()
{}

void BmsBundleOverlayCheckerTest::TearDownTestCase()
{}

void BmsBundleOverlayCheckerTest::SetUp()
{
    if (!bundleMgrService_->IsServiceReady()) {
        bundleMgrService_->OnStart();
        std::this_thread::sleep_for(std::chrono::seconds(WAIT_TIME));
    }
}

void BmsBundleOverlayCheckerTest::TearDown()
{}

const std::shared_ptr<BundleDataMgr> BmsBundleOverlayCheckerTest::GetBundleDataMgr() const
{
    return bundleMgrService_->GetDataMgr();
}

const std::shared_ptr<BundleOverlayInstallChecker> BmsBundleOverlayCheckerTest::GetBundleOverlayChecker() const
{
    return overlayChecker_;
}

/**
 * @tc.number: OverlayCheckerTest_0100
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.the internal overlay hap is entry type.
 *           2.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0100, Function | SmallTest | Level0)
{
    // construct innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    InnerModuleInfo innerModuleInfo;
    innerModuleInfo.isEntry = true;
    innerModuleInfo.name = TEST_MODULE_NAME;
    innerBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME, innerModuleInfo);
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_ERROR_HAP_TYPE);
}

/**
 * @tc.number: OverlayCheckerTest_0200
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.the internal overlay bundle is service.
 *           2.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0200, Function | SmallTest | Level0)
{
    // construct innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    innerBundleInfo.SetEntryInstallationFree(true);
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_ERROR_BUNDLE_TYPE);
}

/**
 * @tc.number: OverlayCheckerTest_0300
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.innerModuleInfos of innerBundleInfo is empty.
 *           2.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0300, Function | SmallTest | Level0)
{
    // construct innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INTERNAL_ERROR);
}

/**
 * @tc.number: OverlayCheckerTest_0400
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.target priority of internal overlay hap is invalid.
 *           2.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0400, Function | SmallTest | Level0)
{
    // construct innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    InnerModuleInfo innerModuleInfo;
    innerModuleInfo.name = TEST_MODULE_NAME;
    innerModuleInfo.targetPriority = INVALID_TARGET_PRIORITY_FIRST;
    innerBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME, innerModuleInfo);
    innerBundleInfo.SetCurrentModulePackage(TEST_MODULE_NAME);
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INVALID_PRIORITY);
}

/**
 * @tc.number: OverlayCheckerTest_0500
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.target priority of internal overlay hap is invalid.
 *           2.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0500, Function | SmallTest | Level0)
{
    // construct innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    InnerModuleInfo innerModuleInfo;
    innerModuleInfo.name = TEST_MODULE_NAME;
    innerModuleInfo.targetPriority = INVALID_TARGET_PRIORITY_SECOND;
    innerBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME, innerModuleInfo);
    innerBundleInfo.SetCurrentModulePackage(TEST_MODULE_NAME);
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INVALID_PRIORITY);
}

/**
 * @tc.number: OverlayCheckerTest_0600
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.target module name of the internal overlay is same as the module name.
 *           2.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0600, Function | SmallTest | Level0)
{
    // construct innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    InnerModuleInfo innerModuleInfo;
    innerModuleInfo.name = TEST_MODULE_NAME;
    innerModuleInfo.targetModuleName = TEST_MODULE_NAME;
    innerModuleInfo.targetPriority = DEFAULT_TARGET_PRIORITY_SECOND;
    innerBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME, innerModuleInfo);
    innerBundleInfo.SetCurrentModulePackage(TEST_MODULE_NAME);
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INVALID_MODULE_NAME);
}

/**
 * @tc.number: OverlayCheckerTest_0700
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.target module is overlay module.
 *           2.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0700, Function | SmallTest | Level0)
{
    // construct innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    InnerModuleInfo innerModuleInfo;
    innerModuleInfo.name = TEST_MODULE_NAME;
    innerModuleInfo.targetModuleName = TARGET_MODULE_NAME;
    innerModuleInfo.targetPriority = DEFAULT_TARGET_PRIORITY_SECOND;
    innerBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME, innerModuleInfo);
    innerBundleInfo.SetCurrentModulePackage(TEST_MODULE_NAME);
    ApplicationInfo applicationInfo;
    applicationInfo.bundleName = TEST_BUNDLE_NAME;
    innerBundleInfo.SetBaseApplicationInfo(applicationInfo);

    // construct target module
    auto dataMgr = GetBundleDataMgr();
    EXPECT_NE(dataMgr, nullptr);

    InnerBundleInfo oldInfo;
    InnerModuleInfo targetModuleInfo;
    targetModuleInfo.name = TARGET_MODULE_NAME;
    targetModuleInfo.targetModuleName = OTHER_TARGET_MODULE_NAME;
    targetModuleInfo.targetPriority = DEFAULT_TARGET_PRIORITY_SECOND;
    ApplicationInfo targetApplicationInfo;
    targetApplicationInfo.bundleName = TEST_BUNDLE_NAME;

    oldInfo.InsertInnerModuleInfo(TARGET_MODULE_NAME, targetModuleInfo);
    oldInfo.SetBaseApplicationInfo(targetApplicationInfo);
    dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::INSTALL_START);
    dataMgr->AddInnerBundleInfo(TEST_BUNDLE_NAME, oldInfo);
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_MODULE_IS_OVERLAY_MODULE);

    dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::UNINSTALL_START);
    bool ret = dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
    EXPECT_TRUE(ret);
}

/**
 * @tc.number: OverlayCheckerTest_0800
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.install overlay hap and non-overlay hap simultaneously.
 *           2.check successfully.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0800, Function | SmallTest | Level0)
{
    // construct overlay innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    InnerModuleInfo innerModuleInfo;
    innerModuleInfo.name = TEST_MODULE_NAME;
    innerModuleInfo.targetModuleName = TARGET_MODULE_NAME;
    innerModuleInfo.targetPriority = DEFAULT_TARGET_PRIORITY_SECOND;
    innerBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME, innerModuleInfo);
    innerBundleInfo.SetCurrentModulePackage(TEST_MODULE_NAME);

    // construct non-overlay innerBundleInfo
    InnerBundleInfo nonOverlayBundleInfo;
    InnerModuleInfo nonInnerModuleInfo;
    nonInnerModuleInfo.name = TEST_MODULE_NAME_SECOND;
    nonOverlayBundleInfo.SetCurrentModulePackage(TEST_MODULE_NAME_SECOND);
    nonOverlayBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME_SECOND, nonInnerModuleInfo);

    std::unordered_map<std::string, InnerBundleInfo> newInfos;
    newInfos.emplace(TEST_PATH_FIRST, innerBundleInfo);
    newInfos.emplace(TEST_PATH_SECOND, nonOverlayBundleInfo);

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_OK);
}

/**
 * @tc.number: OverlayCheckerTest_0900
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.install internal overlay hap.
 *           2.the version code of overlay hap is larger than non-overlay module.
 *           3.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_0900, Function | SmallTest | Level0)
{
    // construct overlay innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    InnerModuleInfo innerModuleInfo;
    innerModuleInfo.name = TEST_MODULE_NAME;
    innerModuleInfo.targetModuleName = TARGET_MODULE_NAME;
    innerModuleInfo.targetPriority = DEFAULT_TARGET_PRIORITY_SECOND;
    innerBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME, innerModuleInfo);
    BundleInfo bundleInfo;
    bundleInfo.versionCode = HIGHER_TEST_VERSION_CODE;
    innerBundleInfo.SetBaseBundleInfo(bundleInfo);
    innerBundleInfo.SetCurrentModulePackage(TEST_MODULE_NAME);
    ApplicationInfo applicationInfo;
    applicationInfo.bundleName = TEST_BUNDLE_NAME;
    innerBundleInfo.SetBaseApplicationInfo(applicationInfo);
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    // construct old innerBundleInfo
    auto dataMgr = GetBundleDataMgr();
    EXPECT_NE(dataMgr, nullptr);

    InnerBundleInfo oldInfo;
    InnerModuleInfo oldInnerModuleInfo;
    oldInnerModuleInfo.name = TEST_MODULE_NAME_SECOND;
    oldInfo.InsertInnerModuleInfo(TEST_MODULE_NAME_SECOND, oldInnerModuleInfo);
    BundleInfo oldBundleInfo;
    oldBundleInfo.versionCode = TEST_VERSION_CODE;
    oldInfo.SetBaseBundleInfo(oldBundleInfo);
    dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::INSTALL_START);
    dataMgr->AddInnerBundleInfo(TEST_BUNDLE_NAME, oldInfo);

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INCONSISTENT_VERSION_CODE);

    dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::UNINSTALL_START);
    bool ret = dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
    EXPECT_TRUE(ret);
}

/**
 * @tc.number: OverlayCheckerTest_1000
 * @tc.name: test CheckInternalBundle interface in BundleOverlayInstallChecker.
 * @tc.desc: 1.install internal overlay hap.
 *           2.the version code of overlay hap is lower than non-overlay module.
 *           3.check failed.
 * @tc.require: issueI6F3H9
 */
HWTEST_F(BmsBundleOverlayCheckerTest, OverlayCheckerTest_1000, Function | SmallTest | Level0)
{
    // construct overlay innerBundleInfo
    InnerBundleInfo innerBundleInfo;
    InnerModuleInfo innerModuleInfo;
    innerModuleInfo.name = TEST_MODULE_NAME;
    innerModuleInfo.targetModuleName = TARGET_MODULE_NAME;
    innerModuleInfo.targetPriority = DEFAULT_TARGET_PRIORITY_SECOND;
    innerBundleInfo.InsertInnerModuleInfo(TEST_MODULE_NAME, innerModuleInfo);
    BundleInfo bundleInfo;
    bundleInfo.versionCode = LOWER_TEST_VERSION_CODE;
    innerBundleInfo.SetBaseBundleInfo(bundleInfo);
    innerBundleInfo.SetCurrentModulePackage(TEST_MODULE_NAME);
    ApplicationInfo applicationInfo;
    applicationInfo.bundleName = TEST_BUNDLE_NAME;
    innerBundleInfo.SetBaseApplicationInfo(applicationInfo);
    std::unordered_map<std::string, InnerBundleInfo> newInfos;

    // construct old innerBundleInfo
    auto dataMgr = GetBundleDataMgr();
    EXPECT_NE(dataMgr, nullptr);

    InnerBundleInfo oldInfo;
    InnerModuleInfo oldInnerModuleInfo;
    oldInnerModuleInfo.name = TEST_MODULE_NAME_SECOND;
    oldInfo.InsertInnerModuleInfo(TEST_MODULE_NAME_SECOND, oldInnerModuleInfo);
    BundleInfo oldBundleInfo;
    oldBundleInfo.versionCode = TEST_VERSION_CODE;
    oldInfo.SetBaseBundleInfo(oldBundleInfo);
    dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::INSTALL_START);
    dataMgr->AddInnerBundleInfo(TEST_BUNDLE_NAME, oldInfo);

    auto overlayChecker = GetBundleOverlayChecker();
    EXPECT_NE(overlayChecker, nullptr);
    auto res = overlayChecker->CheckInternalBundle(newInfos, innerBundleInfo);
    EXPECT_EQ(res, ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INCONSISTENT_VERSION_CODE);

    dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::UNINSTALL_START);
    bool ret = dataMgr->UpdateBundleInstallState(TEST_BUNDLE_NAME, InstallState::UNINSTALL_SUCCESS);
    EXPECT_TRUE(ret);
}
} // OHOS