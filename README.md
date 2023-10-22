# Hob-Physics-Engine

This is a simple 2D physics engine built in C++ with the SDL Graphics Library. It currently supports elastic collisions and gravitational interactions between particles, and is capable of simulating around 2.5k particles in real-time at around 15 FPS. For simulations larger than this, it automatically generates a 30 FPS video of the simulation using the [stb_image library](https://github.com/nothings/stb).

I have implemented the space-partitioning optimization for collision detection, and the Barnes-Hut algorithm with Quadtrees to optimize the gravity simulation. 

# Sample Videos

QuadTree Generation: https://drive.google.com/file/d/1rVcm2hXOToGibQfTZuRkBOLEZxE9q4IN/view?usp=share_link

Gravity-Based Implosion: https://drive.google.com/file/d/1NMl27XJuC6lvx2AZc1MpIqjskYfA40kA/view?usp=sharing

Multiple Galaxies Forming: https://drive.google.com/file/d/1UyovO_xXs-sqNeUtyNm9_bPYpWUJhfa3/view?usp=sharing

Comparison Photo For Galaxies: 
My Simulation             |  Hubble Image
:-------------------------:|:-------------------------:
![](https://...Dark.png)  |  ![]([https://...Ocean.png](https://cdn.theatlantic.com/thumbor/HLOtKC-Sb-E-EGlFXiRsrRyhQjo=/0x1078:4164x3420/976x549/media/img/mt/2017/05/STSCI_H_p1720a_f_4164x4634/original.png))

![Screenshot 2023-10-22 at 1 11 51 PM](https://github.com/sameeragrawal536/SDL-Physics-Engine/assets/65458627/3901f3e4-26c9-4dfa-b206-d0fad971783c)
