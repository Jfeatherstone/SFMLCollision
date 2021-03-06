# Working notes

This file contains my notes from working on the project, and in no way is a complete documentation of the code.

## 7-24-2019
### Issues with the current circle collision model
- Momentum is not properly conserved, so circles that have some horizontal motion tend to lose that component when colliding with the flat bin object
- Final velocity should not solely be calculated based on the initial velocities of the objects, though in a more thorough model involving forces, this shouldn't so much be a problem (or it may manifest in different ways
- Balls tend to vibrate instead of coming to rest even with the use of the minimum cutoff velocity. This is likely because the balls on top of each other are pushing each other enough to exceed this cutoff

### Normal calculation
- When trying to display the individual normal vectors for each intersecting line, I am getting odd results, namely that (0, 0) seems to always show up. All of the other normals are also 1. not normal to the line, and 2. seem to connect the intersecting points

## 7-25-2019
### Converting resolution to be force based
- There are several benefits of changing the intersection resolution to be based on force rather than individual resolutions for each object. For one, this would make resolving collisions in which multiple objects are colliding more streamlined and easier. On top of this, it presents a more realistic model, as it would pertain to the equations.
- One of the biggest benefits of this system though is that it means I can assume that a force struct (or similar) will be passed to the resolution method properly, and I can work on the post force resolution for once, as opposed to trying to calculate that force right now. Hopefully this change of pace will prove to be helpful in actually making some progress.
- Instead of using two different objects, one for a force and one for a torque, it seems easier to instead describe a Force struct that is capable of calculating the torque applied by that force.
- As of now, I think the crux of the force calculation is finding the penetration of one object into the other, as this would allow for the calculation of the object's velocity such that the two objects no longer intersect each other. 
