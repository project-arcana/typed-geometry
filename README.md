# typed-geometry

Header-only strongly typed math library for graphics and geometry.


## Usage / Example

Type definitions only, no functions:
```
#include <tg/typed-geometry-lean.hh>
```

All functionality:
```
#include <tg/typed-geometry.hh>

tg::vec3 v;
tg::pos3 p;

// TODO
```

Helpers for interacting with the standard library:
```
#include <tg/typed-geometry-std.hh>

tg::pos3 p;
std::cout << p;                    // std::operator<<(std::ostream&, ...)
std::map<p, int> v_cnts;           // std::less
std::unordered_map<p, int> v_cnts; // std::hash
```

Most functionality is implemented as free functions, overloaded by type and with consistent vocabulary.

### Object Types

Basically all `tg` provided types are _regular_, i.e. have value semantics like `int` or `vector`.
The following categories exist:

* _POD_ types like `vec` and most fixed-size objects
* dynamically sized non-_POD_ values like `polygon` (similar to `vector`)
* _transparent_ types like `vec` where `.x` is part of the interface
* _opaque_ types like `angle` and `quadric` where the representation is implementation detail

### Vectors

Types `vec3`, `ivec4`, `dvec2`, ...

* `normalize(a)`: returns normalized version of `a`
* `dot(a, b)`: dot product
* `cross(a, b)`: cross product
* component-wise math functions: `sin`, `cos`, `abs`, `sqrt`, ...

### Transformations

* Matrix types `mat3`, `imat3x4`, ...
* Quaternion types `quat`, `dquat`, ...
* Transform types `transform3`, `dtransform2`, ...

### Objects

Types `pos3`, `iline2`, `ray3`, `dbox2`, ...

Represent individual points or sets of points.

* `contains(a, b)`: true iff `a` contains `b` (i.e. if each point in `b` is also contained in `a`)
* `intersects(a, b)`: true iff `a` and `b` intersect (i.e. if at least one point is in `a` and in `b`)
* `intersection(a, b)`: returns the appropriate object describing the intersection of two objects

See `docs/Objects.md` for more information.

### Interpolation

* `mix(a, b, t)`: linearly interpolates `a` and `b` with parameter `t` from 0..1.
* `lerp(a, b, t)`: same as `mix`
* `slerp(a, b, t)`: same as `mix` but with spherical interpolation (i.e. for quaternions)
* `smoothstep(x0, x1, x)`: see GLSL smoothstep
* `smootherstep(x0, x1, x)`: see GLSL smootherstep
* `lmap(x, {x0_from, x1_from}, {x0_to, x1_to})`: linearly maps a value from one range to another

TODO: splines

### Random

```
tg::rng rng;
auto v = uniform(rng, -1.0f, 1.0f);
auto p = uniform(rng, obj);
```

* `uniform(rng, a)`: uniformly samples a point from the object `a`
* `uniform(rng, a, b)`: same as `mix(a, b, uniform(t, 0.0f, 1.0f))`

### Naming Scheme

* `foo(a, b)` should read in the same order as english
* if not possible, `a.foo(b)` should make sense (e.g. `contains(a, b)` is interpreted as `a.contains(b)`)
* if still ambiguous: faster changing parameters go last (e.g. `coordinates(tri, point)` or `mix(a, b, t)`)


### C++ 17 Features

* deduction guides: `auto v = tg::vec(1, 2.5f, 3)`, `auto p = tg::pos(v);`
* structured binding: `auto [x, y, z] = tg::vec3(...)`

## Dependencies

None.

## TODO

* Benchmark how compile times are affected by includes / templates
* Add tests that verify optimal assembly generated
* Fractional and bigint data types
