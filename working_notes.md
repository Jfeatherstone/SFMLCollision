# Working notes

This file contains my notes from working on the project, and in no way is a complete documentation of the code.

## 7-24-2019
### Issues with the current circle collision model
- Momentum is not properly conserved, so circles that have some horizontal motion tend to lose that component when colliding with the flat bin object
- Final velocity should not solely be calculated based on the initial velocities of the objects, though in a more thorough model involving forces, this shouldn't so much be a problem (or it may manifest in different ways
- Balls tend to vibrate instead of coming to rest even with the use of the minimum cutoff velocity. This is likely because the balls on top of each other are pushing each other enough to exceed this cutoff

### Normal calculation
- When trying to display the individual normal vectors for each intersecting line, I am getting odd results, namely that (0, 0) seems to always show up. All of the other normals are also 1. not normal to the line, and 2. seem to connect the intersecting points


