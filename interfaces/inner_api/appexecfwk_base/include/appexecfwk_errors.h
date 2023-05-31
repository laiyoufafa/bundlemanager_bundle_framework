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

#ifndef FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_APPEXECFWK_ERRORS_H
#define FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_APPEXECFWK_ERRORS_H

#include "errors.h"

namespace OHOS {
enum {
    APPEXECFWK_MODULE_COMMON = 0x00,
    APPEXECFWK_MODULE_APPMGR = 0x01,
    APPEXECFWK_MODULE_BUNDLEMGR = 0x02,
    // Reserved 0x03 ~ 0x0f for new modules, Event related modules start from 0x10
    APPEXECFWK_MODULE_EVENTMGR = 0x10,
    APPEXECFWK_MODULE_HIDUMP = 0x11
};

// Error code for Common
constexpr ErrCode APPEXECFWK_COMMON_ERR_OFFSET = ErrCodeOffset(SUBSYS_APPEXECFWK, APPEXECFWK_MODULE_COMMON);
enum {
    ERR_APPEXECFWK_SERVICE_NOT_READY = APPEXECFWK_COMMON_ERR_OFFSET + 1,
    ERR_APPEXECFWK_SERVICE_NOT_CONNECTED,
    ERR_APPEXECFWK_INVALID_UID,
    ERR_APPEXECFWK_INVALID_PID,
    ERR_APPEXECFWK_PARCEL_ERROR,
    ERR_APPEXECFWK_FAILED_SERVICE_DIED,
    ERR_APPEXECFWK_OPERATION_TIME_OUT,
    ERR_APPEXECFWK_SERVICE_INTERNAL_ERROR,
};

// Error code for AppMgr
constexpr ErrCode APPEXECFWK_APPMGR_ERR_OFFSET = ErrCodeOffset(SUBSYS_APPEXECFWK, APPEXECFWK_MODULE_APPMGR);
enum {
    ERR_APPEXECFWK_ASSEMBLE_START_MSG_FAILED = APPEXECFWK_APPMGR_ERR_OFFSET + 1,
    ERR_APPEXECFWK_CONNECT_APPSPAWN_FAILED,
    ERR_APPEXECFWK_BAD_APPSPAWN_CLIENT,
    ERR_APPEXECFWK_BAD_APPSPAWN_SOCKET,
    ERR_APPEXECFWK_SOCKET_READ_FAILED,
    ERR_APPEXECFWK_SOCKET_WRITE_FAILED
};

// Error code for BundleMgr
constexpr ErrCode APPEXECFWK_BUNDLEMGR_ERR_OFFSET = ErrCodeOffset(SUBSYS_APPEXECFWK, APPEXECFWK_MODULE_BUNDLEMGR);
enum {
    // the install error code from 0x0001 ~ 0x0020.
    ERR_APPEXECFWK_INSTALL_INTERNAL_ERROR = APPEXECFWK_BUNDLEMGR_ERR_OFFSET + 0x0001, // 8519681
    ERR_APPEXECFWK_INSTALL_HOST_INSTALLER_FAILED,
    ERR_APPEXECFWK_INSTALL_PARSE_FAILED,
    ERR_APPEXECFWK_INSTALL_VERSION_DOWNGRADE,
    ERR_APPEXECFWK_INSTALL_VERIFICATION_FAILED,
    ERR_APPEXECFWK_INSTALL_PARAM_ERROR,
    ERR_APPEXECFWK_INSTALL_PERMISSION_DENIED,
    ERR_APPEXECFWK_INSTALL_ENTRY_ALREADY_EXIST,
    ERR_APPEXECFWK_INSTALL_STATE_ERROR,
    ERR_APPEXECFWK_INSTALL_FILE_PATH_INVALID = 8519690,
    ERR_APPEXECFWK_INSTALL_INVALID_HAP_NAME,
    ERR_APPEXECFWK_INSTALL_INVALID_BUNDLE_FILE,
    ERR_APPEXECFWK_INSTALL_INVALID_HAP_SIZE,
    ERR_APPEXECFWK_INSTALL_GENERATE_UID_ERROR,
    ERR_APPEXECFWK_INSTALL_INSTALLD_SERVICE_ERROR,
    ERR_APPEXECFWK_INSTALL_BUNDLE_MGR_SERVICE_ERROR,
    ERR_APPEXECFWK_INSTALL_ALREADY_EXIST,
    ERR_APPEXECFWK_INSTALL_BUNDLENAME_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_VERSIONCODE_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_VERSIONNAME_NOT_SAME = 8519700,
    ERR_APPEXECFWK_INSTALL_MINCOMPATIBLE_VERSIONCODE_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_VENDOR_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_RELEASETYPE_TARGET_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_RELEASETYPE_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_RELEASETYPE_COMPATIBLE_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_VERSION_NOT_COMPATIBLE,
    ERR_APPEXECFWK_INSTALL_APP_DISTRIBUTION_TYPE_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_APP_PROVISION_TYPE_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_INVALID_NUMBER_OF_ENTRY_HAP,
    ERR_APPEXECFWK_INSTALL_DISK_MEM_INSUFFICIENT,
    ERR_APPEXECFWK_INSTALL_GRANT_REQUEST_PERMISSIONS_FAILED,
    ERR_APPEXECFWK_INSTALL_UPDATE_HAP_TOKEN_FAILED = 8519712,
    ERR_APPEXECFWK_INSTALL_SINGLETON_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_ZERO_USER_WITH_NO_SINGLETON,
    ERR_APPEXECFWK_INSTALL_CHECK_SYSCAP_FAILED,
    ERR_APPEXECFWK_INSTALL_APPTYPE_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_URI_DUPLICATE,
    ERR_APPEXECFWK_INSTALL_TYPE_ERROR,
    ERR_APPEXECFWK_INSTALL_SDK_INCOMPATIBLE,
    ERR_APPEXECFWK_INSTALL_SO_INCOMPATIBLE,
    ERR_APPEXECFWK_INSTALL_AN_INCOMPATIBLE,
    ERR_APPEXECFWK_INSTALL_NOT_UNIQUE_DISTRO_MODULE_NAME,
    ERR_APPEXECFWK_INSTALL_INCONSISTENT_MODULE_NAME,
    ERR_APPEXECFWK_INSTALL_SINGLETON_INCOMPATIBLE,
    ERR_APPEXECFWK_INSTALL_DEVICE_TYPE_NOT_SUPPORTED,
    ERR_APPEXECFWK_INSTALL_COPY_HAP_FAILED,
    ERR_APPEXECFWK_INSTALL_DEPENDENT_MODULE_NOT_EXIST,
    ERR_APPEXECFWK_INSTALL_ASAN_ENABLED_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_ASAN_NOT_SUPPORT,
    ERR_APPEXECFWK_BUNDLE_TYPE_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_SHARE_APP_LIBRARY_NOT_ALLOWED,
    ERR_APPEXECFWK_INSTALL_COMPATIBLE_POLICY_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_FILE_IS_SHARED_LIBRARY,
    ERR_APPEXECFWK_INSTALL_CHECK_PROXY_DATA_URI_FAILED,
    ERR_APPEXECFWK_INSTALL_CHECK_PROXY_DATA_PERMISSION_FAILED,
    ERR_APPEXECFWK_INSTALL_DEBUG_NOT_SAME,
    ERR_APPEXECFWK_INSTALL_ISOLATION_MODE_FAILED,

