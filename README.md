# Hob-Physics-Engine

<img width="560" alt="Screenshot 2023-10-24 at 9 13 03 PM" src="https://github.com/sameeragrawal536/Hob-Physics-Engine/assets/65458627/ff1a0dd5-9954-46e4-8de9-1e78d05aeee8">
![1280px-HD_transparent_picture](https://github.com/sameeragrawal536/Hob-Physics-Engine/assets/65458627/5bb4b193-1973-4dc4-b834-c25ffbc44c6c)

<img width="560" src="https://github.com/sameeragrawal536/Hob-Physics-Engine/assets/65458627/912ba660-ff1f-46f6-a8ee-64848c5cc80a"

It currently supports elastic collisions and gravitational interactions between particles, and is capable of simulating around 1.5k particles in real-time at around 25 FPS. For simulations larger than this, it automatically generates a 30 FPS video of the simulation using the [stb_image library](https://github.com/nothings/stb). In addition, because larger simulations can become unstable, the physics engine runs 5 substeps per frame of video for maximum accuracy and stability.

I have implemented the space-partitioning optimization for collision detection, and the Barnes-Hut algorithm with Quadtrees to optimize the gravity simulation. 

# Sample Videos

QuadTree Generation: https://drive.google.com/file/d/1rVcm2hXOToGibQfTZuRkBOLEZxE9q4IN/view?usp=share_link

Gravity-Based Implosion: https://drive.google.com/file/d/1NMl27XJuC6lvx2AZc1MpIqjskYfA40kA/view?usp=sharing

Multiple "Galaxies" Forming: https://drive.google.com/file/d/1HYNQpwOBfthQ27iSpALz2S0Wdnw6qYbg/view?usp=sharing
