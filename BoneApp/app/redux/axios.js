import axois from "axios";

export const statusURL = "/status";
export const rocketDataURL = "/archdata";

export function api(url = "http://192.168.0.195") {
    let instance = axois.create({ baseURL : url });

    instance.defaults.timeout = 750;

    return instance;
};