    // signature errcode
    ERR_APPEXECFWK_INSTALL_FAILED_INVALID_SIGNATURE_FILE_PATH = 8519740,
    ERR_APPEXECFWK_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE_FILE,
    ERR_APPEXECFWK_INSTALL_FAILED_NO_BUNDLE_SIGNATURE,
    ERR_APPEXECFWK_INSTALL_FAILED_VERIFY_APP_PKCS7_FAIL,
    ERR_APPEXECFWK_INSTALL_FAILED_PROFILE_PARSE_FAIL,
    ERR_APPEXECFWK_INSTALL_FAILED_APP_SOURCE_NOT_TRUESTED,
    ERR_APPEXECFWK_INSTALL_FAILED_BAD_DIGEST,
    ERR_APPEXECFWK_INSTALL_FAILED_BUNDLE_INTEGRITY_VERIFICATION_FAILURE,
    ERR_APPEXECFWK_INSTALL_FAILED_FILE_SIZE_TOO_LARGE,
    ERR_APPEXECFWK_INSTALL_FAILED_BAD_PUBLICKEY,
    ERR_APPEXECFWK_INSTALL_FAILED_BAD_BUNDLE_SIGNATURE = 8519750,
    ERR_APPEXECFWK_INSTALL_FAILED_NO_PROFILE_BLOCK_FAIL,
    ERR_APPEXECFWK_INSTALL_FAILED_BUNDLE_SIGNATURE_VERIFICATION_FAILURE,
    ERR_APPEXECFWK_INSTALL_FAILED_VERIFY_SOURCE_INIT_FAIL,
    ERR_APPEXECFWK_INSTALL_FAILED_INCOMPATIBLE_SIGNATURE,
    ERR_APPEXECFWK_INSTALL_FAILED_INCONSISTENT_SIGNATURE,
    ERR_APPEXECFWK_INSTALL_FAILED_MODULE_NAME_EMPTY,
    ERR_APPEXECFWK_INSTALL_FAILED_MODULE_NAME_DUPLICATE,
    ERR_APPEXECFWK_INSTALL_FAILED_CHECK_HAP_HASH_PARAM,

