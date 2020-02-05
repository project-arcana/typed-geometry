# Changelog


## Upcoming

* breaking changes:

    * `<typed-geometry/feature/[XYZ].hh>` now contains per-feature includes
    * `<typed-geometry/functions/[FEATURE]/...>` now contain granular per-feature includes
    * reworked object model (for example sphere and ball are not separate types anymore)

* new features:

    * `<typed-geometry/feature/fixed_int.hh>` for 128, 192, 256 bit int and uint logic
    * `<typed-geometry/feature/quadrics.hh>` for quadrics (including probabilistic quadrics)
    * `<typed-geometry/feature/interval.hh>` for interval arithmetic
    * `<typed-geometry/feature/fwd_diff.hh>` for forward auto-diff via dual numbers
    * `<typed-geometry/feature/swizzling.hh>` for swizzling
    * `<typed-geometry/feature/bezier.hh>` for bezier curves
    * `<typed-geometry/feature/quat.hh>` for quaternions


## 0.6.0

* renamed `<tg/typed-geometry.hh>` to `<typed-geometry/tg.hh>` for consistency with polymesh and less conflicts with typed graphics (can often be fixed by replacing `tg/typed-geometry` with `typed-geometry/tg` project-wide)