# Hob-Physics-Engine

This is a simple 2D physics engine built in C++ with the SDL Graphics Library. It currently supports elastic collisions and gravitational interactions between particles, and is capable of simulating around 2.5k particles in real-time at around 15 FPS. For simulations larger than this, it automatically generates a 30 FPS video of the simulation using the [stb_image library](https://github.com/nothings/stb).

# Optimizations
I have implemented the space-partitioning optimization for collision detection, and the Barnes-Hut algorithm with Quadtrees to optimize the gravity simulation. 