    // sandbox app install
    ERR_APPEXECFWK_SANDBOX_INSTALL_INTERNAL_ERROR = 8519800,
    ERR_APPEXECFWK_SANDBOX_INSTALL_APP_NOT_EXISTED,
    ERR_APPEXECFWK_SANDBOX_INSTALL_PARAM_ERROR,
    ERR_APPEXECFWK_SANDBOX_INSTALL_WRITE_PARCEL_ERROR,
    ERR_APPEXECFWK_SANDBOX_INSTALL_READ_PARCEL_ERROR,
    ERR_APPEXECFWK_SANDBOX_INSTALL_SEND_REQUEST_ERROR,
    ERR_APPEXECFWK_SANDBOX_INSTALL_USER_NOT_EXIST,
    ERR_APPEXECFWK_SANDBOX_INSTALL_INVALID_APP_INDEX,
    ERR_APPEXECFWK_SANDBOX_INSTALL_NOT_INSTALLED_AT_SPECIFIED_USERID,
    ERR_APPEXECFWK_SANDBOX_INSTALL_NO_SANDBOX_APP_INFO,
    ERR_APPEXECFWK_SANDBOX_INSTALL_UNKNOWN_INSTALL_TYPE,
    ERR_APPEXECFWK_SANDBOX_INSTALL_DELETE_APP_INDEX_FAILED,
    ERR_APPEXECFWK_SANDBOX_APP_NOT_SUPPORTED,
    ERR_APPEXECFWK_SANDBOX_INSTALL_GET_PERMISSIONS_FAILED,
    ERR_APPEXECFWK_SANDBOX_INSTALL_DATABASE_OPERATION_FAILED,

    // sandbox app query
    ERR_APPEXECFWK_SANDBOX_QUERY_PARAM_ERROR,
    ERR_APPEXECFWK_SANDBOX_QUERY_INTERNAL_ERROR,
    ERR_APPEXECFWK_SANDBOX_QUERY_INVALID_USER_ID,
    ERR_APPEXECFWK_SANDBOX_QUERY_NO_SANDBOX_APP,
    ERR_APPEXECFWK_SANDBOX_QUERY_NO_MODULE_INFO,
    ERR_APPEXECFWK_SANDBOX_QUERY_NO_USER_INFO,

