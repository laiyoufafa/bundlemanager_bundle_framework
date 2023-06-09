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

#include "status_receiver_proxy.h"

#include <map>

#include "ipc_types.h"
#include "parcel.h"
#include "string_ex.h"

#include "app_log_wrapper.h"
#include "appexecfwk_errors.h"

namespace OHOS {
namespace AppExecFwk {
namespace {
// struct for transform internal error code/message to result which open to developer
struct ReceivedResult {
    int32_t clientCode = -1;
    std::string clientMessage;
};

const std::string MSG_SUCCESS = "[SUCCESS]";
const std::string MSG_ERR_INSTALL_INTERNAL_ERROR = "[ERR_INSTALL_INTERNAL_ERROR]";
const std::string MSG_ERR_INSTALL_HOST_INSTALLER_FAILED = "[ERR_INSTALL_HOST_INSTALLER_FAILED]";
const std::string MSG_ERR_INSTALL_PARSE_FAILED = "[ERR_INSTALL_PARSE_FAILED]";
const std::string MSG_ERR_INSTALL_VERSION_DOWNGRADE = "[ERR_INSTALL_VERSION_DOWNGRADE]";
const std::string MSG_ERR_INSTALL_VERIFICATION_FAILED = "[ERR_INSTALL_VERIFICATION_FAILED]";
const std::string MSG_ERR_INSTALL_FAILED_INVALID_SIGNATURE_FILE_PATH =
    "[MSG_ERR_INSTALL_FAILED_INVALID_SIGNATURE_FILE_PATH]";
const std::string MSG_ERR_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE_FILE =
    "[MSG_ERR_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE_FILE]";
const std::string MSG_ERR_INSTALL_FAILED_NO_BUNDLE_SIGNATURE = "[MSG_ERR_INSTALL_FAILED_NO_BUNDLE_SIGNATURE]";
const std::string MSG_ERR_INSTALL_FAILED_VERIFY_APP_PKCS7_FAIL = "[MSG_ERR_INSTALL_FAILED_VERIFY_APP_PKCS7_FAIL]";
const std::string MSG_ERR_INSTALL_FAILED_PROFILE_PARSE_FAIL = "[MSG_ERR_INSTALL_FAILED_PROFILE_PARSE_FAIL]";
const std::string MSG_ERR_INSTALL_FAILED_APP_SOURCE_NOT_TRUESTED = "[MSG_ERR_INSTALL_FAILED_APP_SOURCE_NOT_TRUESTED]";
const std::string MSG_ERR_INSTALL_FAILED_BAD_DIGEST = "[MSG_ERR_INSTALL_FAILED_BAD_DIGEST]";
const std::string MSG_ERR_INSTALL_FAILED_BUNDLE_INTEGRITY_VERIFICATION_FAILURE =
    "[MSG_ERR_INSTALL_FAILED_BUNDLE_INTEGRITY_VERIFICATION_FAILURE]";
const std::string MSG_ERR_INSTALL_FAILED_FILE_SIZE_TOO_LARGE = "[MSG_ERR_INSTALL_FAILED_FILE_SIZE_TOO_LARGE]";
const std::string MSG_ERR_INSTALL_FAILED_BAD_PUBLICKEY = "[MSG_ERR_INSTALL_FAILED_BAD_PUBLICKEY]";
const std::string MSG_ERR_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE = "[MSG_ERR_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE]";
const std::string MSG_ERR_INSTALL_FAILED_NO_PROFILE_BLOCK_FAIL = "[MSG_ERR_INSTALL_FAILED_NO_PROFILE_BLOCK_FAIL]";
const std::string MSG_ERR_INSTALL_FAILED_BUNDLE_SIGNATURE_VERIFICATION_FAILURE =
    "[MSG_ERR_INSTALL_FAILED_BUNDLE_SIGNATURE_VERIFICATION_FAILURE]";
const std::string MSG_ERR_INSTALL_FAILED_VERIFY_SOURCE_INIT_FAIL = "[MSG_ERR_INSTALL_FAILED_VERIFY_SOURCE_INIT_FAIL]";
const std::string MSG_ERR_INSTALL_FAILED_INCOMPATIBLE_SIGNATURE = "[ERR_INSTALL_FAILED_INCOMPATIBLE_SIGNATURE]";
const std::string MSG_ERR_INSTALL_FAILED_INCONSISTENT_SIGNATURE = "[MSG_ERR_INSTALL_FAILED_INCONSISTENT_SIGNATURE]";
const std::string MSG_ERR_INSTALL_PARAM_ERROR = "[MSG_ERR_INSTALL_PARAM_ERROR]";
const std::string MSG_ERR_INSTALL_PERMISSION_DENIED = "[MSG_ERR_INSTALL_PERMISSION_DENIED]";
const std::string MSG_ERR_INSTALL_ENTRY_ALREADY_EXIST = "[MSG_ERR_INSTALL_ENTRY_ALREADY_EXIST]";
const std::string MSG_ERR_INSTALL_STATE_ERROR = "[MSG_ERR_INSTALL_STATE_ERROR]";
const std::string MSG_ERR_INSTALL_FILE_PATH_INVALID = "[MSG_ERR_INSTALL_FILE_PATH_INVALID]";
const std::string MSG_ERR_INSTALL_INVALID_BUNDLE_FILE = "[ERR_INSTALL_INVALID_BUNDLE_FILE]";
const std::string MSG_ERR_INSTALL_INVALID_HAP_SIZE = "[ERR_INSTALL_INVALID_HAP_SIZE]";
const std::string MSG_ERR_INSTALL_GENERATE_UID_ERROR = "[ERR_INSTALL_GENERATE_UID_ERROR]";
const std::string MSG_ERR_INSTALL_INSTALLD_SERVICE_ERROR = "[ERR_INSTALL_INSTALLD_SERVICE_ERROR]";
const std::string MSG_ERR_INSTALL_BUNDLE_MGR_SERVICE_ERROR = "[ERR_INSTALL_BUNDLE_MGR_SERVICE_ERROR]";
const std::string MSG_ERR_INSTALL_ALREADY_EXIST = "[ERR_INSTALL_ALREADY_EXIST]";
const std::string MSG_ERR_INSTALL_VERSIONCODE_BUNDLE_NOT_SAME = "[ERR_INSTALL_DIFFERENT_VERSIONCODE_BUNDLENAME]";
const std::string MSG_ERR_INSTALL_BUNDLENAME_NOT_SAME = "[ERR_INSTALL_BUNDLENAME_NOT_SAME]";
const std::string MSG_ERR_INSTALL_VERSIONCODE_NOT_SAME = "[ERR_INSTALL_VERSIONCODE_NOT_SAME]";
const std::string MSG_ERR_INSTALL_VERSIONNAME_NOT_SAME = "[ERR_INSTALL_VERSIONNAME_NOT_SAME]";
const std::string MSG_ERR_INSTALL_MINCOMPATIBLE_VERSIONCODE_NOT_SAME =
    "[ERR_INSTALL_MINCOMPATIBLE_VERSIONCODE_NOT_SAME]";
const std::string MSG_ERR_INSTALL_VENDOR_NOT_SAME = "[ERR_INSTALL_VENDOR_NOT_SAME]";
const std::string MSG_ERR_INSTALL_RELEASETYPE_TARGET_NOT_SAME = "[ERR_INSTALL_RELEASETYPE_TARGET_NOT_SAME]";
const std::string MSG_ERR_INSTALL_RELEASETYPE_NOT_SAME = "[ERR_INSTALL_RELEASETYPE_NOT_SAME]";
const std::string MSG_ERR_INSTALL_RELEASETYPE_COMPATIBLE_NOT_SAME = "[ERR_INSTALL_RELEASETYPE_COMPATIBLE_NOT_SAME]";
const std::string MSG_ERR_INSTALL_VERSION_NOT_COMPATIBLE = "[ERR_INSTALL_VERSION_NOT_COMPATIBLE]";
const std::string MSG_ERR_INSTALL_INVALID_NUMBER_OF_ENTRY_HAP = "[ERR_INSTALL_INVALID_NUMBER_OF_ENTRY_HAP]";
const std::string MSG_ERR_INSTALL_DISK_MEM_INSUFFICIENT = "[MSG_ERR_INSTALL_DISK_MEM_INSUFFICIENT]";
const std::string MSG_ERR_INSTALL_GRANT_REQUEST_PERMISSIONS_FAILED =
    "[MSG_ERR_INSTALL_GRANT_REQUEST_PERMISSIONS_FAILED]";
const std::string MSG_ERR_INSTALL_UPDATE_HAP_TOKEN_FAILED = "[MSG_ERR_INSTALL_UPDATE_HAP_TOKEN_FAILED]";
const std::string MSG_ERR_INSTALL_SINGLETON_NOT_SAME = "[MSG_ERR_INSTALL_SINGLETON_NOT_SAME]";
const std::string MSG_ERR_INSTALL_ZERO_USER_WITH_NO_SINGLETON = "[MSG_ERR_INSTALL_ZERO_USER_WITH_NO_SINGLETON]";
const std::string MSG_ERR_INSTALL_CHECK_SYSCAP_FAILED = "[MSG_ERR_INSTALL_CHECK_SYSCAP_FAILED]";
const std::string MSG_ERR_INSTALL_APPTYPE_NOT_SAME = "[MSG_ERR_INSTALL_APPTYPE_NOT_SAME]";
const std::string MSG_ERR_INSTALL_URI_DUPLICATE = "[MSG_ERR_INSTALL_URI_DUPLICATE]";
const std::string MSG_ERR_INSTALL_TYPE_ERROR = "[ERR_INSTALL_TYPE_ERROR]";
const std::string MSG_ERR_INSTALL_PARSE_UNEXPECTED = "[ERR_INSTALL_PARSE_UNEXPECTED]";
const std::string MSG_ERR_INSTALL_PARSE_MISSING_BUNDLE = "[ERR_INSTALL_PARSE_MISSING_BUNDLE]";
const std::string MSG_ERR_INSTALL_PARSE_MISSING_ABILITY = "[ERR_INSTALL_PARSE_MISSING_ABILITY]";
const std::string MSG_ERR_INSTALL_PARSE_NO_PROFILE = "[ERR_INSTALL_PARSE_NO_PROFILE]";
const std::string MSG_ERR_INSTALL_PARSE_BAD_PROFILE = "[ERR_INSTALL_PARSE_BAD_PROFILE]";
const std::string MSG_ERR_INSTALL_PARSE_PROFILE_PROP_TYPE_ERROR = "[ERR_INSTALL_PARSE_PROFILE_PROP_TYPE_ERROR]";
const std::string MSG_ERR_INSTALL_PARSE_PROFILE_MISSING_PROP = "[ERR_INSTALL_PARSE_PROFILE_MISSING_PROP]";
const std::string MSG_ERR_INSTALL_PARSE_PERMISSION_ERROR = "[ERR_INSTALL_PARSE_PERMISSION_ERROR]";
const std::string MSG_ERR_INSTALL_PARSE_PROFILE_PROP_CHECK_ERROR = "[ERR_INSTALL_PARSE_PROFILE_PROP_CHECK_ERROR]";
const std::string MSG_ERR_INSTALL_PARSE_RPCID_FAILED = "[ERR_INSTALL_PARSE_RPCID_FAILED]";

const std::string MSG_ERR_INSTALLD_CLEAN_DIR_FAILED = "[MSG_ERR_INSTALLD_CLEAN_DIR_FAILED]";
const std::string MSG_ERR_INSTALLD_RNAME_DIR_FAILED = "[MSG_ERR_INSTALLD_RNAME_DIR_FAILED]";
const std::string MSG_ERR_INSTALLD_EXTRACT_FILES_FAILED = "[MSG_ERR_INSTALLD_EXTRACT_FILES_FAILED]";
const std::string MSG_ERR_INSTALLD_REMOVE_DIR_FAILED = "[ERR_INSTALLD_REMOVE_DIR_FAILED]";
const std::string MSG_ERR_INSTALLD_CHOWN_FAILED = "[ERR_INSTALLD_CHOWN_FAILED,]";
const std::string MSG_ERR_INSTALLD_CREATE_DIR_EXIST = "[ERR_INSTALLD_CREATE_DIR_EXIST]";
const std::string MSG_ERR_INSTALLD_CREATE_DIR_FAILED = "[ERR_INSTALLD_CREATE_DIR_FAILED]";
const std::string MSG_ERR_INSTALLD_GET_PROXY_ERROR = "[ERR_INSTALLD_GET_PROXY_ERROR]";
const std::string MSG_ERR_INSTALLD_PARAM_ERROR = "[ERR_INSTALLD_PARAM_ERROR]";

const std::string MSG_ERR_INSTALL_INVALID_HAP_NAME = "[ERR_INSTALL_INVALID_HAP_NAME]";
const std::string MSG_ERR_UNINSTALL_PARAM_ERROR = "[ERR_UNINSTALL_PARAM_ERROR]";
const std::string MSG_ERR_UNINSTALL_PERMISSION_DENIED = "[MSG_ERR_UNINSTALL_PERMISSION_DENIED]";
const std::string MSG_ERR_UNINSTALL_INVALID_NAME = "[ERR_UNINSTALL_INVALID_NAME]";
const std::string MSG_ERR_UNINSTALL_BUNDLE_MGR_SERVICE_ERROR = "[ERR_UNINSTALL_BUNDLE_MGR_SERVICE_ERROR,]";
const std::string MSG_ERR_UNINSTALL_MISSING_INSTALLED_BUNDLE = "[ERR_UNINSTALL_MISSING_INSTALLED_BUNDLE]";
const std::string MSG_ERR_UNINSTALL_MISSING_INSTALLED_MODULE = "[ERR_UNINSTALL_MISSING_INSTALLED_MODULE]";
const std::string MSG_ERR_UNINSTALL_KILLING_APP_ERROR = "[ERR_UNINSTALL_KILLING_APP_ERROR]";
const std::string MSG_ERR_UNINSTALL_SYSTEM_APP_ERROR = "[MSG_ERR_UNINSTALL_SYSTEM_APP_ERROR]";
const std::string MSG_ERR_UNKNOWN = "[ERR_UNKNOWN]";

const std::string MSG_ERR_RECOVER_GET_BUNDLEPATH_ERROR = "[ERR_RECOVER_GET_BUNDLEPATH_ERROR]";
const std::string MSG_ERR_RECOVER_INVALID_BUNDLE_NAME = "[ERR_RECOVER_INVALID_BUNDLE_NAME]";
const std::string MSG_ERR_FAILED_SERVICE_DIED = "[ERR_FAILED_SERVICE_DIED]";
const std::string MSG_ERR_FAILED_GET_INSTALLER_PROXY = "[MSG_ERR_FAILED_GET_INSTALLER_PROXY]";
const std::string MSG_ERR_OPERATION_TIME_OUT = "[MSG_ERR_OPERATION_TIME_OUT]";
const std::string MSG_ERR_USER_NOT_EXIST = "[ERR_USER_NOT_EXIST]";
const std::string MSG_ERR_USER_CREATE_FALIED = "[ERR_USER_CREATE_FALIED]";
const std::string MSG_ERR_USER_REMOVE_FALIED = "[ERR_USER_REMOVE_FALIED]";
const std::string MSG_ERR_USER_NOT_INSTALL_HAP = "[ERR_USER_NOT_INSTALL_HAP]";

const std::map<int32_t, struct ReceivedResult> MAP_RECEIVED_RESULTS {
    {ERR_OK, {IStatusReceiver::SUCCESS, MSG_SUCCESS}},
    {ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR,
        {IStatusReceiver::ERR_INSTALL_INTERNAL_ERROR, MSG_ERR_INSTALL_INTERNAL_ERROR}},
    {ERR_APPEXECFWK_INSTALL_HOST_INSTALLER_FAILED,
        {IStatusReceiver::ERR_INSTALL_HOST_INSTALLER_FAILED, MSG_ERR_INSTALL_HOST_INSTALLER_FAILED}},
    {ERR_APPEXECFWK_INSTALL_PARSE_FAILED, {IStatusReceiver::ERR_INSTALL_PARSE_FAILED, MSG_ERR_INSTALL_PARSE_FAILED}},
    {ERR_APPEXECFWK_INSTALL_VERSION_DOWNGRADE,
        {IStatusReceiver::ERR_INSTALL_VERSION_DOWNGRADE, MSG_ERR_INSTALL_VERSION_DOWNGRADE}},
    {ERR_APPEXECFWK_INSTALL_VERIFICATION_FAILED,
        {IStatusReceiver::ERR_INSTALL_VERIFICATION_FAILED, MSG_ERR_INSTALL_VERIFICATION_FAILED}},
    {ERR_APPEXECFWK_INSTALL_FAILED_INVALID_SIGNATURE_FILE_PATH,
        {IStatusReceiver::ERR_INSTALL_FAILED_INVALID_SIGNATURE_FILE_PATH,
        MSG_ERR_INSTALL_FAILED_INVALID_SIGNATURE_FILE_PATH}},
    {ERR_APPEXECFWK_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE_FILE,
        {IStatusReceiver::ERR_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE_FILE,
            MSG_ERR_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE_FILE}},
    {ERR_APPEXECFWK_INSTALL_FAILED_NO_BUNDLE_SIGNATURE,
        {IStatusReceiver::ERR_INSTALL_FAILED_NO_BUNDLE_SIGNATURE, MSG_ERR_INSTALL_FAILED_NO_BUNDLE_SIGNATURE}},
    {ERR_APPEXECFWK_INSTALL_FAILED_VERIFY_APP_PKCS7_FAIL,
        {IStatusReceiver::ERR_INSTALL_FAILED_VERIFY_APP_PKCS7_FAIL, MSG_ERR_INSTALL_FAILED_VERIFY_APP_PKCS7_FAIL}},
    {ERR_APPEXECFWK_INSTALL_FAILED_PROFILE_PARSE_FAIL,
        {IStatusReceiver::ERR_INSTALL_FAILED_PROFILE_PARSE_FAIL, MSG_ERR_INSTALL_FAILED_PROFILE_PARSE_FAIL}},
    {ERR_APPEXECFWK_INSTALL_FAILED_APP_SOURCE_NOT_TRUESTED,
        {IStatusReceiver::ERR_INSTALL_FAILED_APP_SOURCE_NOT_TRUESTED, MSG_ERR_INSTALL_FAILED_APP_SOURCE_NOT_TRUESTED}},
    {ERR_APPEXECFWK_INSTALL_FAILED_BAD_DIGEST,
        {IStatusReceiver::ERR_INSTALL_FAILED_BAD_DIGEST, MSG_ERR_INSTALL_FAILED_BAD_DIGEST}},
    {ERR_APPEXECFWK_INSTALL_FAILED_BUNDLE_INTEGRITY_VERIFICATION_FAILURE,
        {IStatusReceiver::ERR_INSTALL_FAILED_BUNDLE_INTEGRITY_VERIFICATION_FAILURE,
            MSG_ERR_INSTALL_FAILED_BUNDLE_INTEGRITY_VERIFICATION_FAILURE}},
    {ERR_APPEXECFWK_INSTALL_FAILED_FILE_SIZE_TOO_LARGE,
        {IStatusReceiver::ERR_INSTALL_FAILED_FILE_SIZE_TOO_LARGE, MSG_ERR_INSTALL_FAILED_FILE_SIZE_TOO_LARGE}},
    {ERR_APPEXECFWK_INSTALL_FAILED_BAD_PUBLICKEY,
        {IStatusReceiver::ERR_INSTALL_FAILED_BAD_PUBLICKEY, MSG_ERR_INSTALL_FAILED_BAD_PUBLICKEY}},
    {ERR_APPEXECFWK_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE,
        {IStatusReceiver::ERR_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE, MSG_ERR_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE}},
    {ERR_APPEXECFWK_INSTALL_FAILED_NO_PROFILE_BLOCK_FAIL,
        {IStatusReceiver::ERR_INSTALL_FAILED_NO_PROFILE_BLOCK_FAIL, MSG_ERR_INSTALL_FAILED_NO_PROFILE_BLOCK_FAIL}},
    {ERR_APPEXECFWK_INSTALL_FAILED_BUNDLE_SIGNATURE_VERIFICATION_FAILURE,
        {IStatusReceiver::ERR_INSTALL_FAILED_BUNDLE_SIGNATURE_VERIFICATION_FAILURE,
            MSG_ERR_INSTALL_FAILED_BUNDLE_SIGNATURE_VERIFICATION_FAILURE}},
    {ERR_APPEXECFWK_INSTALL_FAILED_VERIFY_SOURCE_INIT_FAIL,
        {IStatusReceiver::ERR_INSTALL_FAILED_VERIFY_SOURCE_INIT_FAIL, MSG_ERR_INSTALL_FAILED_VERIFY_SOURCE_INIT_FAIL}},
    {ERR_APPEXECFWK_INSTALL_FAILED_INCOMPATIBLE_SIGNATURE,
        {IStatusReceiver::ERR_INSTALL_FAILED_INCOMPATIBLE_SIGNATURE, MSG_ERR_INSTALL_FAILED_INCOMPATIBLE_SIGNATURE}},
    {ERR_APPEXECFWK_INSTALL_FAILED_INCONSISTENT_SIGNATURE,
        {IStatusReceiver::ERR_INSTALL_FAILED_INCONSISTENT_SIGNATURE, MSG_ERR_INSTALL_FAILED_INCONSISTENT_SIGNATURE}},
    {ERR_APPEXECFWK_INSTALL_PARAM_ERROR, {IStatusReceiver::ERR_INSTALL_PARAM_ERROR, MSG_ERR_INSTALL_PARAM_ERROR}},
    {ERR_APPEXECFWK_INSTALL_PERMISSION_DENIED,
        {IStatusReceiver::ERR_INSTALL_PERMISSION_DENIED, MSG_ERR_INSTALL_PERMISSION_DENIED}},
    {ERR_APPEXECFWK_INSTALL_ENTRY_ALREADY_EXIST,
        {IStatusReceiver::ERR_INSTALL_ENTRY_ALREADY_EXIST, MSG_ERR_INSTALL_ENTRY_ALREADY_EXIST}},
    {ERR_APPEXECFWK_INSTALL_STATE_ERROR, {IStatusReceiver::ERR_INSTALL_STATE_ERROR, MSG_ERR_INSTALL_STATE_ERROR}},
    {ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID,
        {IStatusReceiver::ERR_INSTALL_FILE_PATH_INVALID, MSG_ERR_INSTALL_FILE_PATH_INVALID}},
    {ERR_APPEXECFWK_INSTALL_INVALID_HAP_NAME,
        {IStatusReceiver::ERR_INSTALL_INVALID_HAP_NAME, MSG_ERR_INSTALL_INVALID_HAP_NAME}},
    {ERR_APPEXECFWK_INSTALL_INVALID_BUNDLE_FILE,
        {IStatusReceiver::ERR_INSTALL_INVALID_BUNDLE_FILE, MSG_ERR_INSTALL_INVALID_BUNDLE_FILE}},
    {ERR_APPEXECFWK_INSTALL_INVALID_HAP_SIZE,
        {IStatusReceiver::ERR_INSTALL_INVALID_HAP_SIZE, MSG_ERR_INSTALL_INVALID_HAP_SIZE}},
    {ERR_APPEXECFWK_INSTALL_GENERATE_UID_ERROR,
        {IStatusReceiver::ERR_INSTALL_GENERATE_UID_ERROR, MSG_ERR_INSTALL_GENERATE_UID_ERROR}},
    {ERR_APPEXECFWK_INSTALL_INSTALLD_SERVICE_ERROR,
        {IStatusReceiver::ERR_INSTALL_INSTALLD_SERVICE_ERROR, MSG_ERR_INSTALL_INSTALLD_SERVICE_ERROR}},
    {ERR_APPEXECFWK_INSTALL_BUNDLE_MGR_SERVICE_ERROR,
        {IStatusReceiver::ERR_INSTALL_BUNDLE_MGR_SERVICE_ERROR, MSG_ERR_INSTALL_BUNDLE_MGR_SERVICE_ERROR}},
    {ERR_APPEXECFWK_INSTALL_ALREADY_EXIST,
        {IStatusReceiver::ERR_INSTALL_ALREADY_EXIST, MSG_ERR_INSTALL_ALREADY_EXIST}},
    {ERR_APPEXECFWK_INSTALL_BUNDLENAME_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_BUNDLENAME_NOT_SAME, MSG_ERR_INSTALL_BUNDLENAME_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_VERSIONCODE_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_VERSIONCODE_NOT_SAME, MSG_ERR_INSTALL_VERSIONCODE_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_VERSIONNAME_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_VERSIONNAME_NOT_SAME, MSG_ERR_INSTALL_VERSIONNAME_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_MINCOMPATIBLE_VERSIONCODE_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_MINCOMPATIBLE_VERSIONCODE_NOT_SAME,
            MSG_ERR_INSTALL_MINCOMPATIBLE_VERSIONCODE_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_VENDOR_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_VENDOR_NOT_SAME, MSG_ERR_INSTALL_VENDOR_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_RELEASETYPE_TARGET_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_RELEASETYPE_TARGET_NOT_SAME, MSG_ERR_INSTALL_RELEASETYPE_TARGET_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_RELEASETYPE_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_RELEASETYPE_NOT_SAME, MSG_ERR_INSTALL_RELEASETYPE_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_RELEASETYPE_COMPATIBLE_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_RELEASETYPE_COMPATIBLE_NOT_SAME,
            MSG_ERR_INSTALL_RELEASETYPE_COMPATIBLE_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_VERSION_NOT_COMPATIBLE,
        {IStatusReceiver::ERR_INSTALL_VERSION_NOT_COMPATIBLE, MSG_ERR_INSTALL_VERSION_NOT_COMPATIBLE}},
    {ERR_APPEXECFWK_INSTALL_INVALID_NUMBER_OF_ENTRY_HAP,
        {IStatusReceiver::ERR_INSTALL_INVALID_NUMBER_OF_ENTRY_HAP, MSG_ERR_INSTALL_INVALID_NUMBER_OF_ENTRY_HAP}},
    {ERR_APPEXECFWK_INSTALL_GRANT_REQUEST_PERMISSIONS_FAILED,
        {IStatusReceiver::ERR_INSTALL_GRANT_REQUEST_PERMISSIONS_FAILED,
            MSG_ERR_INSTALL_GRANT_REQUEST_PERMISSIONS_FAILED}},
    {ERR_APPEXECFWK_INSTALL_UPDATE_HAP_TOKEN_FAILED,
        {IStatusReceiver::ERR_INSTALL_UPDATE_HAP_TOKEN_FAILED, MSG_ERR_INSTALL_UPDATE_HAP_TOKEN_FAILED}},
    {ERR_APPEXECFWK_INSTALL_SINGLETON_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_SINGLETON_NOT_SAME, MSG_ERR_INSTALL_SINGLETON_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_ZERO_USER_WITH_NO_SINGLETON,
        {IStatusReceiver::ERR_INSTALL_ZERO_USER_WITH_NO_SINGLETON, MSG_ERR_INSTALL_ZERO_USER_WITH_NO_SINGLETON}},
    {ERR_APPEXECFWK_INSTALL_CHECK_SYSCAP_FAILED,
        {IStatusReceiver::ERR_INSTALL_CHECK_SYSCAP_FAILED, MSG_ERR_INSTALL_CHECK_SYSCAP_FAILED}},
    {ERR_APPEXECFWK_INSTALL_APPTYPE_NOT_SAME,
        {IStatusReceiver::ERR_INSTALL_APPTYPE_NOT_SAME, MSG_ERR_INSTALL_APPTYPE_NOT_SAME}},
    {ERR_APPEXECFWK_INSTALL_URI_DUPLICATE,
        {IStatusReceiver::ERR_INSTALL_URI_DUPLICATE, MSG_ERR_INSTALL_URI_DUPLICATE}},
    {ERR_APPEXECFWK_INSTALL_TYPE_ERROR, {IStatusReceiver::ERR_INSTALL_TYPE_ERROR, MSG_ERR_INSTALL_TYPE_ERROR}},
    {ERR_APPEXECFWK_INSTALL_DISK_MEM_INSUFFICIENT,
        {IStatusReceiver::ERR_INSTALL_DISK_MEM_INSUFFICIENT, MSG_ERR_INSTALL_DISK_MEM_INSUFFICIENT}},
    {ERR_APPEXECFWK_PARSE_UNEXPECTED,
        {IStatusReceiver::ERR_INSTALL_PARSE_UNEXPECTED, MSG_ERR_INSTALL_PARSE_UNEXPECTED}},
    {ERR_APPEXECFWK_PARSE_MISSING_BUNDLE,
        {IStatusReceiver::ERR_INSTALL_PARSE_MISSING_BUNDLE, MSG_ERR_INSTALL_PARSE_MISSING_BUNDLE}},
    {ERR_APPEXECFWK_PARSE_MISSING_ABILITY,
        {IStatusReceiver::ERR_INSTALL_PARSE_MISSING_ABILITY, MSG_ERR_INSTALL_PARSE_MISSING_ABILITY}},
    {ERR_APPEXECFWK_PARSE_NO_PROFILE,
        {IStatusReceiver::ERR_INSTALL_PARSE_NO_PROFILE, MSG_ERR_INSTALL_PARSE_NO_PROFILE}},
    {ERR_APPEXECFWK_PARSE_BAD_PROFILE,
        {IStatusReceiver::ERR_INSTALL_PARSE_BAD_PROFILE, MSG_ERR_INSTALL_PARSE_BAD_PROFILE}},
    {ERR_APPEXECFWK_PARSE_PROFILE_PROP_TYPE_ERROR,
        {IStatusReceiver::ERR_INSTALL_PARSE_PROFILE_PROP_TYPE_ERROR, MSG_ERR_INSTALL_PARSE_PROFILE_PROP_TYPE_ERROR}},
    {ERR_APPEXECFWK_PARSE_PROFILE_MISSING_PROP,
        {IStatusReceiver::ERR_INSTALL_PARSE_PROFILE_MISSING_PROP, MSG_ERR_INSTALL_PARSE_PROFILE_MISSING_PROP}},
    {ERR_APPEXECFWK_PARSE_PERMISSION_ERROR,
        {IStatusReceiver::ERR_INSTALL_PARSE_PERMISSION_ERROR, MSG_ERR_INSTALL_PARSE_PERMISSION_ERROR}},
    {ERR_APPEXECFWK_PARSE_PROFILE_PROP_CHECK_ERROR,
        {IStatusReceiver::ERR_INSTALL_PARSE_PROFILE_PROP_CHECK_ERROR, MSG_ERR_INSTALL_PARSE_PROFILE_PROP_CHECK_ERROR}},
    {ERR_APPEXECFWK_PARSE_RPCID_FAILED,
        {IStatusReceiver::ERR_INSTALL_PARSE_RPCID_FAILED, MSG_ERR_INSTALL_PARSE_RPCID_FAILED}},

    {ERR_APPEXECFWK_INSTALLD_CLEAN_DIR_FAILED,
        {IStatusReceiver::ERR_INSTALLD_CLEAN_DIR_FAILED, MSG_ERR_INSTALLD_CLEAN_DIR_FAILED}},
    {ERR_APPEXECFWK_INSTALLD_RNAME_DIR_FAILED,
        {IStatusReceiver::ERR_INSTALLD_RNAME_DIR_FAILED, MSG_ERR_INSTALLD_RNAME_DIR_FAILED}},
    {ERR_APPEXECFWK_INSTALLD_EXTRACT_FILES_FAILED,
        {IStatusReceiver::ERR_INSTALLD_EXTRACT_FILES_FAILED, MSG_ERR_INSTALLD_EXTRACT_FILES_FAILED}},
    {ERR_APPEXECFWK_INSTALLD_REMOVE_DIR_FAILED,
        {IStatusReceiver::ERR_INSTALLD_REMOVE_DIR_FAILED, MSG_ERR_INSTALLD_REMOVE_DIR_FAILED}},
    {ERR_APPEXECFWK_INSTALLD_CHOWN_FAILED, {IStatusReceiver::ERR_INSTALLD_CHOWN_FAILED, MSG_ERR_INSTALLD_CHOWN_FAILED}},
    {ERR_APPEXECFWK_INSTALLD_CREATE_DIR_EXIST,
        {IStatusReceiver::ERR_INSTALLD_CREATE_DIR_EXIST, MSG_ERR_INSTALLD_CREATE_DIR_EXIST}},
    {ERR_APPEXECFWK_INSTALLD_CREATE_DIR_FAILED,
        {IStatusReceiver::ERR_INSTALLD_CREATE_DIR_FAILED, MSG_ERR_INSTALLD_CREATE_DIR_FAILED}},
    {ERR_APPEXECFWK_INSTALLD_GET_PROXY_ERROR,
        {IStatusReceiver::ERR_INSTALLD_GET_PROXY_ERROR, MSG_ERR_INSTALLD_GET_PROXY_ERROR}},
    {ERR_APPEXECFWK_INSTALLD_PARAM_ERROR, {IStatusReceiver::ERR_INSTALLD_PARAM_ERROR, MSG_ERR_INSTALLD_PARAM_ERROR}},

    {ERR_APPEXECFWK_UNINSTALL_MISSING_INSTALLED_MODULE,
        {IStatusReceiver::ERR_UNINSTALL_MISSING_INSTALLED_MODULE, MSG_ERR_UNINSTALL_MISSING_INSTALLED_MODULE}},
    {ERR_APPEXECFWK_UNINSTALL_PARAM_ERROR, {IStatusReceiver::ERR_UNINSTALL_PARAM_ERROR, MSG_ERR_UNINSTALL_PARAM_ERROR}},
    {ERR_APPEXECFWK_UNINSTALL_PERMISSION_DENIED,
        {IStatusReceiver::ERR_UNINSTALL_PERMISSION_DENIED, MSG_ERR_UNINSTALL_PERMISSION_DENIED}},
    {ERR_APPEXECFWK_UNINSTALL_BUNDLE_MGR_SERVICE_ERROR,
        {IStatusReceiver::ERR_UNINSTALL_BUNDLE_MGR_SERVICE_ERROR, MSG_ERR_UNINSTALL_BUNDLE_MGR_SERVICE_ERROR}},
    {ERR_APPEXECFWK_UNINSTALL_MISSING_INSTALLED_BUNDLE,
        {IStatusReceiver::ERR_UNINSTALL_MISSING_INSTALLED_BUNDLE, MSG_ERR_UNINSTALL_MISSING_INSTALLED_BUNDLE}},
    {ERR_APPEXECFWK_UNINSTALL_KILLING_APP_ERROR,
        {IStatusReceiver::ERR_UNINSTALL_KILLING_APP_ERROR, MSG_ERR_UNINSTALL_KILLING_APP_ERROR}},
    {ERR_APPEXECFWK_UNINSTALL_SYSTEM_APP_ERROR,
        {IStatusReceiver::ERR_UNINSTALL_SYSTEM_APP_ERROR, MSG_ERR_UNINSTALL_SYSTEM_APP_ERROR}},
    {ERR_APPEXECFWK_UNINSTALL_INVALID_NAME,
        {IStatusReceiver::ERR_UNINSTALL_INVALID_NAME, MSG_ERR_UNINSTALL_INVALID_NAME}},
    {ERR_APPEXECFWK_RECOVER_GET_BUNDLEPATH_ERROR,
        {IStatusReceiver::ERR_RECOVER_GET_BUNDLEPATH_ERROR, MSG_ERR_RECOVER_GET_BUNDLEPATH_ERROR}},
    {ERR_APPEXECFWK_RECOVER_INVALID_BUNDLE_NAME,
        {IStatusReceiver::ERR_RECOVER_INVALID_BUNDLE_NAME, MSG_ERR_RECOVER_INVALID_BUNDLE_NAME}},
    {ERR_APPEXECFWK_FAILED_SERVICE_DIED,
        {IStatusReceiver::ERR_FAILED_SERVICE_DIED, MSG_ERR_FAILED_SERVICE_DIED}},
    {ERR_APPEXECFWK_FAILED_GET_INSTALLER_PROXY,
        {IStatusReceiver::ERR_FAILED_GET_INSTALLER_PROXY, MSG_ERR_FAILED_GET_INSTALLER_PROXY}},
    {ERR_APPEXECFWK_OPERATION_TIME_OUT,
        {IStatusReceiver::ERR_OPERATION_TIME_OUT, MSG_ERR_OPERATION_TIME_OUT}},
    {ERR_APPEXECFWK_USER_NOT_EXIST,
        {IStatusReceiver::ERR_USER_NOT_EXIST, MSG_ERR_USER_NOT_EXIST}},
    {ERR_APPEXECFWK_USER_CREATE_FALIED,
        {IStatusReceiver::ERR_USER_CREATE_FALIED, MSG_ERR_USER_CREATE_FALIED}},
    {ERR_APPEXECFWK_USER_REMOVE_FALIED,
        {IStatusReceiver::ERR_USER_REMOVE_FALIED, MSG_ERR_USER_REMOVE_FALIED}},
    {ERR_APPEXECFWK_USER_NOT_INSTALL_HAP,
        {IStatusReceiver::ERR_USER_NOT_INSTALL_HAP, MSG_ERR_USER_NOT_INSTALL_HAP}},
};
}  // namespace

StatusReceiverProxy::StatusReceiverProxy(const sptr<IRemoteObject> &object) : IRemoteProxy<IStatusReceiver>(object)
{
    APP_LOGI("create status receiver proxy instance");
}

StatusReceiverProxy::~StatusReceiverProxy()
{
    APP_LOGI("destroy status receiver proxy instance");
}

void StatusReceiverProxy::OnStatusNotify(const int32_t progress)
{
    APP_LOGI("status from service is %{public}d", progress);

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);
    if (!data.WriteInterfaceToken(StatusReceiverProxy::GetDescriptor())) {
        APP_LOGE("fail to OnStatusNotify due to write MessageParcel fail");
        return;
    }

