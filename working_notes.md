# Working notes

This file contains my notes from working on the project, and in no way is a complete documentation of the code.

## 7-24-2019
### Issues with the current circle collision model
- Momentum is not properly conserved, so circles that have some horizontal motion tend to lose that component when colliding with the flat bin object
- Final velocity should not solely be calculated based on the initial velocities of the objects, though in a more thorough model involving forces, this shouldn't so much be a problem (or it may manifest in different ways