    ERR_APPEXECFWK_PARSE_UNEXPECTED = APPEXECFWK_BUNDLEMGR_ERR_OFFSET + 0x00c8, // 8519880
    ERR_APPEXECFWK_PARSE_MISSING_BUNDLE,
    ERR_APPEXECFWK_PARSE_MISSING_ABILITY,
    ERR_APPEXECFWK_PARSE_NO_PROFILE,
    ERR_APPEXECFWK_PARSE_BAD_PROFILE,
    ERR_APPEXECFWK_PARSE_PROFILE_PROP_TYPE_ERROR,
    ERR_APPEXECFWK_PARSE_PROFILE_MISSING_PROP,
    ERR_APPEXECFWK_PARSE_PERMISSION_ERROR,
    ERR_APPEXECFWK_PARSE_PROFILE_PROP_CHECK_ERROR,
    ERR_APPEXECFWK_PARSE_PROFILE_PROP_SIZE_CHECK_ERROR,
    ERR_APPEXECFWK_PARSE_RPCID_FAILED,
    ERR_APPEXECFWK_PARSE_NATIVE_SO_FAILED,
    ERR_APPEXECFWK_PARSE_AN_FAILED,

    ERR_APPEXECFWK_INSTALLD_PARAM_ERROR = 8519893,
    ERR_APPEXECFWK_INSTALLD_GET_PROXY_ERROR,
    ERR_APPEXECFWK_INSTALLD_CREATE_DIR_FAILED,
    ERR_APPEXECFWK_INSTALLD_CREATE_DIR_EXIST,
    ERR_APPEXECFWK_INSTALLD_CHOWN_FAILED,
    ERR_APPEXECFWK_INSTALLD_REMOVE_DIR_FAILED,
    ERR_APPEXECFWK_INSTALLD_EXTRACT_FILES_FAILED,
    ERR_APPEXECFWK_INSTALLD_RNAME_DIR_FAILED,
    ERR_APPEXECFWK_INSTALLD_CLEAN_DIR_FAILED,
    ERR_APPEXECFWK_INSTALLD_MOVE_FILE_FAILED,
    ERR_APPEXECFWK_INSTALLD_COPY_FILE_FAILED,
    ERR_APPEXECFWK_INSTALLD_MKDIR_FAILED,
    ERR_APPEXECFWK_INSTALLD_PERMISSION_DENIED,
    ERR_APPEXECFWK_INSTALLD_SET_SELINUX_LABEL_FAILED,
    ERR_APPEXECFWK_INSTALLD_AOT_EXECUTE_FAILED = 8519907,
    ERR_APPEXECFWK_INSTALLD_AOT_ABC_NOT_EXIST = 8519908,

    ERR_APPEXECFWK_UNINSTALL_SYSTEM_APP_ERROR,
    ERR_APPEXECFWK_UNINSTALL_KILLING_APP_ERROR,
    ERR_APPEXECFWK_UNINSTALL_INVALID_NAME,
    ERR_APPEXECFWK_UNINSTALL_PARAM_ERROR,
    ERR_APPEXECFWK_UNINSTALL_PERMISSION_DENIED,
    ERR_APPEXECFWK_UNINSTALL_BUNDLE_MGR_SERVICE_ERROR,
    ERR_APPEXECFWK_UNINSTALL_MISSING_INSTALLED_BUNDLE,
    ERR_APPEXECFWK_UNINSTALL_MISSING_INSTALLED_MODULE,
    ERR_APPEXECFWK_UNINSTALL_SHARE_APP_LIBRARY_IS_NOT_EXIST,
    ERR_APPEXECFWK_UNINSTALL_SHARE_APP_LIBRARY_IS_RELIED,
    ERR_APPEXECFWK_UNINSTALL_BUNDLE_IS_SHARED_LIBRARY,

    ERR_APPEXECFWK_FAILED_GET_INSTALLER_PROXY,
    ERR_APPEXECFWK_FAILED_GET_BUNDLE_INFO,
    ERR_APPEXECFWK_FAILED_GET_ABILITY_INFO,
    ERR_APPEXECFWK_FAILED_GET_RESOURCEMANAGER,
    ERR_APPEXECFWK_FAILED_GET_REMOTE_PROXY,
    ERR_APPEXECFWK_PERMISSION_DENIED,
    ERR_APPEXECFWK_INPUT_WRONG_TYPE_FILE,
    ERR_APPEXECFWK_ENCODE_BASE64_FILE_FAILED,

