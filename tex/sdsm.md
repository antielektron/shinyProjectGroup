# Sample Distribution Shadow Maps (SDSMs)

- hard shadows
- reduce under/oversampling, geometric aliasing errors
- paper says: scene-dependent/variable performance; i say: screen-space algorithm
- fully automated solution
- optimizing Z-partitions
- constant memory usage/predictable performance
- analyzing distribution of shadow map samples required
- automatically place tight Z-partitions
- tightly bounded light-space partition frusta

# Related Work

- Parallel-Split Shadow Maps (PSSMs) TODO
- other adaptive scene-dependent shadow algorithms
  - variable memory/performance characteristics
  - unpredictable performance
- Z-Partitioning (Lloyd et al. 2006) Cascaded Shadow Maps)
  - hand tuned partitions
- MORE
- object-space methods are limited by scene complexity

# Algorithm

- work on screen-space depth buffer
- min/max reduction on depth to get tight near/far planes
- minimize far/near ratios ~> minimize resolution error
- partition depth range logarithmically
- (other z partitioning algorithms, like K-means are not worth it!)
- other algorithms
  - project corners into light-space
  - compute frustum that wraps corners
- analyze sample distribution
- samples are all points in screen-space with depth in [near, far]
- project into light-space and compute minimal axis aligned bounding box
- rotate camera Z axis to align with light-space X axis
- project into light space and reduce min/maxXYZ corners
- compute frustum that wraps these corners

# Our implementation details

## First Approach

- use depth-buffer of previous frame (actually from two frames before, as we have two cpu readbacks)
- read reduction results back to cpu
- compute Z-partitions on cpu
- problem: artefacts on partition borders
  - when the camera moves, rendered geometry will not be covered by Z-partitions
  - even when taking camera movement into account
  - new geometry might be visible!

## Second Approach

- depth only pass before
- compute Z-partitions on gpu
- can use information from current scene
- had issues with synchronization of memory -> barriers
