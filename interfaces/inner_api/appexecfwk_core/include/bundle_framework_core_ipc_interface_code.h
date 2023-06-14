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

#ifndef OHOS_BUNDLE_APPEXECFWK_CORE_IPC_INTERFACE_CODE_H
#define OHOS_BUNDLE_APPEXECFWK_CORE_IPC_INTERFACE_CODE_H

#include <stdint.h>

/* SAID: 401 */
namespace OHOS {
namespace AppExecFwk {
enum class AppControlManagerInterfaceCode : uint32_t {
    ADD_APP_INSTALL_CONTROL_RULE = 0,
    DELETE_APP_INSTALL_CONTROL_RULE,
    CLEAN_APP_INSTALL_CONTROL_RULE,
    GET_APP_INSTALL_CONTROL_RULE,
    ADD_APP_RUNNING_CONTROL_RULE,
    DELETE_APP_RUNNING_CONTROL_RULE,
    CLEAN_APP_RUNNING_CONTROL_RULE,
    GET_APP_RUNNING_CONTROL_RULE,
    GET_APP_RUNNING_CONTROL_RULE_RESULT,
    SET_DISPOSED_STATUS,
    DELETE_DISPOSED_STATUS,
    GET_DISPOSED_STATUS,
    CONFIRM_APP_JUMP_CONTROL_RULE,
    ADD_APP_JUMP_CONTROL_RULE,
    DELETE_APP_JUMP_CONTROL_RULE,
    DELETE_APP_JUMP_CONTROL_RULE_BY_CALLER,
    DELETE_APP_JUMP_CONTROL_RULE_BY_TARGET,
    GET_APP_JUMP_CONTROL_RULE,
};

enum class BundleEventCallbackInterfaceCode : uint32_t {
    ON_RECEIVE_EVENT,
};

enum class BundleInstallerInterfaceCode : uint32_t {
    INSTALL = 0,
    INSTALL_MULTIPLE_HAPS,
    UNINSTALL,
    UNINSTALL_MODULE,
    UNINSTALL_BY_UNINSTALL_PARAM,
    RECOVER,
    INSTALL_SANDBOX_APP,
    UNINSTALL_SANDBOX_APP,
    CREATE_STREAM_INSTALLER,
    DESTORY_STREAM_INSTALLER,
};

enum class BundleStatusCallbackInterfaceCode : uint32_t {
    ON_BUNDLE_STATE_CHANGED,
};

enum class BundleStreamInstallerInterfaceCode : uint32_t {
    CREATE_STREAM = 0,
    STREAM_INSTALL = 1,
    CREATE_SHARED_BUNDLE_STREAM = 2,
    CREATE_SIGNATURE_FILE_STREAM = 3
};

enum class CleanCacheCallbackInterfaceCode : uint32_t {
    ON_CLEAN_CACHE_CALLBACK,
};

enum class StatusReceiverInterfaceCode : uint32_t {
    ON_STATUS_NOTIFY,
    ON_FINISHED,
};

enum class DefaultAppInterfaceCode : uint32_t {
    IS_DEFAULT_APPLICATION = 0,
    GET_DEFAULT_APPLICATION = 1,
    SET_DEFAULT_APPLICATION = 2,
    RESET_DEFAULT_APPLICATION = 3,
};

enum class OverlayManagerInterfaceCode : uint32_t {
    GET_ALL_OVERLAY_MODULE_INFO = 0,
    GET_OVERLAY_MODULE_INFO_BY_NAME = 1,
    GET_OVERLAY_MODULE_INFO = 2,
    GET_TARGET_OVERLAY_MODULE_INFOS = 3,
    GET_OVERLAY_MODULE_INFO_BY_BUNDLE_NAME = 4,
    GET_OVERLAY_BUNDLE_INFO_FOR_TARGET = 5,
    GET_OVERLAY_MODULE_INFO_FOR_TARGET = 6,
    SET_OVERLAY_ENABLED = 7,
    SET_OVERLAY_ENABLED_FOR_SELF = 8,
};

enum class QuickFixManagerInterfaceCode : uint32_t {
    DEPLOY_QUICK_FIX = 0,
    SWITCH_QUICK_FIX = 1,
    DELETE_QUICK_FIX = 2,
    CREATE_FD = 3
};

enum class QuickFixStatusCallbackInterfaceCode : uint32_t {
    ON_PATCH_DEPLOYED = 1,
    ON_PATCH_SWITCHED = 2,
    ON_PATCH_DELETED = 3
};

enum class BundleMgrInterfaceCode : uint32_t {
    GET_APPLICATION_INFO = 0,
    GET_APPLICATION_INFOS,
    GET_BUNDLE_INFO,
    GET_BUNDLE_PACK_INFO,
    GET_BUNDLE_INFOS,
    GET_UID_BY_BUNDLE_NAME,
    GET_APPID_BY_BUNDLE_NAME,
    GET_BUNDLE_NAME_FOR_UID,
    GET_BUNDLES_FOR_UID,
    GET_NAME_FOR_UID,
    GET_BUNDLE_GIDS,
    GET_BUNDLE_GIDS_BY_UID,
    GET_APP_TYPE,
    CHECK_IS_SYSTEM_APP_BY_UID,
    GET_BUNDLE_INFOS_BY_METADATA,
    QUERY_ABILITY_INFO,
    QUERY_ABILITY_INFOS,
    QUERY_ABILITY_INFO_BY_URI,
    QUERY_ABILITY_INFOS_BY_URI,
    QUERY_KEEPALIVE_BUNDLE_INFOS,
    GET_ABILITY_LABEL,
    GET_ABILITY_LABEL_WITH_MODULE_NAME,
    GET_BUNDLE_ARCHIVE_INFO,
    GET_HAP_MODULE_INFO,
    GET_LAUNCH_WANT_FOR_BUNDLE,
    GET_PERMISSION_DEF,
    CLEAN_BUNDLE_CACHE_FILES,
    CLEAN_BUNDLE_DATA_FILES,
    REGISTER_BUNDLE_STATUS_CALLBACK,
    CLEAR_BUNDLE_STATUS_CALLBACK,
    UNREGISTER_BUNDLE_STATUS_CALLBACK,
    DUMP_INFOS,
    IS_APPLICATION_ENABLED,
    SET_APPLICATION_ENABLED,
    IS_ABILITY_ENABLED,
    SET_ABILITY_ENABLED,
    GET_ABILITY_INFO,
    GET_ABILITY_INFO_WITH_MODULE_NAME,
    GET_ALL_FORMS_INFO,
    GET_FORMS_INFO_BY_APP,
    GET_FORMS_INFO_BY_MODULE,
    GET_SHORTCUT_INFO,
    GET_ALL_COMMON_EVENT_INFO,
    GET_BUNDLE_INSTALLER,
    QUERY_ABILITY_INFO_MUTI_PARAM,
    QUERY_ABILITY_INFOS_MUTI_PARAM,
    QUERY_ALL_ABILITY_INFOS,
    GET_APPLICATION_INFO_WITH_INT_FLAGS,
    GET_APPLICATION_INFOS_WITH_INT_FLAGS,
    GET_BUNDLE_INFO_WITH_INT_FLAGS,
    GET_BUNDLE_PACK_INFO_WITH_INT_FLAGS,
    GET_BUNDLE_INFOS_WITH_INT_FLAGS,
    GET_BUNDLE_ARCHIVE_INFO_WITH_INT_FLAGS,
    GET_BUNDLE_USER_MGR,
    GET_DISTRIBUTE_BUNDLE_INFO,
    QUERY_ABILITY_INFO_BY_URI_FOR_USERID,
    GET_APPLICATION_PRIVILEGE_LEVEL,
    QUERY_EXTENSION_INFO_WITHOUT_TYPE,
    QUERY_EXTENSION_INFO,
    QUERY_EXTENSION_INFO_BY_TYPE,
    VERIFY_CALLING_PERMISSION,
    QUERY_EXTENSION_ABILITY_INFO_BY_URI,
    IS_MODULE_REMOVABLE,
    SET_MODULE_REMOVABLE,
    QUERY_ABILITY_INFO_WITH_CALLBACK,
    UPGRADE_ATOMIC_SERVICE,
    IS_MODULE_NEED_UPDATE,
    SET_MODULE_NEED_UPDATE,
    GET_HAP_MODULE_INFO_WITH_USERID,
    IMPLICIT_QUERY_INFO_BY_PRIORITY,
    IMPLICIT_QUERY_INFOS,
    GET_ALL_DEPENDENT_MODULE_NAMES,
    GET_SANDBOX_APP_BUNDLE_INFO,
    QUERY_CALLING_BUNDLE_NAME,
    GET_DEFAULT_APP_PROXY,
    GET_BUNDLE_STATS,
    CHECK_ABILITY_ENABLE_INSTALL,
    GET_SANDBOX_APP_ABILITY_INFO,
    GET_SANDBOX_APP_EXTENSION_INFOS,
    GET_SANDBOX_MODULE_INFO,
    GET_MEDIA_DATA,
    GET_QUICK_FIX_MANAGER_PROXY,
    GET_STRING_BY_ID,
    GET_ICON_BY_ID,
    GET_UDID_BY_NETWORK_ID,
    GET_APP_CONTROL_PROXY,
    SET_DEBUG_MODE,
    QUERY_ABILITY_INFOS_V9,
    QUERY_EXTENSION_INFO_WITHOUT_TYPE_V9,
    QUERY_EXTENSION_INFO_V9,
    GET_APPLICATION_INFOS_WITH_INT_FLAGS_V9,
    GET_APPLICATION_INFO_WITH_INT_FLAGS_V9,
    GET_BUNDLE_ARCHIVE_INFO_WITH_INT_FLAGS_V9,
    GET_BUNDLE_INFO_WITH_INT_FLAGS_V9,
    GET_BUNDLE_INFOS_WITH_INT_FLAGS_V9,
    GET_SHORTCUT_INFO_V9,
    REGISTER_BUNDLE_EVENT_CALLBACK,
    UNREGISTER_BUNDLE_EVENT_CALLBACK,
    GET_BUNDLE_INFO_FOR_SELF,
    VERIFY_SYSTEM_API,
    GET_OVERLAY_MANAGER_PROXY,
    SILENT_INSTALL,
    PROCESS_PRELOAD,
    GET_APP_PROVISION_INFO,
    GET_PROVISION_METADATA,
    GET_BASE_SHARED_BUNDLE_INFOS,
    GET_ALL_SHARED_BUNDLE_INFO,
    GET_SHARED_BUNDLE_INFO,
    GET_SHARED_BUNDLE_INFO_BY_SELF,
    GET_SHARED_DEPENDENCIES,
    GET_DEPENDENT_BUNDLE_INFO,
    GET_UID_BY_DEBUG_BUNDLE_NAME,
    QUERY_LAUNCHER_ABILITY_INFO,
    GET_SPECIFIED_DISTRIBUTED_TYPE,
    GET_ADDITIONAL_INFO,
    GET_PROXY_DATA_INFOS,
    GET_ALL_PROXY_DATA_INFOS,
    SET_EXT_NAME_OR_MIME_TO_APP,
    DEL_EXT_NAME_OR_MIME_TO_APP,
};

enum class BundleUserMgrInterfaceCode : uint32_t {
    CREATE_USER = 0,
    REMOVE_USER = 1,
};

} // namespace AppExecFwk
} // namespace OHOS
#endif // OHOS_BUNDLE_APPEXECFWK_CORE_IPC_INTERFACE_CODE_H