    ERR_APPEXECFWK_RECOVER_GET_BUNDLEPATH_ERROR = APPEXECFWK_BUNDLEMGR_ERR_OFFSET + 0x0201, // 8520193
    ERR_APPEXECFWK_RECOVER_INVALID_BUNDLE_NAME,
    ERR_APPEXECFWK_RECOVER_NOT_ALLOWED,

    ERR_APPEXECFWK_USER_NOT_EXIST = APPEXECFWK_BUNDLEMGR_ERR_OFFSET + 0x0301,
    ERR_APPEXECFWK_USER_CREATE_FAILED,
    ERR_APPEXECFWK_USER_REMOVE_FAILED,
    ERR_APPEXECFWK_USER_NOT_INSTALL_HAP,

    // error code in prebundle sacn
    ERR_APPEXECFWK_PARSE_FILE_FAILED,

    // debug mode
    ERR_BUNDLEMANAGER_SET_DEBUG_MODE_INTERNAL_ERROR,
    ERR_BUNDLEMANAGER_SET_DEBUG_MODE_PARCEL_ERROR,
    ERR_BUNDLEMANAGER_SET_DEBUG_MODE_SEND_REQUEST_ERROR,
    ERR_BUNDLEMANAGER_SET_DEBUG_MODE_UID_CHECK_FAILED,
    ERR_BUNDLEMANAGER_SET_DEBUG_MODE_INVALID_PARAM,

    // overlay installation errcode
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INTERNAL_ERROR = 8520600,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INVALID_BUNDLE_NAME,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INVALID_MODULE_NAME,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_ERROR_HAP_TYPE,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_ERROR_BUNDLE_TYPE,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_BUNDLE_NAME_MISSED,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_MODULE_NAME_MISSED,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_BUNDLE_NAME_NOT_SAME,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INTERNAL_EXTERNAL_OVERLAY_EXISTED_SIMULTANEOUSLY,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_PRIORITY_NOT_SAME,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INVALID_PRIORITY,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INCONSISTENT_VERSION_CODE,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_SERVICE_EXCEPTION,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_BUNDLE_NAME_SAME_WITH_TARGET_BUNDLE_NAME,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_NO_SYSTEM_APPLICATION_FOR_EXTERNAL_OVERLAY,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_DIFFERENT_SIGNATURE_CERTIFICATE,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_BUNDLE_IS_OVERLAY_BUNDLE,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_MODULE_IS_OVERLAY_MODULE,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_OVERLAY_TYPE_NOT_SAME,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_INVALID_BUNDLE_DIR,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_BUNDLE_IS_NOT_STAGE_MODULE,
    ERR_BUNDLEMANAGER_OVERLAY_INSTALLATION_FAILED_TARGET_BUNDLE_IS_SERVICE,

    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_PARAM_ERROR,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_MISSING_OVERLAY_BUNDLE,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_MISSING_OVERLAY_MODULE,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_NON_OVERLAY_BUNDLE,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_NON_OVERLAY_MODULE,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_BUNDLE_NOT_INSTALLED_AT_SPECIFIED_USERID,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_TARGET_BUNDLE_NOT_EXISTED,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_TARGET_MODULE_NOT_EXISTED,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_NO_OVERLAY_BUNDLE_INFO,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_NO_OVERLAY_MODULE_INFO,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_PERMISSION_DENIED,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_TARGET_MODULE_IS_OVERLAY_MODULE,
    ERR_BUNDLEMANAGER_OVERLAY_QUERY_FAILED_TARGET_BUNDLE_IS_OVERLAY_BUNDLE,
    ERR_BUNDLEMANAGER_OVERLAY_SET_OVERLAY_PARAM_ERROR,