    if (!data.WriteInt32(progress)) {
        APP_LOGE("fail to call OnStatusNotify, for write progress failed");
        return;
    }

    sptr<IRemoteObject> remote = Remote();
    if (!remote) {
        APP_LOGE("fail to call OnStatusNotify, for Remote() is nullptr");
        return;
    }

    int32_t ret =
        remote->SendRequest(static_cast<int32_t>(IStatusReceiver::Message::ON_STATUS_NOTIFY), data, reply, option);
    if (ret != NO_ERROR) {
        APP_LOGE("fail to call OnStatusNotify, for transact is failed, error code is: %{public}d", ret);
    }
}

void StatusReceiverProxy::OnFinished(const int32_t resultCode, const std::string &resultMsg)
{
    APP_LOGI("result from service is %{public}d, %{public}s", resultCode, resultMsg.c_str());
    // transform service error code to client error code.
    TransformResult(resultCode);

    MessageParcel data;
    MessageParcel reply;
    MessageOption option(MessageOption::TF_SYNC);
    if (!data.WriteInterfaceToken(StatusReceiverProxy::GetDescriptor())) {
        APP_LOGE("fail to OnFinished due to write MessageParcel fail");
        return;
    }

    if (!data.WriteInt32(resultCode_)) {
        APP_LOGE("fail to call OnFinished, for write resultCode_ failed");
        return;
    }
    if (!data.WriteString16(Str8ToStr16(resultMsg_))) {
        APP_LOGE("fail to call OnFinished, for write resultMsg_ failed");
        return;
    }

    sptr<IRemoteObject> remote = Remote();
    if (!remote) {
        APP_LOGE("fail to call OnFinished, for Remote() is nullptr");
        return;
    }

    int32_t ret = remote->SendRequest(static_cast<int32_t>(IStatusReceiver::Message::ON_FINISHED), data, reply, option);
    if (ret != NO_ERROR) {
        APP_LOGE("fail to call OnFinished, for transact is failed, error code is: %{public}d", ret);
    }
}

void StatusReceiverProxy::TransformResult(const int32_t resultCode)
{
    auto result = MAP_RECEIVED_RESULTS.find(resultCode);
    if (result != MAP_RECEIVED_RESULTS.end()) {
        resultCode_ = result->second.clientCode;
        resultMsg_ = result->second.clientMessage;
    } else {
        resultCode_ = ERR_UNKNOWN;
        resultMsg_ = MSG_ERR_UNKNOWN;
    }
    APP_LOGD("result transformed is %{public}d, %{public}s", resultCode_, resultMsg_.c_str());
}
}  // namespace AppExecFwk
}  // namespace OHOS