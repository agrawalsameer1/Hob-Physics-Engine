# Hob-Physics-Engine

<img width="560" alt="Screenshot 2023-10-24 at 9 13 03 PM" src="https://github.com/sameeragrawal536/Hob-Physics-Engine/assets/65458627/ff1a0dd5-9954-46e4-8de9-1e78d05aeee8">

<img width="560" height="15" src="https://github.com/sameeragrawal536/Hob-Physics-Engine/assets/65458627/912ba660-ff1f-46f6-a8ee-64848c5cc80a">

The Hob Physics Engine is a 2D physics engine built in C++ with the SDL Graphics Library.

It currently supports elastic collisions and gravitational interactions between particles, and is capable of simulating around 1.3k particles in real-time at 30 FPS. For simulations larger than this, it automatically generates a 30 FPS video of the simulation using the [stb_image library](https://github.com/nothings/stb). 

I have implemented the space-partitioning optimization for collision detection, and the Quadtree Barnes-Hut algorithm to optimize n-body gravity simulation. I have also implemented the 4th Order Runge-Kutta method for maximum simulation stability. 

# Sample Videos

QuadTree Generation: https://drive.google.com/file/d/1rVcm2hXOToGibQfTZuRkBOLEZxE9q4IN/view?usp=share_link

Gravity-Based Implosion: https://drive.google.com/file/d/1NMl27XJuC6lvx2AZc1MpIqjskYfA40kA/view?usp=sharing

Galaxy Formation (50k particles): https://drive.google.com/file/d/1s3-5SS6HO1BUpoq5KWYwV5rh6bHDO5AY/view?usp=drive_link