    // quick fix errcode
    ERR_BUNDLEMANAGER_QUICK_FIX_INTERNAL_ERROR = APPEXECFWK_BUNDLEMGR_ERR_OFFSET + 0x0401, // 8520705
    ERR_BUNDLEMANAGER_QUICK_FIX_PARAM_ERROR,
    ERR_BUNDLEMANAGER_QUICK_FIX_PROFILE_PARSE_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_BUNDLE_NAME_NOT_SAME,
    ERR_BUNDLEMANAGER_QUICK_FIX_VERSION_CODE_NOT_SAME,
    ERR_BUNDLEMANAGER_QUICK_FIX_VERSION_NAME_NOT_SAME,
    ERR_BUNDLEMANAGER_QUICK_FIX_PATCH_VERSION_CODE_NOT_SAME,
    ERR_BUNDLEMANAGER_QUICK_FIX_PATCH_VERSION_NAME_NOT_SAME,
    ERR_BUNDLEMANAGER_QUICK_FIX_PATCH_TYPE_NOT_SAME,
    ERR_BUNDLEMANAGER_QUICK_FIX_MODULE_NAME_SAME,
    ERR_BUNDLEMANAGER_QUICK_FIX_UNKNOWN_QUICK_FIX_TYPE,
    ERR_BUNDLEMANAGER_QUICK_FIX_SO_INCOMPATIBLE,
    ERR_BUNDLEMANAGER_QUICK_FIX_BUNDLE_NAME_NOT_EXIST,
    ERR_BUNDLEMANAGER_QUICK_FIX_MODULE_NAME_NOT_EXIST,
    ERR_BUNDLEMANAGER_QUICK_FIX_SIGNATURE_INFO_NOT_SAME,
    ERR_BUNDLEMANAGER_QUICK_FIX_ADD_HQF_FAILED,                                           // 8520720
    ERR_BUNDLEMANAGER_QUICK_FIX_SAVE_APP_QUICK_FIX_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_VERSION_CODE_ERROR,
    ERR_BUNDLEMANAGER_QUICK_FIX_EXTRACT_DIFF_FILES_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_APPLY_DIFF_PATCH_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_NO_PATCH_IN_DATABASE,
    ERR_BUNDLEMANAGER_QUICK_FIX_INVALID_PATCH_STATUS,
    ERR_BUNDLEMANAGER_QUICK_FIX_NOT_EXISTED_BUNDLE_INFO,
    ERR_BUNDLEMANAGER_QUICK_FIX_REMOVE_PATCH_PATH_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_CREATE_PATCH_PATH_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_MOVE_PATCH_FILE_FAILED,                                   // 8520730
    ERR_BUNDLEMANAGER_QUICK_FIX_HOT_RELOAD_NOT_SUPPORT_RELEASE_BUNDLE,
    ERR_BUNDLEMANAGER_QUICK_FIX_PATCH_ALREADY_EXISTED,
    ERR_BUNDLEMANAGER_QUICK_FIX_HOT_RELOAD_ALREADY_EXISTED,
    ERR_BUNDLEMANAGER_QUICK_FIX_NO_PATCH_INFO_IN_BUNDLE_INFO,
    ERR_BUNDLEMANAGER_QUICK_FIX_OLD_PATCH_OR_HOT_RELOAD_IN_DB,
    ERR_BUNDLEMANAGER_QUICK_FIX_SEND_REQUEST_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_REAL_PATH_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_INVALID_PATH,
    ERR_BUNDLEMANAGER_QUICK_FIX_OPEN_SOURCE_FILE_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_CREATE_FD_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_INVALID_TARGET_DIR,
    ERR_BUNDLEMANAGER_QUICK_FIX_CREATE_TARGET_DIR_FAILED,
    ERR_BUNDLEMANAGER_QUICK_FIX_PERMISSION_DENIED,
    ERR_BUNDLEMANAGER_QUICK_FIX_WRITE_FILE_FAILED,

