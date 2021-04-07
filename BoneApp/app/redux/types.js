// -------------- TELEMETRY TYPES --------------

// APIs Actions
export const api_loadStatus = "api/loadStatus";
export const api_loadingStatus = "api/loadingStatus";
export const api_loadedStatus = "api/loadedStatus";
export const api_loadStatusFailed = "api/loadStatusFailed";

export const api_loadArch = "api/loadArch";
export const api_loadingArch = "api/loadingArch";
export const api_loadedArch = "api/loadedArch";
export const api_loadArchFailed = "api/loadArchFailed";


// Data Actions
export const telemetry_startStatusUpdate = "telemetry/startStatusUpdate";
export const telemetry_stopStatusUpdate = "telemetry/stopStatusUpdate";

export const telemetry_startArchUpdate = "telemetry/startArchUpdate";
export const telemetry_stopArchUpdate = "telemetry/stopArchUpdate";

export const telemetry_resetUpdate = "telemetry/resetData";



// -------------- LAUNCH TYPES --------------
export const launch_launchRocket = "launch/launchRocket";
export const launch_deploySupport = "launch/deploySupport";


// -------------- SETTINGS TYPES --------------

// Local Device Storage Actions
export const settings_storage_load = "settings/storage/load";
export const settings_storage_store = "settings/storage/store";

export const settings_storage_clearIPs = "settings/storage/clearIPs";

// Settings Changes Actions
export const settings_setIP = "settings/setIP";
export const settings_changeCountdownTime = "settings/changeCountdownTime";

// Testing Actions
export const test_flip = "testFlip";