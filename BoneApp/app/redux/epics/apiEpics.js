import { mapTo, map, switchMap, catchError, startWith, exhaustMap, takeUntil } from "rxjs/operators";
import { combineEpics, ofType } from "redux-observable";
import { loadArchFailed, loadedArch, loadedStatus, loadStatusFailed, startArchUpdates, stopArchUpdates } from "../actions/telemetryActions";
import { api_loadedStatus, api_loadStatusFailed, telemetry_startArchUpdate, telemetry_startStatusUpdate, telemetry_stopArchUpdate, telemetry_stopStatusUpdate } from "../types";
import { Observable } from "rxjs/internal/Observable";
import { from, of, timer } from "rxjs";
import axios from "axios";

const ip = "http://192.168.0.195";

const statusUrl = "/status";
const archUrl = "/archdata";

const archPoll = (action$, state$) => action$.pipe(
    ofType(telemetry_startArchUpdate),
    switchMap(() =>
      timer(0, 500).pipe(
        takeUntil(action$.ofType(telemetry_stopArchUpdate)),
        exhaustMap(() =>
          from(axios.get(("http://" + state$.value.settings.ipAddress + archUrl), {timeout: 475})).pipe(
            map(res => loadedArch(res.data)),
            catchError(error => of(loadArchFailed(error)))
          )
        )
      )
    )
);

const statusPoll = (action$, state$) => action$.pipe(
    ofType(telemetry_startStatusUpdate),
    switchMap(() =>
      timer(0, 2000).pipe(
        takeUntil(action$.ofType(telemetry_stopStatusUpdate)),
        exhaustMap(() =>
          from(axios.get(("http://" + state$.value.settings.ipAddress + statusUrl), {timeout: 500})).pipe(
            map(res => loadedStatus(res.data)),
            catchError(error => of(loadStatusFailed(error)))
          )
        )
      )
    )
);

const successStatus = action$ => action$.pipe(
    ofType(api_loadedStatus),
    map(() => startArchUpdates())
);

const failStatus = action$ => action$.pipe(
    ofType(api_loadStatusFailed),
    map(() => stopArchUpdates())
);

export default combineEpics(statusPoll, archPoll, failStatus, successStatus);