    ERR_BUNDLE_MANAGER_APP_CONTROL_INTERNAL_ERROR = APPEXECFWK_BUNDLEMGR_ERR_OFFSET + 0x0501, // 8520961
    ERR_BUNDLE_MANAGER_APP_CONTROL_PERMISSION_DENIED,
    ERR_BUNDLE_MANAGER_APP_CONTROL_RULE_TYPE_INVALID,
    ERR_BUNDLE_MANAGER_BUNDLE_NOT_SET_CONTROL,
    ERR_BUNDLE_MANAGER_APP_CONTROL_DISALLOWED_INSTALL,
    ERR_BUNDLE_MANAGER_APP_CONTROL_DISALLOWED_UNINSTALL,
    
    // query errcode
    ERR_BUNDLE_MANAGER_INTERNAL_ERROR = APPEXECFWK_BUNDLEMGR_ERR_OFFSET + 0x0601, // 8521217
    ERR_BUNDLE_MANAGER_INVALID_PARAMETER,
    ERR_BUNDLE_MANAGER_INVALID_USER_ID,
    ERR_BUNDLE_MANAGER_BUNDLE_NOT_EXIST,
    ERR_BUNDLE_MANAGER_ABILITY_NOT_EXIST,
    ERR_BUNDLE_MANAGER_MODULE_NOT_EXIST,
    ERR_BUNDLE_MANAGER_ABILITY_DISABLED,
    ERR_BUNDLE_MANAGER_APPLICATION_DISABLED,
    ERR_BUNDLE_MANAGER_PARAM_ERROR,
    ERR_BUNDLE_MANAGER_PERMISSION_DENIED,
    ERR_BUNDLE_MANAGER_IPC_TRANSACTION,
    ERR_BUNDLE_MANAGER_GLOBAL_RES_MGR_ENABLE_DISABLED,
    ERR_BUNDLE_MANAGER_CAN_NOT_CLEAR_USER_DATA,
    ERR_BUNDLE_MANAGER_QUERY_PERMISSION_DEFINE_FAILED,
    ERR_BUNDLE_MANAGER_DEVICE_ID_NOT_EXIST,
    ERR_BUNDLE_MANAGER_PROFILE_NOT_EXIST,
    ERR_BUNDLE_MANAGER_INVALID_UID,
    ERR_BUNDLE_MANAGER_INVALID_HAP_PATH,
    ERR_BUNDLE_MANAGER_DEFAULT_APP_NOT_EXIST,
    ERR_BUNDLE_MANAGER_INVALID_TYPE,
    ERR_BUNDLE_MANAGER_ABILITY_AND_TYPE_MISMATCH,
    ERR_BUNDLE_MANAGER_SYSTEM_API_DENIED,
    // zlib errcode
    ERR_ZLIB_SRC_FILE_DISABLED,
    ERR_ZLIB_DEST_FILE_DISABLED,
    ERR_ZLIB_SERVICE_DISABLED,
    ERR_ZLIB_SRC_FILE_FORMAT_ERROR,
    // app jump interceptor
    ERR_BUNDLE_MANAGER_APP_JUMP_INTERCEPTOR_INTERNAL_ERROR = APPEXECFWK_BUNDLEMGR_ERR_OFFSET + 0x0701, // 8521473
    ERR_BUNDLE_MANAGER_APP_JUMP_INTERCEPTOR_PERMISSION_DENIED,
    ERR_BUNDLE_MANAGER_APP_JUMP_INTERCEPTOR_RULE_TYPE_INVALID,
    ERR_BUNDLE_MANAGER_BUNDLE_NOT_SET_JUMP_INTERCPTOR,
};

// Error code for Hidump
constexpr ErrCode APPEXECFWK_HIDUMP = ErrCodeOffset(SUBSYS_APPEXECFWK, APPEXECFWK_MODULE_HIDUMP);
enum {
    ERR_APPEXECFWK_HIDUMP_ERROR = APPEXECFWK_HIDUMP + 1,
    ERR_APPEXECFWK_HIDUMP_INVALID_ARGS,
    ERR_APPEXECFWK_HIDUMP_UNKONW,
    ERR_APPEXECFWK_HIDUMP_SERVICE_ERROR
};

}  // namespace OHOS

#endif  // FOUNDATION_APPEXECFWK_INTERFACES_INNERKITS_APPEXECFWK_BASE_INCLUDE_APPEXECFWK_ERRORS_H
