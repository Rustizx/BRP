import { map, mergeMap, catchError} from "rxjs/operators";
import { combineEpics, ofType } from "redux-observable";
import { launch_launchRocket, launch_deploySupport } from "../types";
import { from, of } from "rxjs";
import axios from "axios";

const url = "/data";

const launchRocketEpic = (action$, state$) => action$.pipe(
    ofType(launch_launchRocket),
    mergeMap(action => 
        from(axios.post(("http://" + state$.value.settings.ipAddress + url), { action: "launch" }, {timeout: 400})).pipe(
            map(response => from(console.log("Launch " + response))),
            catchError(error => from(console.log(error)))
        )
    )
);

const deploySupportEpic = (action$, state$) => action$.pipe(
    ofType(launch_deploySupport),
    mergeMap(action => 
        from(axios.post(("http://" + state$.value.settings.ipAddress + url), { action: "support" }, {timeout: 400})).pipe(
            map(response => from(console.log("Support " + response))),
            catchError(error => from(console.log(error)))
        )
    )
);

export default combineEpics(launchRocketEpic, deploySupportEpic);