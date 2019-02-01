# Objects

Objects are sets of points with `tg::pos` representing a singleton set.

Per default, all objects are closed, i.e. include their boundary.

## Supported Objects

### Singletons

* `pos`

### 1D objects

* `line`
* `segment`
* `ray`
* `polyline`

### 2D objects

* `triangle`
* `polygon`

### nD objects

* `box`
* `tube`
* `cylinder`
* `capsule`
* `sphere`
* `frustum`
* `halfspace`
* `hyperplane`


## Important functions

* `closest_points(a, b)`: returns points `pa` of `a` and `pb` of `b` that are closest to each other
* `project(p, o)`: projects point `p` onto object `o`, i.e. returns closest point on `o` to `p`
* `distance(a, b)`: returns the Euclidean distance between object `a` and `b`, defined as the distance between their closest points
* `distance2(a, b)`: same as `distance` but squared (usually faster because no `sqrt` involved)
* `direction(a, b)`: returns the normalized gradient that decreases the distance between `a` and `b` the most if `a` were moved in that direction. (returns zero if `a` intersects `b`.)
* `contains(a, b)`: true iff all points of `b` are contained in `a` (read: `a.contains(b)?`)
* `intersects(a, b)`: true iff there is a point `p` that is contained in `a` and `b`
* `aabb(...)`: returns the axis-aligned bounding `box` of all passed objects
* `volume(o)`, `area(o)`, `length(o)`: measures the object. Only defined when potentially non-zero (i.e. `volume(triangle)` is not defined).
* `uniform(rng, o)`: returns a uniformly sampled `pos` from `o`
* `coordinates(o, p)`: returns the coordinates of `p` in `o`, e.g. barycentric coordinates if `o` is an triangle. Use `tg::array` as return type for more than two values.

Some functions take optional `eps` tolerances, such as `contains` and `intersects`.


## How to add a new object

* Add an object file in `tg/types/objects/`
* Template appropriately (at least on `class ScalarT`, sometimes on dimension `int D`)
* Naming convention is `snake_case` for value types
* Declare `using`s for common cases (e.g. `pos3` for `pos<3, f32>`)
* Do not include `std`-headers, only use `tg` ones.
* Add support for `std::hash` and `std::less` in `tg/std/`
* Add unit tests (at least an interface check in `tests/geometry/objects.cc`), add to test list in `tests/geometry/std.cc`

### Implementation

Recommended minimal set:

* `uniform(rng, o)` for sampling
* `aabb(o)` for single-arg bounding box
* `project(p, o)` for projecting a point onto the object
* `contains(o, p)` for checking if a point is contained
* `volume`, `area`, `length` for finite objects where appropriate

Some functions have reasonable default implementations:

* `distance` as `sqrt(distance2)`
* `closest_points` as `project` if one argument is a `pos`
* `direction` as `normalize_safe(cb - ca)` for closest points `ca` and `cb`
* `distance2` as `distance2(ca, cb)` for closest points `ca` and `cb`

Good object implementations try to provide:

* `closest_points` for many object-object combinations
* `contains` for many object-object combinations
* `intersects` for many object-object combinations

Optional functions that only make sense for some objects:

* `coordinates`
* `signed_distance` same as `distance` but signed. Makes sense for hull objects with well defined inside-outside such as `plane`.
* `interpolate` with various interpretations
