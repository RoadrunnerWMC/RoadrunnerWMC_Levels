# Trying to prevent the initial zoom-out

*(these notes are much less polished than the others, and I barely understand them myself anymore)*

*(also note that many of these addresses are probably for US v2, not PAL v1)*

`80154954`: stores `3ff5b6db60000000` (1.3571428060531616) to `815e3884`+98 (`815e391c`)

Breakpointed on that field:
* `8007c120`: stores `bff0000000000000` (-1.0) to it (x4)
* `801555fc`: loads it into f1 -- then does *not* take the following branch
* `80155658`: stores `3ff5b6db60000000` into it again
* `801555fc`: loads it into f1 -- then *does* take the following branch

The zoomout does NOT trigger any further reads on this.

dBgGm_c: `81553884`

1.0 hex: `3f800000`

I think `815e3994` is a timer that counts down from 1.0 to 0.0 by 0.02 per frame, which controls this.

BPing on that:

    8000466c: stores 0 to it (this turns out to be __fill_mem)
    8007898c: loads it as this->_110. This is the weird not-a-function below the switch-case in that one float function. Multiplies with f2 (0), then adds to f1 (3ff5b6db60000000), then returns that.
    ^ repeat around 6 times or so
    [
    8007c5f4: stores 0 to it
    then back to 8007898c x4
    ] x3 or so
    80155c50: stores 3ff0000000000000 (1.0) to it!
    80155c88: loads it into f0, and then puts it into sp+0x30 (-> 8043f9b8)
    Breakpoint on that:
        8015f140 (load; first instruction in addCalc__4sLibFPfffff)
        8015f1e4 (load; near the end of that)
        8015607c (load -> f1, back in dBgGm_c::execute). Compared to 0.0, branch is taken
        801560a8: loaded back into f0
    801560d0: stored f0 (from above) back into it
    8007898c x5

The speed at which it zooms out (`dVar33`) depends on `this+90118` (`bVar29`)

`this+0x90114` (`815e3998`) might be the pre-zoom-out timer?

Starts out as 8...

|                      | Mario head | During wait | During zoomout | After |
|----------------------|------------|-------------|----------------|-------|
| `90110` (`815e3994`) | 1.0        | 1.0         | decreasing     | 0.0   |
| `90114` (`815e3998`) | 08         | 08          | 08             | 00    |
| `90118` (`815e399c`) | 0.0        | 0.0         | 0.0            | 0.0   |

* Tried nopping `80155b30`; this just prevents it from ever zooming out.
* That & change the following `li` to 0: wait followed by instant zoomout
* That & change the following `li` to 1: wait followed by instant zoomout
* That & change the following `li` to 2: wait followed by instant zoomout too far
* That & change the following `li` to 3: wait followed by instant zoomout too far
* That & change the following `li` to 5: wait followed by instant zoomin
* That & change the following `li` to 6: nothing
* That & change the following `li` to 7: nothing
* That & change the following `li` to 8: nothing
* That & change the following `li` to 9: wait followed by instant small zoomout (17)
* That & change the following `li` to a: wait followed by instant small zoomout (17)

Tried changing `80155c00`: `cmplwi r0, 0x5a` to other values. `28000000`: yay

This is 100% the timer before zoomout begins. However, 0 behaves very differently (already pre-zoomed-out) from 1+.
